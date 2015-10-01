#ifndef _RANDOM_DEPTH_FIRST_H_
#define	_RANDOM_DEPTH_FIRST_H_
#include <map>
#include "Maze.h"
struct MazePiece;
class RandomDepthFirst {
public:
	RandomDepthFirst(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT]);
	~RandomDepthFirst();

	void Update(double _dt);

	void StartDemonstration();
	void Instant();
protected:
private:

	void Demonstrate();

	MazePiece* m_mazePieces[MAZE_WIDTH][MAZE_HEIGHT];

	bool m_demonstrating;
	std::list<MazePiece*> m_open;

	bool Compare(const MazePiece& first, const MazePiece& second);


	MazePiece* GetRandomNeighbor(MazePiece* _start);

	MazePiece* North(glm::vec2 _pos);
	MazePiece* South(glm::vec2 _pos);
	MazePiece* East(glm::vec2 _pos);
	MazePiece* West(glm::vec2 _pos);
};

#endif