//
// Created by hanyuan on 9/7/20.
//

#ifndef CS537_P1_OPTIONPARSER_H
#define CS537_P1_OPTIONPARSER_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    int p;
    int s;
    int U;
    int S;
    int v;
    int c;
    char* pid;
} OptionFlags;

/* Input:
 *  @flag: pointer to the flag to set
 * Output:
 *  @flag: pointer to the flag to set
 * Return: 0 if flag set succeeded, otherwise failed
 */
int setBinaryOption(int* flag);

/* Input:
 *  @argc: command line argc
 *  @argv: command line argv
 *  @optionFlags: the flags to set
 * Output:
 *  @optionFlags: the flags to set
 * Return: 0 if flags set succeeded, otherwise failed
 */
int parseOption(int argc, char* argv[], OptionFlags* optionFlags);


#endif //CS537_P1_OPTIONPARSER_H
