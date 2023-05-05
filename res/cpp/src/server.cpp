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

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/JSON/Parser.h>
#include <Poco/StreamCopier.h>
#include <Poco/DynamicAny.h>

#include <fstream>
#include <string>
#include <sstream>

#include "server.h"
#include "chatbot.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace std;

void MyRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    std::string requestPath = request.getURI();
    if(request.getContentType() == "application/json") {
        // Read the request body
        std::stringstream ss;
        Poco::StreamCopier::copyStream(request.stream(), ss);
        std::string requestBody = ss.str();

        // Parse the JSON data from the request body
        Parser parser;
        Var jsonData = parser.parse(requestBody);
        Object::Ptr jsonObject = jsonData.extract<Object::Ptr>();
        std::string message = jsonObject->get("message").toString();
        logger.information("Input text sent from client: " + message);
        Chatbot::getInstance()->sendMessage(message);
        ostream &out=response.send();
        out.flush();
        out<<Chatbot::getInstance()->getOutputText();
        out.flush();
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.send();
    }
    else {
        // From after / to the first .
        std::string fileName = requestPath.substr(1, requestPath.find(".") - 1);
        std::string extension = requestPath.substr(requestPath.find(".") + 1);

        if(requestPath == "/") {
            logger.information("=================================\n\tRefreshed");
        }
        logger.information("Requested " + requestPath);

        // Hardcoded for root path
        if(requestPath == "/") {
            serveResponse(response, "chat", "html");
        }
        else {
            serveResponse(response, fileName, extension);
        }

        // resp.sendFile("../res/views/pages/index.html", "text/html");
        //     ostream &out = resp.send();
        // out << buffer.str();
        // out.flush();
        // out << "<h1 style=\"font-size: 100; text-align: center; color: red;\">Reeee</h1>"
        //     << "<p>Count: " << ++count << "</p>"
        //     << "<p>Host: " << req.getHost() << "</p>"
        //     << "<p>Method: " << req.getMethod() << "</p>"
        //     << "<p>URI: " << req.getURI() << "</p>";
        // out.flush();
    }
}

// Imi permite sa adaug tipuri noi de fisiere atata tip cat pastrez structura
// 1. sa fie direct in res
// 2. sa se faca un folder nou, avand ca nume extensia
void MyRequestHandler::serveResponse(Poco::Net::HTTPServerResponse &response, const std::string& fileName, const std::string& extension) {
    response.setContentType("text/" + extension);
    std::ifstream file("res/" + extension + "/" + fileName + "." + extension);
    if (file.is_open()) {
        logger.information("Sending " + fileName + "." + extension);
        response.sendFile("res/" + extension + "/" + fileName + "." + extension, "text/" + extension);
        file.close();
    }
    else {
        logger.error(fileName + "." + extension + " not found");
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.send();
    }
}

Poco::Net::HTTPRequestHandler *MyRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &) {
    return new MyRequestHandler;
}