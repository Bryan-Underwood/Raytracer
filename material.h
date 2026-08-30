#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"


//abstract class with a virtual scatter() function
//This forces future materials to implement scatter() to funciton
class material {
    public: virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const {
            return false;
        }
};

//albedo = fractional reflectance - how much of the RGB channel is reflected
//lambertian = matte reflection
class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            //get random scatter direction
            auto scatter_direction = rec.normal + random_unit_vector();

            //Catch degenterate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            
            //Send ray in reflected direction
            scattered = ray(rec.p, scatter_direction);
            //save color change
            attenuation = albedo;


            return true;
        }

    private:
        color albedo;
};


class metal : public material {
    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            //Get reflection direction
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            //Apply "fuzz" or slight randomness to scratch up the metal
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            //Send ray in reflected direction
            scattered = ray(rec.p, reflected);
            //Save color change
            attenuation = albedo;
            //Return false if reflection is going into the surface
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};

#endif