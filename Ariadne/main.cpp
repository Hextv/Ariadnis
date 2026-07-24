// Personal Comment

// Need to make 2x2 triangle grid

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Ariadne", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
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

	// Vertex Shader
	const char* vertexShaderSource = "#version 330 core \n"
		"layout (location = 0) in vec3 aPos; \n"
		"void main() \n"
		"{ \n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
		"} \0";

	// Create shader object and shader source code
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if compilation was successful
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
		"void main() \n"
		"{ \n"
		" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
		"} \0";

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check Fragment Shader compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER / FRAGMENT / COMPILATION_FAILED \n" << infoLog << std::endl;
	}

	// Link Shaders into Shader Program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check Shader Program linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER / PROGRAM / LINKING_FAILED \n" << infoLog << std::endl;
	}

	// Delete Shaders after linking (no longer needed)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Define Vertices [For Triangle]
	float vertices[]{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// Vertex Array Object & Vertex Buffer Object
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. Bind VAO first!
	glBindVertexArray(VAO);

	// 2. Bind and fill VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// Select VBO as the active vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Copy vertex data into the GPU's memory

	// 3. Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // Turn on location 0 so the GPU can use this data

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// Use of Input Function
		processInput(window);

		// Specify Color
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the object [For Triangle]
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Tell OpenGL to draw 3 vertices as a triangle!

		// Render frame and handle events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up resources before closing
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}