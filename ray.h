#pragma once

#include <math.h>

#include "vec3.h"

using Point3 = Vec3;

class Ray {
   private:
    Point3 originVar;
    Vec3 directionVar;

   public:
    Ray(const Point3& point, const Vec3& vector) {
        originVar = point;
        directionVar = vector.normalized();
    }

    const Point3& origin() const { return originVar; }
    const Point3& direction() const { return directionVar; }

    Point3 at(double distance) const {
        return originVar + distance * directionVar;
    }
};