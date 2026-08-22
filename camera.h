#ifndef CAMERA_H
#define CAMERA_H
#include "hittable.h"
#include <fstream>

class camera {
    public:
        //Image
        double aspect_ratio = 1.;
        int image_width = 100;

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
                        //Move pixel location based on origin pixel i = x-axis j = y-axis
                        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

                        //Send the ray from the camera to the viewport
                        auto ray_direction = pixel_center - center;
                        ray r(center, ray_direction);

                        //Give the ray color and display it
                        color pixel_color = ray_color(r, world);
                        write_color(out, pixel_color);
                    }   
                }

            std::clog << "\rDone.                        \n";
    }

    private:
            int image_height;
            point3 center;
            point3 pixel00_loc;
            vec3 pixel_delta_u;
            vec3 pixel_delta_v;

        
        void initialize() {
            //Calculate the image height, and ensure that it is at least 1
            image_height = int(image_width/ aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

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

        //color pixels based on ray collision and normal orientation
        color ray_color(const ray& r, const hittable& world) { 

            //Sphere color
            hit_record rec;

            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + color(1,1,1));
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