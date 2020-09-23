//
// Created by Hanyuan Wu on 9/7/20.
// team members: Hanyuan Wu, Zhihao Shu

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
    const char* const optStr = "p:s::U::S::v::c::m:";
    int opt;
    int argFlag = 0;
    opterr = 0;
    while((opt=getopt(argc, argv, optStr)) != -1){
        switch(opt){
            case 'p':
                if(optionFlags->p >= MAX_LINE){
                    printf("too many -p options");
                    return 1;
                }
                if(!(optionFlags->pids[optionFlags->p] = (char*)malloc(sizeof(char)*MAX_LINE))
                    ||!strncpy(optionFlags->pids[optionFlags->p], optarg, MAX_LINE)){
                    printf("Unable to parse pid.\n");
                    return 1;
                }
                optionFlags->pids[optionFlags->p][MAX_LINE-1] = '\0';
                (optionFlags->p)++;
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
		        optionFlags->m = 1;
                if(optarg == NULL || argv[optind] == NULL // check arguments
                    || (optind+1 < argc && argv[optind+1][0] != '-'))
			        argFlag = 1;
                if(!sscanf(optarg, "%llx", &(optionFlags->addr))
                    || !(optionFlags->len = atoll(argv[optind]))){
                    printf("Unable to parse mem address and length.\n");
                    return 1;
                }
                break;
            default:
                printf("Unknown option: -%c\n"
                       "Usage: ps537 [-p <pid>] [-s[-]] [-U[-]] [-S[-]] [-v[-]] [-c[-]] [-m <addr> <len>]\n", optopt);
                return 1;
        }
        if(optionFlags->m && optionFlags->p != 1){
            printf("-m supported only when -p <pid> occurs exactly once.");
            return 1;
        }
        if(argFlag != 0) {
            printf("Invalid argument for option -%s\n", optarg);
            return 1;
        }
    }
    return 0;
}
