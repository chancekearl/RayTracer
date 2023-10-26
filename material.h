//
// Created by Chance Kearl on 2/22/23.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "constants.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>
#include <utility>
#include <math.h>

struct hit_record;

class material {
public:
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, bool shadow) const = 0;
};

class phong : public material {
public:
    phong(const color& a, const color& l, const color& am, const vec3& ld, const vec3& sc,
            const double& ka, const double& kd, const double& ks, const double& kgls)
        : albedo(a), light_color(l), ambient_color(am), light_dir(ld), spec_color(sc),
        ambient_co(ka), diff_co(kd), spec_co(ks), phong_exp(kgls) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, bool shadow
    ) const override {
        auto scatter_dir = rec.normal + random_unit_vector();
        if (scatter_dir.near_zero())
            scatter_dir = rec.normal;
        scattered = ray(rec.p, scatter_dir/scatter_dir.length());

        color amb = ambient_co * ambient_color * albedo;

        double angle1 = dot(rec.normal, light_dir);
        if (angle1 < 0)
            angle1 = 0;
        color diff = diff_co * light_color * albedo * angle1;

        vec3 R = ((2 * dot(rec.normal, light_dir)) * rec.normal) - light_dir;
        R /= R.length();
        double angle2 = dot(-r_in.direction()/r_in.direction().length(), R);
        if (angle2 < 0)
            angle2 = 0;
        color spec = spec_co * light_color * spec_color * pow(angle2,phong_exp);

        if (shadow) {
            amb = color(0.0,0.0,0.0);
            diff = color(0.0,0.0,0.0);
            spec = color(0.0,0.0,0.0);
        }
        attenuation = amb + diff + spec;
        return true;
    };

public:
    const double ambient_co;
    const double diff_co;
    const double spec_co;
    const double phong_exp;

    color albedo;
    const color light_color;
    const color spec_color;
    const color ambient_color;

    const vec3 light_dir;

    const hittable_list w;
};

class metal : public material {
public:
    metal(const color& a, const vec3& ld, const double& sc, const vec3& sco, const double& gls, const vec3& lc)
        : albedo(a), light_dir(ld), spec_co(sc), spec_color(sco), phong_exp(gls), light_color(lc) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, bool shadow
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
        scattered = ray(rec.p, reflected);



        vec3 R = ((2 * dot(rec.normal, light_dir)) * rec.normal) - light_dir;
        R /= R.length();
        double angle2 = dot(r_in.direction(), R);
        if (angle2 < 0)
            angle2 = 0;
        color spec = spec_co * light_color * spec_color * pow(angle2,phong_exp);

        attenuation = albedo + spec;

        return (dot(scattered.direction(), rec.normal) > 0);
    }
public:
    color albedo;
    vec3 light_dir;
    vec3 light_color;
    double spec_co;
    vec3 spec_color;
    double phong_exp;
};


#endif //RAYTRACER_MATERIAL_H
