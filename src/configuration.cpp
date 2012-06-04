#include "configuration.h"

#include <string.h>

// returns 0 on success
// 1 on help-flag set
// 2 on not enough arguments
// 3 on bad pattern
int parseArguments(Configuration &cfg, int argc, char **argv) {
    cfg.match_mode         = 0;
    cfg.printPathOnly     = false;
    cfg.recurse           = false;
    int pattern_cflags    = 0;
    char* pattern         = NULL;
    char* artist_pattern  = NULL;
    char* release_pattern = NULL;
    char* title_pattern   = NULL;

    // parse options
    for (int i = 1; i < argc; ++i) {
        // check for options - they begin with a dash
        if (argv[i][0] == '-'){
            for (int j = strlen(argv[i])-1; j>0; --j) {
                if (argv[i][j] == 'h') {
                    return 1; // help was demanded, return
                } else if (argv[i][j] == 'i') {
                    pattern_cflags |= REG_ICASE;
                } else if (argv[i][j] == 'E') {
                    pattern_cflags |= REG_EXTENDED;
                } else if (argv[i][j] == 'o') {
                    cfg.printPathOnly = true;
                } else if (argv[i][j] == 'R') {
                    cfg.recurse = true;
                } else if (argv[i][j] == 'a') {
                    if (i+1 < argc)
                        artist_pattern = argv[++i];
                    else
                        return 3;
                } else if (argv[i][j] == 'r') {
                    if (i+1 < argc)
                        release_pattern = argv[++i];
                    else
                        return 3;
                } else if (argv[i][j] == 't') {
                    if (i+1 < argc)
                        title_pattern = argv[++i];
                    else
                        return 3;
                }
                /*else if (argv[i][j] == '-') {
                    char *substr = strndup(argv[i]+2, 5);

                    if (strcmp(substr, "artis") == 0) {
                        artist_pattern = argv[++i];
                    } else if (strcmp(substr, "relea") == 0) {
                        release_pattern = argv[++i];
                    } else if (strcmp(substr, "title") == 0) {
                        title_pattern = argv[++i];
                    }

                    delete substr;
                }*/
            }
        } else if (artist_pattern == NULL &&
                   release_pattern == NULL &&
                   title_pattern   == NULL   &&
                   pattern         == NULL) {
            // the pattern was not set yet, so it has to be the argument
            // after the options, as it always comes before the path!
            pattern = argv[i];
        } else {
            if (cfg.recurse) {
                cfg.fileList.push_back(argv[i]);
            } else {
                cfg.fileList.push_back(argv[i]);
            }
        }
    }

    // compile the patterns...will they blend?
    if (pattern != NULL) {
        if (regcomp(&cfg.pattern, pattern, pattern_cflags) != 0) {
            return 3;
        } else {
            cfg.match_mode = Configuration::MM_GREEDY;
        }
    } else {
        if (artist_pattern != NULL) {
            if (regcomp(&cfg.artist_pattern, artist_pattern, pattern_cflags) != 0) {
                return 4;
            } else {
                cfg.match_mode |= Configuration::MM_ARTIST;
            }
        }

        if (release_pattern != NULL) {
            if (regcomp(&cfg.release_pattern, release_pattern, pattern_cflags) != 0) {
                return 5;
            } else {
                cfg.match_mode |= Configuration::MM_RELEASE;
            }
        }

        if (title_pattern != NULL) {
            if (regcomp(&cfg.title_pattern, title_pattern, pattern_cflags) != 0) {
                return 6;
            } else {
                cfg.match_mode |= Configuration::MM_TITLE;
            }
        }
    }

    if (pattern == NULL && cfg.fileList.empty()) {
        return 2;
    }

    return 0;
}
