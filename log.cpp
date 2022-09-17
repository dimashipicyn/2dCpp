//
//  log.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 15.09.2022.
//

#include "log.h"
#include <iostream>

namespace {
    int log_level = LOG_LEVEL_OFF;
    std::string levels[] = {
        "INFO",
        "DEBUG",
        "WARNING",
        "ERROR",
    };
}

namespace logging {
    void log(int level, const std::string& s) {
        if (level > log_level) {
            std::cerr << "|" << levels[level] << "|" << s << std::endl;
        }
    }
    
    void set_log_level(int level) {
        if (level < LOG_LEVEL_INFO) {
            log_level = LOG_LEVEL_INFO;
        }
        else if (level >= LOG_LEVEL_OFF) {
            log_level = LOG_LEVEL_OFF;
        }
        else {
            log_level = level;
        }
    }
}
