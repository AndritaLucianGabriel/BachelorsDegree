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

#ifndef SERVER_H_
#define SERVER_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Logger.h>

#include "logger.h"
#include "chatbot.h"

/**
 * @brief Wrapper class over Poco::Net::HTTPRequestHandler. This class contains the logic to serve the responses
 * based on the user's requests.
 */
class MyRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        /**
         * @brief This method receives the request from the user, and first checks the type of it. If it's a
         * JSON payload, then it's in fact the message sent by the user via javascript. If the content is 
         * a simple request of a file (or it's links), then it passes it to @ref serveResponse "serveResponse".
         * 
         * @param req The request received from the user.
         * @param resp The response that will be sent from the server.
         */
        virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);

    private:
        /**
         * @brief This method will send through the response the actual file requested by the client or a HTML page.
         * 
         * @param response The response that will be sent from the server.
         * @param fileName The name of the file that's been requested.
         * @param extension The extension of the file. It is need since the extension is the actual folder where 
         * it is placed.
         */
        void serveResponse(Poco::Net::HTTPServerResponse &response, const std::string& fileName, const std::string& extension);

        /**
         * @brief Creates a class data member while using an already existing reference to a MyLogger object.
         * @see MyLogger::getLogger()
         */
        Poco::Logger &logger = MyLogger::getLogger();

}; // class MyRequestHandler

/**
 * @brief Factory class that will create a new handler for each request.
 */
class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        /**
         * @brief Creates a new request handler for each individual request.
         * 
         * @return Poco::Net::HTTPRequestHandler* The handler of the request.
         */
        virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &);

}; // class MyRequestHandlerFactory

#endif // SERVER_H_