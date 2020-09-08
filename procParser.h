//
// Created by hanyuan on 9/7/20.
//

#ifndef CS537_P1_PROCPARSER_H
#define CS537_P1_PROCPARSER_H
#include "optionParser.h"

int parseSingleProc(const char* rootName, OptionFlags optionFlags, int checkUser);
int checkUser(const char* fullDir);

#endif //CS537_P1_PROCPARSER_H
