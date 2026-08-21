#ifndef INTERVAL_H
#define INTERVAL_H

#include "utility.h"

class interval {
    public:
        double min, max;

        //Default interval is empty
        interval() : min(+infinity), max(-infinity) {}
        //Constructor
        interval(double min, double max) : min(min), max(max) {}

        //Size getter function
        double size() const {
            return max - min;
        }
        
        //Checks if point is in interval
        bool contains(double x) const {
            return min <= x && x <= max;
        }

        //Checks if point is surrounded by interval
        //Stricter than contains due to < vs <=
        bool surrounds(double x) const {
            return min < x && x <= max;
        }

        //Constants - empty: nothing, universe: everyting
        static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif