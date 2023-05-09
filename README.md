# Bachelor's Degree Documentation
## Introduction
&emsp;This is a C++ template application that uses Google Cloud Platform's Dialogflow ES for human-chatbot interactions in different contexts, depending on the use cases of the maintainer. The project was created as a Bachelor Degree for the Computers and Information Technology specialization of the Faculty of Mathematics and Computer Science from the University of Bucharest, Romania, author <a href="https://github.com/AndritaLucianGabriel">Andrita Lucian-Gabriel</a>.
## Installing the libraries
&emsp;The user must have an access key for a service account on Google Cloud Platform in order to access the Dialogflow ES API. This key must be placed under res folder. Read more <a href="https://cloud.google.com/iam/docs/keys-create-delete">here</a>.

&emsp;This key will be set to **GOOGLE_APPLICATION_CREDENTIALS** enviroment variable that will work as a login credentials for the platform.

&emsp;In the root directory of the project, there is a bash script called **getting_started.sh** (Note: if the script can't be run on a Unix platform, I recommend first to check for the bad line endings so run **dos2unix** on the script and check again). When ran from an Unix platform, this will install all the required dependencies (see [First Time Setup](#first-time-setup)). After it's completed, the user still has to manually run the first time setup for the GC CLI:
```
./google-cloud-sdk/install.sh
source ~/.bashrc
./google-cloud-sdk/bin/gcloud init
```
Note: the first-time setup will take a **long** time (especially on older machines).
## First time setup 
The script's (**getting_started.sh**) flow is as follows:

**Important note**: It must be run with **-i** or **--install** flag and after it's completion, the user has to run the setup for the gcloud (see above steps).
1. Eviroment setup:

    1. The submodules are initialized.
    2. Based on the missmatch of line endings, both the **build.sh** and **sass** executable will be converted to unix-like.
    3. Installation of both **doxygen** and it's dependency **graphviz**.
2. Reverting the libs to the wanted versions:

    1. **Poco v1.12.4** is being used.
    2. **VCPKG v2023.04.15 Release** is being used.
    3. **GCP v2.9.1** is being used.
3. Poco setup. Read more <a href="https://github.com/pocoproject/poco">here</a>:

    1. Installation of the dependencies (**openssl, libssl-dev, git, g++, make, cmake**).
    2. Configuring CMakeLists.txt.
    3. Building the configuration file.
    4. Installing the library.
4. Npm setup:

    1. Intallation of **npm** via the default package manager.
5. Google Cloud CLI setup. Read more <a href="https://cloud.google.com/sdk/docs/install">here</a>:

    1. Download the archive containing the source code.
    2. Extracting the archive.
6. Sass setup:

    1. Create a symbolic link(symlink) and force it's creation even if it exists to the provided binary (see **libs/sass**).
7. VCPKG setup. Read more <a href="https://github.com/Microsoft/vcpkg">here</a>:

    1. Installation of the dependencies (**curl, zip, unzip, tar**).
    2. First time setup of the vcpkg manager via it's startup script (**libs/vcpkg/bootstrap-vcpkg.sh**).
8. Google Cloud Platform setup. Read more <a href="https://github.com/googleapis/google-cloud-cpp/tree/main/google/cloud/dialogflow_es/quickstart">here</a>:

    1. Intallation of the actual modules that we are going to use in the project (**core** and **dialogflow-es**) via **vcpkg**.
## How to build
From the root directory of the project, we can use the **build.sh** script to build and run the application. By default, the server will use **9090** port, but it can be changed from the **constants.h** (see **globals::serverPort**).

There are a couple of options that you can run the script with:
1. -b or --build => build and run the project
2. -bscss or --build-scss => compile only the scss files to update the HTML's aspect
3. -docs or --generate-docs => generate the technical documentation using Doxygen
4. -c or --clean => clean the project
5. -v or --verbose => optional, print more output to the console
6. -h or --help => show this help

Note: do **NOT** run **-c** with other options since the cleaning is meant to be a solo process.
## Detailed information
Both scripts share the same code regarding user input and error handling.
1. They are processed by *func_read_cli_options* and cross-referenced with the default list (it's mostly different for both of them since they have distinct roles, but **-v** and **-h** are common since they have a shared utility).

    1. Common flags:
        1. The **verbose** flag allows for the scripts to print more information (both from them and the application logs) to the console. This option will set the **VERBOSE** global variable to true.
        2. The **help** flag will just print the usage help.
    2. Specific flags for **getting_started.sh**:
        1. The **install** flag installs and configures all the prerequisites for the project.
    3. Specific flags for **build.sh**:
        1. The **build** flag will build and run the project.
        2. The **build-scss** flag will compile just the scss files to update the HTML's aspect.
        3. The **generate-docs** flag will generate the technical documentation using Doxygen.
        4. The **clean** flag will clean the project from temporary files, logs, executables, binaries and so on.
2. If the flag exists in the wanted list COMMAND variable is stores the wanted action and then it is passed to *execute*.
3. The *execute* function will then call the method that has the wanted functionality. This level of abstractization allows to chain multiple commands under the same name (if wanted).

Note: From now on, we will analyse each script individually since they have different methods for their different use cases. 

1. For **getting_started.sh**:

    The *execute* method can only call the *install* function. This method will go through a list of commands and install the prerequisites (see [First Time Setup](#first-time-setup)).
2. For **build.sh**:

    The *execute* method can call one of the following functions:

    1. The \a compile_sass function will iterate over all the scss files from \b res/scss subfolder and will compile them and generate the wanted css files in \b res/css subfolder. These files will not contain the auto-generated css maps because there are not a lot of css to begin with and there's no need to create extra logic for the server to handle these requests.
    2. The *generate_docs* function will generate the technical documentation. In the Doxyfile, the following tags have been changed:
        1. PROJECT_NAME = "Bachelor's Degree"
        2. PROJECT_BRIEF = "A C++ banking application that uses Google Cloud Platform's Dialogflow ES for human-chatbot interactions in a banking context."
        3. OUTPUT_DIRECTORY = "./res/docs"
        4. FULL_PATH_NAMES = NO
        5. BUILTIN_STL_SUPPORT = YES
        6. EXTRACT_ALL = YES
        7. EXTRACT_PRIVATE = YES
        8. EXTRACT_STATIC = YES
        9. INPUT = ./res
        10. RECURSIVE = YES
        11. HTML_OUTPUT = .
        12. DISABLE_INDEX = YES
        13. GENERATE_TREEVIEW = YES
        14. GENERATE_LATEX = NO
        15. CALL_GRAPH = YES
        16. CALLER_GRAPH = YES
        17. ALPHABETICAL_INDEX = NO
    3. The *build* function is top-level method that has the goal of building and running the final executable. That executable is called **licenta_EXECUTABLE** and can be found in the project's root directory. It can be run with two flags: **DISABLE_DEBUG** or **ENABLE_DEBUG**. These flags depend on the **VERBOSE** global. If it is *true*, then the project's logging system (**MyLogger** - a wrapper over [Poco's Loger](https://docs.pocoproject.org/current/Poco.Logger.html) class) will create a [Poco::ConsoleChannel](https://docs.pocoproject.org/current/Poco.ConsoleChannel.html) that will be provided to a [Poco::SplitterChannel](https://docs.pocoproject.org/current/Poco.SplitterChannel.html). This way, whenever will be generated a new log, it will also be printed into the console, besides the **logs** folder.
        1. It calls the *compile_sass* function. See above for further explications.
        1. The *compile_cpp* function first configures the CMakeLists.txt file and passes the *CMAKE_TOOLCHAIN_FILE* variable to the Google Cloud Platform's CMake. Because this is not used directly in this CMake, we will also add the **--no-warn-unused-cli** flag to surpress the warning regarding this variable. If the configuring is succesfull, then actual building of the project happens.
        2. The *configure_gcp* function will export the *GOOGLE_APPLICATION_CREDENTIALS* variable so it is visible to every process and sub-process that starts from the script.

&emsp;The project's CMake uses *C++17* standard, and output's the executable in the project's root. The libraries used for the linking are **Poco::Foundation Poco::Net Poco::Util google-cloud-cpp::dialogflow_es**.