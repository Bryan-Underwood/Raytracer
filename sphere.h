#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "utility.h"

//Sphere shape created using center and radius
class sphere : public hittable {
    public:
        //Constructor that defines the spheres radius, center, and material
        sphere(const point3& center, double radius, shared_ptr<material> mat)
         : center(center), radius(std::fmax(0,radius)), mat(mat) {}

        //Hit function for sphere shape
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            //Vector from ray origin to sphere center
            vec3 oc = center - r.origin();

            //Coefficients of the quadratic in t
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;
            auto discriminant = h*h - a*c;

            //Negative discriminant = no ray collision
            if (discriminant < 0) 
                return false;

            auto sqrtd = std::sqrt(discriminant);

            //Find the nearest root that lies in the interval.
            auto root = (h - sqrtd) / a;
            //Check if near root is invalid
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                //Check if far root is invalid
                if (!ray_t.surrounds(root))
                    return false;
            }

            //record the data of the hit - distance, hit point, normal, material
            rec.t = root;
            rec.p = r.at(rec.t);
            //Normalize surface normal
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;

};

#endif