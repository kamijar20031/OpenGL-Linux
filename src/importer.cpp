#include "importer.h"

#include <iostream>

std::shared_ptr<const std::vector<std::shared_ptr<Mesh>>> modelImporter::loadModel(const char* file)
{
    fileStr = std::string(file);
    dir = fileStr.substr(0, fileStr.find_last_of('/'));

    int found = -1;
    for (int i=0;i<loadedModels.size();i++)
    {
        if (dir==loadedModels[i])
        {
            found=i;
            break;
        }
    }

    if (found!=-1)
        return loadedMeshes[found];
    else
    {
        scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
        meshes = std::make_shared<std::vector<std::shared_ptr<Mesh>>>();
        crawlNodes(scene->mRootNode);
        loadedModels.push_back(dir);
        loadedMeshes.push_back(meshes);
        return meshes;
    }
}

void modelImporter::crawlNodes(aiNode* node)
{
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes->push_back(std::make_shared<Mesh>(fillMesh(mesh)));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        crawlNodes(node->mChildren[i]);
    }
}

Mesh modelImporter::fillMesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 temp;

        temp.x = mesh->mVertices[i].x;
        temp.y = mesh->mVertices[i].y;
        temp.z = mesh->mVertices[i].z;
        vertex.position = temp;
        
        temp.x = mesh->mNormals[i].x;
        temp.y = mesh->mNormals[i].y;
        temp.z = mesh->mNormals[i].z;
        vertex.normal = temp;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
        }
        else
            vertex.texUV = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> modelImporter::loadTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
    std::vector<Texture> textures;
    int unit = 0;
    std::string tempDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string name = str.C_Str();
        if (name.find("/")== std::string::npos)
        {
            name = tempDir + name;
        }
        bool check = true;

        for (unsigned int j = 0; j < loadedNames.size(); j++)
        {
            if (loadedNames[j] == str.C_Str())
            {
                textures.push_back(loadedTex[j]);
                check = false;
                break;
            }
        }
        if (check)
        {   
            Texture texture(name.c_str(), typeName, unit++);
            textures.push_back(texture);
            loadedTex.push_back(texture);
            loadedNames.push_back(str.C_Str());
        }
    }
    return textures;
}