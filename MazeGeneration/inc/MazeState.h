/*
Author: Shane Coates
*/

#ifndef _MAZE_GAMESTATE_H_
#define	_MAZE_GAMESTATE_H_
#define MAZE_COUNT 1
#include <gl_core_4_4.h>
#include "GameStateManager.h"
struct GLFWwindow;
class FlyCamera;
class Maze;
class MazeRenderer;


class MazeState : public IGameState
{
public:
	//Constructor inherits from IGameState and calls the Init function
	MazeState(GLFWwindow* _window, GameStateManager* _gameStateManager) : IGameState()
	{
		Init(_window, _gameStateManager);
	}
	//Destructor
	~MazeState();

	//Initialise the gamestate
	void Init(GLFWwindow* _window, GameStateManager* _gameStateManager);

	//Update/Draw functions
	void Update(double _dt);
	void Draw();



private:

	void DrawGUI();

	GLFWwindow* m_window;
	GameStateManager* m_gameStateManager;

	FlyCamera* m_camera;

	Maze* m_maze[MAZE_COUNT];
	int m_currentMaze = 0;

	MazeRenderer* m_mazeRenderers[3];

	double m_timer;

	bool toggleGUIButtonDown = false;
};

#endif