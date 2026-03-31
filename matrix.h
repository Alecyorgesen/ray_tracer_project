#pragma once

#include "vec4.h"

class Matrix4 {
   private:
    Vec4 rows[4];

   public:
    Matrix4() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i == j) {
                    this->rows[i][j] = 1;
                } else {
                    this->rows[i][j] = 0;
                }
            }
        }
    }

    Matrix4(Vec4 values[4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                this->rows[i][j] = values[i][j];
            }
        }
    }

    Matrix4(double values[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                this->rows[i][j] = values[i][j];
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& out, Matrix4& mat) {
        for (int i = 0; i < 3; i++) {
            out << mat.rows[i] << " " << '\n';
        }
        out << mat.rows[3] << '\n';
        return out;
    }

    Matrix4 transpose() {
        auto m = Matrix4();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m.set(i, j, this->at(j, i));
            }
        }
        return m;
    }

    Matrix4 t() { return this->transpose(); }

    Vec4 operator*(Vec4 v) {
        double values[4];
        for (int i = 0; i < 4; i++) {
            values[i] = v.dot(this->rows[i]);
        }
        return Vec4(values);
    }

    Matrix4 operator*(Matrix4 v) {
        Vec4 values[4];
        auto vt = v.t();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                values[i][j] = this->rows[i]->dot(vt.rows[j]);
            }
        }
        return Matrix4(values);
    }

    double at(int x, int y) { return this->rows[x][y]; }
    void set(int x, int y, double value) { this->rows[x][y] = value; }
};

// get_rotation_matrix()

Matrix4 get_scale_matrix(double x, double y, double z) {
    auto scale_matrix = Matrix4();
    scale_matrix.set(0, 0, x);
    scale_matrix.set(1, 1, y);
    scale_matrix.set(2, 2, z);
    return scale_matrix;
}

Matrix4 get_translation_matrix(double x, double y, double z) {
    auto translation_matrix = Matrix4();
    translation_matrix.set(0, 3, x);
    translation_matrix.set(1, 3, y);
    translation_matrix.set(1, 3, z);
    return translation_matrix;
}