//
// Created by hanyuan on 9/7/20.
//zhihao shu

#include "procParser.h"
#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>


int parseSingleProc(const char* const rootName, const OptionFlags optionFlags, const int checkUserFlag){
    const int MAX_LINE = 2048;
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
        if(!checkUser(fullDir))
            return 0; // skip this proc
    }
    // read info
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
    fclose(fp);
    fp = NULL;
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/statm")||!(fp = fopen(fullDir, "r"))){
        printf("Library call failed 3.\n");
        return 1;
    }
    if(fscanf(fp, "%lu",&size) != 1)
        printf("statm file format failed.\n");
    fclose(fp);
    fp = NULL;
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
    fclose(fp);
    fp = NULL;
    
    //read from mem by using ptrace
    if(!strcpy(fullDir, procDir)||!strcat(fullDir, "/mem")){
        printf("Library call failed 5.\n");
        return 1;
    }
    int status;
    char* mem;
    mem = (char *) calloc(optionFlags.len+1,sizeof(char));
    int pid;
    pid = fork();
    if (pid < 0)
    perror("fork");
    if(pid == 0){
        ptrace(PTRACE_ATTACH, optionFlags.pid, NULL, NULL);
        waitpid(pid, NULL, 0);
	int f_read = open(fullDir, O_RDONLY);
	if(f_read < 0){
		perror("f_read open");
		exit(1);
	}
        lseek(f_read, optionFlags.addr, SEEK_SET);
        read(f_read, mem, optionFlags.len);
        mem[optionFlags.len] = '\0';
        ptrace(PTRACE_DETACH, optionFlags.pid, NULL, NULL);
        //close the file
	if(close(f_read)<0){
		perror("f_read close");
		exit(1);
	}
    }
    
    
    //wait child to be finished
    wait(&status);
    
    
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
        printf("%s",cmdLine);
    if(optionFlags.m){
        for(int i=0; i<optionFlags.len;i++){
            printf("%-5c",mem[i]);
        }
    }
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
    while(fgets(buf, MAX_LINE, fp)){
        if(strncmp(buf, "Uid:", 4) == 0){
            fclose(fp);
            fp = NULL;
            uid_t realUid;
            if(sscanf(buf, "%*s\t%d", &realUid)!=1){
                // Unable to read uid: should belong to another user
                //printf("Unable to read uid.\n");
                return 0;
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

