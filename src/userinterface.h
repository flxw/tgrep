#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <stdio.h>

#include <iostream>


class UserInterface
{
public:
    UserInterface();

    static int printHelpMessage();
    static int printErrorMessage(int e);

    static void printPatternMatch(const char* fname, const int &mt, bool pathOnly);
    static void printFileError(const char* fname);

public:

private:

private:
    static const char* version;
    static const char* name;
};

#endif // USERINTERFACE_H
