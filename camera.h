#ifndef CAMERA_H
#define CAMERA_H
#include "hittable.h"
#include "material.h" 

#include <fstream>

class camera {
    public:
        //Image
        double aspect_ratio = 1.;
        int image_width = 100;
        int samples_per_pixel = 10;
        int max_depth = 10;

            void render(const hittable& world) {
                initialize();

                //Render
                //PPM Viewer parsing rules
                std::ofstream out("image.ppm");
                out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

                //Rendering Loop
                for (int j = 0; j < image_height; j++) {
                    //Progress Indicator
                    std::clog << "\rScanlines Remaining: " << (image_height - j) << ' ' << std::flush; 
                    for (int i = 0; i < image_width; i++) {
                        
                        //Sampling loop for antialiasing
                        color pixel_color(0,0,0);
                        for (int sample = 0; sample < samples_per_pixel; sample++) {

                            //For each sample add ray color to pixel color
                            ray r = get_ray(i,j);
                            pixel_color += ray_color(r, max_depth, world);
                        }
                        //Average out the color and set final pixel color
                        write_color(out, pixel_samples_scale * pixel_color);
                    }   
                }

            std::clog << "\rDone.                        \n";
    }

    private:
            int image_height;           //Rendered image height in pixels
            double pixel_samples_scale; //Averaging factor = 1 / samples_per_pixel
            point3 center;               //Camera center (ray origin)
            point3 pixel00_loc;          //Location of pixel (0,0)'s center
            vec3 pixel_delta_u;          //Offset to pixel to the right
            vec3 pixel_delta_v;          //Offset to pixel below

        
        void initialize() {
            //Calculate the image height, and ensure that it is at least 1
            image_height = int(image_width/ aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1. / samples_per_pixel;

            center = point3(0,0,0);

            //Camera
            //Distance from viewport
            auto focal_length = 1.;
            
            //Find viewport width
            auto viewport_height = 2.;
            auto viewport_width = viewport_height * (double(image_width)/ image_height);

            //Calculate the vectors across the horizontal and down the vertical veiwport edges
            auto viewport_u = vec3(viewport_width, 0, 0);

            //Viewport points top to bottom hence the negation of the height component
            auto viewport_v = vec3(0, -viewport_height, 0);

            //Calculate the horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            //Calculate the location of viewports upper left
            auto viewport_upper_left = center -vec3(0, 0, focal_length)
                                                    - viewport_u/2 - viewport_v/2;

            //Find the first pixels location based on the upper left corner
            pixel00_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);


        }

        //Get a ray that is randomly placed around the pixel center
        ray get_ray(int i, int j) const {

            auto offset = sample_square();
            auto pixel_sample = pixel00_loc
                    + ((i + offset.x()) * pixel_delta_u)
                    + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        //Randomly place create a vec3 offset anywhere in the pixels given 1x1 square.
        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }
        //Recursively trace a ray through the scene, accumulating color via material scattering
        color ray_color(const ray& r, int depth, const hittable& world) { 

            //Stop recursion after too many bounces
            if (depth <= 0)
                return color(0,0,0);
                
            //Store info on ray intersection
            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                
                ray scattered;
                color attenuation;
                // Ask the material how (or whether) this ray scatters
                if (rec.mat -> scatter(r, rec, attenuation, scattered))
                    //collect color data for final image
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0,0,0);
            }

            //Background
            //Normalize to hold within [-1,1] values
            vec3 unit_direction = unit_vector(r.direction());
            //Hold to [0,1]
            auto a = 0.5*(unit_direction.y() + 1.);
            //Linear interpolation between blue and white leading to a gradient.   
            return (1. - a) * color(1.,1.,1.) + a*color(0.5, 0.7, 1.);
        }
};

    
#endif