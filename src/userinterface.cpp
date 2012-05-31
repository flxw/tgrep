#include "userinterface.h"

const char* UserInterface::version = "0.1";
const char* UserInterface::name    = "tgrep";

UserInterface::UserInterface() {
}

int UserInterface::printHelpMessage() {
    std::cout << UserInterface::name << " version " << UserInterface::version;
    std::cout << std::endl;
    std::cout << "Usage: " << UserInterface::name << "[OPTIONS] PATTERN FILE";
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << " -i\t Make pattern-matching insensitive" << std::endl;
    std::cout << " -E\t Use extended regular expressions" << std::endl;

    return 0;
}

int UserInterface::printErrorMessage(int e) {
    const char* msg = 0;

    switch (e) {
    case 1: printHelpMessage(); return 0; break;
    case 2: msg = "Please pass at least 2 arguments! Exiting..."; break;
    case 3: msg = "The pattern is invalid! Exiting..."; break;
    }

    std::cerr << UserInterface::name << ": " << msg << std::endl;

    return e;
}

void UserInterface::printPatternMatch(const char *fname) {
    std::cout << UserInterface::name << ": `" << fname << "'' matched the given pattern!" << std::endl;
}
