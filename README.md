    Boost v1.67.0
we use only header files for now
    
    Poco v1.12.4
https://github.com/pocoproject/poco/tree/master
sudo apt-get install openssl libssl-dev
sudo apt-get -y update && sudo apt-get -y install git g++ make cmake libssl-dev
cd libs/poco/build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

    SASS v1.58.3
vim ~/.bashrc
export PATH="absolute_path_to_executable"

    NPM v8.19.2
vim ~/.bashrc
export PATH=$PATH:/mnt/c/Users/INSERT-USER/AppData/Roaming/npm

    GCP v2.9.1
https://github.com/googleapis/google-cloud-cpp

    GC CLI v427.0.0
https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/google-cloud-cli-427.0.0-linux-x86_64.tar.gz
    Commands for quickstart dialogflow-es
gcloud config set project PROJECT_ID
gcloud config set billing/quota_project PROJECT_ID
    We create a new key for an already existing service account (the default one)
Docs: https://cloud.google.com/iam/docs/keys-create-delete#iam-service-account-keys-create-console
    After we download the json we set the enviroment variable for authentication for the GCP apis
export GOOGLE_APPLICATION_CREDENTIALS="/path/to/your/keyfile.json"

    VCPKG v2023.04.15 Release
https://github.com/microsoft/vcpkg.git

    How to build:
Open the Proiect folder as workspace, make sure you have executable 
rights on the build.sh file and then run it using: ./build.sh -b