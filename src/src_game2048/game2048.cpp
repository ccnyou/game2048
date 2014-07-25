#include "stdafx.h"
#include "Game2048.h"


CGame2048::CGame2048()
{
    memset(&m_numbers, 0, sizeof(m_numbers));
    memset(&m_mergeFlags, 0, sizeof(m_mergeFlags));

    m_score = 0;
    m_bWin = FALSE;
}

CGame2048::~CGame2048()
{

}

void CGame2048::Init()
{
    GenerateNumber();
    GenerateNumber();
    // 	m_numbers[0][0] = 1024;
    // 	m_numbers[0][1] = 1024;
    // 	m_numbers[0][3] = 2;
}

void CGame2048::GenerateNumber()
{
    ASSERT(IsFull() == FALSE);

    int x, y, val;
    do 
    {
        x = m_random.GetInt(4);
        y = m_random.GetInt(4);

        // 80% �ĸ�������һ�� 2��20% �ĸ�������һ�� 4
        val = 2;
        int randVal = m_random.GetInt(10);
        if (randVal >= 8)
        {
            val = 4;
        }
    } while (m_numbers[x][y] != 0);

    _FC_LOG("�����µ�λ��(%d, %d) = %d", x, y, val);
    m_numbers[x][y] = val;
}

BOOL CGame2048::IsFull() const
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (m_numbers[i][j] == 0)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

int CGame2048::GetAt( int x, int y ) const
{
    return m_numbers[x][y];
}

void CGame2048::SlideUp()
{	
    BOOL bGenerate = FALSE;
    for (int i = 1; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (m_numbers[i][j] == 0)
            {
                // ����ǿո�
                continue;
            }

            if (m_numbers[i - 1][j] != 0 && m_numbers[i - 1][j] != m_numbers[i][j])
            {
                // �ϱߵ�һ�����ǿ��Ҳ�ͬ�������ƶ�
                continue;
            }

            for (int k = 0; k < i; ++k)
            {
                if (m_numbers[k][j] == 0)
                {
                    //�ҵ�һ���հ׵�
                    m_numbers[k][j] = m_numbers[i][j]; 
                    m_numbers[i][j] = 0;
                    bGenerate = TRUE;
                    break;
                }
                else if (m_numbers[i][j] == m_numbers[k][j]  && m_mergeFlags[k][j] == FALSE)
                {
                    //�ҵ���ͬ�ģ�������û���ϰ���
                    if (HasObstacle(i, j, k, j))
                    {
                        continue;
                    }

                    m_numbers[k][j] = m_numbers[i][j] * 2;
                    m_numbers[i][j] = 0;
                    m_mergeFlags[k][j] = TRUE;

                    _FC_LOG("���ƣ�(%d,%d) �� (%d, %d) �ϲ����õ� %d", k, j, i, j, m_numbers[k][j]);
                    if (m_numbers[k][j] == 2048)
                    {
                        m_bWin = TRUE;
                    }
                    m_score += m_numbers[k][j];
                    bGenerate = TRUE;
                }
            }
        }
    }

    if (bGenerate)
    {
        GenerateNumber();
    }
    memset(&m_mergeFlags, 0, sizeof(m_mergeFlags));
}

void CGame2048::SlideDown()
{
    BOOL bGenerate = FALSE;

    for (int i = 2; i >= 0; --i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (m_numbers[i][j] == 0)
            {
                // ����ǿո�
                continue;
            }

            if (m_numbers[i + 1][j] != 0 && m_numbers[i + 1][j] != m_numbers[i][j])
            {
                // �±ߵ�һ�����ǿ��Ҳ�ͬ�������ƶ�
                continue;
            }

            for (int k = 3; k > i; --k)
            {
                if (m_numbers[k][j] == 0)
                {
                    //�ҵ�һ���հ׵�
                    m_numbers[k][j] = m_numbers[i][j]; 
                    m_numbers[i][j] = 0;
                    bGenerate = TRUE;
                    break;
                }
                else if (m_numbers[i][j] == m_numbers[k][j] && m_mergeFlags[k][j] == FALSE)
                {
                    if (HasObstacle(i, j, k, j))
                    {
                        continue;
                    }

                    m_numbers[k][j] = m_numbers[i][j] * 2;
                    m_numbers[i][j] = 0;
                    m_mergeFlags[k][j] = TRUE;

                    _FC_LOG("���ƣ�(%d,%d) �� (%d, %d) �ϲ����õ� %d", i, j, k, j, m_numbers[k][j]);
                    if (m_numbers[k][j] == 2048)
                    {
                        m_bWin = TRUE;
                    }
                    m_score += m_numbers[k][j];
                    bGenerate = TRUE;
                }
            }
        }
    }

    if (bGenerate)
    {
        GenerateNumber();
    }
    memset(&m_mergeFlags, 0, sizeof(m_mergeFlags));
}

