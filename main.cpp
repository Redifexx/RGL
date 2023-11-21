#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitives.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char	*tempVertShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* tempFragShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int winWidth = 900;
	int winHeight = 675;
	//Creating Window Object
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "RGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Initialize GLAD : GLAD manages function pointers for OGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	 
	//Rendering Window for OGL + Window Resize
	glViewport(0, 0, winWidth, winHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//VBOs - Memory on the GPU allocated towards large batches of vertex data
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//VAOs - Store attribute data for VBOs, makes updating VBOs far simpler
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//EBOs - prevents vertex overlap using indices
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//Bind EBOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);


	//Bind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triforce), triforce, GL_STATIC_DRAW);

	//Vertex Shader Stuff
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &tempVertShader, NULL);
	glCompileShader(vertexShader);

	//Fragment Shader Stuff
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &tempFragShader, NULL);
	glCompileShader(fragmentShader);

	//Shader Program, baby
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//How to interpret the vertex data per vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//glClearColor(0.0f, 0.13f, 0.17f, 1.0f); //Bckground Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//use it
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//draw it
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 9);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//We dont need these anymore, gpu has them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glfwTerminate();

	cout << "HELLO WORLD" << endl;
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cout << width << " " << height << endl;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}