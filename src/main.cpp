#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

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

	// Fragment Shader with a uniform color for dynamic terrain tinting
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

	// Define Vertices [For Rectangle]
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,		// top right
		 0.5f, -0.5f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		// bottom left
		-0.5f,  0.5f, 0.0f 		// top left
	};
	unsigned int indices[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};

	// Vertex Array Object, Vertex Buffer Object & Element Buffer Object
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// 1. Bind VAO first!
	glBindVertexArray(VAO);

	// 2. Bind and fill VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// Select VBO as the active vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Copy vertex data into the GPU's memory

	// 3. Bind and fill EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										// Select EBO as the active element array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);// Copy index data into the GPU's memory

	// 4. Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // Turn on location 0 so the GPU can use this data

	// Get color uniform location
	int colorLoc = glGetUniformLocation(shaderProgram, "color");

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// Use of Input Function
		processInput(window);

		// Dark editor backdrop (#1A1C23)
		glClearColor(0.10f, 0.11f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Triangle 1: High terrain grass green (#4E9F3D)
		glUniform4f(colorLoc, 0.31f, 0.62f, 0.24f, 1.0f);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

		// Triangle 2: Low terrain meadow green (#3B7A2E)
		glUniform4f(colorLoc, 0.23f, 0.48f, 0.18f, 1.0f);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

		// Wireframe seam lines: Dark slate separator (#1E222A)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0f);
		glUniform4f(colorLoc, 0.12f, 0.13f, 0.16f, 1.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Render frame and handle events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up resources before closing
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}