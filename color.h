//
// Created by Chance Kearl on 2/21/23.
//

#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    /*out << static_cast<int>(256 * clamp(r,0.0,1.0)) << ' '
        << static_cast<int>(256 * clamp(g,0.0,1.0)) << ' '
        << static_cast<int>(256 * clamp(b,0.0,1.0)) << '\n';*/
    out << static_cast<int>(256 * r) << ' '
        << static_cast<int>(256 * g) << ' '
        << static_cast<int>(256 * b) << '\n';
}

#endif //RAYTRACER_COLOR_H
