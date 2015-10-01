#ifndef _WILSONS_ALGORITHM_H_
#define	_WILSONS_ALGORITHM_H_
#include <list>
#include "Maze.h"
struct MazePiece;
class Wilsons {
public:
	Wilsons(MazePiece* _mazePieces[MAZE_SIZE][MAZE_SIZE]);
	~Wilsons();

	void Update(double _dt);

	void StartDemonstration();
	void Instant();
protected:
private:

	void Demonstrate();
	void Build();
	void Cut(MazePiece* _start);
	MazePiece* m_mazePieces[MAZE_SIZE][MAZE_SIZE];

	bool m_demonstrating;
	std::list<MazePiece*> m_open;

	MazePiece* GetRandomNeighbor(MazePiece* _start);

	MazePiece* North(glm::vec2 _pos);
	MazePiece* South(glm::vec2 _pos);
	MazePiece* East(glm::vec2 _pos);
	MazePiece* West(glm::vec2 _pos);
};

#endif