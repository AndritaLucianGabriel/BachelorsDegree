/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "server.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/JSON/Parser.h>
#include <Poco/StreamCopier.h>
#include "Poco/DynamicAny.h"
#include <sstream>

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
        chatbot.sendMessage(message);
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        ostream &out=response.send();
        out.flush();
        out<<chatbot.getOutputText();
        out.flush();
        // response.send();
    }
    else {
        logger.information("Requested " + requestPath);

        // From after / to the first .
        std::string fileName = requestPath.substr(1, requestPath.find(".") - 1);
        std::string extension = requestPath.substr(requestPath.find(".") + 1);

        // Hardcoded for root path
        if(requestPath == "/") {
            serveResponse(response, "index", "html");
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
        logger.information("Sending " + fileName + "/" + extension);
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