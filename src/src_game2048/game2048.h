///////////////////////////////////////////////////////////////
//
// FileName : Game2048.h
// Creator : �´���
// Date : 2014-7-14 
// Comment : ��Ϸ�߼���ʵ���Լ����ݵ����� 
//
/////////////////////////////////////////////////////////////////

#ifndef Game2048_h__
#define Game2048_h__


class CGame2048
{
public:
    CGame2048();
    ~CGame2048();

    void Init();
    int GetAt(int x, int y) const;
    int GetScore() const;

    void SlideUp();
    void SlideDown();
    void SlideRight();
    void SlideLeft();

    BOOL	IsWin() const;
    BOOL	IsFull() const;
    BOOL	IsFail() const;

private:


    /**
    * @brief �������һ�������֣�80% �ĸ�������һ�� 2��20% �ĸ�������һ�� 4
    */
    void GenerateNumber();

    /**
    * @brief �ж�����֮���Ƿ����ϰ���
    */
    BOOL HasObstacle( int i, int j, int x, int y ) const;

private:
    BOOL		m_bWin;
    int			m_score;
    int			m_numbers[4][4];
    BOOL		m_mergeFlags[4][4];		// ��¼������ǲ��Ǹպϲ���
    cc::Random	m_random;
};

#endif // Game2048_h__
