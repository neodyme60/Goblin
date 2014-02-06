#ifndef GOBLIN_TRIANGLE_H
#define GOBLIN_TRIANGLE_H

#include "GoblinGeometry.h"

namespace Goblin {
    class ObjMesh;
    class Triangle : public Geometry {
    public:
        Triangle(ObjMesh* parentMesh, size_t index);
        bool intersect(const Ray& ray) const;
        bool intersect(const Ray& ray, float* epsilon, 
            Fragment* fragment) const;
        Vector3 sample(float u1, float u2, Vector3* normal) const;
        float area() const;
        BBox getObjectBound();
    private:
        ObjMesh* mParentMesh;
        size_t mIndex;
    };
}

#endif //GOBLIN_TRIANGLE_H
