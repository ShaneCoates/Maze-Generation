#include "Maze.h"
#include "aieutilities\Gizmos.h"
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include "MazeRenderer.h"
#include "RandomTraversal.h"
#include "RandomDepthFirst.h"
#include "RandomPrims.h"
#include "Wilsons.h"
#include "AStar.h"

Maze::Maze() {
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			m_mazePieces[x][z] = new MazePiece();
			m_mazePieces[x][z]->Position = glm::vec3(x, 0, z);
			m_mazePieces[x][z]->Wall = true;
			m_mazePieces[x][z]->Traversed = false;
			m_mazePieces[x][z]->InOpenList = false;

		}
	}
	m_wireFrame = false;
	m_flooding = false;
	m_floodingCount = 0;
	m_timer = 0.0f;
	m_randomTraversal = new RandomTraversal(m_mazePieces);
	m_randomDepthFirst = new RandomDepthFirst(m_mazePieces);
	m_randomPrims = new RandomPrims(m_mazePieces);
	m_wilsons = new Wilsons(m_mazePieces);
	
	m_position = glm::vec3(0);
	ResetMaze();

}
Maze::~Maze() {

}
void Maze::Update(double _dt) {
	if (m_flooding) {
		for (int i = 0; i < ITERATIONS; i++)
		{
			if (m_flooding)
				Flood();
		}
	}
	m_randomTraversal->Update(_dt);
	m_randomDepthFirst->Update(_dt);
	m_randomPrims->Update(_dt);
	m_wilsons->Update(_dt);
	m_timer += _dt * 4;
}

void Maze::Draw(Camera* _camera) {
	Gizmos::addAABBFilled(m_position + glm::vec3((MAZE_WIDTH * 0.05f) - 0.05f, -0.075f, (MAZE_HEIGHT * 0.05f) - 0.05f), glm::vec3((MAZE_WIDTH * 0.05f) + 0.05f, 0.05f, (MAZE_HEIGHT * 0.05f) + 0.05f), glm::vec4(1, 1, 1, 1));
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			MazePiece* mp = m_mazePieces[x][z];
			glm::vec4 pColour = glm::vec4(1);
			if (mp->InOpenList) {
				pColour = glm::vec4(1, 0, 0, 1);
			}
			else {
				pColour = glm::vec4(0, 0, 0, 1);
			}
			if (mp->Wall) {
				if (m_wireFrame) {
					Gizmos::addAABB(m_position + glm::vec3(mp->Position.x * 0.1f, 0, mp->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), pColour);
				}
				else {
					Gizmos::addAABBFilled(m_position + glm::vec3(mp->Position.x * 0.1f, 0, mp->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), pColour, pColour);
				}
			}
		}
	}
	if (m_floodingOpen.size() > 0) {
		typedef std::map<MazePiece*, float>::iterator it_type;
		for (it_type i = m_floodingOpen.begin(); i != m_floodingOpen.end(); i++) {
			glm::vec4 colour(1.0f);
			
			colour.r = sinf(i->second * 3 + 0) * 0.5f + 0.5f;
			colour.g = sinf(i->second * 3 + 2) * 0.5f + 0.5f;
			colour.b = sinf(i->second * 3 + 4) * 0.5f + 0.5f;


			//colour.r = (sinf(i->second * 5) + 1) * 0.5f;
			//colour.g = (cosf(i->second * 2) + 1) * 0.5f;
			//colour.b = (sinf(i->second * 3) + 1) * 0.5f;
			//colour = glm::normalize(colour);
			colour.a = 1.0f;
			//colour.a = ((sin(m_timer) + 1) * 0.25f) + 0.5f;
			if (m_wireFrame) {
				Gizmos::addAABB(m_position + glm::vec3(i->first->Position.x * 0.1f, 0, i->first->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), colour);
			}
			else {
				Gizmos::addAABBFilled(m_position + glm::vec3(i->first->Position.x * 0.1f, 0, i->first->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), colour, colour);
			}
		}
	}
}

void Maze::ClearPathfinding()
{
	m_floodingOpen.clear();
}

MazePiece* Maze::GetRandomNeighbor(MazePiece* _start) {
	std::vector<MazePiece*> m_neighborList;
	glm::vec2 pos = _start->Position.xz;
	if (North(pos)->Wall) m_neighborList.push_back(North(pos));
	if (South(pos)->Wall) m_neighborList.push_back(South(pos));
	if (East(pos)->Wall) m_neighborList.push_back(East(pos));
	if (West(pos)->Wall) m_neighborList.push_back(West(pos));

	int count = m_neighborList.size();
	return m_neighborList[rand() % count];
}

