#include "lighting.h"

DirLight::DirLight(const glm::vec3 direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, float shininess)
	: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
{
}

void DirLight::draw(Shader& shader)
{
	shader.setVec3("dirLight.direction", direction);
	shader.setVec3("dirLight.ambient", ambient);
	shader.setVec3("dirLight.diffuse", diffuse);
	shader.setVec3("dirLight.specular", specular);
	shader.setFloat("material.shininess", shininess);
}


