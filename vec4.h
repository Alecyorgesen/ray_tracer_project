#pragma once

#include <math.h>

#include <ostream>

class Vec4 {
   public:
    double elements[4];
    Vec4() {
        this->elements[0] = 0;
        this->elements[1] = 0;
        this->elements[2] = 0;
        this->elements[3] = 0;
    };
    Vec4(double x, double y, double z, double h) {
        this->elements[0] = x;
        this->elements[1] = y;
        this->elements[2] = z;
        this->elements[3] = z;
    };
    Vec4(double v[4]) {
        this->elements[0] = v[0];
        this->elements[1] = v[1];
        this->elements[2] = v[2];
        this->elements[3] = v[3];
    };

    Vec4* operator->() { return this; }

    double x() const { return elements[0]; }
    double y() const { return elements[1]; }
    double z() const { return elements[2]; }
    double h() const { return elements[3]; }

    friend std::ostream& operator<<(std::ostream& out, Vec4 elements) {
        return out << elements[0] << " " << elements[1] << " " << elements[2]
                   << " " << elements[3];
    }

    Vec4 operator-() {
        return Vec4(-this->elements[0], -this->elements[1], -this->elements[2],
                    -this->elements[3]);
    }
    double operator[](int index) const { return elements[index]; }
    double& operator[](int index) { return elements[index]; }
    Vec4 operator+=(Vec4 otherVector) {
        elements[0] += otherVector.elements[0];
        elements[1] += otherVector.elements[1];
        elements[2] += otherVector.elements[2];
        elements[3] += otherVector.elements[3];
        return *this;
    }
    Vec4 operator+(Vec4 v) const {
        return Vec4(this->elements[0] + v[0], this->elements[1] + v[1],
                    this->elements[2] + v[2], this->elements[3] + v[3]);
    };
    Vec4 operator-=(Vec4 otherVector) {
        elements[0] -= otherVector.elements[0];
        elements[1] -= otherVector.elements[1];
        elements[2] -= otherVector.elements[2];
        elements[3] -= otherVector.elements[3];
        return *this;
    }
    Vec4 operator-(Vec4 v) const {
        return Vec4(this->elements[0] - v[0], this->elements[1] - v[1],
                    this->elements[2] - v[2], this->elements[3] - v[3]);
    };
    Vec4 operator*=(double scalar) {
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;
        elements[3] *= scalar;
        return *this;
    }
    Vec4 operator*(double scalar) const {
        return Vec4(this->elements[0] * scalar, this->elements[1] * scalar,
                    this->elements[2] * scalar, this->elements[3] * scalar);
    };
    Vec4 operator*=(Vec4 v) {
        elements[0] *= v[0];
        elements[1] *= v[1];
        elements[2] *= v[2];
        elements[3] *= v[3];
        return *this;
    }
    Vec4 operator*(Vec4 v) const {
        return Vec4(this->elements[0] * v[0], this->elements[1] * v[1],
                    this->elements[2] * v[2], this->elements[3] * v[3]);
    };
    Vec4 operator/=(double scalar) {
        elements[0] /= scalar;
        elements[1] /= scalar;
        elements[2] /= scalar;
        elements[3] /= scalar;
        return *this;
    }
    Vec4 operator/(double scalar) const {
        return Vec4(this->elements[0] / scalar, this->elements[1] / scalar,
                    this->elements[2] / scalar, this->elements[3] / scalar);
    };
    double dot(Vec4 v) const {
        return (this->x() * v.x()) + (this->y() * v.y()) + (this->z() * v.z()) +
               (this->h() * v.h());
    }
    double length_squared() const {
        // It is better not to use the sqrt operator if at all possible for
        // performance reasons. That is why this function exists separately from
        // length.
        return this->dot(*this);
    }
    double length() const { return sqrt(length_squared()); }
    Vec4 normalized() const { return *this / this->length(); }
};

Vec4 operator*(double scalar, Vec4 v) { return v * scalar; }
Vec4 operator/(double scalar, Vec4 v) { return v / scalar; }