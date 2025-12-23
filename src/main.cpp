#include <stdio.h>
#include <stdlib.h>

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

	GLuint programID = LoadShaders( "vertexshader.glsl", "fragmentshader.glsl" );

	// Get a handle for every view matrices
	GLuint ProjMatID = glGetUniformLocation(programID, "ProjMat");
	GLuint ViewMatID = glGetUniformLocation(programID, "ViewMat");
	GLuint ModelMatID = glGetUniformLocation(programID, "ModelMat");

	// Setup Camera Matrices
	glm::mat4 ProjMat;
	glm::mat4 ViewMat;
	
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 CubeModel = glm::mat4(1.0f);

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f,-1.0f, // triangle 1 : début
		-1.0f,-1.0f, 1.0f, 
		-1.0f, 1.0f, 1.0f, // triangle 1 : fin 
		1.0f, 1.0f,-1.0f, // triangle 2 : début 
		-1.0f,-1.0f,-1.0f, 
		-1.0f, 1.0f,-1.0f, // triangle 2 : fin 
		1.0f,-1.0f, 1.0f, 
		-1.0f,-1.0f,-1.0f, 
		1.0f,-1.0f,-1.0f, 
		1.0f, 1.0f,-1.0f, 
		1.0f,-1.0f,-1.0f, 
		-1.0f,-1.0f,-1.0f, 
		-1.0f,-1.0f,-1.0f, 
		-1.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f,-1.0f, 
		1.0f,-1.0f, 1.0f, 
		-1.0f,-1.0f, 1.0f, 
		-1.0f,-1.0f,-1.0f, 
		-1.0f, 1.0f, 1.0f, 
		-1.0f,-1.0f, 1.0f, 
		1.0f,-1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f,-1.0f,-1.0f, 
		1.0f, 1.0f,-1.0f, 
		1.0f,-1.0f,-1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f,-1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f,-1.0f, 
		-1.0f, 1.0f,-1.0f, 
		1.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f,-1.0f, 
		-1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f, 1.0f, 
		1.0f,-1.0f, 1.0f, // fin du cube
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	const double cap = (double) pow(2, 31) - 1;
	static GLfloat g_color_buffer_data[13 * 3 * 3];
	for (int v = 0; v < 13 * 3; v++) {
		g_color_buffer_data[3*v+0] = (double) random() / cap; 
		g_color_buffer_data[3*v+1] = (double) random() / cap; 
		g_color_buffer_data[3*v+2] = (double) random() / cap;
	}

	GLuint colorbuffer; 
	glGenBuffers(1, &colorbuffer); 
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	static const GLfloat g_uv_buffer_data[] = { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 
		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 
		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f 
	};

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	GLuint textureID = loadBMP_custom("./texture.bmp");

    // Setup vertex attribute 0 (position)
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

	// second tampon d'attributs : couleurs 
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer); 
	glVertexAttribPointer( 
		1,                                // identifiant pour les shaders
		3,                                // taille 
		GL_FLOAT,                         // type 
		GL_FALSE,                         // normalisé ? 
		0,                                // nombre d'octets séparant deux sommets dans le tampon 
		(void*)0                          // décalage du tableau de tampons 
	);

	// troisième tempon d'attributs : uv coordinates
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer); 
	glVertexAttribPointer( 
		2,                                
		2,
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

        // Draw the cube
		glUniformMatrix4fv(ModelMatID, 1, GL_FALSE, &CubeModel[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

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

