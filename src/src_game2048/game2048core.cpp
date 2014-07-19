#include "stdafx.h"
#include "game2048core.h"


CGame2048Core::CGame2048Core()
{
	m_nScore = 0;
	m_bWin = FALSE;
}

CGame2048Core::~CGame2048Core()
{
}

BOOL CGame2048Core::Init( GAME_MODE gameMode /*= GAME_MODE_4X4*/ )
{
	m_gameMode = gameMode;

	_InitGameRect();
	_InitNumbers();
	_InitMergeFlags();

	return TRUE;
}


BOOL CGame2048Core::_InitGameRect()
{
	ASSERT(m_gameMode == GAME_MODE_4X4 || m_gameMode == GAME_MODE_8x8);

	if (m_gameMode == GAME_MODE_4X4)
	{
		m_nRowCount = m_nColumnCount = 4;
	}
	else if (m_gameMode == GAME_MODE_8x8)
	{
		m_nRowCount = m_nColumnCount = 8;
	}

	return TRUE;
}

BOOL CGame2048Core::_InitNumbers()
{
	m_numbers.clear();
	
	IntVector tmpVector;
	tmpVector.insert(tmpVector.begin(), m_nRowCount, 0);
	m_numbers.insert(m_numbers.begin(), m_nColumnCount, tmpVector);

	return TRUE;
}


BOOL CGame2048Core::_InitMergeFlags()
{
	m_mergeFlags.clear();

	IntVector tmpVector;
	tmpVector.insert(tmpVector.begin(), m_nRowCount, FALSE);
	m_mergeFlags.insert(m_mergeFlags.begin(), m_nColumnCount, tmpVector);

	return TRUE;
}


BOOL CGame2048Core::Uninit()
{
	return TRUE;
}


void CGame2048Core::Start()
{
	_GenerateNumber(INIT_NUMBER_COUNT);

	// 	 	int number[4][4] = {
	// 	 		{2,		4,		2,		2},
	// 	 		{16,	32,		16,		8},
	// 	 		{32,	64,		256,	64},
	// 	 		{128,	2,		512,	128}
	// 	 	};
	// 	 	memcpy(&m_numbers, &number, sizeof(number));

// 	m_numbers[0][0] = 2;
// 	m_numbers[0][1] = 2;
// 	m_numbers[0][2] = 0;
// 	m_numbers[0][3] = 2;
}

