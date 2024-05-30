#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Player.h"

// FUNCTION PROTOTYPES
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// GLOBAL VARIABLES

// All GameObjects
vector<GameObject*> GameObjects;

// Source and Data directories
string sourceDirStr = "E:/Setup_Windows/SkeletalAnimation/SkeletalAnimation";
string modelDirStr = "E:/Setup_Windows/data";

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLFWwindow *mainWindow = NULL;

// Camera Position
glm::vec3 CameraPosition(0.0f, 4.0f, 5.0f);
glm::vec3 CameraTargetPosition(0.0f, 1.6f, 0.0f);
int ZoomAmount = 0;
glm::vec3 TargetOffset(0.0f, 0.0f, 0.0f);
glm::vec3 CameraOffset(0.0f, -1.0f, 0.0f);

// camera
Camera camera(CameraPosition);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Input Models
Player* player;
string player_vs = sourceDirStr + "/player.vs";
string player_fs = sourceDirStr + "/player.fs";
string playerModelPath = modelDirStr + "/FastRun/FastRun.dae";

int main()
{
    mainWindow = glAllInit();
	glfwSetKeyCallback(mainWindow, key_callback);

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(mainWindow))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.Position = CameraPosition;
		camera.Front = CameraTargetPosition - CameraPosition;

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// render the loaded model
		// Update
		for (GameObject* gameObject : GameObjects)
		{
			gameObject->Update(deltaTime);
			gameObject->Render(projection, view);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

GLFWwindow *glAllInit()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Campus Run", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

	// Initialize Player
	player = new Player();
	GameObjects.push_back((GameObject*)player);
	player->SetShader(player_vs, player_fs);
	player->SetModel(playerModelPath);
    
    return window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	for (GameObject* gameObject : GameObjects)
	{
		gameObject->RunKeyCallback(window, key, scancode, action, mods);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		if (ZoomAmount == 2) return;

		ZoomAmount++;
		CameraTargetPosition += TargetOffset;
		CameraPosition += CameraOffset;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		if (ZoomAmount == 0) return;

		ZoomAmount--;
		CameraTargetPosition -= TargetOffset;
		CameraPosition -= CameraOffset;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
