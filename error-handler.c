#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error-handler.h"

void exit_with_sys_err(const char* s){
    perror(s);
    exit(EXIT_FAILURE);
}

void exit_with_err_num(const char *s,int err){
    fprintf(stderr,"%s: %s\n",s,strerror(err));
    exit(EXIT_FAILURE);
}

void exit_with_err_msg(const char *s){
    fprintf(stderr, "%s\n",s);
}