#include "RandomDepthFirst.h"
#include <vector>
#include <algorithm>
#include <iterator>

RandomDepthFirst::RandomDepthFirst(MazePiece* _mazePieces[MAZE_SIZE][MAZE_SIZE]) {
	for (unsigned int x = 0; x < MAZE_SIZE; x++) {
		for (unsigned int y = 0; y < MAZE_SIZE; y++) {
			m_mazePieces[x][y] = _mazePieces[x][y];
		}
	}
	m_demonstrating = false;

}
RandomDepthFirst::~RandomDepthFirst() {

}
void RandomDepthFirst::Update(double _dt) {
	if (m_demonstrating) {
		Demonstrate();
	}
}
void RandomDepthFirst::StartDemonstration() {
	if (!m_demonstrating) {
		m_open.push_back(m_mazePieces[1][1]);
		m_mazePieces[1][1]->Traversed = true;
		m_mazePieces[1][1]->InOpenList = true;
		m_mazePieces[0][1]->Wall = false;
		m_demonstrating = true;
	}

}
void RandomDepthFirst::Demonstrate() {
	if (m_open.size() > 0) {
		//std::vector<MazePiece*> m_tempVector{ std::begin(m_open), std::end(m_open) };
		//std::sort(m_tempVector.begin(), m_tempVector.end());
		//m_open.clear();
		//std::copy(m_tempVector.begin(), m_tempVector.end(), std::back_inserter(m_open));
		
		MazePiece* current = nullptr;
		unsigned int maxCost = 0;
		std::vector<MazePiece*> m_tempVector;
		for (auto i = m_open.begin(); i != m_open.end(); ++i) {
			if ((*i)->cost > maxCost) {
				m_tempVector.clear();
				maxCost = (*i)->cost;
				m_tempVector.push_back(*i);
			}
			else if ((*i)->cost == maxCost) {
				m_tempVector.push_back(*i);
			}
		}
		int count = m_tempVector.size();
		current = m_tempVector[rand() % count];
		current->Wall = false;
		current->InOpenList = true;
		unsigned int cost = current->cost + 1;
		glm::vec2 pos = current->Position.xz;
		if (pos.x > 1) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x - 1][(int)pos.y];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x - 2][(int)pos.y]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
						nextPiece->cost = cost;
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		if (pos.x < MAZE_SIZE - 2) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x + 1][(int)pos.y];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x + 2][(int)pos.y]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
						nextPiece->cost = cost;
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		if (pos.y > 1) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x][(int)pos.y - 1];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x][(int)pos.y - 2]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
						nextPiece->cost = cost;
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		if (pos.y < MAZE_SIZE - 2) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x][(int)pos.y + 1];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x][(int)pos.y + 2]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
						nextPiece->cost = cost;
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		current->InOpenList = false;
		current->cost = 0;
		m_open.remove(current);

	}
	else {
		m_demonstrating = false;
		m_mazePieces[MAZE_SIZE - 2][MAZE_SIZE - 1]->Wall = false;
		m_mazePieces[MAZE_SIZE - 2][MAZE_SIZE - 2]->Wall = false;
		for (unsigned int x = 0; x < MAZE_SIZE; x++) {
			for (unsigned int z = 0; z < MAZE_SIZE; z++) {
				m_mazePieces[x][z]->Traversed = false;
				m_mazePieces[x][z]->InOpenList = false;
				m_mazePieces[x][z]->cost = 0;
			}
		}
	}
}
void RandomDepthFirst::Instant() {
	StartDemonstration();
	while (m_demonstrating)
		Demonstrate();
}
bool RandomDepthFirst::Compare(const MazePiece& first, const MazePiece& second) {
	return first > second;
}
MazePiece* RandomDepthFirst::GetRandomNeighbor(MazePiece* _start) {
	std::vector<MazePiece*> m_neighborList;
	glm::vec2 pos = _start->Position.xz;
	if (North(pos)->Wall &&
		!North(pos)->Traversed && 
		North(North(pos)->Position.xz)->Wall) {
		m_neighborList.push_back(North(pos));
	}
	if (South(pos)->Wall && 
		!South(pos)->Traversed && 
		South(South(pos)->Position.xz)) {
		m_neighborList.push_back(South(pos));
	}
	if (East(pos)->Wall && 
		!East(pos)->Traversed && 
		East(East(pos)->Position.xz))   {
		m_neighborList.push_back(East(pos));
	}
	if (West(pos)->Wall && 
		!West(pos)->Traversed && 
		West(West(pos)->Position.xz))   {
		m_neighborList.push_back(West(pos));
	}
	if (m_neighborList.size() > 0) {
		int count = m_neighborList.size();
		return m_neighborList[rand() % count];
	}
	return nullptr;
}

MazePiece* RandomDepthFirst::North(glm::vec2 _pos) {
	if (_pos.y < MAZE_SIZE - 1) {
		return m_mazePieces[(int)_pos.x][(int)_pos.y + 1];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* RandomDepthFirst::South(glm::vec2 _pos) {
	if (_pos.y > 0) {
		return m_mazePieces[(int)_pos.x][(int)_pos.y - 1];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* RandomDepthFirst::East(glm::vec2 _pos) {
	if (_pos.x < MAZE_SIZE - 1) {
		return m_mazePieces[(int)_pos.x + 1][(int)_pos.y];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
MazePiece* RandomDepthFirst::West(glm::vec2 _pos) {
	if (_pos.x > 0) {
		return m_mazePieces[(int)_pos.x - 1][(int)_pos.y];
	}
	else {
		return m_mazePieces[(int)_pos.x][(int)_pos.y];
	}
}
