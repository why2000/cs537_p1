//
// Created by hanyuan on 9/7/20.
// zhihao shu

#include "optionParser.h"


int setBinaryOption(int* const flag){
    if(optarg != NULL && strcmp(optarg, "-") == 0){
        *flag = 0;
    }
    else if(optarg == NULL){
        *flag = 1;
    }
    else{
        return 1;
    }
    return 0;
}

int parseOption(const int argc, char* argv[], OptionFlags* const optionFlags){
    const int MAX_LINE = 1024;
    const char* const optStr = "p:s::U::S::v::c::m:";
    int opt;
    int argFlag = 0;
    opterr = 0;
    int count = 0;
    while((opt=getopt(argc, argv, optStr)) != -1){
        switch(opt){
            case 'p':
		    if(count > 1){
			printf("cant have multiple -p.\n");
			return 1;
		    }
		    count++;
                optionFlags->p=1;
                if(!(optionFlags->pid = (char*)malloc(sizeof(char)*MAX_LINE))
                   ||!strncpy(optionFlags->pid, optarg, MAX_LINE)){
                    printf("Unable to parse pid.\n");
                    return 1;
                }
                optionFlags->pid[MAX_LINE-1] = '\0';
                break;
            case 's':
                argFlag = setBinaryOption(&optionFlags->s);
                break;
            case 'U':
                argFlag = setBinaryOption(&optionFlags->U);
                break;
            case 'S':
                argFlag = setBinaryOption(&optionFlags->S);
                break;
            case 'v':
                argFlag = setBinaryOption(&optionFlags->v);
                break;
            case 'c':
                argFlag = setBinaryOption(&optionFlags->c);
                break;
            case 'm':
                argFlag = 0;
		optionFlags->m = 1;
                if(argv[optind] == NULL || argv[optind+1] != NULL){ // if only have one argument or more than 2 argument 
			argFlag = 1;
		}
	//	printf("flag: %ls\n",&optionFlags->m);
	//	printf("%d\n",argFlag);
		if(!(optionFlags->addr = atoi(optarg))
                   || !(optionFlags->len = atoi(argv[optind]))){
                    printf("Unable to parse mem address and length.\n");
                    return 1;
                }
                break;
            default:
                printf("Unknown option: -%c\n"
                       "Usage: ps537 [-p <pid>] [-s[-]] [-U[-]] [-S[-]] [-v[-]] [-c[-]] [-m <addr> <len>]\n", optopt);
                return 1;
        }
        if(argFlag != 0) {
            printf("Invalid argument for option -%s\n", optarg);
            return 1;
        }
    }
    return 0;
}
