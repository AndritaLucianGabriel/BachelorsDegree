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

#include <iostream>
#include <sstream>

#include "chatbot.h"
#include "constants.h"

Chatbot* Chatbot::agent = nullptr;

Chatbot* Chatbot::getInstance() {
	if(agent == nullptr) {
		agent = new Chatbot();
		return agent;
	}
	return agent;
}

void Chatbot::sendMessage(const std::string& message) {
	if(agent != nullptr) {
		try {
			agent->request.mutable_query_input()->mutable_text()->set_text(message);

			auto response = agent->client.DetectIntent(request);
			// If the response is valid, and the request didn't time out
			if (!response) {
				logger.error("Error in DetectIntent: " + response.status().message());
				return;
			}
			// If the response is valid, but the request timed out, we try again
			else if (response->query_result().fulfillment_text() == "") {
				logger.warning("Retrying the request!");
				response = agent->client.DetectIntent(request);
				if (!response) {
					logger.error("Error in DetectIntent: " + response.status().message());
					return;
				}
			}

			const v2::QueryResult& query_result = response->query_result();
			if(query_result.fulfillment_text() == "") {
				agent->setOutputText("Something went wrong. Can you try again?");
			}
			else {
				agent->setOutputText(query_result.fulfillment_text());
			}
			// logger.information(agent->convertQueryForDebug(query_result));
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
	agent->outputText = outputParam;
}

std::string Chatbot::getOutputText() {
	logger.information("Output text retrieved from agent: " + agent->outputText);
	return agent->outputText;
}

void Chatbot::setUp() {
	try {
		request.mutable_query_input()->mutable_text()->set_language_code(globals::agentLanguage);
		request.set_session("projects/" + globals::projectID + "/agent/sessions/" + globals::sessionID);
		logger.information("The agent has been succesfully configured!");
	} catch (google::cloud::Status const& status) {
		logger.error("google::cloud::Status thrown: " + status.message());
	}
}