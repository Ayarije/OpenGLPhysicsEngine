#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/windowlib.hpp"
#include "common/controls.hpp"

int main( void )
{
	if (!winlib::initGLFW(&window)) return -1;
	if (!controls::initControls(window)) return -1;

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "shaders/vertexshader.glsl", "shaders/fragmentshader.glsl" );

	// Get a handle for every uniform data
	GLuint ProjMatID = glGetUniformLocation(programID, "ProjMat");
	GLuint ViewMatID = glGetUniformLocation(programID, "ViewMat");
	GLuint ModelMatID = glGetUniformLocation(programID, "ModelMat");
	GLuint LightPositionID = glGetUniformLocation(programID, "LightPosition");

	// Set lightPos
	glm::vec3 lightPos = glm::vec3(4.0f, 4.0f, 4.0f);

	// Setup Camera Matrices
	glm::mat4 ProjMat;
	glm::mat4 ViewMat;
	
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 CubeModel = glm::mat4(1.0f);

	// Get data from .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;

	if (!loadOBJ("models/suzanne/suzanne.obj", vertices, uvs, normals)) return -1;
	GLuint cubeTexID = loadDDS("models/suzanne/suzanne.DDS");

	/* ----[ SETUP BUFFERS CONTENT ]---- */
	GLuint elementbuffer; 
	glGenBuffers(1, &elementbuffer); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    /* ----[ SETUP BUFFERS PARAMETERS ]---- */
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,        // attribute 0
        3,        // size (x,y,z)
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void*)0  // array buffer offset
    );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer); 
	glVertexAttribPointer(
		1,                                
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer( 
		2,                                
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	do {
		// Clear color and depth each frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

		controls::computeMatricesfromInputs(&ProjMat, &ViewMat);

        glUniformMatrix4fv(ProjMatID, 1, GL_FALSE, &ProjMat[0][0]);
		glUniformMatrix4fv(ViewMatID, 1, GL_FALSE, &ViewMat[0][0]);
		glUniform3f(LightPositionID, lightPos.x, lightPos.y, lightPos.z);

        // Draw the model
		glUniformMatrix4fv(ModelMatID, 1, GL_FALSE, &CubeModel[0][0]);
        // Index buffer 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer); 
		
		// Dessine les triangles  
		glDrawElements( 
			GL_TRIANGLES,      // mode 
			indices.size(),    // nombre
			GL_UNSIGNED_INT,   // type 
			(void*)0           // décalage  du tableau de tampons
		);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

	// disable attribute array when shutting down
	glDisableVertexAttribArray(0);
 
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
 
    return 0;
 }

