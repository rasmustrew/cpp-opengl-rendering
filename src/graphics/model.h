#pragma once

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>



class Model {
public:
	Model(const std::string& path, std::vector<Texture>& textureCache);
	void Draw(Shader& shader);
private:
	// model data
	std::vector<Mesh> Meshes;
	std::string Directory;
	std::vector<Texture>& TextureCache;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

