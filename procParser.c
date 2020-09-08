//
// Created by hanyuan on 9/7/20.
//

#include "procParser.h"

int parseSingleProc(const char* const rootName, const OptionFlags optionFlags, const int checkUserFlag){
    const int MAX_LINE = 2048;
    FILE* fp;
    char procDir[MAX_LINE], fullDir[MAX_LINE];
    if(!strncpy(procDir, rootName, MAX_LINE)||(procDir[MAX_LINE-1] = '\0', !strcat(procDir, optionFlags.pid))){
        printf("Library call failed.\n");
        return 1;
    }
    // now all strings are safe, use strcpy for convenience
    if(checkUserFlag){
        if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/status")){
            printf("String action failed.\n");
            return 1;
        }
        if(!checkUser(fullDir))
            return 0; // skip this proc
    }
    // read info
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/stat")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed.\n");
        return 1;
    }
    char state;
    char cmdLine[MAX_LINE];
    unsigned long utime, stime, size;
    if(fscanf(fp, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu",
              &state, &utime, &stime) != 3)
        printf("stat file format failed.\n");
    fclose(fp);
    fp = NULL;
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/statm")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed.\n");
        return 1;
    }
    if(fscanf(fp, "%lu",&size) != 1)
        printf("statm file format failed.\n");
    fclose(fp);
    fp = NULL;
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/cmdline")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed.\n");
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fSize = ftell(fp);
    if(fSize>=MAX_LINE||fread(cmdLine, sizeof(char), fSize, fp)<fSize){
        printf("Unable to read cmdLine\n");
    }
    for(int i = 0; i < fSize; i++){
        if(cmdLine[i] == '\0'&&i != fSize-1)
            cmdLine[i] = ' ';
    }
    cmdLine[fSize] = '\0';
    fclose(fp);
    fp = NULL;
    // output
    printf("%s\t", optionFlags.pid);
    if(optionFlags.s)
        printf("%c\t", state);
    if(optionFlags.s)
        printf("%c\t", state);
    if(optionFlags.U)
        printf("%lu\t", utime);
    if(optionFlags.S)
        printf("%lu\t", stime);
    if(optionFlags.v)
        printf("%lu\t", size);
    if(optionFlags.c)
        printf("%s",cmdLine);
    printf("\n");
}

/*
 * Return: 1 if passed user check, 0 if not
 */
int checkUser(const char* const fullDir){
    const int MAX_LINE = 2048;
    char buf[MAX_LINE];
    uid_t uid = getuid();
    FILE* fp;
    if(!(fp = fopen(fullDir, "r"))){
        printf("Unable to open file.\n");
    }
    int sameUidFlag = 0;
    while(fgets(buf, MAX_LINE, stdin)){
        if(strncmp(buf, "Uid:", 4) == 0){
            fclose(fp);
            fp = NULL;
            uid_t realUid;
            if(sscanf(buf, "%*s\t%d", &realUid)!=2){
                printf("Unable to read uid.\n");
                return 1;
            }
            if(realUid == uid)
                sameUidFlag = 1;
            else
                return 0;
            break;
        }
    }
    // No Uid found in status
    if(!sameUidFlag){
        fclose(fp);
        fp = NULL;
        return 1;
    }
}

