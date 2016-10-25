/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author Dave Hickin
 *
 */

#ifndef EXAMPLERPC_POINT_H
#define EXAMPLERPC_POINT_H


#ifdef epicsExportSharedSymbols
#   define pointExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>

#include <epicsThread.h>

#include <iostream>

#ifdef pointExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef pointExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace exampleRPC {

epicsShareClass class Point
{
public:
    Point()
    : x(0), y(0)
    {
    }

    Point(double x, double y)
    : x(x), y(y)
    {
    }

    Point(const Point & point)
    : x(point.x), y(point.y)
    {
    }

    Point operator=(const Point &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    double x;
    double y;
};


inline bool operator==(const Point & lhs, const Point &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Point & lhs, const Point &rhs)
{
    return !(lhs == rhs);
}

inline std::ostream & operator<< (std::ostream& os, const Point& point)
{
    os << "(" << point.x << "," << point.y << ")";
    return os;
}


}}}

#endif //EXAMPLERPC_POINT_H


