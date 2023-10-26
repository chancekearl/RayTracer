//
// Created by Chance Kearl on 3/6/23.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
public:
    triangle() {}

    triangle(point3 one, point3 two, point3 three, shared_ptr<material> m)
        : v0(one), v1(two), v2(three), mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;



public:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> mat_ptr;
};

bool triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    const float EPSILON = 0.0000001;
    vec3 edge1, edge2,h,s,q;
    float a,f,u,v;
    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = cross(r.direction(),edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;
    f = 1.0/a;
    s = r.origin() - v0;
    u = f * dot(s,h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = cross(s,edge1);
    v = f * dot(r.direction(),q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    float t = f * dot(edge2,q);

    if (t < t_min || t > t_max) {
        return false;
    }

    if (t > EPSILON) {
        rec.p = r.at(t);
        rec.mat_ptr = mat_ptr;
        rec.set_face_normal(r, cross(edge1,edge2));
        rec.t = t;
        return true;
    } else {
        return false;
    }
}



#endif //RAYTRACER_TRIANGLE_H
