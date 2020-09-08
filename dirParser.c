//
// Created by hanyuan on 9/7/20.
//  zhihao shu

#include "dirParser.h"

int parseDir(const char* const rootName, OptionFlags optionFlags){
    uid_t uid = getuid();
    DIR* rootDir;
    struct dirent* procDirent;
    if(!(rootDir = opendir(rootName)))
        printf("Unable to open ");
    while((procDirent = readdir(rootDir)) != NULL){
        if(parsePid(procDirent->d_name, &optionFlags.pid) == 0){
            parseSingleProc(rootName, optionFlags, 1);
        }
        free(optionFlags.pid);
    }
    return 0;
}

int parsePid(const char name[], char** pid){
    const int MAX_LINE = 1024;
    if(!(*pid = (char*)malloc(sizeof(char)*MAX_LINE))){
        printf("Unable to allocate space.");
        return 1;
    }
    unsigned int len;
    if((len = strlen(name))>=MAX_LINE||len==0){
        printf("Dir name length not supported.");
        return 1;
    }
    for(unsigned int i = 0; i < len; i++){
        if(name[i]>='0'&&name[i]<='9'){
            (*pid)[i] = name[i];
        }
        else {
            return 1;
        }
    }
    (*pid)[len] = '\0';
    return 0;
}
