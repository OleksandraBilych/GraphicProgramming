#include <stdio.h>
#include <string.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

// Windows dimentions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader, uniformXMove;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.6f;
float triIncrement = 0.0005f;

// Vertex Shader
static const char* vShader = "                                 \n\
#version 330                                                   \n\
                                                               \n\
layout (location = 0 ) in vec3 pos;                            \n\
                                                               \n\
uniform float xMove;                                           \n\
                                                               \n\
void main()                                                    \n\
{                                                              \n\
    gl_Position = vec4(0.4*pos.x+xMove, 0.4*pos.y, pos.z, 1.0);\n\
}";

//Fragment shader
static const char* fShader = "                           \n\
#version 330                                             \n\
                                                         \n\
out vec4 colour;                                         \n\
                                                         \n\
void main()                                              \n\
{                                                        \n\
	colour = vec4(1.0, 0.0, 0.0,1.0);                    \n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader( GLuint theProgram, const char* shaderCode, GLenum shaderType )
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (result == 0)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), nullptr, eLog);
    	printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
    	return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (shader == 0)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (result == 0)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
    	printf("Error linking program: '%s'\n", eLog);
    	return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (result == 0)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
    	printf("Error validating program: '%s'\n", eLog);
    	return;
    }

    // get location of uniform variable
    uniformXMove = glGetUniformLocation(shader, "xMove");

}

int main()
{
	// Inititalization GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Main Window", nullptr, nullptr);

	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set Contex for GLAD to use
	glfwMakeContextCurrent(mainWindow);

	// Glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Loop untip window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
    	// Get + Handle user input events
    	glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (fabs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }

    	// Clear window
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT);

    	glUseProgram(shader);

        glUniform1f(uniformXMove, triOffset);

    	glBindVertexArray(VAO);
    	glDrawArrays(GL_TRIANGLES, 0, 3);
    	glBindVertexArray(0);

    	glUseProgram(0);

    	glfwSwapBuffers(mainWindow);
    }

	return 0;
}
