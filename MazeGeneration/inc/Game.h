/*
	Author: Shane Coates
	Description: Main Game class
*/

#ifndef _GAME_H_
#define	_GAME_H_

struct GLFWwindow;
class GameStateManager;

class Game
{
public:

	//Constructor
	Game();
	//Destructor
	~Game();

	//Main Game loop
	void Run();

	static const unsigned int WINDOW_WIDTH = 1920;
	static const unsigned int WINDOW_HEIGHT = 1080;

protected:
private:
	//Function to return DeltaTime
	double GetDeltaTime();

	//Variables to calculate Delta Time
	double currentFrame;
	double deltaTime;
	double lastFrame;
	
	//Window
	GLFWwindow* m_gameWindow;
	
	//Game State Manager
	GameStateManager* m_gameStateManager;
};

#endif