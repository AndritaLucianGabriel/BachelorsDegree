#!/bin/bash

clear
read -r -d '\0' USAGE <<- EOM
Usage: per default the script doesn't run.
  -b|--build          - build and run the project
  -bscss|--build-scss - compile only the scss files to update the html's aspect
  -c|--clean          - clean the project
  -v|--verbose        - optional, print more output to the console
  -h|--help           - show this help
Notes: do NOT run -c with other options since the cleaning is meant to be a solo process.
EOM

VERBOSE="false"

#### functions
err_exit() { echo -e "$(tput setaf 1)$(tput bold)"ERROR: "$(tput sgr0)$* \n" >&2; exit 1; }
wrn() { echo "$(tput setaf 3)$(tput bold)"WARNING: "$(tput sgr0)$*" >&2; }
msg() { echo -e "$(tput bold)"INFO: "$(tput sgr0)$* \n"; }
msg_ne() { echo -ne "$(tput bold)"INFO: "$(tput sgr0)$*"; }
succ() { echo -e "$(tput setaf 2)$(tput bold)"SUCCESS: "$(tput sgr0)$* \n";}
inf() { echo -e "$(tput bold)"INFO: "$(tput sgr0)$* \n"; }

clean() {
    msg "Cleaning..."
    rm -rf "build" "licenta_EXECUTABLE" "logs"
    GOOGLE_APPLICATION_CREDENTIALS="" # we ll clean the enviroment variable for the GCP's credentials
    if [ $? -ne 0 ]; then
        err_exit "Failed with exit status $?!\n"
        exit
    else
        succ "Done"
    fi
}

compile_sass() {
    msg "Compiling sass files..."
    sass "res/scss/text.scss" "res/css/text.css" > /dev/null
    if [ $? -ne 0 ]; then
        err_exit "Failed with exit status $?!\n"
        exit
    else
        succ "Done"
    fi
}

compile_cpp() {
    msg "Configuring cmake files..."
    if [ "${VERBOSE}" = "false" ]; then
        #### --no-warn-unused-cli for no warnings regarding unused clis, -B build to specify the output folder
        cmake "CMakeLists.txt" --no-warn-unused-cli -B build -DCMAKE_TOOLCHAIN_FILE=../libs/vcpkg/scripts/buildsystems/vcpkg.cmake > /dev/null
    else
        cmake "CMakeLists.txt" --no-warn-unused-cli -B build -DCMAKE_TOOLCHAIN_FILE=../libs/vcpkg/scripts/buildsystems/vcpkg.cmake
    fi
    if [ $? -ne 0 ]; then
        err_exit "Failed with exit status $?!\n"
        exit
    else
        cd "./build"
        succ "Done"
        msg "Building project..."
        if [ "${VERBOSE}" = "false" ]; then
            make > /dev/null
        else
            make
        fi
        if [ $? -ne 0 ]; then
            err_exit "Failed with exit status $?!\n"
            exit
        else
            succ "Done"
        fi
        cd ..
    fi
}

configure_gcp() {
    msg "Configuring enviroment for GCP..."
    LOGIN_FILE=$(find ./res -maxdepth 1 -name *.json)
    if [ -f "$LOGIN_FILE" ];
    then
        export GOOGLE_APPLICATION_CREDENTIALS="$LOGIN_FILE"
        succ "Done"
    else
        wrn "No login key found! Create a new login key for a service account and put it in ./res!"
        err_exit "Failed with exit status $?!\n"
    fi
}

build() {
    compile_sass
    compile_cpp
    configure_gcp
    if [ "${VERBOSE}" = "false" ]; then
        clear
        ./licenta_EXECUTABLE DISABLE_DEBUG
    else
        ./licenta_EXECUTABLE ENABLE_DEBUG
    fi
}

execute() {
    case $COMMAND in
        'BUILD')
            build
        ;;
        'CLEAN')
            clean
        ;;
        'BUILD-SCSS')
            compile_sass
        ;;
        'NONE')
            err_exit "No command was given!"
        ;;
    esac
}

func_read_cli_options() {
    while [ $# -ne 0 ]; do
        case "$1" in
            '-c'|'--clean')
                COMMAND="CLEAN"
        shift;;
            '-b'|'--build')
                COMMAND="BUILD"
        shift;;
            '-bscss'|'--build-scss')
                COMMAND="BUILD-SCSS"
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