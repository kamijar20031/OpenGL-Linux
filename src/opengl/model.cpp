#include "model.h"

glModel::glModel(const char* filer, modelImporter* importer)
{
	meshes = importer->loadModel(filer);
	importer->clear();
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	defColor = glm::vec3(0.0f,0.0f,0.0f);
}

void glModel::draw(Shaders* shader, Camera* camera)
{
	glm::mat4 modelMatrix = getMatrix();
    for (const auto& mesh : *meshes)
        mesh->draw(shader, camera, modelMatrix, defColor);
}

glm::mat4 glModel::getMatrix()
{
	glm::mat4 t = glm::mat4(1.0f);
	glm::mat4 r = glm::mat4(1.0f);
	glm::mat4 s = glm::mat4(1.0f);

	t = glm::translate(t, this->translation);
	r = glm::mat4_cast(this->rotation);
	s = glm::scale(s, this->scale);

	return t * r * s;
}