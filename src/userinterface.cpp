#include "userinterface.h"

const char* UserInterface::version = "0.2";
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
    std::cout << " -o\t Print filenames only" << std::endl;
    std::cout << std::endl;
    std::cout << " -a PATTERN\t Use this pattern to match agains the artist tag" << std::endl;
    std::cout << " -r PATTERN\t Use this pattern to match agains the release tag" << std::endl;
    std::cout << " -t PATTERN\t Use this pattern to match agains the title tag" << std::endl;
    std::cout << "Note that these patterns are AND connected. That means:" << std::endl;
    std::cout << "If one of the given patterns matches and the other" << std::endl;
    std::cout << "pattern does not, nothing is printed" << std::endl;

    return 0;
}

int UserInterface::printErrorMessage(int e) {
    const char* msg = 0;

    switch (e) {
    case 1: printHelpMessage(); return 0; break;
    case 2: msg = "Please pass at least 2 arguments! Exiting..."; break;
    case 3: msg = "The pattern is invalid! Exiting..."; break;
    case 4: msg = "The pattern for matching the artist tag is invalid! Exiting..."; break;
    case 5: msg = "The pattern for matching the album/release tag is invalid! Exiting..."; break;
    case 6: msg = "The pattern for matching the title tag is invalid! Exiting..."; break;
    }

    std::cerr << UserInterface::name << ": " << msg << std::endl;

    return e;
}

void UserInterface::printPatternMatch(const char *fname, const int &mt, bool pathOnly) {
    if (pathOnly) {
        std::cout << fname << std::endl;
    } else {
        std::cout << UserInterface::name << ": `" << fname << "' matched: ";

        if ((mt & 0x100) == 0x100) {
            std::cout << "artist ";
        }

        if ((mt & 0x010) == 0x010) {
            std::cout << "album ";
        }

        if ((mt & 0x001) == 0x001) {
            std::cout << "title";
        }

        std::cout << std::endl;
    }
}

void UserInterface::printFileError(const char *fname) {
    std::cerr << UserInterface::name << ": Could not read file `" << fname << "'" << std::endl;
}
