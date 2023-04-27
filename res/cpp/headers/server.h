/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
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

class MyRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);

    private:
        void serveResponse(Poco::Net::HTTPServerResponse &response, const std::string& fileName, const std::string& extension);
        Poco::Logger &logger = MyLogger::getLogger();
        static int count;

}; // class MyRequestHandler

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &);

}; // class MyRequestHandlerFactory

#endif // SERVER_H_