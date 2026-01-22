#include "softRectangular.h"

int idx(int x, int y, int z, int N)
{
    if (x<0 || x>=N || y<0 || y>=N || z<0 || z>=N) return -1;
    return x + y * N + z * N * N;
}

SoftRectangular::SoftRectangular(modelImporter* importer, glm::vec3 center, glm::vec3 extents, int resolution, glm::vec3 color = glm::vec3(1.0f,0.0f,0.0f)): SoftBody(importer) 
{
    float difX = 2*extents.x/float(resolution-1);
    float difY = 2*extents.y/float(resolution-1);
    float difZ = 2*extents.z/float(resolution-1);
    float half = (resolution - 1) * 0.5f;

    for (int k = 0; k < resolution; ++k)
        for (int j = 0; j < resolution; ++j)
            for (int i = 0; i < resolution; ++i)
            {
                float fx = (i - half) * difX;
                float fy = (j - half) * difY;
                float fz = (k - half) * difZ;

                glm::vec3 pos(
                    center.x + fx,
                    center.y + fy,
                    center.z + fz
                );

                vertices.emplace_back(
                    std::make_shared<SoftBodyVertex>(
                        importer,
                        pos,
                        glm::vec3(0.0f),
                        color
                    )
                );
            }
    for (int k=0;k<resolution; k++)
        for (int j=0;j<resolution; j++)
            for (int i=0;i<resolution; i++)
            {
                int id = idx(i,j,k,resolution);
                if (i+1 < resolution) 
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i+1,j,k,resolution)], difX));

                if (j+1 < resolution) 
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i,j+1,k,resolution)], difY));

                if (k+1 < resolution) 
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i,j,k+1,resolution)], difZ));
                
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
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i+1,j+1,k,resolution)], difXY,0.7f));

                if (i+1<resolution && k+1<resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i+1,j,k+1,resolution)], difXZ,0.7f));

                if (j+1<resolution && k+1<resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i,j+1,k+1,resolution)], difZY, 0.7f));
            }

    float difXYZ = sqrt(difX*difX + difY*difY + difZ*difZ);

    for (int k=0;k<resolution; k++)
        for (int j=0;j<resolution; j++)
            for (int i=0;i<resolution; i++)
            {
                int id = idx(i,j,k,resolution);
                if (i+1<resolution && j+1< resolution && k+1 < resolution)
                    springs.emplace_back(std::make_shared<SoftBodySpring>(vertices[id], vertices[idx(i+1,j+1,k+1,resolution)], difXYZ,0.5f));
            }

}
