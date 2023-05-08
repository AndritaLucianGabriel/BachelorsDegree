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

/** 
 * @mainpage Bachelor's Degree Documentation
 * @section intro_sec Introduction
 * This is a C++ banking application that uses Google Cloud Platform's Dialogflow ES for human-chatbot interactions in a banking context. The project was created as a Bachelor Degree for the Computers and Information Technology specialization for the Faculty of Mathematics and Computer Science from the University of Bucharest, Romania, author <a href="https://github.com/AndritaLucianGabriel">Andrita Lucian-Gabriel</a>.
 * @section general_info General information
 * @subsection install_library Installing Library
 * The user must have an access key for a service account on Google Cloud Platform in order to access the Dialogflow ES API. Read more <a href="https://cloud.google.com/iam/docs/keys-create-delete">here</a>. 
 * \n This key will be set to \a GOOGLE_APPLICATION_CREDENTIALS enviroment variable that will work as a login credentials for the platform.
 * \n In the root directory of the project, there is a bash script called \b getting_started.sh (Note: if the script can't be run on a Unix platform, I recommend first to check for the bad line endings so run \b dos2unix on the script and check again). When ran from an Unix platform, this will install all the required dependencies (see \ref getting_started_info). After it's completed, the user still has to manually run the first time setup for the GC CLI:
 * 1. \code {.bash} ./google-cloud-sdk/install.sh \endcode
 * 2. \code {.bash} source ~/.bashrc \endcode
 * 3. \code {.bash} ./google-cloud-sdk/bin/gcloud init \endcode
 *
 * Note: the first-time setup will take a \b long time (especially on older machines).
 * @subsection getting_started_info First time setup 
 * The script's (\b getting_started.sh) flow is as follows: \n
 * \b Important \b note: It must be run with \b - \b i or \b - \b - \b install flag and after it's completion, the user has to run the setup for the gcloud (see above steps).
 * 1. Eviroment setup:
 * 		1. The submodules are initialized.
 * 		2. Based on the missmatch of line endings, both the \b build.sh and \b sass executable will be converted to unix-like.
 * 		3. Installation of both \b doxygen and it's dependency \b graphviz
 * 
 * 2. Reverting the libs to the wanted versions:
 * 		1. \b Poco \b v1.12.4 is being used.
 * 		2. \b VCPKG \b v2023.04.15 \b Release is being used.
 * 		3. \b GCP \b v2.9.1
 * 3. Poco setup. Read more <a href="https://github.com/pocoproject/poco">here</a>:
 * 		1. Installation of the dependencies (\b openssl , \b libssl-dev , \b git , \b g++ , \b make , \b cmake).
 * 		2. Configuring CMakeLists.txt.
 * 		3. Building the configuration file.
 * 		4. Installing the library.
 * 4. Npm setup:
 * 		1. Intallation of \b npm via the default package manager.
 * 5. Google Cloud CLI setup. Read more <a href="https://cloud.google.com/sdk/docs/install">here</a>:
 * 		1. Download the archive containing the source code.
 * 		2. Extracting the archive.
 * 6. Sass setup:
 * 		1. Create a symbolic link(symlink) and force it's creation even if it exists to the provided binary (see \b libs/sass).
 * 7. VCPKG setup. Read more <a href="https://github.com/Microsoft/vcpkg">here</a>:
 * 		1. Installation of the dependencies (\b curl , \b zip , \b unzip , \b tar).
 * 		2. First time setup of the vcpkg manager via it's startup script (\b libs/vcpkg/bootstrap-vcpkg.sh).
 * 8. Google Cloud Platform setup. Read more <a href="https://github.com/googleapis/google-cloud-cpp/tree/main/google/cloud/dialogflow_es/quickstart">here</a>:
 * 		1. Intallation of the actual modules that we are going to use in the project (\b core and \b dialogflow-es) via \b vcpkg.
 * @subsection how_to_build How to build
 * From the root directory of the project, we can use the \b build.sh script to build and run the application. By default, the server will use \b 9090 port, but it can be changed from the \b constants.h (see \b globals::serverPort).
 * \n There are a couple of options that you can run the script with:
 * 1. - b or - - build => build and run the project
 * 2. - bscss or - - build-scss => compile only the scss files to update the HTML's aspect
 * 3. - docs or - - generate-docs => generate the technical documentation using Doxygen
 * 4. - c or - - clean => clean the project
 * 5. - v or - - verbose => optional, print more output to the console
 * 6. - h or - - help => show this help
 * 
 * Note: do \b NOT run \b -c with other options since the cleaning is meant to be a solo process.
 * @subsection more_info Detailed information
 * Both scripts share the same code regarding user input and error handling.
 * 1. They are processed by \a func_read_cli_options and cross-referenced with the default list (it's mostly different for both of them since they have distinct roles, but \b -v and \b -h are common since they have a shared utility).
 * 		1. Common flags:
 * 			1. The \b verbose flag allows for the scripts to print more information (both from them and the application logs) to the console. This option will set the \b VERBOSE global variable to true.
 * 			2. The \b help flag will just print the usage help.
 * 		2. Specific flags for \b getting_started.sh:
 * 			1. The \b install flag installs and configures all the prerequisites for the project.
 * 		3. Specific flags for \b build.sh:
 * 			1. The \b build flag will build and run the project.
 * 			2. The \b build-scss flag will compile just the scss files to update the HTML's aspect.
 * 			3. The \b generate-docs flag will generate the technical documentation using Doxygen.
 * 			4. The \b clean flag will clean the project from temporary files, logs, executables, binaries and so on.
 * 2. If the flag exists in the wanted list COMMAND variable is stores the wanted action and then it is passed to \a execute.
 * 3. The \a execute function will then call the method that has the wanted functionality. This level of abstractization allows to chain multiple commands under the same name (if wanted).
 *
 * Note: From now on, we will analyse each scrip individually since they have different methods for their different use cases. 
 * 1. For  \b getting_started.sh:
 * \n The \a execute method can only call the \a install function. This method will go through a list of commands and install the prerequisites (see \ref getting_started_info).
 * 2. For  \b build.sh:
 * \n The \a execute method can call one of the following functions:
 * 		1. The \a compile_sass function will iterate over all the scss files from \b res/scss subfolder and will compile them and generate the wanted css files in \b res/css subfolder. These files will not contain the auto-generated css maps because there are not a lot of css to begin with and there's no need to create extra logic for the server to handle these requests.
 * 		2. The \a generate_docs function will generate the technical documentation. In the Doxyfile, the following tags have been changed:
 * 			1. PROJECT_NAME = "Bachelor's Degree"
 * 			2. PROJECT_BRIEF = "A C++ banking application that uses Google Cloud Platform's Dialogflow ES for human-chatbot interactions in a banking context."
 * 			3. OUTPUT_DIRECTORY = "./res/docs"
 * 			4. FULL_PATH_NAMES = NO
 * 			5. BUILTIN_STL_SUPPORT = YES
 * 			6. EXTRACT_ALL = YES
 * 			7. EXTRACT_PRIVATE = YES
 * 			8. EXTRACT_STATIC = YES
 * 			9. INPUT = ./res
 * 			10. RECURSIVE = YES
 * 			11. HTML_OUTPUT = .
 * 			12. DISABLE_INDEX = YES
 * 			13. GENERATE_TREEVIEW = YES
 * 			14. GENERATE_LATEX = NO
 * 			15. CALL_GRAPH = YES
 * 			16. CALLER_GRAPH = YES
 * 			17. ALPHABETICAL_INDEX = NO
 * 		3. The \a build function is top-level method that has the goal of building and running the final executable. That executable is called \b licenta_EXECUTABLE and can be found in the project's root directory. It can be run with two flags: \b DISABLE_DEBUG or \b ENABLE_DEBUG. These flags depend on the \b VERBOSE global. If it is \a true, then the project's logging system (\b MyLogger - a wrapper onto Poco's Loger class) will create a Poco::ConsoleChannel that will be provided to a Poco::SplitterChannel. This way, whenever will be generated a new log, it will also be printed into the console, besides the \b logs folder.
 * 			1. It calls the compile_sass function. See above for further explications.
 * 			1. The \a compile_cpp function first configures the CMakeLists.txt file and passes the \a CMAKE_TOOLCHAIN_FILE variable to the Google Cloud Platform's CMake. Because this is not used directly in this CMake, we will also add the \b --no-warn-unused-cli flag to surpress the warning regarding this variable. If the configuring is succesfull, then actual building of the project happens.
 * 			2. The \a configure_gcp function will export the \a GOOGLE_APPLICATION_CREDENTIALS variable so it is visible to every process and sub-process that starts from the script.
 *
 * The project's CMake uses \a C++17 standard, and output's the executable in the project's root. The libraries used for the linking are \b Poco \b :: \b Foundation \b Poco \b :: \b Net \b Poco \b :: \b Util and \b google-cloud-cpp \b :: \b dialogflow_es.
 */

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>

