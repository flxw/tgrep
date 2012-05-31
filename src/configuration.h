#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <regex.h>

#include <list>

struct Configuration {
    bool  greedy;

    int   errorCode;

    regex_t pattern;
    regex_t artist_pattern;
    regex_t release_pattern;
    regex_t title_pattern;

    std::list<char*> fileList;
};

int parseArguments(struct Configuration& cfg, int argc, char** argv);

#endif // CONFIGURATION_H
