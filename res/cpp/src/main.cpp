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
            logger.information("\n\tServer started...\n\n");
            if(MyLogger::getDebug() == false){
                std::cout<<"\n\tServer started...\n\n";
            }

            waitForTerminationRequest(); // wait for CTRL-C or kill

            MyLogger::cleanUp();
            logger.information("\n\tShutting down...\n");
            if(MyLogger::getDebug() == false){
                std::cout<<"\n\tShutting down...\n";
            }
            server.stop();

            return Application::EXIT_OK;
        }
    private:
        Poco::Logger& logger = MyLogger::getLogger();
};

int main(int argc, char **argv) {
    if (std::string(argv[1]) == "ENABLE_DEBUG") {
        MyLogger::init(true);
    }
    else if (std::string(argv[1]) == "DISABLE_DEBUG") {
        MyLogger::init(false);
    }

    MyServerApp app;

    return app.run(argc, argv);
}