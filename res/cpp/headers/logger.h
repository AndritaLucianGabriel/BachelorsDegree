/* Copyright 2023 Andrita Lucian-Gabriel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Poco/Logger.h"

/**
 * @brief Singleton wrapper class over Poco::Logger. @see https://docs.pocoproject.org/current/Poco.Logger.html
 */
class MyLogger{
    public:
        /**
         * @brief Checks if the @ref logger "logger" has already been instantiated. If not, creates a new
         * reference, otherwise it returns the existing one. The steps for the creation of the service are
         * the following: a channel for the console stream it is first created, then a new one for the file
         * system stream. A couple of parameters are set for the service, and then based on the way the 
         * application was ran (with ENABLE_DEBUG or DISABLE_DEBUG), the logging system will use the console stream
         * as a debuging utility.
         *
         * @return Reference to an already existing object, or an existing one
         */
        static Poco::Logger &getLogger();

        /**
         * @brief Sets @ref debug "debug" to true or false based on the way the app was ran.
         * 
         * @param debugParam Passed value from main.
         */
        static void init(bool debugParam);

        /**
         * @brief Get the value of the debug data member.
         * 
         * @return true If the app was ran with ENABLE_DEBUG (uses console channel)
         * @return false  If the app was ran with DISABLE_DEBUG (will use only the file channel)
         */
        static bool getDebug();

        /**
         * @brief Sets @ref logger "logger" to nullptr. The Poco::AutoPtr is a smart pointer wrapper that is
         * similar to a shared_ptr and checks the number of uses of the reference. Since it's always one (the 
         * class is singleton), if this refernce is set to nullptr, then it automatically handles the memory.
         */
        static void cleanUp();

    private:
        /**
         * @brief Stores the option for the logger to use or not the console stream.
         */
        static bool debug;

        /**
         * @brief Logger reference that will be used ONLY through @ref getLogger() "getLogger()".
         */
        static Poco::Logger *logger;

}; // class MyLogger

#endif // LOGGER_H_