#include "Maze.h"
#include "aieutilities\Gizmos.h"
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include "RandomTraversal.h"
#include "RandomDepthFirst.h"
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
	m_navigating = false;
	m_navigatingCount = 0;
	m_timer = 0.0f;
	m_randomTraversal = new RandomTraversal(m_mazePieces);
	m_randomDepthFirst = new RandomDepthFirst(m_mazePieces);
	ResetMaze();
}
Maze::~Maze() {

}
void Maze::Update(double _dt) {
	if (m_navigating) {
		Navigate();
	}
	m_randomTraversal->Update(_dt);
	m_randomDepthFirst->Update(_dt);
	m_timer += _dt * 4;
}

void Maze::Draw(Camera* _camera) {
	Gizmos::addAABBFilled(glm::vec3((MAZE_WIDTH * 0.05f) - 0.05f, -0.075f, (MAZE_HEIGHT * 0.05f) - 0.05f), glm::vec3((MAZE_WIDTH * 0.05f) + 0.05f, 0.05f, (MAZE_HEIGHT * 0.05f) + 0.05f), glm::vec4(0, 0.2f, 0, 1));
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			MazePiece* mp = m_mazePieces[x][z];
			glm::vec4 pColour = glm::vec4(1);
			if (mp->InOpenList) {
				pColour = glm::vec4(1, 0, 1, 1);
			}
			else {
				pColour = glm::vec4(0, 0, 0, 1);
			}
			if (mp->Wall) {
				if (m_wireFrame) {
					Gizmos::addAABB(glm::vec3(mp->Position.x * 0.1f, 0, mp->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), pColour);
				}
				else {
					Gizmos::addAABBFilled(glm::vec3(mp->Position.x * 0.1f, 0, mp->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), pColour, pColour);
				}
			}
		}
	}
	if (m_navigatingOpen.size() > 0) {
		typedef std::map<MazePiece*, float>::iterator it_type;
		for (it_type i = m_navigatingOpen.begin(); i != m_navigatingOpen.end(); i++) {
			glm::vec4 colour(1.0f);
			
			colour.r = abs(sinf(i->second * 2));
			colour.g = abs(cosf(i->second));
			colour.b = abs(tanf(i->second * 4));


			//colour.a = ((sin(m_timer) + 1) * 0.25f) + 0.5f;
			if (m_wireFrame) {
				Gizmos::addAABB(glm::vec3(i->first->Position.x * 0.1f, 0, i->first->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), colour);
			}
			else {
				Gizmos::addAABBFilled(glm::vec3(i->first->Position.x * 0.1f, 0, i->first->Position.z * 0.1f), glm::vec3(0.05f, 0.025f, 0.05f), colour, colour);
			}
		}
	}
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

}

void Maze::ResetMaze() {
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			m_mazePieces[x][z]->Position = glm::vec3(x, 0, z);
			m_mazePieces[x][z]->Wall = true;
			m_mazePieces[x][z]->Traversed = false;
			m_mazePieces[x][z]->InOpenList = false;
			m_mazePieces[x][z]->cost = 0;
		}
	}
	m_navigatingCount = 0;
	m_navigatingOpen.clear();
	m_navigating = false;
	Stop();
}

void Maze::Navigate() {
	if (m_navigating = false) {
		m_navigatingCount = 0;
		typedef std::map<MazePiece*, float>::iterator it_type;
		for (it_type i = m_navigatingOpen.begin(); i != m_navigatingOpen.end(); i++) {
			i->first->InOpenList = false;
		}
		m_navigatingOpen.clear();
	}
	m_navigatingCount += .04f;
	m_navigating = true;
	if (m_navigatingOpen.size() == 0) {
		m_navigatingOpen.insert(std::pair<MazePiece*, float>(m_mazePieces[1][1], m_navigatingCount));
	}
	else {
		std::map<MazePiece*, float> tempMap;
		typedef std::map<MazePiece*, float>::iterator it_type;
		for (it_type i = m_navigatingOpen.begin(); i != m_navigatingOpen.end(); i++) {
			if (!North(i->first->Position.xz)->Wall &&
				!North(i->first->Position.xz)->InOpenList) {
				North(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(North(i->first->Position.xz), m_navigatingCount));
			}
			if (!South(i->first->Position.xz)->Wall &&
				!South(i->first->Position.xz)->InOpenList) {
				South(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(South(i->first->Position.xz), m_navigatingCount));
			}
			if (!East(i->first->Position.xz)->Wall &&
				!East(i->first->Position.xz)->InOpenList) {
				East(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(East(i->first->Position.xz), m_navigatingCount));
			}
			if (!West(i->first->Position.xz)->Wall &&
				!West(i->first->Position.xz)->InOpenList) {
				West(i->first->Position.xz)->InOpenList = true;
				tempMap.insert(std::pair<MazePiece*, float>(West(i->first->Position.xz), m_navigatingCount));
			}
		}
		m_navigatingOpen.insert(tempMap.begin(), tempMap.end());
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