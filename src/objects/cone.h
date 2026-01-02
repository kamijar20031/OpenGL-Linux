#ifndef CONE_H_
#define CONE_H_

#include "gameObject.h"

class Cone : public GameObject
{
    public:
    ~Cone() = default;
    Cone(modelImporter *importer, float radius, float height, glm::vec3 position, glm::vec3 velocity,glm::vec3 color, bool isStatic=false, bool visible=true);
};

#endif