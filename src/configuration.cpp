#include "configuration.h"

#include <string.h>

// returns 0 on success
// 1 on help-flag set
// 2 on not enough arguments
// 3 on bad pattern
int parseArguments(Configuration &cfg, int argc, char **argv) {
    int pattern_cflags = 0;
    char* pattern      = NULL;

    // parse options
    for (int i = 1; i < argc; ++i) {
        // check for options - they begin with a dash
        if (argv[i][0] == '-'){
            for (int j = strlen(argv[i]); j>0; --j) {
                if (argv[i][j] == 'h') {
                    // help was demanded, return
                    return 1;
                } else if (argv[i][j] == 'i') {
                    pattern_cflags |= REG_ICASE;
                } else if (argv[i][j] == 'E') {
                    pattern_cflags |= REG_EXTENDED;
                }
            }
        } else if (pattern == NULL) {
            // the pattern was not set yet, so it has to be the argument
            // after the options, as it always comes before the path!
            pattern = argv[i];
        } else {
            cfg.fileList.push_back(argv[i]);
        }
    }

    if (pattern == NULL && cfg.fileList.empty()) {
        return 2;
    }

    // assign pattern
    // but...will it blend?
    if (regcomp(&cfg.pattern, pattern, pattern_cflags) != 0) {
        return 3;
    }

    return 0;
}
