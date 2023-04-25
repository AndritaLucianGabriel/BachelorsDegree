/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/SecureServerSocket.h>

#include <iostream>

#include "server.h"
#include "logger.h"
#include "constants.h"

using namespace Poco::Util;

class MyServerApp : public ServerApplication
{
protected:
    int main(const std::vector<std::string> &)
    {
        Poco::Net::HTTPServer s(new MyRequestHandlerFactory, Poco::Net::ServerSocket(9090), new Poco::Net::HTTPServerParams);

        s.start();
        std::cout<<"\n\tServer started...\n\n";

        waitForTerminationRequest(); // wait for CTRL-C or kill

        MyLogger::cleanUp();
        std::cout<<"\n\tShutting down...\n";
        s.stop();

        return Application::EXIT_OK;
    }
};

int main(int argc, char **argv)
{
    if (std::string(argv[1]) == "ENABLE_DEBUG")
    {
        MyLogger::init(true);
    }
    else if (std::string(argv[1]) == "DISABLE_DEBUG")
    {
        MyLogger::init(false);
    }

    MyServerApp app;

    Poco::Logger &logger = MyLogger::getLogger();

    logger.information("=======================================================");

    return app.run(argc, argv);
}

// #include "google/cloud/dialogflow_es/agents_client.h"
// #include "google/cloud/project.h"
// #include <iostream>

// int main(int argc, char* argv[]) try {
//   if (argc != 2) {
//     std::cerr << "Usage: " << argv[0] << " project-id\n";
//     return 1;
//   }
//   namespace dialogflow_es = ::google::cloud::dialogflow_es;
//   auto client =
//       dialogflow_es::AgentsClient(dialogflow_es::MakeAgentsConnection());

//   auto const project = google::cloud::Project(argv[1]);
//   for (auto a : client.SearchAgents(project.FullName())) {
//     if (!a) throw std::move(a).status();
//     std::cout << a->DebugString() << "\n";
//   }

//   return 0;
// } catch (google::cloud::Status const& status) {
//   std::cerr << "google::cloud::Status thrown: " << status << "\n";
//   return 1;
// }