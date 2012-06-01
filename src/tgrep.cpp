#include <regex.h>

#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "userinterface.h"
#include "configuration.h"

using namespace std;

bool matchAgainstAll(const TagLib::FileRef &fr, const struct Configuration &cfg, int &type);
bool matchAgainstArtist(const TagLib::FileRef &fr, const struct Configuration &cfg);
bool matchAgainstRelease(const TagLib::FileRef &fr, const struct Configuration &cfg);
bool matchAgainstTitle(const TagLib::FileRef &fr, const struct Configuration &cfg);

int main(int argc, char** argv) {

    // create an instance of the object that holds the program parameters
    // and check if the stuff passed was good
    struct Configuration config;

    int mType;
    int parseResult = parseArguments(config, argc, argv);

    if (parseResult != 0) {
        return UserInterface::printErrorMessage(parseResult);
    }

    // now check each file for a match with the given pattern
    for (std::list<char*>::const_iterator it = config.fileList.begin();
            it != config.fileList.end();
            ++it) {
        TagLib::FileRef file(*it, false);

        if (!file.isNull()) {
            mType = 0;
            bool patternMatch = false;

            if (config.match_mode == Configuration::MM_GREEDY) {
                patternMatch = matchAgainstAll(file, config, mType);
            } else {
                patternMatch = true;

                if ((config.match_mode & Configuration::MM_ARTIST) == Configuration::MM_ARTIST) {
                    if (matchAgainstArtist(file, config)) {
                        mType |= 0x100;
                    } else {
                        continue;
                    }
                }

                if ((config.match_mode & Configuration::MM_RELEASE) == Configuration::MM_RELEASE) {
                    if (matchAgainstRelease(file, config)) {
                        mType |= 0x010;
                    } else {
                        continue;
                    }
                }

                if ((config.match_mode & Configuration::MM_TITLE) == Configuration::MM_TITLE) {
                    if (matchAgainstTitle(file, config)) {
                        mType |= 0x001;
                    } else {
                        continue;
                    }
                }
            }

            if (patternMatch) {
                UserInterface::printPatternMatch(*it, mType, config.printPathOnly);
            }
        } else {
            UserInterface::printFileError(*it);
        }
    }
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
