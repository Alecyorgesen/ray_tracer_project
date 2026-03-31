#pragma once

#include <memory>

#include "ray.h"
#include "vec3.h"

class Object {
   public:
    virtual std::unique_ptr<Vec3> getIntersection(const Ray& ray) = 0;
    virtual Vec3 surface_normal(const Vec3& intersection) = 0;
    Vec3 Od;
    Vec3 Os;
    double kd;
    double ks;
    double ka;
    double kgls;
};

class Sphere : public Object {
   public:
    Vec3 center;
    double radius;

    Sphere(Vec3& center, double radius, Vec3& Od, Vec3& Os, double kd, double ks, double ka, double kgls) {
        this->center = center;
        this->radius = radius;
        this->Od = Od;
        this->Os = Os;
        this->kd = kd;
        this->ks = ks;
        this->ka = ka;
        this->kgls = kgls;
    }

    Vec3 surface_normal(const Vec3& point_on_sphere) {
        auto norm = point_on_sphere - center;
        return norm.normalized();
    }

    std::unique_ptr<Vec3> getIntersection(const Ray& ray) {
        auto OC = this->center - ray.origin();
        bool inside_sphere = false;
        if (OC.length() < this->radius) {
            inside_sphere = true;
        }
        auto Tca = ray.direction().dot(OC);
        if (Tca < 0 && !inside_sphere) {
            return nullptr;
        }
        auto Thc = this->radius * this->radius - OC.length_squared() + Tca * Tca;
        if (Thc < 0) {
            return nullptr;
        }
        double t;
        if (inside_sphere) {
            t = Tca + sqrt(Thc);
        } else {
            t = Tca - sqrt(Thc);
        }
        auto intersection = std::make_unique<Vec3>(ray.at(t));
        return intersection;
    }
};