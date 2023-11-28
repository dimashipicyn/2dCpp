//
//  Header.h
//  2dCpp
//
//  Created by Dmitry Shipicyn on 15.09.2022.
//

#ifndef log_h
#define log_h

#include <string>

namespace logging {
    void log(int level, const std::string& s);
    void set_log_level(int level);
}

#define LOG_LEVEL_INFO 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_OFF 4

#define LOG(level, msg) log::log(level, msg)

#define LOG_INFO(msg) logging::log(LOG_LEVEL_INFO, msg)
#define LOG_DEBUG(msg) logging::log(LOG_LEVEL_DEBUG, msg)
#define LOG_WARNING(msg) logging::log(LOG_LEVEL_WARNING, msg)
#define LOG_ERROR(msg) logging::log(LOG_LEVEL_ERROR, msg)

#define SET_LOG_LEVEL(level) logging::set_log_level(level)

#endif /* Header_h */
