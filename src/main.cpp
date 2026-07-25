// Current Function: Testing

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "terrain/terrain.h"

// Input Function
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Main Function
int main()
{
	// GLFW Initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window Object
	GLFWwindow* window = glfwCreateWindow(800, 600, "Ariadnis", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD Initialize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Viewport
	glViewport(0, 0, 800, 600);

	// Vertex Shader (translates X/Z coordinates to center and tilt the flat terrain)
	const char* vertexShaderSource = "#version 330 core \n"
		"layout (location = 0) in vec3 aPos; \n"
		"void main() \n"
		"{ \n"
		" float posX = aPos.x - 0.5; \n"
		" float posY = aPos.y - (aPos.z * 0.5) + 0.2; \n"
		" gl_Position = vec4(posX, posY, 0.0, 1.0); \n"
		"} \0";

	// Create shader object and shader source code
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER / VERTEX / COMPILATION_FAILED \n" << infoLog << std::endl;
	}

	// Fragment Shader
	const char* fragmentShaderSource = "#version 330 core \n"
		"out vec4 FragColor; \n"
		"uniform vec4 color; \n"
		"void main() \n"
		"{ \n"
		" FragColor = color; \n"
		"} \0";

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER / FRAGMENT / COMPILATION_FAILED \n" << infoLog << std::endl;
	}

	// Link Shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER / PROGRAM / LINKING_FAILED \n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int colorLoc = glGetUniformLocation(shaderProgram, "color");

	// Instantiate and setup Terrain (Width, Depth, CellSize)
	Terrain myTerrain(10, 10, 0.1f);
	myTerrain.generateMesh();
	myTerrain.setupBuffers();

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.10f, 0.11f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// Render as wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5f);

		// Set wireframe color (Green)
		glUniform4f(colorLoc, 0.31f, 0.62f, 0.24f, 1.0f);

		// Draw the full terrain grid
		myTerrain.draw();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up shader program
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}