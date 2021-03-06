#include "RandomPrims.h"
#include <vector>
#include <algorithm>
#include <iterator>

RandomPrims::RandomPrims(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT]) {
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int y = 0; y < MAZE_HEIGHT; y++) {
			m_mazePieces[x][y] = _mazePieces[x][y];
		}
	}
	m_demonstrating = false;

}
RandomPrims::~RandomPrims() {

}
void RandomPrims::Update(double _dt) {
	if (m_demonstrating) {
		for (int i = 0; i < ITERATIONS; i++)
			Demonstrate();
	}
}
void RandomPrims::StartDemonstration() {
	if (!m_demonstrating) {

		for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
			for (unsigned int y = 0; y < MAZE_HEIGHT; y++) {
				m_mazePieces[x][y]->cost = rand() % 10;
			}
		}

		m_open.push_back(m_mazePieces[1][1]);
		m_mazePieces[1][1]->Traversed = true;
		m_mazePieces[1][1]->InOpenList = true;
		m_mazePieces[0][1]->Wall = false;
		m_demonstrating = true;
	}

}
void RandomPrims::Demonstrate() {
	if (m_open.size() > 0) {
		std::vector<MazePiece*> m_tempVector{ std::begin(m_open), std::end(m_open) };
		std::sort(m_tempVector.begin(), m_tempVector.end(), sortCheck());
		m_open.clear();
		std::copy(m_tempVector.begin(), m_tempVector.end(), std::back_inserter(m_open));

		m_open.front()->Wall = false;
		m_open.front()->InOpenList = true;

		glm::vec2 pos = m_open.front()->Position.xz;
		if (pos.x > 1) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x - 1][(int)pos.y];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x - 2][(int)pos.y]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		if (pos.x < MAZE_WIDTH - 2) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x + 1][(int)pos.y];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x + 2][(int)pos.y]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
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
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		if (pos.y < MAZE_HEIGHT - 2) {
			MazePiece* nextPiece = m_mazePieces[(int)pos.x][(int)pos.y + 1];
			if (!nextPiece->Traversed) {
				nextPiece->Traversed = true;
				if (m_mazePieces[(int)pos.x][(int)pos.y + 2]->Wall) {
					if (!(std::find(m_open.begin(), m_open.end(), nextPiece) != m_open.end())) {
						m_open.push_back(nextPiece);
						nextPiece->InOpenList = true;
					}
				}
			}
			else {
				m_open.remove(nextPiece);
				nextPiece->InOpenList = false;
			}
		}
		m_open.front()->InOpenList = false;
		m_open.pop_front();

	}
	else {
		m_demonstrating = false;
		m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 1]->Wall = false;
		m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 2]->Wall = false;
		if (m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 3]->Wall == false
			&& m_mazePieces[MAZE_WIDTH - 3][MAZE_HEIGHT - 2]->Wall == false)
		{

			if (m_mazePieces[MAZE_WIDTH - 3][MAZE_HEIGHT - 3]->Wall == true &&
				m_mazePieces[MAZE_WIDTH - 4][MAZE_HEIGHT - 2]->Wall == true)
			{
				m_mazePieces[MAZE_WIDTH - 3][MAZE_HEIGHT - 2]->Wall = true;
			}
			else
			{
				m_mazePieces[MAZE_WIDTH - 2][MAZE_HEIGHT - 3]->Wall = true;
			}
		}
	
		for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
			for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
				m_mazePieces[x][z]->Traversed = false;
				m_mazePieces[x][z]->InOpenList = false;

			}
		}
	}
}
void RandomPrims::Instant() {
	StartDemonstration();
	while (m_demonstrating) {
		Demonstrate();
	}
}
void RandomPrims::Stop() {
	m_demonstrating = false;
	m_open.clear();
}