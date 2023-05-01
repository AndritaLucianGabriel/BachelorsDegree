    We create a new key for an already existing service account (the default one)
https://console.cloud.google.com/iam-admin/serviceaccounts/details/106927336924460841238/keys?walkthrough_id=iam--create-service-account-keys&project=licenta-383311&supportedpurview=project
    After we download the json we store in ./res. Name is not relevant as long as there we'll be only one json 
file in that folder's root.

    VCPKG v2023.04.15 Release
https://github.com/microsoft/vcpkg.git

    Steps for enviroment setup:
Steps:
    git submodule update --init --recursive
    git checkout to a specific version (for each library)
    dos2unix for ./build.sh and ./libs/dar-sass/sass

        Poco v1.12.4
    1. sudo apt-get install openssl libssl-dev
    2. sudo apt-get -y update && sudo apt-get -y install git g++ make cmake libssl-dev
    3. cd ./libs/poco/build
    4. cmake ..
    5. cmake --build . --config Release
    6. sudo cmake --build . --target install

        Npm v8.19.2
    1. sudo apt-get install npm

        GC CLI v427.0.0
    1. curl -O https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/google-cloud-cli-427.0.0-linux-x86_64.tar.gz
    2. tar -xf google-cloud-cli-427.0.0-linux-x86.tar.gz
    3. ./google-cloud-sdk/install.sh
    4. source ~/.bashrc
    5. ./google-cloud-sdk/bin/gcloud init

        SASS v1.58.3
    1. sudo ln -s /mnt/c/Users/gabyl/OneDrive/Desktop/FACULTATE/Anul-4/BachelorsDegree/libs/dart-sass/sass /usr/local/bin/sass

        VCPKG v2023.04.15 Release
    1. ./libs/vcpkg/bootstrap-vcpkg.sh

        GCP v2.9.1 (only core and dialogflow-es)
    1. cd ./libs/vcpkg
    2. ./vcpkg install google-cloud-cpp[core,dialogflow-es]

Optional steps(requires gc cli installed(steps 0.2-0.6)):
gcloud config set project PROJECT_ID
gcloud config set billing/quota_project PROJECT_ID

    How the chatbot works:
Docs: https://googleapis.dev/cpp/google-cloud-dialogflow_es/latest/index.html
Notes: am nevoie de o sesiune care reprezinta interactiunea dintre agentul Dialogflow si end-user, se stocheaza contextul
timp de 20 min (sursa: https://cloud.google.com/dialogflow/es/docs/api-overview). 

    How to build:
Open the Proiect folder as workspace, make sure you have executable rights on the build.sh file and it's formatted for unix (dos2unix) and then run it using: ./build.sh -b ./google-cloud-sdk/bin/gcloud init ./google-cloud-sdk/install.sh