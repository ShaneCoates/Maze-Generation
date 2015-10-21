#ifndef _RANDOM_PRIMS_H_
#define	_RANDOM_PRIMS_H_
#include <list>
#include "Maze.h"
struct MazePiece;
class RandomPrims {
public:
	RandomPrims(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT]);
	~RandomPrims();

	void Update(double _dt);

	void StartDemonstration();
	void Instant();

	void Stop();

protected:
private:

	void Demonstrate();

	MazePiece* m_mazePieces[MAZE_WIDTH][MAZE_HEIGHT];

	bool m_demonstrating;
	std::list<MazePiece*> m_open;

	struct sortCheck
	{
		inline bool operator() (const MazePiece* struct1, const MazePiece* struct2)
		{
			return (struct1->cost < struct2->cost);
		}
	};

};

#endif