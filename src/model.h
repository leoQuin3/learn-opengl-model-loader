#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "shader.cpp"
#include "mesh.h"

class Model
{
public:
	// Constructor
	Model(std::string const &path);

	// Methods
	void draw(Shader& shader);

private:
	// Properties
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;

	// Methods
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};