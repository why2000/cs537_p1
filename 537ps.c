//
// Created by Hanyuan Wu on 9/7/20.
// team members: Hanyuan Wu, Zhihao Shu

#include "procParser.h"
#include "optionParser.h"
#include "dirParser.h"

int main(int argc, char* argv[]){

    OptionFlags optionFlags = {.p=0,.s=0,.U=1,.S=0,.v=0,.c=1,.m=0};
    const char* rootName = "/proc/";
    if(parseOption(argc, argv, &optionFlags) != 0)
        return 1;
    printf("%-10s", "PID");
    if(optionFlags.s)
        printf("%-10s", "STATE");
    if(optionFlags.U)
        printf("%-10s", "UTIME");
    if(optionFlags.S)
        printf("%-10s", "STIME");
    if(optionFlags.v)
        printf("%-10s", "SIZE");
    if(optionFlags.c)
        printf("%-10s", "CMDLINE");
    if(optionFlags.m)
        printf("%-10s", "MEM");
    printf("\n");
    if(optionFlags.p){
        for(int i = 0; i < optionFlags.p; i++) {
            if(!(optionFlags.pid = (char*)malloc(sizeof(char)*MAX_LINE))
               ||!strncpy(optionFlags.pid, optionFlags.pids[i], MAX_LINE)){
                //printf("Unable to parse pid.\n");
                return 1;
            }
            parseSingleProc(rootName, optionFlags, 0);
            free(optionFlags.pid);
        }
    }
    else{
        parseDir(rootName, optionFlags);
    }

    return 0;
}


