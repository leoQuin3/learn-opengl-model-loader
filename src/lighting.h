#pragma once
#include <glm/glm.hpp>
#include "shader.cpp"

struct DirLight
{
	// Properties
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	// Constructor
	DirLight(const glm::vec3 direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, float shininess);

	// Methods
	void draw(Shader& shader);
};