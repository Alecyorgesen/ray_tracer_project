#pragma once

#include <math.h>

#include "vec3.h"

using Point3 = Vec3;
using Color = Vec3;

class Light {
   public:
    Color color;
    virtual Vec3 getDirectionToLight(Point3 intersection) = 0;
};

class DirectionLight : public Light {
   public:
    Vec3 directionToLight;

    DirectionLight(Vec3 directionToLight, Color color) {
        this->directionToLight = directionToLight.normalized();
        this->color = color;
    }

    Vec3 getDirectionToLight(Point3 intersection) { return directionToLight; }
};

class PointLight : public Light {
   public:
    Point3 position;

    PointLight(const Point3& position, Color color) {
        this->position = position;
        this->color = color;
    }

    Vec3 getDirectionToLight(Point3 intersection) {
        auto directionToLight = position - intersection;
        return directionToLight.normalized();
    }
};