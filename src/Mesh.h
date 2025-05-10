#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.cpp"

struct Vertex {
	/*
	Structs have a special property in C++ such that their members are consecutive in memory. If we
	were to create a Vertex struct...

		Vertex vertex;
		vertex.Position  = glm::vec3(0.2f, 0.4f, 0.6f);
		vertex.Normal    = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);

	then the memory layout of the members will be equivalent of this:

		[0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f];

	And therefore, we can pass a pointer to even a LIST of Vertex structs as the buffer's data,
	translating perfectly as an arguement for glBufferData:

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	The best part? We can add another attribute to our Vertex, and our rendering code wont break (ie. we
	dont'a have to retype the arguements!):

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));  
	*/
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// Properties
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector <Texture> textures;
	
	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// Methods
	void draw(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};