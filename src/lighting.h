#pragma once
#include <glm/glm.hpp>
#include "shader.cpp"

// TODO: Maybe use inheritance later, but I need to flesh this out first...
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
	void use(Shader& shader);
};

struct PointLight
{
	// Properties
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	// Constructor
	PointLight();
	PointLight(glm::vec3 position, float ambient, float diffuse, float specular, float shininess);
	PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess);

	// Methods
	void use(Shader& shader);
};