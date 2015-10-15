#include "Wilsons.h"
#include <vector>
#include <algorithm>
#include <iterator>

Wilsons::Wilsons(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT]) {
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int y = 0; y < MAZE_HEIGHT; y++) {
			m_mazePieces[x][y] = _mazePieces[x][y];
		}
	}
	m_demonstrating = false;
}
Wilsons::~Wilsons() {

}
void Wilsons::Update(double _dt) {
	if (m_demonstrating) {
		Demonstrate();
	}
}
void Wilsons::StartDemonstration() {
	if (!m_demonstrating) {
		m_open.push_back(m_mazePieces[1][1]);
		m_mazePieces[1][1]->Traversed = true;
		m_mazePieces[1][1]->InOpenList = true;
		m_mazePieces[0][1]->Wall = false;
		m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 1]->Wall = false;
		m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 2]->Wall = false;
		m_demonstrating = true;
	}

}
void Wilsons::Demonstrate() {
	if (m_open.size() > 0) {
		MazePiece* nextPiece = nullptr;
		glm::vec2 pos = m_open.back()->Position.xz;

		m_open.push_back(GetRandomNeighbor(m_open.back()));
		m_open.back()->InOpenList = true;



	}
	else {
		m_demonstrating = false;
		for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
			for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
				m_mazePieces[x][z]->InOpenList = false;
			}
		}
	}
}
void Wilsons::Instant() {
	StartDemonstration();
	while (m_demonstrating)
		Demonstrate();
}
MazePiece* Wilsons::GetRandomNeighbor(MazePiece* _start) {
	std::vector<MazePiece*> m_neighborList;
	glm::vec2 pos = _start->Position.xz;
	
	switch (rand() % 4)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}

	return nullptr;
}

MazePiece* Wilsons::North(glm::vec2 _pos) {
	if (_pos.y < MAZE_HEIGHT - 1) {
		return m_mazePieces[(int)_pos.x][(int)_pos.y + 1];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* Wilsons::South(glm::vec2 _pos) {
	if (_pos.y > 0) {
		return m_mazePieces[(int)_pos.x][(int)_pos.y - 1];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* Wilsons::East(glm::vec2 _pos) {
	if (_pos.x < MAZE_WIDTH - 1) {
		return m_mazePieces[(int)_pos.x + 1][(int)_pos.y];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* Wilsons::West(glm::vec2 _pos) {
	if (_pos.x > 0) {
		return m_mazePieces[(int)_pos.x - 1][(int)_pos.y];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
void Wilsons::Build() {
	for (auto i = m_open.begin(); i != m_open.end(); ++i) {
		(*i)->InOpenList = false;
		(*i)->Wall = false;
	}
	m_open.clear();
}
void Wilsons::Cut(MazePiece* _start) {
	for (auto i = m_open.begin(); i != m_open.end(); ++i) {
		if ((*i) == _start) {
			m_open.erase(i, m_open.end());
			return;
		}
	}
}