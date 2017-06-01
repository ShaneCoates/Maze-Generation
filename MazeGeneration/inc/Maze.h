#ifndef _MAZE_H_
#define	_MAZE_H_
#define MAZE_WIDTH 100
#define MAZE_HEIGHT 100
#define ITERATIONS 2

#include "glm.hpp"
#include <list>
#include <vector>
#include <map>
class RandomTraversal;
class RandomDepthFirst;
class RandomPrims;
class Wilsons;
class Camera;
struct GLFWwindow;
struct MazePiece {
	glm::vec3 Position;
	bool Wall;
	bool Traversed;
	bool InOpenList;
	unsigned int cost;
	bool operator<(const MazePiece& piece1) const { return cost < piece1.cost; }
	bool operator>(const MazePiece& piece1) const { return cost > piece1.cost; }
	bool operator==(const MazePiece& piece1) const { return cost == piece1.cost; }

	//A Star
	MazePiece* Parent = nullptr;
	bool AStarOpen = false;
	bool AStarPath = false;
};
class Maze {
public:
	Maze();
	~Maze();

	void Update(double _dt);
	void Draw(Camera* _camera);

	void ResetMaze();
	void Stop();

	void Flood();

	void InstantRandomTraversal();
	void DemonstrateRandomTraversal();
	void InstantRandomDepthFirst();
	void DemonstrateRandomDepthFirst();
	void InstantRandomPrims();
	void DemonstrateRandomPrims();
	void InstantWilsons();
	void DemonstrateWilsons();
	void InstantAStar();
	bool m_wireFrame;

	void ClearPathfinding();

	glm::vec3 m_position;

protected:
private:

	double m_timer;

	bool m_flooding;
	std::map<MazePiece*, float> m_floodingOpen;
	float m_floodingCount;
	MazePiece* GetRandomNeighbor(MazePiece* _start);

	MazePiece* North(glm::vec2 _pos);
	MazePiece* South(glm::vec2 _pos);
	MazePiece* East(glm::vec2 _pos);
	MazePiece* West(glm::vec2 _pos);


	MazePiece* m_mazePieces[MAZE_WIDTH][MAZE_HEIGHT];

	RandomTraversal* m_randomTraversal;
	RandomDepthFirst* m_randomDepthFirst;
	RandomPrims* m_randomPrims;
	Wilsons* m_wilsons;
};

#endif