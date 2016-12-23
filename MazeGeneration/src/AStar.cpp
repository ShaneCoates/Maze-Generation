#include "AStar.h"
void AStar::Instant(MazePiece* _mazePieces[MAZE_WIDTH][MAZE_HEIGHT], MazePiece* _start, MazePiece* _end, std::map<MazePiece*, float>& _floodingOpen)
{
	std::list<MazePiece*> _openList;
	MazePiece* _currentNode = _start;
	_openList.push_back(_start);

	if (_currentNode == nullptr || _end == nullptr)
		return;


	float _dx2 = _currentNode->Position.x - _end->Position.x;
	float _dy2 = _currentNode->Position.z - _end->Position.z;

	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			//_dx1 = _mazePieces[x][z]->Position.x - _end->Position.x;
			//_dy1 = _mazePieces[x][z]->Position.z - _end->Position.z;
			//_cross = abs(_dx1*_dy2 - _dx2*_dy1);
			_mazePieces[x][z]->cost = 1;
		}
	}

	if (_currentNode == nullptr)
		return;

	while (_currentNode != _end && _openList.size() > 0)
	{
		//create a list of neihbours
		std::list<MazePiece*> _neighbours;
		
		if (_currentNode->Position.z < MAZE_HEIGHT - 1) {
			if (!_mazePieces[(int)_currentNode->Position.x][(int)_currentNode->Position.z + 1]->Wall)
			{
				_neighbours.push_back(_mazePieces[(int)_currentNode->Position.x][(int)_currentNode->Position.z + 1]);
			}
			
		}
		if (_currentNode->Position.z > 0) {
			if (!_mazePieces[(int)_currentNode->Position.x][(int)_currentNode->Position.z - 1]->Wall)
			{
				_neighbours.push_back(_mazePieces[(int)_currentNode->Position.x][(int)_currentNode->Position.z - 1]);
			}
		}
		if (_currentNode->Position.x < MAZE_WIDTH - 1) {
			if (!_mazePieces[(int)_currentNode->Position.x + 1][(int)_currentNode->Position.z]->Wall)
			{
				_neighbours.push_back(_mazePieces[(int)_currentNode->Position.x + 1][(int)_currentNode->Position.z]);
			}
		}
		if (_currentNode->Position.x > 0) {
			if (!_mazePieces[(int)_currentNode->Position.x - 1][(int)_currentNode->Position.z]->Wall)
			{
				_neighbours.push_back(_mazePieces[(int)_currentNode->Position.x - 1][(int)_currentNode->Position.z]);
			}
		}

		//iterate through the neighbours and add them to the open list
		for (auto itr = _neighbours.begin(); itr != _neighbours.end(); itr++)
		{
			//check for a target
			if ((*itr) == _end)
			{
				(*itr)->Parent = _currentNode;
				_currentNode = *itr;
				break;
			}

			//make sure it hasn't already been searched
			if ((*itr)->Parent == nullptr)
			{
				//set each neighbour's 'open' variable to true
				(*itr)->AStarOpen = true;
				//set each neighbour's parent
				(*itr)->Parent = _currentNode;
				//set each neighbour's GScore
				(*itr)->cost = ((*itr)->Parent->cost + (*itr)->cost);
				//push the neighbour onto the open list
				_openList.push_back(*itr);

			}
		}
		if (_currentNode == _end)
			break;

		//set the current node to closed
		_currentNode->AStarOpen = false;
		//remove the parent from the open list
		_openList.pop_front();

		if (_openList.size() <= 0)
			break;
		else
		{
			//make sure the next node on the open list is still actually open
			while (!_openList.front()->AStarOpen)
				_openList.pop_front();

			//sort the open list in terms of GScore
			_openList.sort([](MazePiece* const a, MazePiece* const b)
			{
				return (a->cost + a->cost) < (b->cost + b->cost);
			});

			//set the current node to be the next in the open list
			_currentNode = _openList.front();
			_currentNode->AStarOpen = true;
		}
	}

	if (_currentNode == _end && _currentNode != _start)
	{
		_floodingOpen.insert(std::pair<MazePiece*, float>(_currentNode, (_currentNode->Parent->cost + _currentNode->cost) * 0.01f));
		//outPath.push_back(_currentNode);
		_currentNode->AStarPath = true;
		while (_currentNode->Parent != _start)
		{
			_currentNode->AStarPath = true;

			_floodingOpen.insert(std::pair<MazePiece*, float>(_currentNode, _currentNode->cost * 0.01f));
			_currentNode = _currentNode->Parent;
		}
		_currentNode->AStarPath = true;
		_currentNode->Parent->AStarPath = true;
		_floodingOpen.insert(std::pair<MazePiece*, float>(_currentNode, _currentNode->cost * 0.01f));
		_floodingOpen.insert(std::pair<MazePiece*, float>(_currentNode->Parent, _currentNode->Parent->cost * 0.01f));


	}
	//clean up
	for (unsigned int x = 0; x < MAZE_WIDTH; x++) {
		for (unsigned int z = 0; z < MAZE_HEIGHT; z++) {
			_mazePieces[x][z]->cost = 0;
			_mazePieces[x][z]->AStarOpen = false;
			_mazePieces[x][z]->Parent = nullptr;
		}
	}

	return;
}