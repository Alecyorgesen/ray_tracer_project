#pragma once

#include <cmath>

class Vec3 {
   public:
    double elements[3];
    Vec3() {
        this->elements[0] = 0;
        this->elements[1] = 0;
        this->elements[2] = 0;
    };
    Vec3(double x, double y, double z) {
        this->elements[0] = x;
        this->elements[1] = y;
        this->elements[2] = z;
    };

    double x() const { return elements[0]; }
    double y() const { return elements[1]; }
    double z() const { return elements[2]; }

    friend std::ostream& operator<<(std::ostream& out, const Vec3& v) {
        return out << v.elements[0] << " " << v.elements[1] << " " << v.elements[2];
    }

    Vec3 operator-() const {
        return Vec3(-this->elements[0], -this->elements[1], -this->elements[2]);
    }
    double operator[](int index) const { return elements[index]; }
    double& operator[](int index) { return elements[index]; }
    Vec3 operator+=(const Vec3& otherVector) {
        elements[0] += otherVector.elements[0];
        elements[1] += otherVector.elements[1];
        elements[2] += otherVector.elements[2];
        return *this;
    }
    Vec3 operator+(const Vec3& v) const {
        return Vec3(this->elements[0] + v[0], this->elements[1] + v[1],
                    this->elements[2] + v[2]);
    };
    Vec3 operator-=(const Vec3& otherVector) {
        elements[0] -= otherVector.elements[0];
        elements[1] -= otherVector.elements[1];
        elements[2] -= otherVector.elements[2];
        return *this;
    }
    Vec3 operator-(const Vec3& v) const {
        return Vec3(this->elements[0] - v[0], this->elements[1] - v[1],
                    this->elements[2] - v[2]);
    };
    Vec3 operator*=(double scalar) {
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;
        return *this;
    }
    Vec3 operator*(double scalar) const {
        return Vec3(this->elements[0] * scalar, this->elements[1] * scalar,
                    this->elements[2] * scalar);
    };
    Vec3 operator*=(const Vec3& v) {
        elements[0] *= v[0];
        elements[1] *= v[1];
        elements[2] *= v[2];
        return *this;
    }
    Vec3 operator*(const Vec3& v) const {
        return Vec3(this->elements[0] * v[0], this->elements[1] * v[1],
                    this->elements[2] * v[2]);
    };
    Vec3 operator/=(const double& scalar) {
        elements[0] /= scalar;
        elements[1] /= scalar;
        elements[2] /= scalar;
        return *this;
    }
    Vec3 operator/(double scalar) const {
        return Vec3(this->elements[0] / scalar, this->elements[1] / scalar,
                    this->elements[2] / scalar);
    };

    double dot(const Vec3& v) const {
        return (this->x() * v.x()) + (this->y() * v.y()) + (this->z() * v.z());
    }
    double length_squared() const {
        // It is better not to use the sqrt operator if at all possible for
        // performance reasons. That is why this function exists separately from
        // length.
        return this->dot(*this);
    }
    double length() const { return std::sqrt(length_squared()); }
    Vec3 normalized() const {
        double length = this->length();
        if (length == 0) {
            throw(std::runtime_error("You shouldn't divide by zero, mate."));
        }
        return *this / length;
    }

    Vec3 clamp(double low, double high) {
        double nums[3];
        for (int i = 0; i < 3; i++) {
            nums[i] = elements[i];
            if (elements[i] < low) {
                nums[i] = low;
            } else if (elements[i] > high) {
                nums[i] = high;
            }
        }
        return Vec3(nums[0], nums[1], nums[2]);
    }

    Vec3 cross(Vec3 v) {
        auto result = Vec3(0, 0, 0);
        result[0] = this->y() * v.z() - this->z() * v.y();
        result[1] = this->z() * v.x() - this->x() * v.z();
        result[2] = this->x() * v.y() - this->y() * v.x();
        return result;
    }
};

Vec3 operator*(double scalar, const Vec3& v) { return v * scalar; }
Vec3 operator/(double scalar, const Vec3& v) { return v / scalar; }