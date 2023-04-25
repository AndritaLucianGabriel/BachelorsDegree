/* Copyright 2023 Andrita Lucian-Gabriel
 * Contact: gabylucian2000@gmail.com
 * All rights reserved.
 *
 * This code is provided for personal use only and may not be distributed or
 * modified without the express written consent of the author. The author makes
 * no warranties or representations as to the accuracy or completeness of the
 * code or its suitability for any purpose. Use of the code is at your own risk.
 */

#include "google/cloud/dialogflow_es/agents_client.h"
#include "google/cloud/project.h"

#include "chatbot.h"
#include "constants.h"

#include <iostream>

namespace dialogflow_es = ::google::cloud::dialogflow_es;

void Chatbot::call() {
  auto client = dialogflow_es::AgentsClient(dialogflow_es::MakeAgentsConnection());

  auto const project = google::cloud::Project(globals::projectID);
  for (auto a : client.SearchAgents(project.FullName())) {
    if (!a) throw std::move(a).status();
    std::cout << a->DebugString() << "\n";
  }
// } catch (google::cloud::Status const& status) {
//   std::cerr << "google::cloud::Status thrown: " << status << "\n";
//   return 1;
// }
}