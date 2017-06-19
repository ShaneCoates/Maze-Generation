#ifndef _MAZE_RENDERER_H_
#define _MAZE_RENDERER_H_

#include "glm.hpp"
#include <gl_core_4_4.h>
#include "GLFW\glfw3.h"
#include "Game.h"
class Maze;
class MazeRenderer
{
public:
	MazeRenderer(GLFWwindow* _window);
	~MazeRenderer();

	void Update(double _dt);
	void Draw();

	void UpdateTexture(Maze* _maze);

protected:
private:
	
	void CreateProgram();

	GLFWwindow* m_window;

	unsigned int m_programID;
	unsigned int m_textureID;

	GLuint m_vao;
	GLuint m_vbo;

	GLfloat m_vertices[16] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	GLuint m_positionAttributeLoc;
	GLuint m_texcoordAttributeLoc;
	
	float m_theta = 0.0f;										// Horizontal angle
	float m_phi = 0.0f;											// Vertical angle
	glm::vec3 m_camUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));	// The upward-vector of the image plane
	glm::vec3 m_camRight = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));	// The right-vector of the image plane
	glm::vec3 m_camForward = cross(m_camRight, m_camUp);			// The forward-vector of the image plane
	glm::vec3 m_eye = glm::vec3(0.0f, 0.0f, -2.0f);				// The eye position in the world
	float m_focalLength = 1.67f;								// Distance between eye and image-plane
	float m_zNear = 0.0f;									// Near plane distance from camera
	float m_zFar = 100.0f;								// Far plane distance from camera
	float m_moveSpeed = 0.1f;

	// Raymarch parameters
	int m_rmSteps = 64;
	float m_rmEpsilon = 0.001f;

	glm::vec4 m_skyColor = glm::vec4(0.31f, 0.47f, 0.67f, 1.0f);
	glm::vec4 m_ambient = glm::vec4(0.15, 0.2f, 0.32f, 1.0f);
	glm::vec3 m_light0Position = glm::vec3(0.25f, 2.0f, 0.0f);
	glm::vec4 m_light0Color = glm::vec4(0.67f, 0.87f, 0.93f, 1.0f);

	const int m_windowWidth = Game::WINDOW_WIDTH;
	const int m_windowHeight = Game::WINDOW_HEIGHT;
	float m_aspectRatio = m_windowWidth / (float)m_windowHeight;

	GLuint m_resolutionLoc;
	GLuint m_camUpLoc;
	GLuint m_camRightLoc;
	GLuint m_camForwardLoc;
	GLuint m_eyeLoc;
	GLuint m_focalLengthLoc;
	GLuint m_zNearLoc;
	GLuint m_zFarLoc;
	GLuint m_aspectRatioLoc;
	GLuint m_rmStepsLoc;
	GLuint m_rmEpsilonLoc;
	GLuint m_skyColorLoc;
	GLuint m_ambientLoc;
	GLuint m_light0PosLoc;
	GLuint m_light0ColorLoc;

};

#endif