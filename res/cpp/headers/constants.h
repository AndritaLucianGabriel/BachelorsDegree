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

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "string.h"

/**
 * @brief Namespace that stores configuration variables.
 */
namespace globals {
    /**
     * @brief The port that will be used for the server.
     */
    const int serverPort = 9090;

    /**
     * @brief The name of the project (check on Google Cloud Platform).
     */
    const std::string projectID = "licenta-383311";

    /**
     * @brief The ID of the session that's being used for the client.
     */
    const std::string sessionID = "123456789";

    /**
     * @brief The language of the session.
     */
    const std::string agentLanguage = "en";

} // namespace globals

#endif // CONSTANTS_H_