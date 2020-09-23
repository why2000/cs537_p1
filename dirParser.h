//
// Created by Hanyuan Wu on 9/7/20.
// team members: Hanyuan Wu, Zhihao Shu

#ifndef CS537_P1_DIRPARSER_H
#define CS537_P1_DIRPARSER_H
#include <dirent.h>
#include "procParser.h"
int parseDir(const char* rootName, OptionFlags optionFlags);
int parsePid(const char name[], char** pid);
#endif //CS537_P1_DIRPARSER_H
