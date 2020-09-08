//
// Created by hanyuan on 9/7/20.
//

#include "procParser.h"
#include "optionParser.h"
#include "dirParser.h"

int main(int argc, char* argv[]){

    OptionFlags optionFlags = {.p=0,.s=0,.U=1,.S=1,.v=0,.c=1};
    const char* rootName = "/proc/";
    if(parseOption(argc, argv, &optionFlags) != 0)
        return 1;
    if(optionFlags.p){
        parseSingleProc(rootName, optionFlags, 0);
        free(optionFlags.pid);
    }
    else{
        parseDir(rootName, optionFlags);
    }

    return 0;
}


