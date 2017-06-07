#ifndef _MAZE_RENDERER_H_
#define _MAZE_RENDERER_H_

#include <gl_core_4_4.h>
class Maze;
class MazeRenderer
{
public:
	MazeRenderer();
	~MazeRenderer();

	void Update(double _dt);
	void Draw();

	void UpdateTexture(Maze* _maze);

protected:
private:
	
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
	


};

#endif