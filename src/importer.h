#ifndef IMPORTER_H
#define IMPORTER_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include "mesh.h"

class modelImporter
{
	const aiScene* scene;
	Assimp::Importer importer;

	
	std::shared_ptr<std::vector<std::shared_ptr<Mesh>>> meshes;

	std::string fileStr;
	std::string dir;

	std::vector <std::string> loadedNames;
	std::vector <Texture> loadedTex;

	std::vector<std::string> loadedModels;
	std::vector<std::shared_ptr<std::vector<std::shared_ptr<Mesh>>>> loadedMeshes;

	void crawlNodes(aiNode* node);
	Mesh fillMesh(aiMesh* mesh);
	

public:
	modelImporter() {};
	std::shared_ptr<const std::vector<std::shared_ptr<Mesh>>> loadModel(const char* file);
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type, const char* typeName);
	void clear() { meshes = {}; loadedNames = {}; loadedTex = {}; }
};

#endif