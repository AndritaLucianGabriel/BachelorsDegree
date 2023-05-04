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

class MyRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);

    private:
        void serveResponse(Poco::Net::HTTPServerResponse &response, const std::string& fileName, const std::string& extension);
        Poco::Logger &logger = MyLogger::getLogger();

}; // class MyRequestHandler

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &);

}; // class MyRequestHandlerFactory

#endif // SERVER_H_