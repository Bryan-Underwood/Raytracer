#ifndef VEC3_H
#define VEC3_H

#include "utility.h"

//Vec 3 class
//Includes color and point3

//Class definition
class vec3 {
    public:

    //create a list e of 3 values
    double e[3];

    //construtor for vec3 class
    //vec3 is a tuple of 3 doubles

    //initalizes vec3 as all zeros as a base value
    vec3() : e{0,0,0} {}
    //Takes 3 doubles as initialization values
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    //Allows for retrival of vec3 elements
    double x() const { return e[0];}
    double y() const { return e[1];}
    double z() const { return e[2];}

    //Negation of vec3
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    //Allows for vec3 to be treated as an array

    //return a copy
    double operator[](int i) const { return e[i]; }
    //return a reference
    double & operator[](int i) { return e[i]; }

    //Vector addition
    vec3& operator += (const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    
    //Vector multiplication
    vec3& operator *= (double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    //Vector division
    vec3& operator /= (double t) {
        return *this *= 1/t;
    }

    //Retrives true length of vec using Pythagorean Theorem
    double length() const {
        return std::sqrt(length_squared());
    }

    //Gets squared length of a vector
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const {
        //Return true if the vector is close to zero in all dimentions.
        auto s = 1e-8;
        //fabs() returns the absolute value of a float 
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);

    }

};

//Alias for vec3
using point3 = vec3;

//Vector Utility Functions

//Print vectors
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out <<v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

//Add vectors
inline vec3 operator + (const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

//Subtract vectors
inline vec3 operator - (const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

//Multiply vectors
inline vec3 operator * (const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

//Scale vectors
inline vec3 operator * (double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

//Also scales in reverse operand order
inline vec3 operator * (const vec3& v, double t) {
    return t * v;
}

//Divide vectors
inline vec3 operator / (const vec3& v, double t) {
    return (1/t) * v;
}

//Dot product of vectors
inline double dot (const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

//Cross product of vectors
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

//Creates a unit vector
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while (true) {
        //get a random vector in a cube
        auto p = vec3::random(-1,1);
        //check if vector is inside unit sphere inside of cube
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
            //normalize if it is
            return p / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n) * n;
}

#endif
