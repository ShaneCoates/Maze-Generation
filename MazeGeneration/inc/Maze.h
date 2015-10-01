#ifndef _MAZE_H_
#define	_MAZE_H_
#define MAZE_SIZE 30
#include "glm.hpp"
#include <list>
#include <vector>
#include <map>
class RandomTraversal;
class RandomDepthFirst;
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
	void InstantWilsons();
	void DemonstrateWilsons();


	bool m_wireFrame;

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


	MazePiece* m_mazePieces[MAZE_SIZE][MAZE_SIZE];

	RandomTraversal* m_randomTraversal;
	RandomDepthFirst* m_randomDepthFirst;
	Wilsons* m_wilsons;
};

#endif