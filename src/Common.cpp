//
//  Сommon.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 18/03/2023.
//

#include "Common.hpp"

#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#else
#include <sys/select.h>
#endif

void sleep(int millisec)  {
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = millisec * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

Point TWODCPP_EXPORT operator+(const Point& lhs, const Point& rhs)
{
    return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

Point TWODCPP_EXPORT operator-(const Point& lhs, const Point& rhs)
{
    return Point(lhs.x - rhs.x, lhs.y - rhs.y);
}
