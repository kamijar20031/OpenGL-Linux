#ifndef SOFT_RECTANGULAR_H_
#define SOFT_RECTANGULAR_H_

#include "softBody.h"

class SoftRectangular : public SoftBody
{
    public:
    SoftRectangular(modelImporter* importer, glm::vec3 center, glm::vec3 extents, int resolution, float stiffness, float damping, glm::vec3 color);
};

#endif