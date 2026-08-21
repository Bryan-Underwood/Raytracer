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

#endif
