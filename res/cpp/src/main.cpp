// /* Copyright 2023 Andrita Lucian-Gabriel
//  * Contact: gabylucian2000@gmail.com
//  * All rights reserved.
//  *
//  * This code is provided for personal use only and may not be distributed or
//  * modified without the express written consent of the author. The author makes
//  * no warranties or representations as to the accuracy or completeness of the
//  * code or its suitability for any purpose. Use of the code is at your own risk.
//  */

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>

#include <iostream>

#include "server.h"
#include "logger.h"
#include "chatbot.h"

using namespace Poco::Util;

class MyServerApp : public ServerApplication {
    protected:
        int main(const std::vector<std::string> &) {
            Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
            pParams->setMaxQueued(100);
            pParams->setMaxThreads(4);
            Poco::Net::HTTPServer server(new MyRequestHandlerFactory, Poco::Net::ServerSocket(9090), pParams);

            server.start();
            std::cout<<"\n\tServer started...\n\n";

            waitForTerminationRequest(); // wait for CTRL-C or kill

            MyLogger::cleanUp();
            std::cout<<"\n\tShutting down...\n";
            server.stop();

            return Application::EXIT_OK;
        }
};

int main(int argc, char **argv) {
    if (std::string(argv[1]) == "ENABLE_DEBUG") {
        MyLogger::init(true);
    }
    else if (std::string(argv[1]) == "DISABLE_DEBUG") {
        MyLogger::init(false);
    }

    MyServerApp app;

    Poco::Logger &logger = MyLogger::getLogger();

    logger.information("=======================================================");

    return app.run(argc, argv);
}