void CGame2048::SlideRight()
{
    BOOL bGenerate = FALSE;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 2; j >= 0; --j)
        {
            if (m_numbers[i][j] == 0)
            {
                // ����ǿո�
                continue;
            }

            if (m_numbers[i][j + 1] != 0 && m_numbers[i][j + 1] != m_numbers[i][j])
            {
                // �ұߵ�һ�����ǿգ������ƶ�
                continue;
            }

            for (int k = 3; k > j; --k)
            {
                if (m_numbers[i][k] == 0)
                {
                    // �ҵ�һ���հ׵�
                    m_numbers[i][k] = m_numbers[i][j]; 
                    m_numbers[i][j] = 0;
                    bGenerate = TRUE;
                    break;
                }
                else if (m_numbers[i][j] == m_numbers[i][k] && m_mergeFlags[i][k] == FALSE)
                {
                    if (HasObstacle(i, j, i, k))
                    {
                        continue;
                    }

                    m_numbers[i][k] = m_numbers[i][j] * 2;
                    m_numbers[i][j] = 0;
                    m_mergeFlags[i][k] = TRUE;
                    _FC_LOG("���ƣ�(%d,%d) �� (%d, %d) �ϲ����õ� %d", i, j, i, k, m_numbers[i][k]);

                    if (m_numbers[i][k] == 2048)
                    {
                        m_bWin = TRUE;
                    }
                    m_score += m_numbers[i][k];
                    bGenerate = TRUE;
                }
            }
        }
    }

    if (bGenerate)
    {
        GenerateNumber();
    }
    memset(&m_mergeFlags, 0, sizeof(m_mergeFlags));
}

void CGame2048::SlideLeft()
{
    BOOL bGenerate = FALSE;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 1; j < 4; ++j)
        {
            if (m_numbers[i][j] == 0)
            {
                // ����ǿո�
                continue;
            }

            if (m_numbers[i][j - 1] != 0 && m_numbers[i][j - 1] != m_numbers[i][j])
            {
                // ��ߵ�һ�����ǿգ������ƶ�
                continue;
            }

            for (int k = 0; k < j; ++k)
            {
                if (m_numbers[i][k] == 0)
                {
                    //�ҵ�һ���հ׵�
                    m_numbers[i][k] = m_numbers[i][j]; 
                    m_numbers[i][j] = 0;
                    bGenerate = TRUE;
                    break;
                }
                else if (m_numbers[i][j] == m_numbers[i][k] && m_mergeFlags[i][k] == FALSE)
                {
                    if (HasObstacle(i, j, i, k))
                    {
                        continue;
                    }

                    m_numbers[i][k] = m_numbers[i][j] * 2;
                    m_numbers[i][j] = 0;
                    m_mergeFlags[i][k] = TRUE;
                    _FC_LOG("���ƣ�(%d,%d) �� (%d, %d) �ϲ����õ� %d", i, j, i, k, m_numbers[i][k]);
                    if (m_numbers[i][k] == 2048)
                    {
                        m_bWin = TRUE;
                    }
                    m_score += m_numbers[i][k];
                    bGenerate = TRUE;
                }
            }
        }
    }

    if (bGenerate)
    {
        GenerateNumber();
    }
    memset(&m_mergeFlags, 0, sizeof(m_mergeFlags));
}

BOOL CGame2048::IsWin() const
{
    return m_bWin;
}

BOOL CGame2048::IsFail() const
{
    if (IsFull() == FALSE)
    {
        return FALSE;
    }

    //�����������û�и�������һ���ľ���ʧ����
    for (int i = 1; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (m_numbers[i - 1][j] == m_numbers[i][j])
            {
                return FALSE;
            }

            if (j > 0 && m_numbers[i][j - 1] == m_numbers[i][j])
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

int CGame2048::GetScore() const
{
    return m_score;
}


BOOL CGame2048::HasObstacle( int x1, int y1, int x2, int y2 ) const
{
    int maxY = y1 > y2 ? y1 : y2;
    int maxX = x1 > x2 ? x1 : x2;

    int minY =  y1 > y2 ? y2 : y1;
    int minX =  x1 > x2 ? x2 : x1;

    if (x1 == x2)
    {
        for (int j = minY + 1; j < maxY; j++)
        {
            if (m_numbers[x1][j] != 0)
            {
                return TRUE;
            }
        }
    }
    else if (y1 == y2)
    {
        for (int i = minX + 1; i < maxX; i++)
        {
            if (m_numbers[i][y1] != 0)
            {
                return TRUE;
            }
        }
    }
    else
    {
        ASSERT(FALSE);
    }

    return FALSE;
}
