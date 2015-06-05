#ifndef _RANDOM_TRAVERSAL_H_
#define	_RANDOM_TRAVERSAL_H_
#include <list>
#include "Maze.h"
struct MazePiece;
class RandomTraversal {
public:
	RandomTraversal(MazePiece* _mazePieces[MAZE_SIZE][MAZE_SIZE]);
	~RandomTraversal();

	void Update(double _dt);
	
	void StartDemonstration();
	void Instant();
protected:
private:

	void Demonstrate();

	MazePiece* m_mazePieces[MAZE_SIZE][MAZE_SIZE];

	bool m_demonstrating;
	std::list<MazePiece*> m_open;

};

#endif