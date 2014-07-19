///////////////////////////////////////////////////////////////
//
// FileName : Game2048.h
// Creator : 陈聪宁
// Date : 2014-7-14 
// Comment : 游戏逻辑的实现以及数据的生成 
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
	* @brief 随机生成一个新数字，80% 的概率生成一个 2，20% 的概率生成一个 4
	*/
	void GenerateNumber();

	/**
	* @brief 判断两点之间是否有障碍物
	*/
	BOOL HasObstacle( int i, int j, int x, int y ) const;

private:
	BOOL		m_bWin;
	int			m_score;
	int			m_numbers[4][4];
	BOOL		m_mergeFlags[4][4];		// 记录这个点是不是刚合并的
	cc::Random	m_random;
};

#endif // Game2048_h__
