/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/FileChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Exception.h"

#include "logger.h"

#include <iostream>
#include <fstream>

Poco::Logger *MyLogger::logger = nullptr;
bool MyLogger::debug = false;

Poco::Logger &MyLogger::getLogger()
{
    if (!logger)
    {
        // Create a ConsoleChannel for logging to the console
        Poco::AutoPtr<Poco::ConsoleChannel> pConsoleChannel;
        if (debug)
        {
            pConsoleChannel = new Poco::ConsoleChannel;
        }

        // Create a FileChannel for logging to a file
        Poco::AutoPtr<Poco::FileChannel> pFileChannel(new Poco::FileChannel);
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
        if (debug)
        {
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

void MyLogger::init(bool pDebug)
{
    debug = pDebug;
}

// When you use Poco::Logger, you don't have to worry about freeing the logger instance directly, as Poco::Logger takes care of memory management internally. Poco uses a reference counting mechanism for logger management. The logger is destroyed automatically when its reference count drops to zero.
void MyLogger::cleanUp() {
    logger = nullptr;

}