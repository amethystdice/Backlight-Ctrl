#ifndef __ERROR_HANDLING__
#define __ERROR_HANDLING__

void exit_with_sys_err(const char* s);
void exit_with_err_msg(const char* s);
void exit_with_err_num(const char* s,int err);

#endif