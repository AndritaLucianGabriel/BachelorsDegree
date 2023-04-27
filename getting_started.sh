#!/bin/bash
clear
read -r -d '\0' USAGE <<- EOM
Usage: per default the script doesn't run.
  -i|--install       - installs and configures all the prerequisites for the project
  -v|--verbose       - optional, print more output to the console
  -h|--help          - show this help
EOM

VERBOSE="false"

#### functions
err_exit() { echo -e "$(tput setaf 1)$(tput bold)"ERROR: "$(tput sgr0)$* \n" >&2; exit 1; }
wrn() { echo "$(tput setaf 3)$(tput bold)"WARNING: "$(tput sgr0)$*" >&2; }
msg() { echo -e "$(tput bold)"INFO: "$(tput sgr0)$* \n"; }
msg_ne() { echo -ne "$(tput bold)"INFO: "$(tput sgr0)$*"; }
succ() { echo -e "$(tput setaf 2)$(tput bold)"SUCCESS: "$(tput sgr0)$* \n";}
inf() { echo -e "$(tput bold)"INFO: "$(tput sgr0)$* \n"; }

if [[ $EUID -ne 0 ]]; then
   err_exit "This script must be run with sudo privileges." 1>&2
   exit 1
fi

change_versions() {
    msg "Getting wanted versions for submodules..."
    cd ./libs
        # Poco v1.12.4
    cd ./poco
    git checkout poco-1.12.4-release
    cd ..
        # VCPKG v2023.04.15 Release
    cd ./vcpkg
    git checkout 2023.04.15
    cd ..
        # GCP v2.9.1
    cd ./google-cloud-cpp
    git checkout v2.9.1
    cd ..
    cd ..
}

enviroment_setup() {
    msg "Enviroment setup..."
    git submodule update --init --recursive
    change_versions
    dos2unix ./build.sh ./libs/dart-sass/sass
}

poco_setup() {
    msg "Poco setup..."
    apt-get update
    apt-get install openssl libssl-dev --fix-missing
    apt-get -y update && apt-get -y install git g++ make cmake libssl-dev --fix-missing
    cd ./libs/poco/build
    cmake ..
    cmake --build . --config Release
    cmake --build . --target install
    cd ../../../
}

npm_setup() {
    msg "Npm setup..."
    apt-get install npm --fix-missing
}

gc_cli_setup() {
    msg "GC CLI setup..."
    curl -O https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/google-cloud-cli-427.0.0-linux-x86_64.tar.gz
    tar -xf google-cloud-cli-427.0.0-linux-x86.tar.gz
    ./google-cloud-sdk/install.sh
    source ~/.bashrc
    ./google-cloud-sdk/bin/gcloud init
}

sass_setup() {
    msg "Sass setup..."
    ln -s /mnt/c/Users/gabyl/OneDrive/Desktop/FACULTATE/Anul-4/BachelorsDegree/libs/dart-sass/sass /usr/local/bin/sass
}

vcpkg_setup() {
    msg "VCPKG setup..."
    ./libs/vcpkg/bootstrap-vcpkg.sh
}

gcp_setup() {
    msg "GCP setup (this will take ~1.5h)..."
    ./libs/vcpkg install google-cloud-cpp[core,dialogflow-es]
}

commands=("enviroment_setup" "poco_setup" "npm_setup" "gc_cli_setup" "sass_setup" "vcpkg_setup" "gcp_setup")

install() {
    for command in "${commands[@]}"; do
        # Remove the '_setup' suffix
        cmd_without_suffix=${command%_setup}

        # Capitalize the first letter
        capitalized_cmd="${cmd_without_suffix^}"
        msg "$capitalized_cmd setup..."

        if [ "${VERBOSE}" = "false" ]; then
            eval "$command" > /dev/null
            exit_status=$?
        else
            eval "$command"
            exit_status=$?
        fi
        if [[ $exit_status -ne 0 ]]; then
            err_exit "Failed with exit status $exit_status!\n"
        else
            succ "Done"
        fi
    done
}

execute() {
    case $COMMAND in
        'INSTALL')
            install
        ;;
        'NONE')
            err_exit "No command was given!"
        ;;
    esac
}

func_read_cli_options() {
    while [ $# -ne 0 ]; do
        case "$1" in
            '-i'|'--install')
                COMMAND="INSTALL"
        shift;;
            '-v'|'--verbose')
                # verbose mode
                VERBOSE="true"
        shift;;
            '-h'|'--help') 
                msg "$USAGE"; 
                exit 0 
        ;;

        *) msg "$USAGE"; err_exit "Unknown parameter '$1'" ;;

        '') break ;;
    esac
    done
    execute
}

MY_EXIT_CODE=0;
COMMAND="NONE"
func_read_cli_options "$@";
echo
exit ${MY_EXIT_CODE}