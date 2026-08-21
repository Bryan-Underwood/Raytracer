
//Utility class to link files and add usefull constants/funcitons

#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

//C++ Std usings
using std::make_shared;
using std::shared_ptr;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.;
}

//Common Headers - must be included after utility to insure they are declared
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif
