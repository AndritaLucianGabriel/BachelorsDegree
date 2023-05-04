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

#ifndef CHATBOT_H_
#define CHATBOT_H_

#include "google/cloud/dialogflow_es/sessions_client.h"
#include "google/cloud/project.h"

#include "Poco/Logger.h"

#include "logger.h"

namespace dialogflow_es = ::google::cloud::dialogflow_es;
namespace v2 = ::google::cloud::dialogflow::v2;
/**
 * @brief Ajutor
 * 
 */
class Chatbot {
    public:
        static Chatbot* getInstance();
        void sendMessage(const std::string& message);

        void setOutputText(const std::string& outputParam);
        std::string getOutputText();
        virtual ~Chatbot() {delete agent;}
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