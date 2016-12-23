#include "MazeState.h"
#include "FlyCamera.h"
#include "imgui.h"
#include "aieutilities\Gizmos.h"
#include "Maze.h"
#include "GLFW\glfw3.h"
#include "imgui_impl_glfw_gl3.h"

void MazeState::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_gameStateManager = _gameStateManager;
	m_camera = new FlyCamera(5.0f);
	m_camera->SetInputWindow(m_window);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	m_camera->SetLookAt(glm::vec3(MAZE_WIDTH * 0.05f, 15, MAZE_HEIGHT * 0.05f), glm::vec3(MAZE_WIDTH * 0.05f, 0, MAZE_HEIGHT * 0.05f), glm::vec3(0, 0, 1));

	for (int i = 0; i < 1; i++)
	{
		m_maze[i] = new Maze();
		m_maze[i]->m_position.x -= (i * (MAZE_WIDTH * 0.11f));
	}

	Gizmos::create(0, ((MAZE_HEIGHT * MAZE_WIDTH) + 2) * 36);
	m_timer = 30;
}

MazeState::~MazeState() {
	delete m_camera;
	Gizmos::destroy();
}

void MazeState::Update(double _dt) {
	//m_timer += _dt;
	//if ((int)m_timer == 30)
	//{
	//	m_maze[0]->ResetMaze();
	//	//m_maze[1]->ResetMaze();
	//	//m_maze[2]->ResetMaze();
	//	m_maze[0]->DemonstrateRandomTraversal();
	//	//m_maze[1]->DemonstrateRandomPrims();
	//	//m_maze[2]->DemonstrateRandomDepthFirst();
	//	m_timer++;
	//}
	//else if ((int)m_timer == 90)
	//{
	//	m_maze[0]->Flood();
	//	//m_maze[1]->Flood();
	//	//m_maze[2]->Flood();
	//	m_timer++;
	//}
	//else if ((int)m_timer == 120)
	//{
	//	m_maze[0]->ClearPathfinding();
	//	//m_maze[1]->ClearPathfinding();
	//	//m_maze[2]->ClearPathfinding();
	//	m_maze[0]->InstantAStar();
	//	//m_maze[1]->InstantAStar();
	//	//m_maze[2]->InstantAStar();
	//	m_timer = 0;
	//}



	m_camera->Update(_dt);
	for (int i = 0; i < 1; i++)
	{
		m_maze[i]->Update(_dt);
	}
}
void MazeState::Draw() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	Gizmos::clear();
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < 1; i++)
	{
		m_maze[i]->Draw(m_camera);
	}
	

	Gizmos::draw(m_camera->GetProjectionView());
	DrawGUI();
}

void MazeState::DrawGUI() {
	//ImGui::SliderInt("Maze", &m_currentMaze, 0, 2);
	if (ImGui::CollapsingHeader("Maze Generation")) {
			if (ImGui::TreeNode("General")) {
				ImGui::Checkbox("Wireframe", &m_maze[m_currentMaze]->m_wireFrame);
				if (ImGui::Button("Stop")) {
					m_maze[m_currentMaze]->Stop();
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset")) {
					m_maze[m_currentMaze]->ResetMaze();
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Random Traversal")) {
				if (ImGui::Button("Demonstrate")) {
					m_maze[m_currentMaze]->ResetMaze();
					m_maze[m_currentMaze]->DemonstrateRandomTraversal();
				}
				ImGui::SameLine();
				if (ImGui::Button("Instant")) {
					m_maze[m_currentMaze]->ResetMaze();
					m_maze[m_currentMaze]->InstantRandomTraversal();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Random Prims")) {
				if (ImGui::Button("Demonstrate")) {
					m_maze[m_currentMaze]->ResetMaze();
					m_maze[m_currentMaze]->DemonstrateRandomPrims();
				}
				ImGui::SameLine();
				if (ImGui::Button("Instant")) {
					m_maze[m_currentMaze]->ResetMaze();
					m_maze[m_currentMaze]->InstantRandomPrims();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Random Depth First")) {
				if (ImGui::Button("Demonstrate")) {
					m_maze[m_currentMaze]->ResetMaze();
					m_maze[m_currentMaze]->DemonstrateRandomDepthFirst();
				}
				ImGui::SameLine();
				if (ImGui::Button("Instant")) {
					m_maze[m_currentMaze]->ResetMaze();
					m_maze[m_currentMaze]->InstantRandomDepthFirst();
				}
				ImGui::TreePop();
			}
		}
	
		/*
		if (ImGui::TreeNode("Wilsons")) {
			if (ImGui::Button("Demonstrate")) {
				m_maze->ResetMaze();
				m_maze->DemonstrateWilsons();
			}
			ImGui::SameLine();
			if (ImGui::Button("Instant")) {
				m_maze->ResetMaze();
				m_maze->InstantWilsons();
			}
			ImGui::TreePop();
		}
		*/
		if (ImGui::CollapsingHeader("Pathfinding")) {

			if (ImGui::Button("AStar"))
			{
				m_maze[m_currentMaze]->ClearPathfinding();
				m_maze[m_currentMaze]->InstantAStar();
			}
			ImGui::SameLine();
			if (ImGui::Button("Flood"))
			{
				m_maze[m_currentMaze]->ClearPathfinding();
				m_maze[m_currentMaze]->Flood();
			}
			if (ImGui::Button("Clear"))
			{
				m_maze[m_currentMaze]->ClearPathfinding();
			}
		}
}
