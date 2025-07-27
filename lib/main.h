#ifndef _MAIN_
#define _MAIN_

#include <stdio.h>

int displaySelector();
char formatChecker(int argc,char** argv);
long brightnessSelector(char** argv,char brightness_format,FILE* max_brightness_file);
void openFiles(FILE** brightness_file,FILE** max_brightness_file,int display_controller,char* brightness_file_path,char* max_brightness_file_path);
void noArgumentsCondition(FILE* brightness_file,FILE* max_brightness_file);
void printInFile(FILE* brightness_file,char* brightness_file_path,long current_brightness);
void closeFiles(FILE* brightness_file,FILE* max_brightness_file);

#endif