#include "board.h"

unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 800;

unsigned int GameComponents::MAX_FPS =60;
float GameComponents::fpsTime = 1 / float(MAX_FPS);

void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WINDOW_HEIGHT=height;
	WINDOW_WIDTH=width;
}

GameComponents::GameComponents()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW init fault!";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gladLoadGL();
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	shaderProgram = Shaders("resources/shaders/default.vert", "resources/shaders/default.frag");
	shaderProgram.useShader();
	glUniform4f(glGetUniformLocation(shaderProgram.getID(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	PhysicsModule::init(&importer, &shaderProgram);
	camera = Camera(glm::vec3(0.0f, 0.2f, 0.5f));
	GUI = GuiModule(window);
	// objects.push_back(new ingameObject("resources/models/sbunny/scene.gltf", &importer));
}

void GameComponents::inputs()
{
	camera.inputs(window, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void GameComponents::render()
{

	glfwPollEvents();
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.updateMat(45.0f, 0.1f, 100.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

	PhysicsModule::process((float)duration, &shaderProgram, &camera);
	if (PhysicsModule::guiEnabled)
		GUI.draw();
	this->inputs();
	// Jak na razie nie jest potrzebny ale moze sie przydac w przyszlosci
	Clock += GameComponents::fpsTime;
	
	glfwSwapBuffers(window);
	double now = glfwGetTime();
	duration = now - previousTime;
	previousTime = now;

}

void GameComponents::end()
{
	shaderProgram.deleteShader();
	glfwDestroyWindow(window);
	glfwTerminate();
}