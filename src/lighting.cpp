#include <glm/glm.hpp>
#include "lighting.h"

// Direcitonal Light
DirLight::DirLight(const glm::vec3 direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, float shininess)
	: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
}

void DirLight::use(Shader& shader)
{
	shader.setVec3("dirLight.direction", direction);
	shader.setVec3("dirLight.ambient", ambient);
	shader.setVec3("dirLight.diffuse", diffuse);
	shader.setVec3("dirLight.specular", specular);
	shader.setFloat("material.shininess", shininess);
}

// Point Light
PointLight::PointLight()
	: position(glm::vec3(0)), ambient(glm::vec3(0)), diffuse(glm::vec3(0)), specular(glm::vec3(0)), shininess(0.f)
{
}

PointLight::PointLight(glm::vec3 position, float ambient, float diffuse, float specular, float shininess)
	: position(position), ambient(glm::vec3(ambient)), diffuse(glm::vec3(diffuse)), specular(glm::vec3(specular)), shininess(shininess)
{
}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
	: position(position), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
}