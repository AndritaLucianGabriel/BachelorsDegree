/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Poco/Logger.h"

#include <memory>
#include <optional>

class MyLogger
{

public:
    static Poco::Logger &getLogger();
    static void init(bool pDebug);
    static void cleanUp();
private:
    static bool debug;
    static Poco::Logger *logger;
};

#endif // LOGGER_H_