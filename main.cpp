#include "constants.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "vec3.h"
#include "material.h"

#include <iostream>
#include <fstream>

color ray_color(const ray& r, const hittable& world, int depth, vec3& light_dir) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    if(world.hit(r,0.001,infinity,rec)) {

        hit_record temp_rec;
        ray shadow = ray(rec.p, light_dir);
        bool inShadow = false;
        if (world.hit(shadow, 0.001,infinity,temp_rec))
            inShadow = true;
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r,rec,attenuation,scattered, inShadow))
            return attenuation * ray_color(scattered,world,depth-1,light_dir);
        return color(0.0,0.0,0.0);
    }
    //vec3 unit_direction = unit_vector(r.direction());
    //auto t = 0.5 * (unit_direction.y() + 1.0);
    //return (1.0 - t) * color(1.0,1.0,1.0) + t * color(0.1,0.3,0.8);
    return color(0.1,0.1,0.1);
}

int main() {
    //image size
    const auto aspect_ratio = 1.0 / 1.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int> (IMAGE_WIDTH/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 10;

    //world
    hittable_list world;

    //camera
    camera cam;

    color light = color(1.0,1.0,1.0);
    color ambient = color(0.0,0.0,0.0);
    vec3 light_dir = vec3(0.0,1.0,1.0);

    //tri 1

    point3 v0 = point3(1.0,-0.7,-0.5);
    point3 v1 = point3(1.0,1.4,-1.5);
    point3 v2 = point3(0.0,-0.7,-2.0);
    double kd = 0.6;
    double ks = 0.3;
    double ka = 0.1;
    color od = color(0.0,0.0,5.0);
    color spec = color(1.0,1.0,1.0);
    double gls = 4.0;

    auto blue = make_shared<phong>(od, light, ambient, light_dir, spec, ka,kd,ks,gls);
    world.add(make_shared<triangle>(v0,v1,v2,blue));

    //tri 2

    v0 = point3(-1.0,-0.7,-0.5);
    v1 = point3(0.0,-0.7,-2.0);
    v2 = point3(-1.0,1.4,-1.5);
    kd = 0.7;
    ks = 0.2;
    ka = 0.1;
    od = color(5.0,5.0,0.0);
    spec = color(1.0,1.0,1.0);
    gls = 4.0;

    auto yellow = make_shared<phong>(od, light, ambient, light_dir, spec, ka,kd,ks,gls);
    world.add(make_shared<triangle>(v0,v1,v2,yellow));

    //sphere
    auto met = make_shared<metal>(color(1.0,1.0,1.0),-light_dir,0.1,color(1.0,1.0,1.0),4.0,color(1.0,1.0,1.0));
    world.add(make_shared<sphere>(point3(0.0,0.0,-1.0),0.5,met));

    //sphere 2
    auto s2mat = make_shared<phong>(color(0.0,1.0,1.0),light,ambient,light_dir,spec,0.1,0.9,0.0,0.0);
    world.add(make_shared<sphere>(point3(0.0,-100.5,-1.0),100.0,s2mat));

    //sphere 3
    auto met2 = make_shared<metal>(color(1.0,0.0,1.0),-light_dir,0.1,color(1.0,1.0,1.0),4.0,color(1.0,1.0,1.0));
    world.add(make_shared<sphere>(point3(0.0,1.0,-1.0),0.5,met2));

    //out file render
    std::ofstream image ("../image.ppm");
    if (image.is_open()) {
        image << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";
        for (int j = IMAGE_HEIGHT - 1; j >= 0; --j) {
            for (int i = 0; i < IMAGE_WIDTH; ++i) {
                color pixel_color(0,0,0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (IMAGE_WIDTH - 1);
                    auto v = (j + random_double()) / (IMAGE_HEIGHT - 1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world, max_depth, light_dir);
                }
                write_color(image, pixel_color, samples_per_pixel);
            }
        }
    } else {
        std::cout << "Unable to open file";
    }
}
