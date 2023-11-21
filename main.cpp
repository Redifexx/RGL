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

const char* tempFragShaderRed = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

const char* tempFragShaderGreen = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

const char* tempFragShaderBlue = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
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
		std::cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Initialize GLAD : GLAD manages function pointers for OGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	 
	//Rendering Window for OGL + Window Resize
	glViewport(0, 0, winWidth, winHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//VBOs - Memory on the GPU allocated towards large batches of vertex data
	unsigned int VBOs[3];
	glGenBuffers(3, VBOs);

	//VAOs - Store attribute data for VBOs, makes updating VBOs far simpler
	unsigned int VAOs[3];
	glGenVertexArrays(3, VAOs);

	//EBOs - prevents vertex overlap using indices
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//Bind EBOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);


	//Bind VBOs for Triangle 1
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triforceOne), triforceOne, GL_STATIC_DRAW);

	//Interprets the vertex data per vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Triangle 2
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triforceTwo), triforceTwo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Triangle 3
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triforceThree), triforceThree, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	//Vertex Shader Stuff
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &tempVertShader, NULL);
	glCompileShader(vertexShader);

	//Fragment Shader Stuff
	unsigned int fragmentShader[3];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, &tempFragShaderRed, NULL);
	glCompileShader(fragmentShader[0]);

	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, &tempFragShaderGreen, NULL);
	glCompileShader(fragmentShader[1]);

	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, &tempFragShaderBlue, NULL);
	glCompileShader(fragmentShader[2]);

	//Shader Program, baby
	unsigned int shaderProgram[3];
	shaderProgram[0] = glCreateProgram();
	shaderProgram[1] = glCreateProgram();
	shaderProgram[2] = glCreateProgram();
	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);

	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);

	glAttachShader(shaderProgram[2], vertexShader);
	glAttachShader(shaderProgram[2], fragmentShader[2]);

	glLinkProgram(shaderProgram[0]);
	glLinkProgram(shaderProgram[1]);
	glLinkProgram(shaderProgram[2]);

	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//glClearColor(0.0f, 0.13f, 0.17f, 1.0f); //Bckground Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Uses the Shader Program
		glUseProgram(shaderProgram[0]);

		//Bind it then draw it
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram[2]);
		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Some Toggles
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		//EBO stuff
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//We dont need these anymore, gpu has them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[2]);

	glfwTerminate();

	std::cout << "HELLO WORLD" << endl;
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << width << " " << height << endl;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}