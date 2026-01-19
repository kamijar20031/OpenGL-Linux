#include "softRectangular.h"

int idx(int x, int y, int z, int N)
{
    return x + y * N + z * N * N;
}

SoftRectangular::SoftRectangular(modelImporter* importer, glm::vec3 center, glm::vec3 extents, int resolution, float stiffness, float damping, glm::vec3 color = glm::vec3(1.0f,0.0f,0.0f)): SoftBody(importer) 
{
    float difX = 2*extents.x/float(resolution-1);
    float difY = 2*extents.y/float(resolution-1);
    float difZ = 2*extents.z/float(resolution-1);

    float start = -((float)(resolution-1))/2.0f;
    float end = -start;
    for (float k=start; k<=end; k++)
        for (float j=start; j<=end; j++)
            for (float i=start; i<=end; i++)
            {
                glm::vec3 pos = glm::vec3(center.x + i*difX, center.y + j*difY, center.z + k*difZ);
                vertices.emplace_back(std::make_shared<SoftBodyVertex>(importer, pos, glm::vec3(0.0f), color));
            }
    for (int k=0;k<resolution; k++)
        for (int j=0;j<resolution; j++)
            for (int i=0;i<resolution; i++)
            {
                int id = idx(i,j,k,resolution);
                if (i+1 < resolution) 
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i+1,j,k,resolution)].get(), damping, stiffness, difX));

                if (j+1 < resolution) 
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i,j+1,k,resolution)].get(), damping, stiffness, difY));

                if (k+1 < resolution) 
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i,j,k+1,resolution)].get(), damping, stiffness, difZ));
                
            }
    float difXY = sqrt(difX*difX + difY*difY);
    float difXZ = sqrt(difX*difX + difZ*difZ);
    float difZY = sqrt(difZ*difZ + difY*difY);

    for (int k=0;k<resolution; k++)
        for (int j=0;j<resolution; j++)
            for (int i=0;i<resolution; i++)
            {
                int id = idx(i,j,k,resolution);
                if (i+1<resolution && j+1<resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i+1,j+1,k,resolution)].get(), damping, 0.7f*stiffness, difXY));

                if (i+1<resolution && k+1<resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i+1,j,k+1,resolution)].get(), damping, 0.7f*stiffness, difXZ));

                if (j+1<resolution && k+1<resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i,j+1,k+1,resolution)].get(), damping, 0.7f*stiffness, difZY));
            }

    float difXYZ = sqrt(difX*difX + difY*difY + difZ*difZ);

    for (int k=0;k<resolution; k++)
        for (int j=0;j<resolution; j++)
            for (int i=0;i<resolution; i++)
            {
                int id = idx(i,j,k,resolution);
                if (i+1<resolution && j+1< resolution && k+1 < resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id].get(), vertices[idx(i+1,j+1,k+1,resolution)].get(), damping, 0.5f*stiffness, difXYZ));
            }

}
