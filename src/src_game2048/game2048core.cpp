#include "stdafx.h"
#include "game2048core.h"

const int       GAME_GLOBAL_NUMBER = 2048;
const float     PROBABILITY_OF_2 = 0.8f;


CGame2048Core::CGame2048Core()
{
    m_gameMode = GAME_MODE_INVALID;
    m_bWin = FALSE;					// 游戏是否已经赢了
    m_nScore = 0;				// 记录游戏当前分数
    m_nRowCount = 0;
    m_nColumnCount = 0;
}

CGame2048Core::~CGame2048Core()
{
}

BOOL CGame2048Core::Init( GAME_MODE gameMode /*= GAME_MODE_4X4*/ )
{
    BOOL bRet = FALSE;

    if (gameMode == GAME_MODE_INVALID)
    {
        goto Exit0;
    }

    m_gameMode = gameMode;

    if (!_InitRandom())
    {
        goto Exit0;
    }

    if(!_InitGameRect())
    {
        goto Exit0;
    }

    if(!_InitNumbers())
    {
        goto Exit0;
    }

    if(!_InitMergeFlags())
    {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:

    return bRet;
}

BOOL CGame2048Core::_InitRandom()
{
    return m_random.Init();
}

BOOL CGame2048Core::_InitGameRect()
{
    BOOL bRet = FALSE;

    if (m_gameMode != GAME_MODE_4X4 && m_gameMode != GAME_MODE_8x8)
    {
        goto Exit0;
    }

    if (m_gameMode == GAME_MODE_4X4)
    {
        m_nRowCount = m_nColumnCount = 4;
    }
    else if (m_gameMode == GAME_MODE_8x8)
    {
        m_nRowCount = m_nColumnCount = 8;
    }

    bRet = TRUE;

Exit0:

    return bRet;
}

BOOL CGame2048Core::_InitNumbers()
{
    BOOL bRet = FALSE;
    IntVector tmpVector;

    if (m_nRowCount == 0 || m_nColumnCount == 0)
    {
        goto Exit0;
    }

    m_numbers.clear();

    tmpVector.insert(tmpVector.begin(), m_nRowCount, 0);
    m_numbers.insert(m_numbers.begin(), m_nColumnCount, tmpVector);

    bRet = TRUE;

Exit0:
    return bRet;
}


BOOL CGame2048Core::_InitMergeFlags()
{
    BOOL bRet = FALSE;
    IntVector tmpVector;

    if (m_nRowCount == 0 || m_nColumnCount == 0)
    {
        goto Exit0;
    }

    m_mergeFlags.clear();
    tmpVector.insert(tmpVector.begin(), m_nRowCount, FALSE);
    m_mergeFlags.insert(m_mergeFlags.begin(), m_nColumnCount, tmpVector);

    bRet = TRUE;

Exit0:
    return bRet;
}


BOOL CGame2048Core::Uninit()
{
    return TRUE;
}


void CGame2048Core::Start()
{
    //_GenerateNumber(INIT_NUMBER_COUNT);

    m_numbers[0][0] = 2;
    m_numbers[0][1] = 2;
}

BOOL CGame2048Core::_GetRandomPoint(CGame2048Core::POINT* pResult) const
{
    BOOL bRet = FALSE;
    int x = 0, y = 0;
    ASSERT(IsFull() == FALSE && pResult != NULL);

    do 
    {
        x = m_random.GetInt(m_nRowCount);
        y = m_random.GetInt(m_nColumnCount);
    } while (GetAt(x, y) != 0);

    pResult->x = x;
    pResult->y = y;

    return bRet;
}

void CGame2048Core::_GenerateNumber(int nCount /*= 1*/)
{
    POINT point = {0};
    int nVal = 0;
    ASSERT(IsFull() == FALSE);

    for (int i = 0; i < nCount; i++)
    {
        _GetRandomPoint(&point);
        nVal = _Get2Or4(PROBABILITY_OF_2);
        _SetAt(point, nVal);
        _FC_LOG("生成新点位于(%d, %d) = %d", point.x, point.y, nVal);

        if (IsFull())
        {		
            break;
        }
    }
}


int CGame2048Core::_Get2Or4( float fProbabilityOf2 ) const
{
    ASSERT(fProbabilityOf2 < 1.0);

    double fRandomNumber = m_random.GetDouble();
    if (fRandomNumber > fProbabilityOf2)
    {
        return 4;
    }

    return 2;
}


BOOL CGame2048Core::IsFull() const
{
    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nColumnCount; ++j)
        {
            if (m_numbers[i][j] == 0)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

int CGame2048Core::GetAt( int x, int y ) const
{
    int nRet = INVALID_GAME_NUMBER;

    POINT point = {x, y};
    nRet = GetAt(point);

    return nRet;
}

int CGame2048Core::GetAt( const POINT& point ) const
{
    int nRet = INVALID_GAME_NUMBER;

    if (!_IsPointValid(point))
    {
        _FC_LOG("invalid position to get");
        goto Exit0;
    }

    nRet = m_numbers[point.x][point.y];

Exit0:

    return nRet;
}

BOOL CGame2048Core::_IsPointValid(const POINT& point) const
{
    BOOL bRet = FALSE;

    if (point.x < 0 || point.y < 0)
    {
        goto Exit0;
    }

    if (point.x >= m_nRowCount || point.y >= m_nColumnCount)
    {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:

    return bRet;
}


void CGame2048Core::_GetNextPoint( const POINT& point, POINT* pResultPoint, CGame2048Core::DIRECTION dirction ) const
{
    ASSERT(pResultPoint != NULL);

    POINT nextPoint = point;

    if (pResultPoint == NULL)
    {
        goto Exit0;
    }

    switch(dirction)
    {
    case DIRECTION_DOWN:
        nextPoint.x += 1;
        break;

    case DIRECTION_UP:
        nextPoint.x -= 1;
        break;

    case DIRECTION_LEFT:
        nextPoint.y -= 1;
        break;

    case DIRECTION_RIGHT:
        nextPoint.y += 1;
        break;

    default:
        nextPoint.x = nextPoint.y = -1;
        break;
    }

    *pResultPoint = nextPoint;

Exit0:

    return;
}


BOOL CGame2048Core::_CanPointMove( const POINT& point, CGame2048Core::DIRECTION dirction ) const
{
    BOOL bRet = FALSE;
    POINT nextPoint = {-1, -1};
    int nNumber = 0;
    int nNextNumber = 0;

    if (_IsBlank(point))
    {
        goto Exit0;
    }

    _GetNextPoint(point, &nextPoint, dirction);
    if (!_IsPointValid(nextPoint))
    {
        goto Exit0;
    }

    nNextNumber = GetAt(nextPoint);
    nNumber = GetAt(point);
    if (nNumber != nNextNumber && !_IsBlank(nextPoint))
    {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:

    return bRet;
}

void CGame2048Core::_MovePoint(CGame2048Core::POINT* pMovingPoint, CGame2048Core::DIRECTION direction)
{
    ASSERT(pMovingPoint != NULL && _IsPointValid(*pMovingPoint));

    POINT nextPoint = {-1, -1};

    if (pMovingPoint == NULL || _IsPointValid(*pMovingPoint) == FALSE)
    {
        goto Exit0;
    }

    do 
    {		
        _MovePointToNext(pMovingPoint, direction);
        _GetNextPoint(*pMovingPoint, &nextPoint, direction);
        if (!_CanPointMove(nextPoint, direction))
        {
            break;
        }
        *pMovingPoint = nextPoint;

    } while (TRUE);

Exit0:

    return;
}

BOOL CGame2048Core::_MovePointToNext(CGame2048Core::POINT* pMovingPoint, CGame2048Core::DIRECTION direction)
{
    ASSERT(pMovingPoint != NULL && _IsPointValid(*pMovingPoint));

    BOOL bIsMoved = FALSE;
    POINT nextPoint = {-1, -1};

    if (pMovingPoint == NULL || _IsPointValid(*pMovingPoint) == FALSE)
    {
        goto Exit0;
    }

    _GetNextPoint(*pMovingPoint, &nextPoint, direction);
    if (!_IsPointValid(nextPoint))
    {
        return bIsMoved;
    }

    int nMovingNumber = GetAt(*pMovingPoint);
    if (_IsBlank(nextPoint))
    {
        _SetAt(nextPoint, nMovingNumber);
        _SetAt(*pMovingPoint, 0);
        bIsMoved = TRUE;
    }

Exit0:

    return bIsMoved;
}

void CGame2048Core::_Slide( CGame2048Core::DIRECTION dirction )
{
    BOOL bGenerate = FALSE;
    POINT movingPoint = { 0 };
    POINT currentPoint = {-1, -1};
    BOOL bNeedMove = FALSE;
    
    bNeedMove = _GetFirstMovePoint(&currentPoint, dirction);
    if (bNeedMove == FALSE)
    {
        goto Exit0;
    }

    while (bNeedMove)
    {
        if (!_CanPointMove(currentPoint, dirction))
        {
            bNeedMove = _GetNextMovePoint(&currentPoint, dirction);
            continue;
        }

        movingPoint = currentPoint;
        _MovePoint(&movingPoint, dirction);
        if (_CanPointMerge(movingPoint, dirction))
        {
            _MergePoint(movingPoint, dirction);
        }

        bGenerate = TRUE;
        bNeedMove = _GetNextMovePoint(&currentPoint, dirction);
    } 

    if (bGenerate)
    {
        int nGenerateCount = _GetGenerateCount(m_gameMode);
        _GenerateNumber(nGenerateCount);
    }

    _ClearMergeFlags();

Exit0:

    return;

}


void CGame2048Core::SlideUp()
{	
    _Slide(DIRECTION_UP);
}

void CGame2048Core::SlideDown()
{
    _Slide(DIRECTION_DOWN);
}

void CGame2048Core::SlideRight()
{
    _Slide(DIRECTION_RIGHT);
}

void CGame2048Core::SlideLeft()
{
    _Slide(DIRECTION_LEFT);
}

BOOL CGame2048Core::IsWin() const
{
    for (int i = 0; i < m_nRowCount; i++)
    {
        for (int j = 0; j < m_nColumnCount; j++)
        {
            if (GetAt(i, j) == GAME_GLOBAL_NUMBER)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CGame2048Core::IsFail() const
{
    BOOL bRet = FALSE;

    if (IsFull() == FALSE)
    {
        goto Exit0;
    }

    if (_HasAdjacentSame())
    {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:

    return bRet;
}

int CGame2048Core::GetScore() const
{
    return m_nScore;
}


BOOL CGame2048Core::_IsBlank( const POINT& point ) const
{
    BOOL bRet = FALSE;
    int nNumber = INVALID_GAME_NUMBER;
    
    nNumber = GetAt(point);
    if (nNumber == 0)
    {
        bRet = TRUE;
    }

    return bRet;
}

void CGame2048Core::_SetAt( const POINT& point, int nNumber )
{
    if (!_IsPointValid(point))
    {
        _FC_LOG("Invalid point");
        return;
    }

    m_numbers[point.x][point.y] = nNumber;
}

BOOL CGame2048Core::_GetFirstMovePoint( POINT* pPoint, DIRECTION dirction ) const
{
    ASSERT(pPoint != NULL);

    BOOL bRet = FALSE;
    POINT firstPoint = {-1, -1};
    
    if (pPoint == NULL)
    {
        goto Exit0;
    }

    switch(dirction)
    {
    case DIRECTION_LEFT:
        firstPoint.x = 0;
        firstPoint.y = 1;
        break;

    case DIRECTION_UP:
        firstPoint.x = 1;
        firstPoint.y = 0;
        break;

    case DIRECTION_DOWN:
        firstPoint.x = m_nRowCount - 2;
        firstPoint.y = m_nColumnCount - 1;
        break;

    case DIRECTION_RIGHT:
        firstPoint.x = m_nRowCount - 1;
        firstPoint.y = m_nColumnCount - 2;
        break;

    default:
        bRet = FALSE;
        break;
    }

    if (_IsPointValid(firstPoint))
    {
        *pPoint = firstPoint;
        bRet = TRUE;
    }

Exit0:
    return bRet;
}

BOOL CGame2048Core::_GetNextMovePointOrder( POINT* pPoint ) const
{
    ASSERT(pPoint != NULL);

    BOOL bRet = FALSE;
    POINT nextPoint = {0};
        
    if (pPoint == NULL)
    {
        goto Exit0;
    }

    nextPoint = *pPoint;
    nextPoint.y += 1;
    if (nextPoint.y > m_nColumnCount - 1)
    {
        nextPoint.y = 0;
        nextPoint.x++;
    }

    *pPoint = nextPoint;

    bRet = TRUE;

Exit0:

    return bRet;
}

BOOL CGame2048Core::_GetNextMovePointReverse( POINT* pPoint ) const
{
    ASSERT(pPoint != NULL);

    BOOL bRet = FALSE;
    POINT nextPoint = {0};

    if (pPoint == NULL)
    {
        goto Exit0;
    }
        
    nextPoint = *pPoint;
    nextPoint.y -= 1;
    if (nextPoint.y < 0)
    {
        nextPoint.y = m_nColumnCount - 1;
        nextPoint.x--;
    }
    *pPoint = nextPoint;

    bRet = TRUE;

Exit0:

    return bRet;
}

BOOL CGame2048Core::_GetNextMovePoint( POINT* pPoint, DIRECTION dirction ) const
{
    ASSERT(pPoint != NULL);

    BOOL bRet = FALSE;
    POINT nextPoint = {0};

    if (pPoint == NULL)
    {
        goto Exit0;
    }

    nextPoint = *pPoint;

    switch(dirction)
    {
    case DIRECTION_LEFT:
    case DIRECTION_UP:
        _GetNextMovePointOrder(&nextPoint);
        break;

    case DIRECTION_DOWN:
    case DIRECTION_RIGHT:
        _GetNextMovePointReverse(&nextPoint);
        break;

    default:
        bRet = FALSE;
        break;
    }

    if (_IsPointValid(nextPoint))
    {
        *pPoint = nextPoint;
        bRet = TRUE;
    }

Exit0:

    return bRet;
}

BOOL CGame2048Core::_IsPointFromMerge( const POINT& point ) const
{
    ASSERT(_IsPointValid(point));
    return m_mergeFlags[point.x][point.y];
}

void CGame2048Core::_MarkPointMerge( const POINT& point )
{
    ASSERT(_IsPointValid(point));
    m_mergeFlags[point.x][point.y] = TRUE;
}

void CGame2048Core::_ClearMergeFlags()
{
    _InitMergeFlags();
}

int CGame2048Core::GetRowCount() const
{
    return m_nRowCount;
}

int CGame2048Core::GetColumnCount() const
{
    return m_nColumnCount;
}

GAME_MODE CGame2048Core::GetGameMode() const
{
    return m_gameMode;
}

BOOL CGame2048Core::_HasAdjacentSame() const
{
    BOOL bRet = FALSE;

    for (int i = 0; i < m_nRowCount; ++i)
    {
        for (int j = 0; j < m_nColumnCount; ++j)
        {
            if (i > 0 && m_numbers[i - 1][j] == m_numbers[i][j])
            {
                bRet = TRUE;
                goto Exit0;
            }

            if (j > 0 && m_numbers[i][j - 1] == m_numbers[i][j])
            {
                bRet = TRUE;
                goto Exit0;
            }
        }
    }

Exit0:

    return bRet;
}

int CGame2048Core::_GetGenerateCount( GAME_MODE gameMode /*= GAME_MODE_INVALID*/ ) const
{
    int nRet = 0;

    if (gameMode == GAME_MODE_INVALID)
    {
        gameMode = m_gameMode;
    }
    
    switch(gameMode)
    {
    case GAME_MODE_4X4:
        nRet = 1;
        break;

    case GAME_MODE_8x8:
        nRet = 2;
        break;

    default:
        _FC_LOG("Unknown game mode!");
        break;
    }

    return nRet;
}	

BOOL CGame2048Core::_CanPointMerge( const POINT& point, DIRECTION dirction ) const
{
    BOOL bRet = FALSE;
    POINT nextPoint = {-1, -1};

    if (!_IsPointValid(point))
    {
        goto Exit0;
    }

    _GetNextPoint(point, &nextPoint, dirction);
    if (!_IsPointValid(nextPoint))
    {
        goto Exit0;
    }

    if (_IsPointFromMerge(nextPoint))
    {
        goto Exit0;
    }

    if (GetAt(point) == GetAt(nextPoint))
    {
        bRet = TRUE;
    }

Exit0:

    return bRet;
}

void CGame2048Core::_MergePoint( const POINT& point, DIRECTION dirction )
{
    ASSERT(_CanPointMerge(point, dirction));

    POINT nextPoint = {-1, -1};
    _GetNextPoint(point, &nextPoint, dirction);

    ASSERT(_IsPointValid(nextPoint));
    ASSERT(GetAt(point) == GetAt(nextPoint));

    int nValue = GetAt(nextPoint);
    _SetAt(nextPoint, 2 * nValue);
    _SetAt(point, 0);
    m_nScore += nValue;

    _MarkPointMerge(nextPoint);
}


