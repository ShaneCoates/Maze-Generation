#ifndef _A_STAR_H_
#define	_A_STAR_H_
#include "Maze.h"
#include <map>
#include <vector>
#include "glm.hpp"
static class AStar
{
public:
	static std::vector<glm::vec3> Instant(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT], MazePiece* _start, MazePiece* _end, std::map<MazePiece*, float>& _floodingOpen);

};

#endif