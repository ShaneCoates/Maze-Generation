#include "MazeRenderer.h"
#include "Maze.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "MazeState.h"
MazeRenderer::MazeRenderer(GLFWwindow* _window)
{

	m_window = _window;

	CreateProgram();

}

MazeRenderer::~MazeRenderer()
{

}

void MazeRenderer::Update(double _dt)
{


	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
		CreateProgram();

	if (glfwGetKey(m_window, GLFW_KEY_A))
		m_eye -= m_camRight * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_D))
		m_eye += m_camRight * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_W))
		m_eye += m_camForward * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_S))
		m_eye -= m_camForward * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
		m_eye += m_camUp * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL))
		m_eye -= m_camUp * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_LEFT))
		m_light0Position -= m_camRight * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_RIGHT))
		m_light0Position += m_camRight * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_UP))
		m_light0Position += m_camUp * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
		m_light0Position -= m_camUp * m_moveSpeed;


	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2))
	{
		double mposX, mposY;

		glfwGetCursorPos(m_window, &mposX, &mposY);
		glfwSetCursorPos(m_window, m_windowWidth / 2, m_windowHeight / 2);
		int dx = mposX - m_windowWidth / 2;
		int dy = mposY - m_windowHeight / 2;
		m_theta += dx * 0.01f;
		static const float TWO_PI = 6.28318530718f;
		if (m_theta > TWO_PI) m_theta -= TWO_PI;
		else if (m_theta < 0.0f) m_theta += TWO_PI;

		m_phi += dy * 0.01f;
		if (m_phi > TWO_PI) m_phi -= TWO_PI;
		else if (m_phi < 0.0f) m_phi += TWO_PI;

		float sintheta = sinf(m_theta);
		float costheta = cosf(m_theta);
		float sinphi = sinf(m_phi);
		float cosphi = cosf(m_phi);
		m_camForward = glm::vec3(cosphi * sintheta, -sinphi, cosphi * costheta);
		m_camRight = glm::vec3(costheta, 0.0f, -sintheta);
		m_camUp = glm::normalize(cross(m_camForward, m_camRight));
	}
}

void MazeRenderer::Draw()
{
	
		ImGui::SliderInt("Raymarch steps", &m_rmSteps, 0, 1000);
		ImGui::SliderFloat("Far Clipping plane", &m_zFar, 0, 1000);

		ImGui::ColorEdit4("Sky Color", glm::value_ptr(m_skyColor));

		ImGui::ColorEdit4("Ambient Color", glm::value_ptr(m_ambient));
		ImGui::SliderFloat3("Light Position", glm::value_ptr(m_light0Position), -100.0f, 100.0f);
		ImGui::ColorEdit4("Light Color", glm::value_ptr(m_light0Color));

	
	glUseProgram(m_programID);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_positionAttributeLoc);
	glVertexAttribPointer(m_positionAttributeLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glEnableVertexAttribArray(m_texcoordAttributeLoc);
	glVertexAttribPointer(m_texcoordAttributeLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	
	glUniform2f(m_resolutionLoc, m_windowWidth, m_windowHeight);
	glUniform1f(m_zNearLoc, m_zNear);
	glUniform1f(m_zFarLoc, m_zFar);
	glUniform1f(m_aspectRatioLoc, m_aspectRatio);
	glUniform1f(m_rmEpsilonLoc, m_rmEpsilon);
	glUniform1i(m_rmStepsLoc, m_rmSteps);
	glUniform3fv(m_camUpLoc, 1, value_ptr(m_camUp));
	glUniform3fv(m_camRightLoc, 1, value_ptr(m_camRight));
	glUniform3fv(m_camForwardLoc, 1, value_ptr(m_camForward));
	glUniform3fv(m_eyeLoc, 1, value_ptr(m_eye));
	glUniform1f(m_focalLengthLoc, m_focalLength);
	glUniform4fv(m_skyColorLoc, 1, value_ptr(m_skyColor));
	glUniform4fv(m_ambientLoc, 1, value_ptr(m_ambient));
	glUniform3fv(m_light0PosLoc, 1, value_ptr(m_light0Position));
	glUniform4fv(m_light0ColorLoc, 1, value_ptr(m_light0Color));
	glUniform1i(m_texSizeLoc, MAZE_WIDTH);



	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MazeRenderer::CreateProgram()
{	
	//Create program
	m_programID = ShaderLoader::LoadProgram("res/shaders/basicTexture.vs", "res/shaders/basicTexture.fs");

	//Create Texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//Create buffer objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	m_positionAttributeLoc = glGetAttribLocation(m_programID, "position");
	m_texcoordAttributeLoc = glGetAttribLocation(m_programID, "texcoord");
	m_resolutionLoc = glGetUniformLocation(m_programID, "m_resolution");
	m_camUpLoc = glGetUniformLocation(m_programID, "m_camUp");
	m_camRightLoc = glGetUniformLocation(m_programID, "m_camRight");
	m_camForwardLoc = glGetUniformLocation(m_programID, "m_camForward");
	m_eyeLoc = glGetUniformLocation(m_programID, "m_eye");
	m_focalLengthLoc = glGetUniformLocation(m_programID, "m_focalLength");
	m_zNearLoc = glGetUniformLocation(m_programID, "m_zNear");
	m_zFarLoc = glGetUniformLocation(m_programID, "m_zFar");
	m_aspectRatioLoc = glGetUniformLocation(m_programID, "m_aspectRatio");
	m_rmStepsLoc = glGetUniformLocation(m_programID, "m_rmSteps");
	m_rmEpsilonLoc = glGetUniformLocation(m_programID, "m_rmEpsilon");
	m_skyColorLoc = glGetUniformLocation(m_programID, "m_skyColor");
	m_ambientLoc = glGetUniformLocation(m_programID, "m_ambient");
	m_light0PosLoc = glGetUniformLocation(m_programID, "m_light0Position");
	m_light0ColorLoc = glGetUniformLocation(m_programID, "m_light0Color");
	m_texSizeLoc = glGetUniformLocation(m_programID, "m_texSize");


}

void MazeRenderer::UpdateTexture(Maze* _maze)
{
	GLubyte m_pixelData[MAZE_WIDTH][MAZE_HEIGHT];// [4];
	glm::vec4 currentColor = glm::vec4(0.0f);
	for (int x = 0; x < MAZE_WIDTH; x++)
	{
		for (int y = 0; y < MAZE_HEIGHT; y++)
		{
			currentColor = _maze->GetPieceColor(x, y);
			m_pixelData[x][y] = (GLubyte)(currentColor.r * 255);
			//m_pixelData[x][y][1] = (GLubyte)(currentColor.g * 255);
			//m_pixelData[x][y][2] = (GLubyte)(currentColor.b * 255);
			//m_pixelData[x][y][3] = (GLubyte)(currentColor.a * 255);
		}
	}
	
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, MAZE_WIDTH, MAZE_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, m_pixelData);

}