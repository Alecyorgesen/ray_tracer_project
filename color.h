#pragma once

#include <iostream>

#include "vec3.h"

using Color = Vec3;

void print_color(std::ostream& out, Color& color) {
    out << int(color.x() * 255.999) << " " << int(color.y() * 255.999) << " "
        << int(color.z() * 255.999) << "\n";
}