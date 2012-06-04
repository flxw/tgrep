#include <regex.h>
#include <ftw.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "userinterface.h"
#include "configuration.h"

using namespace std;

static int handleDirectoryEntry(const char *fpath,
                                const struct stat *sb,
                                int typeflag,
                                bool init,
                                struct Configuration cfg);
static int handleDirectoryEntryHandler(const char *fpath,
                                       const struct stat *sb,
                                       int typeflag);

static bool matchAgainstAll(const TagLib::FileRef &fr, const struct Configuration &cfg, int &type);
static bool matchAgainstArtist(const TagLib::FileRef &fr, const struct Configuration &cfg);
static bool matchAgainstRelease(const TagLib::FileRef &fr, const struct Configuration &cfg);
static bool matchAgainstTitle(const TagLib::FileRef &fr, const struct Configuration &cfg);

static void handleFileMatching(const char *fname, const struct Configuration &config);

int main(int argc, char** argv) {

    // create an instance of the object that holds the program parameters
    // and check if the stuff passed was good
    struct Configuration config;

    int parseResult = parseArguments(config, argc, argv);

    if (parseResult != 0) {
        return UserInterface::printErrorMessage(parseResult);
    }

    // now check each file for a match with the given pattern
    for (std::list<char*>::const_iterator it = config.fileList.begin();
         it != config.fileList.end();
         ++it) {
        if (config.recurse) {
            // initialize the walker function
            handleDirectoryEntry(NULL, NULL, 0, true, config);

            ftw(*it, handleDirectoryEntryHandler, 20);
        } else {
            handleFileMatching(*it, config);
        }
    }
}

static int handleDirectoryEntry(const char *fpath,
                                const struct stat *sb,
                                int typeflag,
                                bool init=false,
                                struct Configuration cfg=Configuration()) {

    static struct Configuration conf;
    if (init) {
        conf = cfg;
        return 0;
    }
    // only handle entry if file
    if (typeflag == FTW_F) {
        TagLib::FileRef file(fpath, false);

        if (!file.isNull() && file.tag() != NULL) {
            handleFileMatching(fpath, conf);
        }
    }

    return 0;
}

static int handleDirectoryEntryHandler(const char *fpath, const struct stat *sb, int typeflag) {
    return handleDirectoryEntry(fpath, sb, typeflag);
}

bool matchAgainstAll(const TagLib::FileRef &fr, const struct Configuration &cfg, int &type) {
    type = 0;

    bool artistMatch = (regexec(&cfg.pattern, fr.tag()->artist().toCString(true), 0, NULL, 0) == 0);
    bool releaseMatch = (regexec(&cfg.pattern, fr.tag()->album().toCString(true), 0, NULL, 0) == 0);
    bool titleMatch = (regexec(&cfg.pattern, fr.tag()->title().toCString(true), 0, NULL, 0) == 0);

    if (artistMatch)
        type |= 0x100;
    if (releaseMatch)
        type |= 0x010;
    if (titleMatch)
        type |= 0x001;

    return artistMatch || releaseMatch || titleMatch;
}

bool matchAgainstArtist(const TagLib::FileRef &fr, const Configuration &cfg) {
    return regexec(&cfg.artist_pattern, fr.tag()->artist().toCString(true), 0, NULL, 0) == 0;
}

bool matchAgainstRelease(const TagLib::FileRef &fr, const Configuration &cfg) {
    return regexec(&cfg.release_pattern, fr.tag()->album().toCString(true), 0, NULL, 0) == 0;
}

bool matchAgainstTitle(const TagLib::FileRef &fr, const Configuration &cfg) {
    return regexec(&cfg.title_pattern, fr.tag()->title().toCString(true), 0, NULL, 0) == 0;
}

static void handleFileMatching(const char *fname, const Configuration &config) {
    TagLib::FileRef file(fname, false);

    if (!file.isNull() && file.tag() != NULL) {
        int matchType = 0;
        bool patternMatch = false;

        if (config.match_mode == Configuration::MM_GREEDY) {
            patternMatch = matchAgainstAll(file, config, matchType);
        } else {
            patternMatch = true;

            if ((config.match_mode & Configuration::MM_ARTIST) == Configuration::MM_ARTIST) {
                if (matchAgainstArtist(file, config)) {
                    matchType |= 0x100;
                } else {
                    return;
                }
            }

            if ((config.match_mode & Configuration::MM_RELEASE) == Configuration::MM_RELEASE) {
                if (matchAgainstRelease(file, config)) {
                    matchType |= 0x010;
                } else {
                    return;
                }
            }

            if ((config.match_mode & Configuration::MM_TITLE) == Configuration::MM_TITLE) {
                if (matchAgainstTitle(file, config)) {
                    matchType |= 0x001;
                } else {
                    return;
                }
            }
        }

        if (patternMatch) {
            UserInterface::printPatternMatch(fname, matchType, config.printPathOnly);
        }
    } else {
        UserInterface::printFileError(fname);
    }
}
