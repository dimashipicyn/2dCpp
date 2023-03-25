//
//  Сommon.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 18/03/2023.
//

#include "Common.hpp"

#include <sys/select.h>

void sleep(int millisec)  {
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = millisec * 1000;
	select(0, NULL, NULL, NULL, &tv);
}
