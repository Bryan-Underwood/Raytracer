#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "utility.h"


#include <vector>




class hittable_list : public hittable {
    public:
    //List of pointers which are storing object data
    std::vector<shared_ptr<hittable>> objects;

    //Constructors
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    //Clearing object data
    void clear() {objects.clear(); }

    //Adding a shape to the vector
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    //checks whether a ray has hit an object
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        //Store hit info in a temp object and initialize the two tracked variables
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        //Loop through every object in the vector and check if the ray has hit it
        for (const auto& object : objects) {
            //If this object is hit and closer than any previous hit, update the closest record
            if(object ->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        
        return hit_anything;
    }
};

#endif