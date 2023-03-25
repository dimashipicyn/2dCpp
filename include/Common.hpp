//
//  Сommon.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 18/03/2023.
//

#ifndef _сommon_hpp
#define _сommon_hpp

#include <memory>
#include <chrono>

template<class T>
using Ptr = std::shared_ptr<T>;

template<class T, class... Args>
Ptr<T> CreatePtr(Args&& ...args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

struct Chrono
{
	Chrono() {
		last_time = std::chrono::high_resolution_clock::now();
	}

	double reset() {
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff = now - last_time;
		last_time = now;
		return diff.count();
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
};

void sleep(int millisec);

#endif /* _сommon_hpp */
