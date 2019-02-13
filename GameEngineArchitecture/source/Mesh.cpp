#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> p_Vertices, std::vector<unsigned int> p_Indices, std::vector<Texture> p_Textures) {
	this->m_Vertices = p_Vertices;
	this->m_Indices = p_Indices;
	this->m_Textures = p_Textures;

	// Initialise the mesh data within vertex buffers.
	SetupMesh();
}

// Render the mesh with a given shader program.
void Mesh::Render(const unsigned int p_ShaderProgram) {
	// Bind the appropriate textures.
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = m_Textures[i].m_Type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		// Now set the sampler to the correct texture unit.
		glUniform1i(glGetUniformLocation(p_ShaderProgram, (name + number).c_str()), i);
		// and finally bind the texture.
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].m_ID);
	}

	// Set the model component of our shader to the cube model.
	//glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Draw mesh.
	glBindVertexArray(m_VertexArrayObject);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Return to default texture.
	glActiveTexture(GL_TEXTURE0);
}

// Initialises all the buffer arrays.
void Mesh::SetupMesh() {
	// Create buffers/arrays.
	glGenVertexArrays(1, &m_VertexArrayObject);
	glGenBuffers(1, &m_VertexBufferObject);
	glGenBuffers(1, &m_ElementBufferObject);

	glBindVertexArray(m_VertexArrayObject);
	// Load data into vertex buffers.
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

	// Set the memory address as the first vertex struct.
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers.
	// Positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Normals.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal));
	// Texture coordinates.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TextureCoordinates));

	glBindVertexArray(0);
}