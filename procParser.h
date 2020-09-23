//
// Created by hanyuan on 9/7/20.
// zhihao shu

#ifndef CS537_P1_PROCPARSER_H
#define CS537_P1_PROCPARSER_H
#include "optionParser.h"
#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

/*
 * Return: 0 if parse succeeded, otherwise not
 */
int parseSingleProc(const char* rootName, OptionFlags optionFlags, int checkUser);

/*
 * Return: 0 if mem parse succeeded, otherwise not
 */
int parseMem(unsigned char** mem, long long* realLen, OptionFlags optionFlags, const char* procDir);

/*
 * Return: 0 if uid==current user's id, otherwise not
 */
int checkUser(const char* fullDir);

#endif //CS537_P1_PROCPARSER_H