#include <iostream>

#include "server.h"
#include "logger.h"
#include "chatbot.h"
#include "constants.h"

using namespace Poco::Util;

/**
 * @brief Wrapper class over Poco::Util::ServerApplication that is used for implementing server applications.
 * It allows the application to run as a Unix daemon.
 */
class MyServerApp : public ServerApplication {
    protected:
        /**
         * @brief Creates the actual daemon for the server. The server will work on maximum 4 threads and will accept
         * a limit of 100 connections.
         * 
         * @return int Application::EXIT_OK
         */
        int main(const std::vector<std::string> &) {
            Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
            pParams->setMaxQueued(100);
            pParams->setMaxThreads(4);
            Poco::Net::HTTPServer server(new MyRequestHandlerFactory, Poco::Net::ServerSocket(globals::serverPort), pParams);

            server.start();
            logger.information("\n\tServer started...\n\n");
            if(MyLogger::getDebug() == false){
                std::cout<<"\n\tServer started...\n\n";
            }

            waitForTerminationRequest(); // wait for CTRL-C or kill

            MyLogger::cleanUp();
            logger.information("\n\tShutting down...\n");
            if(MyLogger::getDebug() == false){
                std::cout<<"\n\tShutting down...\n";
            }
            server.stop();

            return Application::EXIT_OK;
        }
    private:
        /**
         * @brief Creates a class data member while using an already existing reference to a MyLogger object.
         * @see MyLogger::getLogger()
         */
        Poco::Logger& logger = MyLogger::getLogger();
};

/**
 * @brief Main method of the application
 * 
 * @param argc NA
 * @param argv It can be ENABLE_DEBUG or DISABLE_DEBUG based on the existence of -v flag of build.sh
 */
int main(int argc, char **argv) {
    // If the script was ran with -v flag, then we will print to the console stream too.
    if (std::string(argv[1]) == "ENABLE_DEBUG") {
        MyLogger::init(true);
    }
    // If not, we will keep just the log files.
    else if (std::string(argv[1]) == "DISABLE_DEBUG") {
        MyLogger::init(false);
    }

    MyServerApp app;

    return app.run(argc, argv);
}