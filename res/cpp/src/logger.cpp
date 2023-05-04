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

#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/FileChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Exception.h"
#include "Poco/File.h"

#include <iostream>
#include <fstream>

#include "logger.h"

Poco::Logger *MyLogger::logger = nullptr;
bool MyLogger::debug = false;

Poco::Logger &MyLogger::getLogger() {
    if (!logger) {
        // Create a ConsoleChannel for logging to the console
        Poco::AutoPtr<Poco::ConsoleChannel> pConsoleChannel;
        if (debug) {
            pConsoleChannel = new Poco::ConsoleChannel;
        }

        // Create a FileChannel for logging to a file
        Poco::AutoPtr<Poco::FileChannel> pFileChannel(new Poco::FileChannel);
        Poco::File dir("./logs");
        if (!dir.exists()) {
            dir.createDirectories();
        }
        pFileChannel->setProperty("path", "./logs/licenta.log");
        pFileChannel->setProperty("rotation", "1M"); // keeps 1 file of 1MB max
        pFileChannel->setProperty("archive", "number"); // newest archive is always .0
        pFileChannel->setProperty("flush", "true"); // will always log even in a system crash situation
        pFileChannel->setProperty("rotateOnOpen", "true"); // will create a new log each time the server is started
        pFileChannel->setProperty("purgeCount", "3"); // will keep a max 3 log archives before it starts to delete the oldest

        Poco::AutoPtr<Poco::PatternFormatter> pPF(new Poco::PatternFormatter);
        pPF->setProperty("pattern", "[%Y-%m-%d] [%H:%M:%S] [%p]: %t");

        Poco::AutoPtr<Poco::FormattingChannel> pFC(new Poco::FormattingChannel(pPF, pFileChannel));

        // Create a SplitterChannel for logging to both console and file
        Poco::AutoPtr<Poco::SplitterChannel> pSplitterChannel(new Poco::SplitterChannel);
        if (debug) {
            pSplitterChannel->addChannel(pConsoleChannel);
        }
        pSplitterChannel->addChannel(pFC);

        // Configure the logger
        Poco::Logger::root().setChannel(pSplitterChannel);
        Poco::Logger::root().setLevel("information");

        logger = &Poco::Logger::create("Licenta", pSplitterChannel, Poco::Message::PRIO_INFORMATION);
    }
    return *logger;
}

void MyLogger::init(bool debugParam) {
    debug = debugParam;
}

bool MyLogger::getDebug() {
    return debug;
}

// When you use Poco::Logger, you don't have to worry about freeing the logger instance directly, as Poco::Logger takes care of memory management internally. Poco uses a reference counting mechanism for logger management. The logger is destroyed automatically when its reference count drops to zero.
void MyLogger::cleanUp() {
    logger = nullptr;
}