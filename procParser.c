//
// Created by Hanyuan Wu on 9/7/20.
// team members: Hanyuan Wu, Zhihao Shu

#include "procParser.h"

int parseSingleProc(const char* const rootName, const OptionFlags optionFlags, const int checkUserFlag){
    FILE* fp;
    char procDir[MAX_LINE], fullDir[MAX_LINE];
    if(!strncpy(procDir, rootName, MAX_LINE)||(procDir[MAX_LINE-1] = '\0', !strcat(procDir, optionFlags.pid))){
        printf("Library call failed 1.\n");
        return 1;
    }
    // now all strings are safe, use strcpy for convenience
    if(checkUserFlag){
        if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/status")){
            printf("String action failed.\n");
            return 1;
        }
        if(checkUser(fullDir) != 0)
            return 0; // skip this proc
    }
    //stat
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/stat")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed 2.\n");
        return 1;
    }
    char state;
    char cmdLine[MAX_LINE];
    unsigned long utime, stime, size;
    if(fscanf(fp, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu",
              &state, &utime, &stime) != 3)
        printf("stat file format failed.\n");
    if (fclose(fp) < 0) {
        perror("file close error");
        exit(1);
    }
    fp = NULL;
    //statm
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/statm")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed 3.\n");
        return 1;
    }
    if(fscanf(fp, "%lu",&size) != 1)
        printf("statm file format failed.\n");
    if (fclose(fp) < 0) {
        perror("file close error");
        exit(1);
    }
    fp = NULL;
    //cmdline
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/cmdline")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed 4.\n");
        return 1;
    }
    unsigned int fSize;
    fSize = fread(cmdLine, sizeof(char), MAX_LINE, fp);
    if(fSize == MAX_LINE)
        printf("cmdline length exceed limit");
    for(int i = 0; i < fSize; i++){
        if(cmdLine[i] == '\0'&&i != fSize-1)
            cmdLine[i] = ' ';
    }
    cmdLine[fSize] = '\0';
    if (fclose(fp) < 0) {
        perror("file close error");
        exit(1);
    }
    fp = NULL;

    
    // output
    printf("%-10s", optionFlags.pid);
    if(optionFlags.s)
        printf("%-10c", state);
    if(optionFlags.U)
        printf("%-10lu", utime);
    if(optionFlags.S)
        printf("%-10lu", stime);
    if(optionFlags.v)
        printf("%-10lu", size);
    if(optionFlags.c)
        printf("%s ",cmdLine);
    if(optionFlags.m){
        printf("\t%llx: ", optionFlags.addr);
        unsigned char *mem = (unsigned char*) malloc(sizeof(unsigned char)*optionFlags.len);
        long long realLen = 0;
        if(parseMem(&mem, &realLen, optionFlags, procDir) == 0) {
            for (int i = 0; i < realLen ; i++) {
                if (i != 0)
                    printf(" ");
                printf("%02x", mem[i]);
            }

            free(mem);
            mem = NULL;
        }
        else
            printf("NOT AVAILABLE");
    }
    printf("\n");
}

int parseMem(unsigned char** mem, long long* realLen, const OptionFlags optionFlags, const char* const procDir){
    char buf[MAX_LINE], fullDir[MAX_LINE];
    long long memBegin, memEnd;
    int mapsFlag = 0;
    FILE* fp;
    //maps
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/maps")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed 3.\n");
        return 1;
    }

    while(fgets(buf, MAX_LINE, fp)){
        if(sscanf(buf, "%llx-%llx%*s", &memBegin, &memEnd) == 2){
            if(optionFlags.addr >= memBegin && optionFlags.addr < memEnd){
                *realLen = memEnd-optionFlags.addr;
                *realLen = (*realLen < optionFlags.len? *realLen: optionFlags.len);
                mapsFlag = 1;
            }
        }
    }
    if (fclose(fp) < 0) {
        perror("file close error");
        exit(1);
    }
    fp = NULL;
    if(!mapsFlag)
        return 1;
    //read from mem by using ptrace
    if (!strcpy(fullDir, procDir) || !strcat(fullDir, "/mem")) {
        printf("Library call failed 5.\n");
        return 1;
    }


    pid_t pid = atoi(optionFlags.pid);

    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1)
        return 1;
    waitpid(pid, NULL, 0);
    int f_read; //permission denied at this line
    if ((f_read = open(fullDir, O_RDONLY)) < 0)
        return 1;
    if (lseek(f_read, optionFlags.addr, SEEK_SET) < 0)
        return 1;
    if (read(f_read, *mem, *realLen) < 0)
        return 1;
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    //close the file
    if (close(f_read) < 0) {
        perror("file close error");
        exit(1);
    }
    return 0;
}



int checkUser(const char* const fullDir){
    char buf[MAX_LINE];
    uid_t uid = getuid();
    FILE* fp;
    if(!(fp = fopen(fullDir, "r"))){
        printf("Unable to open file.\n");
    }
    int sameUidFlag = 0;
    while(fgets(buf, MAX_LINE, fp)){
        if(strncmp(buf, "Uid:", 4) == 0){
            // close fp before return
            if (fclose(fp) < 0) {
                perror("file close error");
                exit(1);
            }
            fp = NULL;
            uid_t realUid;
            if(sscanf(buf, "%*s\t%d", &realUid)!=1){
                // Unable to read uid: should belong to another user
                //printf("Unable to read uid.\n");
                return 1;
            }
            if(realUid == uid){
                return 0;
            }
            else
                return 1;
        }
    }
    if (fclose(fp) < 0) {
        perror("file close error");
        exit(1);
    }
    fp = NULL;
    // if no line contains uid
    return 1;
}