void CGame2048Core::_GenerateNumber(int nCount /*= 1*/)
{
	ASSERT(IsFull() == FALSE);

	for (int i = 0; i < nCount; i++)
	{
		int x = 0, y = 0, nVal = 0;
		do 
		{
			x = m_random.GetInt(m_nRowCount);
			y = m_random.GetInt(m_nColumnCount);
			nVal = _Get2Or4(0.8f);
		} while (m_numbers[x][y] != 0);

		_FC_LOG("生成新点位于(%d, %d) = %d", x, y, nVal);
		m_numbers[x][y] = nVal;

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
	POINT point = {x, y};
	return GetAt(point);
}

int CGame2048Core::GetAt( const POINT& point ) const
{
	if (!_IsPointValid(point))
	{
		_FC_LOG("invalid position to get");
		return INVALID_GAME_NUMBER;
	}

	int nNumber = m_numbers[point.x][point.y];
	return nNumber;
}

BOOL CGame2048Core::_IsPointValid(const POINT& point) const
{
	if (point.x < 0 || point.y < 0)
	{
		return FALSE;
	}

	if (point.x >= m_nRowCount || point.y >= m_nColumnCount)
	{
		return FALSE;
	}

	return TRUE;
}


void CGame2048Core::_GetNextPoint( const POINT& point, POINT* pResultPoint, CGame2048Core::DIRECTION dirction ) const
{
	ASSERT(pResultPoint != NULL);

	POINT nextPoint = point;
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
}


BOOL CGame2048Core::_CanPointMove( const POINT& point, CGame2048Core::DIRECTION dirction ) const
{
	POINT nextPoint = {-1, -1};
	_GetNextPoint(point, &nextPoint, dirction);

	if (!_IsPointValid(nextPoint))
	{
		return FALSE;
	}

	int nNextNumber = GetAt(nextPoint);
	int nNumber = GetAt(point);
	if (nNumber != nNextNumber && nNextNumber != 0)
	{
		return FALSE;
	}

	return TRUE;
}

void CGame2048Core::_MovePoint(CGame2048Core::POINT* pMovingPoint, CGame2048Core::DIRECTION direction)
{
	ASSERT(pMovingPoint != NULL && _IsPointValid(*pMovingPoint));

	POINT& movingPoint = *pMovingPoint;
	POINT nextPoint = {-1, -1};
	do 
	{		
		_MovePointToNext(&movingPoint, direction);
		_GetNextPoint(movingPoint, &nextPoint, direction);
		if (!_CanPointMove(nextPoint, direction))
		{
			break;
		}
		movingPoint = nextPoint;			
	} while (TRUE);
}

BOOL CGame2048Core::_MovePointToNext(CGame2048Core::POINT* pMovingPoint, CGame2048Core::DIRECTION direction)
{
	ASSERT(pMovingPoint != NULL && _IsPointValid(*pMovingPoint));

	BOOL bIsMoved = FALSE;
	POINT nextPoint = {-1, -1};
	POINT& movingPoint = *pMovingPoint;
	_GetNextPoint(movingPoint, &nextPoint, direction);

	if (!_IsPointValid(nextPoint))
	{
		return bIsMoved;
	}

	int nMovingNumber = GetAt(movingPoint);
	if (_IsBlank(nextPoint))
	{
		_SetAt(nextPoint, nMovingNumber);
		_SetAt(movingPoint, 0);
		bIsMoved = TRUE;
	}

	return bIsMoved;
}

void CGame2048Core::_Slide( CGame2048Core::DIRECTION dirction )
{
	BOOL bGenerate = FALSE;
	POINT currentPoint = {-1, -1};
	BOOL bNeedMove = _GetFirstMovePoint(&currentPoint, dirction);
	if (bNeedMove == FALSE)
	{
		return;
	}

	while (bNeedMove)
	{
		if (!_CanPointMove(currentPoint, dirction))
		{
			bNeedMove = _GetNextMovePoint(&currentPoint, dirction);
			continue;
		}

		POINT movingPoint = currentPoint;
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
	if (IsFull() == FALSE)
	{
		return FALSE;
	}

	if (_HasAdjacentSame())
	{
		return FALSE;
	}

	return TRUE;
}

int CGame2048Core::GetScore() const
{
	return m_nScore;
}


BOOL CGame2048Core::_IsBlank( const POINT& point ) const
{
	int nNumber = GetAt(point);
	if (nNumber == 0)
	{
		return TRUE;
	}

	return FALSE;
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

	POINT firstPoint = {-1, -1};
	BOOL bRet = FALSE;

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

	return bRet;
}

BOOL CGame2048Core::_GetNextMovePointOrder( POINT* pPoint ) const
{
	ASSERT(pPoint != NULL);

	POINT nextPoint = *pPoint;

	nextPoint.y += 1;
	if (nextPoint.y > m_nColumnCount - 1)
	{
		nextPoint.y = 0;
		nextPoint.x++;
	}

	*pPoint = nextPoint;

	return TRUE;
}

BOOL CGame2048Core::_GetNextMovePointReverse( POINT* pPoint ) const
{
	ASSERT(pPoint != NULL);

	POINT nextPoint = *pPoint;
	nextPoint.y -= 1;
	if (nextPoint.y < 0)
	{
		nextPoint.y = m_nColumnCount - 1;
		nextPoint.x--;
	}
	*pPoint = nextPoint;

	return TRUE;
}

BOOL CGame2048Core::_GetNextMovePoint( POINT* pPoint, DIRECTION dirction ) const
{
	ASSERT(pPoint != NULL);

	POINT nextPoint = *pPoint;
	BOOL bRet = FALSE;

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
	for (int i = 0; i < m_nRowCount; ++i)
	{
		for (int j = 0; j < m_nColumnCount; ++j)
		{
			if (i > 0 && m_numbers[i - 1][j] == m_numbers[i][j])
			{
				return TRUE;
			}

			if (j > 0 && m_numbers[i][j - 1] == m_numbers[i][j])
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CGame2048Core::_GetGenerateCount( GAME_MODE gameMode /*= GAME_MODE_INVALID*/ ) const
{
	if (gameMode == GAME_MODE_INVALID)
	{
		gameMode = m_gameMode;
	}

	int nRet = 0;
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
	if (!_IsPointValid(point))
	{
		return FALSE;
	}

	POINT nextPoint = {-1, -1};
	_GetNextPoint(point, &nextPoint, dirction);
	if (!_IsPointValid(nextPoint))
	{
		return FALSE;
	}

	if (_IsPointFromMerge(nextPoint))
	{
		return FALSE;
	}

	if (GetAt(point) == GetAt(nextPoint))
	{
		return TRUE;
	}

	return FALSE;
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