MazePiece* Maze::North(glm::vec2 _pos) {
	if (_pos.y < MAZE_HEIGHT - 1) {
		return m_mazePieces[(int)_pos.x][(int)_pos.y + 1];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* Maze::South(glm::vec2 _pos) {
	if (_pos.y > 0) {
		return m_mazePieces[(int)_pos.x][(int)_pos.y - 1];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* Maze::East(glm::vec2 _pos) {
	if (_pos.x < MAZE_WIDTH - 1) {
		return m_mazePieces[(int)_pos.x + 1][(int)_pos.y];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* Maze::West(glm::vec2 _pos) {
	if (_pos.x > 0) {
		return m_mazePieces[(int)_pos.x - 1][(int)_pos.y];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}


void Maze::Stop() {
	m_randomTraversal->Stop();
	m_randomDepthFirst->Stop();
}

void Maze::ResetMaze() {
	Stop();

	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			m_mazePieces[x][z]->Position = glm::vec3(x, 0, z);
			m_mazePieces[x][z]->Wall = true;
			m_mazePieces[x][z]->Traversed = false;
			m_mazePieces[x][z]->InOpenList = false;
			m_mazePieces[x][z]->cost = 0;
		}
	}
	m_floodingCount = 0;
	m_floodingOpen.clear();
	m_flooding = false;
}

void Maze::Flood() {
	if (m_flooding == false) {
		m_floodingCount = 0.01f;
		typedef std::map<MazePiece*, float>::iterator it_type;
		for (it_type i = m_floodingOpen.begin(); i != m_floodingOpen.end(); i++) {
			i->first->InOpenList = false;
			i->first->cost = 0;
		}
		m_floodingOpen.clear();
	}
	m_floodingCount += .01f;
	m_flooding = true;
	if (m_floodingOpen.size() == 0) {
		m_floodingOpen.insert(std::pair<MazePiece*, float>(m_mazePieces[1][1], m_floodingCount));
	}
	else {
		std::map<MazePiece*, float> tempMap;
		typedef std::map<MazePiece*, float>::iterator it_type;
		for (it_type i = m_floodingOpen.begin(); i != m_floodingOpen.end(); i++) {
			if (!North(i->first->Position.xz)->Wall &&
				!North(i->first->Position.xz)->InOpenList) {
				North(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(North(i->first->Position.xz), m_floodingCount));
			}
			if (!South(i->first->Position.xz)->Wall &&
				!South(i->first->Position.xz)->InOpenList) {
				South(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(South(i->first->Position.xz), m_floodingCount));
			}
			if (!East(i->first->Position.xz)->Wall &&
				!East(i->first->Position.xz)->InOpenList) {
				East(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(East(i->first->Position.xz), m_floodingCount));
			}
			if (!West(i->first->Position.xz)->Wall &&
				!West(i->first->Position.xz)->InOpenList) {
				West(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(West(i->first->Position.xz), m_floodingCount));
			}
		}
		m_floodingOpen.insert(tempMap.begin(), tempMap.end());
	}
	if (m_floodingOpen.size() > MAZE_WIDTH) {
		unsigned int count = 0;
		for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
			for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
				if (!m_mazePieces[x][z]->Wall) {
					++count;
				}
			}
		}
		if (m_floodingOpen.size() >= count) {
			m_flooding = false;
		}
	}
}
void Maze::InstantRandomTraversal() {
	m_randomTraversal->Instant();
}
void Maze::DemonstrateRandomTraversal() {
	m_randomTraversal->StartDemonstration();
}
void Maze::InstantRandomDepthFirst() {
	m_randomDepthFirst->Instant();
}
void Maze::DemonstrateRandomDepthFirst() {
	m_randomDepthFirst->StartDemonstration();
}
void Maze::InstantRandomPrims() {
	m_randomPrims->Instant();
}
void Maze::DemonstrateRandomPrims() {
	m_randomPrims->StartDemonstration();
}
void Maze::InstantWilsons() {
	m_wilsons->Instant();
}
void Maze::DemonstrateWilsons() {
	m_wilsons->StartDemonstration();
}
void Maze::InstantAStar()
{
	AStar::Instant(m_mazePieces, m_mazePieces[0][1], m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 1], m_floodingOpen);
}
glm::vec4 Maze::GetPieceColor(unsigned int _x, unsigned int _y)
{
	MazePiece* mp = m_mazePieces[_x][_y];
	glm::vec4 pColour = glm::vec4(1);
	if (mp->Wall)
		pColour = glm::vec4(0, 0, 0, 1);
	if(mp->InOpenList)
		pColour = glm::vec4(1, 0, 0, 1);
	return pColour;
}