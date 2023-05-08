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
 * @brief Chatbot singleton class used for sending and receiving the information
 * from Google Cloud Platform. This class' methods will be called via @ref agent "agent"
 * data member. @see getInstance.
 */
class Chatbot {
    public:
        /**
         * @brief Checks if the @ref agent "agent" has already been instantiated. If not, creates a new 
         * reference, otherwise it returns the existing one.
         * 
         * @return Reference to an already existing object, or an existing one
         */
        static Chatbot* getInstance();

        /**
         * @brief Receives the input string from the client (via javascript parsing) and sets the input 
         * text for the @ref request "request". Then the @ref client "client" "commits" the information 
         * to the cloud and it waits for a response (until then a chat bubble will appear as a pleaceholder). 
         * If the fulfillment text of the response is empty, a default message will be stored inside the @ref agent "agent's" @ref outputText "outputText".
         * 
         * Note: considering the current plan of the cloud service, and the limited computing power, somestimes
         * queries time out. The server sends the request one more time, but if that still doesn't work it displays
         * the error message.
         * 
         * @param message String received from javascript, after it was parsed.
         */
        void sendMessage(const std::string& message);

        /**
         * @brief Sets the outputText of the @ref agent "agent" to the given string.
         * 
         * @param outputParam String wanted to be stored inside the @ref agent "agent".
         */
        void setOutputText(const std::string& outputParam);

        /**
         * @brief Retrieves the outputText from the @ref agent "agent".
         * 
         * @return std::string The text stored inside the @ref agent "agent".
         */
        std::string getOutputText();

        /**
         * @brief Destroys the Chatbot object.
         */
        virtual ~Chatbot() {delete agent;}

    private:
        /**
         * @brief Constructs a new Chatbot object. It initializes the client data member using an initializer list and
         * also calls @ref setUp "setUp()". Since it is private, it is not intended to create new objects using this
         * constructor unless they are created through @ref getInstance "getInstance()".
         */
        Chatbot():client(dialogflow_es::MakeSessionsConnection()){setUp();}

        /**
         * @brief This method will set the language of requests (see @ref globals::agentLanguage "agentLanguage") while
         * also creating a new session for the agent. A session represents the actual change of inputs/ouputs between 
         * the client and the agent. @see https://cloud.google.com/dialogflow/es/docs/entities-session
         */
        void setUp();

        /**
         * @brief Formats the information from the agent's result in a readable manner.
         * 
         * @param query The query's result after then agent's processing.
         * @return std::string A formatted string to be displayed in the logs or console.
         */
        std::string convertQueryForDebug(const v2::QueryResult& query);

        /**
         * @brief Creates a class data member while using an already existing reference to a MyLogger object.
         * @see MyLogger::getLogger()
         */
        Poco::Logger& logger = MyLogger::getLogger();

        /**
         * @brief A service used for session interactions. This will store the context of the conversation. 
         *
         * Note: the MakeSessionsConnection() from the @ref Chatbot() "Chatbot's" constructor uses the enviroment variables 
         * that was set in the build.sh script.
         */
		dialogflow_es::SessionsClient client;

        /**
         * @brief The actual request that will be send through the @ref client "client" to the agent to be processed.
         */
        v2::DetectIntentRequest request;

        /**
         * @brief The fulfillment text after the request has been processed.
         */
        std::string outputText = "";

        /**
         * @brief The agent instance that will store/process the information. Will be used ONLY through @ref getInstance()
         * "getInstance()".
         */
        static Chatbot* agent;

}; // class Chatbot

#endif // CHATBOT_H_