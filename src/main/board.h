#ifndef BOARD_H
#define BOARD_H
	
#include "physics.h"
#include "GUIDrawn.h"
#include <thread>
#include <chrono>

class GameComponents
{
	unsigned int MAX_FPS = 120;
	double fpsTime = 1 / double(MAX_FPS);
	modelImporter importer;
	Shaders shaderProgram;
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Camera camera;
	PhysicsModule physics;
	GuiModule GUI;
	
	double previousTime = 0;
	double duration = 0;
	double Clock = 0;
	double current = 0;


public:
	GLFWwindow* window;

	GameComponents();
	
	void inputs();
	void render();
	void end();
};


#endif