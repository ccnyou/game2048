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

#include <vector>

enum GAME_MODE
{
	GAME_MODE_INVALID,
	GAME_MODE_4X4,
	GAME_MODE_8x8
};

const int GAME_GLOBAL_NUMBER	= 2048;
const int INIT_NUMBER_COUNT		= 2;
const int INVALID_GAME_NUMBER	= -1;

class CGame2048Core
{
	typedef std::vector<int>			IntVector;
	typedef std::vector<IntVector>		IntVector2;

	typedef IntVector::iterator			IntVectorIterator;
	typedef IntVector2::iterator		IntVector2Iterator;

public:
	struct POINT
	{
		int x, y;
	};

public:
	CGame2048Core();
	~CGame2048Core();

	BOOL	Init(GAME_MODE gameMode = GAME_MODE_4X4);
	BOOL	Uninit();

	void	Start();

	void	SlideUp();
	void	SlideDown();
	void	SlideRight();
	void	SlideLeft();

	int			GetAt(const POINT& point) const;
	int			GetAt(int x, int y) const;
	int			GetScore() const;
	int			GetRowCount() const;
	int			GetColumnCount() const;
	GAME_MODE	GetGameMode() const;

	BOOL	IsWin() const;
	BOOL	IsFull() const;
	BOOL	IsFail() const;

private:
	enum DIRECTION
	{
		DIRECTION_INVALID,
		DIRECTION_LEFT,
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_RIGHT
	};

private:

	BOOL	_InitGameRect();
	BOOL	_InitNumbers();
	BOOL	_InitMergeFlags();

	void	_Slide(DIRECTION dirction);

	void	_GenerateNumber(int nCount = 1);
	int		_GetGenerateCount(GAME_MODE gameMode = GAME_MODE_INVALID) const;
	int		_Get2Or4(float fProbabilityOf2) const;
	void	_ClearMergeFlags();
	BOOL	_IsPointFromMerge(const POINT& point) const;
	void	_MarkPointMerge(const POINT& point);
	void	_SetAt(const POINT& point, int nNumber);
	BOOL	_CanPointMove(const POINT& point, CGame2048Core::DIRECTION dirction) const;
	BOOL	_CanPointMerge(const POINT& point, CGame2048Core::DIRECTION dirction) const;
	BOOL	_IsPointValid(const POINT& point) const;
	BOOL	_IsBlank(const POINT& point) const;
	BOOL	_HasAdjacentSame() const;
	void	_GetNextPoint(const POINT& point, POINT* pResultPoint, CGame2048Core::DIRECTION dirction) const;

	BOOL	_GetFirstMovePoint(POINT* pPoint, CGame2048Core::DIRECTION dirction) const;
	BOOL	_GetNextMovePoint(POINT* pPoint, CGame2048Core::DIRECTION dirction) const;

	void	_MovePoint(CGame2048Core::POINT* pMovingPoint, CGame2048Core::DIRECTION direction);
	void	_MergePoint( const CGame2048Core::POINT& movingPoint, CGame2048Core::DIRECTION dirction );
	BOOL	_MovePointToNext(CGame2048Core::POINT* pMovingPoint, CGame2048Core::DIRECTION direction);
	BOOL	_GetNextMovePointOrder( POINT* pPoint ) const;
	BOOL	_GetNextMovePointReverse( POINT* pPoint ) const;
private:
	GAME_MODE		m_gameMode;				// 游戏模式
	BOOL			m_bWin;					// 游戏是否已经赢了
	int				m_nScore;				// 记录游戏当前分数
	IntVector2		m_numbers;				// 记录每个格子里面的数字
	IntVector2		m_mergeFlags;			// 记录这个点是不是刚合并的
	cc::Random		m_random;				// 随机数产生器 
	int				m_nRowCount;
	int				m_nColumnCount;

};

#endif // Game2048_h__
