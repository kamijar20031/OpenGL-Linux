#ifndef RECTANGULAR_H_
#define RECTANGULAR_H__
#include "gameObject.h"

class Rectangular : public GameObject
{
    public:
    ~Rectangular() = default;
    Rectangular(modelImporter *importer, glm::vec3 size, glm::vec3 position, glm::vec3 velocity,glm::vec3 color, bool isStatic=false, bool visible=true);
    virtual bool collides() {return true;}
};



#endif
