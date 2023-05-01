/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#include "chatbot.h"
#include "constants.h"

#include <iostream>
#include <sstream>

void Chatbot::sendMessage(const std::string& message) {
	if(init == true) {
		try {
			this->request.mutable_query_input()->mutable_text()->set_text(message);

			auto response = this->client.DetectIntent(this->request);
			if (!response) {
				logger.error("Error in DetectIntent: " + response.status().message());
				return;
			}

			const v2::QueryResult& query_result = response->query_result();
			this->setOutputText(query_result.fulfillment_text());
			// logger.information(convertQueryForDebug(query_result));
		} catch (google::cloud::Status const& status) {
			logger.error("google::cloud::Status thrown: " + status.message());
		}
	}
	else {
		logger.error("The agent's session has not been configured!");
	}
}

std::string Chatbot::convertQueryForDebug(const v2::QueryResult& query) {
	std::string text;
	text="Query text: " + query.query_text() + "\n"
		+ "Intent: " + query.intent().display_name() + "\n"
		+ "Response: " + query.fulfillment_text();
	return text;
}

void Chatbot::setOutputText(const std::string& outputParam) {
	this->outputText = outputParam;
	logger.information("Output text sent to agent: " + this->outputText);
}

std::string Chatbot::getOutputText() {
	logger.information("Output text retrieved from agent: " + this->outputText);
	return this->outputText;
}

void Chatbot::setUp() {
	if(init == false) {
		try{
			init = true;
			request.mutable_query_input()->mutable_text()->set_language_code(globals::agentLanguage);
			this->request.set_session("projects/" + globals::projectID + "/agent/sessions/" + globals::sessionID);
			logger.information("The agent has been succesfully configured!");
		} catch (google::cloud::Status const& status) {
			logger.error("google::cloud::Status thrown: " + status.message());
		}
	}
	else {
		logger.warning("The agent's session was already configured!");
	}
}