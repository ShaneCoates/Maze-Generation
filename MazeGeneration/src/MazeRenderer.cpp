#include "MazeRenderer.h"
#include "glm.hpp"
#include "GLFW\glfw3.h"
#include <gl_core_4_4.h>
#include "Maze.h"
#include "ShaderLoader.h"

MazeRenderer::MazeRenderer()
{

	//Create program
	m_programID = ShaderLoader::LoadProgram("res/shaders/basicTexture.vs", "res/shaders/basicTexture.fs");

	//Create Texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//Create buffer objects
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	m_positionAttributeLoc = glGetAttribLocation(m_programID, "position");
	m_texcoordAttributeLoc = glGetAttribLocation(m_programID, "texcoord");

	
	
}

MazeRenderer::~MazeRenderer()
{

}

void MazeRenderer::Update(double _dt)
{

}

void MazeRenderer::Draw()
{
	glUseProgram(m_programID);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_positionAttributeLoc);
	glVertexAttribPointer(m_positionAttributeLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glEnableVertexAttribArray(m_texcoordAttributeLoc);
	glVertexAttribPointer(m_texcoordAttributeLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MazeRenderer::UpdateTexture(Maze* _maze)
{
	GLubyte m_pixelData[MAZE_WIDTH][MAZE_HEIGHT][4];
	glm::vec4 currentColor = glm::vec4(0.0f);
	for (int x = 0; x < MAZE_WIDTH; x++)
	{
		for (int y = 0; y < MAZE_HEIGHT; y++)
		{
			currentColor = _maze->GetPieceColor(x, y);
			m_pixelData[x][y][0] = (GLubyte)(currentColor.r * 255);
			m_pixelData[x][y][1] = (GLubyte)(currentColor.g * 255);
			m_pixelData[x][y][2] = (GLubyte)(currentColor.b * 255);
			m_pixelData[x][y][3] = (GLubyte)(currentColor.a * 255);
		}
	}
	
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MAZE_WIDTH, MAZE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixelData);

}