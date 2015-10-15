#ifndef _RANDOM_TRAVERSAL_H_
#define	_RANDOM_TRAVERSAL_H_
#include <list>
#include "Maze.h"
struct MazePiece;
class RandomTraversal {
public:
	RandomTraversal(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT]);
	~RandomTraversal();

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

};

#endif