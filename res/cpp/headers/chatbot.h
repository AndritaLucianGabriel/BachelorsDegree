/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#ifndef CHATBOT_H_
#define CHATBOT_H_

#include "google/cloud/dialogflow_es/sessions_client.h"
#include "google/cloud/project.h"

#include "Poco/Logger.h"

#include "logger.h"

#include <memory.h>

namespace dialogflow_es = ::google::cloud::dialogflow_es;
namespace v2 = ::google::cloud::dialogflow::v2;

class Chatbot {
    public:
        static Chatbot* getInstance();
        void sendMessage(const std::string& message);

        void setOutputText(const std::string& outputParam);
        std::string getOutputText();
    private:
        Chatbot():client(dialogflow_es::MakeSessionsConnection()){setUp();}
        void setUp();
        std::string convertQueryForDebug(const v2::QueryResult& query);
        Poco::Logger& logger = MyLogger::getLogger();

        //Am nevoie de asta pt conversatie. MakeSessionsConnection foloseste enviroment variable setat in script prin json
		dialogflow_es::SessionsClient client;
        v2::DetectIntentRequest request;
        bool init = false;
        std::string inputText = "";
        std::string outputText = "";
        static Chatbot* agent;

}; // class Chatbot

#endif // CHATBOT_H_