#pragma once

#include <memory>
#include <vector>

#include "ray.h"
#include "vec3.h"

class Object {
   public:
    virtual std::unique_ptr<Vec3> getIntersection(const Ray& ray) = 0;
    virtual Vec3 surface_normal(const Vec3& intersection, const Vec3& direction) = 0;
    Vec3 Od;
    Vec3 Os;
    double kd;
    double ks;
    double ka;
    double kgls;
    double refl;
};

class Triangle : public Object {
   public:
    Point3 points[3];
    Vec3 normal;
    double d;

    Triangle(Point3 points[], Vec3& Od, Vec3& Os, double kd, double ks, double ka, double kgls, double refl) {
        this->points[0] = points[0];
        this->points[1] = points[1];
        this->points[2] = points[2];
        this->Od = Od;
        this->Os = Os;
        this->kd = kd;
        this->ks = ks;
        this->ka = ka;
        this->kgls = kgls;
        this->refl = refl;
        auto edge0 = points[0] - points[1];
        auto edge1 = points[2] - points[1];
        auto normal = edge1.cross(edge0).normalized();
        this->normal = normal;
        this->d = -normal.dot(points[0]);
    }

    Vec3 surface_normal(const Vec3& intersection, const Vec3& direction) {
        double dot_result = normal.dot(direction);
        if (dot_result < 0) {
            return this->normal;
        } else {
            return -this->normal;
        }
    }

    std::unique_ptr<Vec3> getIntersection(const Ray& ray) {
        auto vd = normal.dot(ray.direction());

        if (std::abs(vd) < 0.0001) {
            return nullptr;
        }

        auto vo = -(normal.dot(ray.origin()) + d);
        double t = vo / vd;
        if (t < 0) {
            return nullptr;
        }
        auto intersection = ray.origin() + (ray.direction() * t);

        auto edge0 = points[1] - points[0];
        auto edge1 = points[2] - points[1];
        auto edge2 = points[0] - points[2];

        auto c0 = intersection - points[0];
        auto c1 = intersection - points[1];
        auto c2 = intersection - points[2];

        if ((normal.dot(edge0.cross(c0)) > 0 &&
             normal.dot(edge1.cross(c1)) > 0 &&
             normal.dot(edge2.cross(c2)) > 0) ||
            (normal.dot(edge0.cross(c0)) < 0 &&
             normal.dot(edge1.cross(c1)) < 0 &&
             normal.dot(edge2.cross(c2)) < 0)) {
            return std::make_unique<Vec3>(intersection);
        }
        return nullptr;
    }
};

class Sphere : public Object {
   public:
    Vec3 center;
    double radius;

    Sphere(Vec3& center, double radius, Vec3& Od, Vec3& Os, double kd, double ks, double ka, double kgls, double refl) {
        this->center = center;
        this->radius = radius;
        this->Od = Od;
        this->Os = Os;
        this->kd = kd;
        this->ks = ks;
        this->ka = ka;
        this->kgls = kgls;
        this->refl = refl;
    }

    Vec3 surface_normal(const Vec3& point_on_sphere, const Vec3& direction) {
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