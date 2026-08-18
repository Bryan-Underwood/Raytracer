#include <iostream>
#include <fstream>

#include "vec3.h"
#include "color.h"
#include "ray.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
    //Ray starting from camera going to sphere center
    vec3 oc = center - r.origin();

    //Coefficients of the quadratic in t
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) 
        return -1.;
    
    else
        //Solve quadratic for t, the scalar value of the ray when it intersects the sphere
        return (h - std::sqrt(discriminant)) / a;
    
}

color ray_color(const ray& r) { 

    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        //Surface normal = surface point - center point
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5 * color(N.x()+1, N.y()+1, N.z()+1);
    }

    //Normalize to hold within [-1,1] values
    vec3 unit_direction = unit_vector(r.direction());
    //Hold to [0,1]
    auto a = 0.5*(unit_direction.y() + 1.);
    //Linear interpolation between blue and white leading to a gradient.   
    return (1. - a) * color(1.,1.,1.) + a*color(.5,.7, 1.);
}

int main() {

    //Image
    auto aspect_ratio = 16. /9.;
    int image_width = 400;

    //Calculate the image height, and ensure that it is at least 1
    int image_height = int(image_width/ aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //Camera

    //Distance from viewport
    auto focal_length = 1.;
    
    //Find viewport width
    auto viewport_height = 2.;
    auto viewport_width = viewport_height * (double(image_width)/ image_height);

    //Camera origin
    auto camera_center = point3(0,0,0);

    //Calculate the vectors across the horizontal and down the vertical veiwport edges
    auto viewport_u = vec3(viewport_width, 0, 0);

    //Viewport points top to bottom hence the negation of the height component
    auto viewport_v = vec3(0, -viewport_height, 0);

    //Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    //Calculate the location of viewports upper left
    auto viewport_upper_left = camera_center -vec3(0, 0, focal_length)
                                             - viewport_u/2 - viewport_v/2;

    //Find the first pixels location based on the upper left corner
    auto pixel00_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);

    
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
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            //Give the ray color and display it
            color pixel_color = ray_color(r);
            write_color(out, pixel_color);

        }   
    }

    std::clog << "\rDone.                        \n";
}