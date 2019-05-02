#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB_IMAGE/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// Simple primitive model renderer (instead of using assimp) 
class Primitives
{

private:
	//All the VAO/VBOs for the models
	unsigned int m_sphereVAO;
	unsigned int m_sphereIndices;

	unsigned int m_cubeVAO;
	unsigned int m_cubeVBO;

	unsigned int m_quadVAO;
	unsigned int m_quadVBO;

	// Prepare the models so they are ready for rendering
	void prepareSphere();
	void prepareCube();
	void prepareQuad();
public:
	// default constructor that prepares the models
	Primitives();

	// Render the model when called (binds the VAOs and unbinds after draw call)
	void renderSphere();
	void renderCube();
	void renderQuad();
};