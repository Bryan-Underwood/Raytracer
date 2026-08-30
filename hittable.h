#ifndef HITTABLE_H
#define HITTABLE_H

//Tell compiler that class will be defined later
#include "utility.h"

class material;



class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;
        shared_ptr<material> mat;
        

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            //Sets the hit record normal vector.
            //Outward normal is assumed to have a unit length

            front_face = dot(r.direction(), outward_normal) < 0;

            //Normal should always point against the ray
            normal = front_face ? outward_normal : -outward_normal;
        }
};

//Abstract class ensuring all geometry can be deleted and has a unique hit method
class hittable {
    public:
    
        //Virtual destructor ensures derived shape destructors
        //run correctly when deleted via a hittable pointer
        virtual ~hittable() = default;

        //pure virtual function that must be overridden for every shape
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    
};


#endif