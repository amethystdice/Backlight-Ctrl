#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../lib/error-handler.h"

#define BUFFER_SIZE 10
#define INTEL_BRIGHTNESS_FILE_PATH "/sys/class/backlight/intel_backlight/brightness"
#define MAX_INTEL_BRIGHTNESS_FILE_PATH "/sys/class/backlight/intel_backlight/max_brightness"
#define AMD_BRIGHTNESS_FILE_PATH "/sys/class/backlight/amdgpu_bl1/brightness"
#define MAX_AMD_BRIGHTNESS_FILE_PATH "/sys/class/backlight/amdgpu_bl1/max_brightness"

#define INTEL 0
#define AMD 1


#define GITHUB_LINK "\033]8;;https://github.com/amethystdice/Backlight-Ctrl\033\\\033[34mgithub\033[0m\033]8;;\033\\"


int displaySelector(){
    int display_controller = -1;
    if(access(INTEL_BRIGHTNESS_FILE_PATH,R_OK)==0)
        display_controller = INTEL;
    else
        if(access(AMD_BRIGHTNESS_FILE_PATH,R_OK)==0)
            display_controller = AMD;

    if(display_controller==-1){
        fprintf(stderr, 
            "It looks like this program doesn't support your laptop!\n"
            "Make sure that your devices uses `intel_backlight' or `amdgpu_bl1`.\n"
            "If it does and this message still appears make sure to report this bug on %s\n\n"
            "intel_backlight directory: %s\n"
            "amdgpu_bl1 directory:      %s\n"
            ,GITHUB_LINK,INTEL_BRIGHTNESS_FILE_PATH,AMD_BRIGHTNESS_FILE_PATH);
        exit(EXIT_FAILURE);
    }
    return display_controller;
}

char argumentFormatChecker(int argc,char** argv){

    // check for unsupported syntax
    if(argc>2){
        if(fprintf(stderr,"Backlight-Ctrl: Number of arguments unsupported\nTry 'Backlight-Ctrl --help' for more information\n")<0)
            exit_with_sys_err("fprintf()");
        exit(EXIT_FAILURE);
    }

    if((!strncmp(argv[1], "-h", 2)) || (!strncmp(argv[1], "--help", 6))){
        if(printf(
        "Usage: Backlight-Ctrl\n"
        "or   : Backlight-Ctrl [BRIGHTNESS VALUE]\n"
        "or   : Backlight-Ctrl max/min\n"    
        )<0)
            exit_with_sys_err("printf()");
        exit(EXIT_SUCCESS);
    }

    switch(argv[1][0]){
        case '%':
            return '%';
        break;

        case 'm':
            return 'm';
        break;

        case 'M':
            return 'm';
        break;

        default:
            return 'v';
    }

    if(argv[1][0]=='%')
        return '%';
    else
        return 'v';

}

long brightnessSelector(char** argv,char brightness_format,FILE* max_brightness_file){


    long current_brightness;
    long max_brightness;

    if(fscanf(max_brightness_file, "%ld",&max_brightness)==EOF)
        exit_with_sys_err("fscanf()");

    if(brightness_format=='m'){ //min max
        if(strlen(argv[1])<3) //security check
            exit_with_err_msg("unsupported argument detected\n");

        if((!strncmp(argv[1], "max", 3)) || (!strncmp(argv[1], "MAX", 3))){
            return current_brightness=max_brightness;
        }

        if ((!strncmp(argv[1], "min", 3)) || (!strncmp(argv[1], "MIN", 3))){
            return current_brightness=0;
        }
    }

    if(brightness_format=='v'){ //value
        current_brightness=atol(argv[1]);
            if((current_brightness<0) || (current_brightness>max_brightness)){
                if(fprintf(stderr, "Brightness out of the permitted range (0-%ld)\n",max_brightness)<0)
                    exit_with_sys_err("fprintf()");
            }
        return current_brightness;
    }

    if(brightness_format=='%'){
        long percent=atol(argv[1]+1);
        return current_brightness = (percent * max_brightness) / 100;
    }
    exit_with_err_msg("no format has been chosen, something has gone wrong\n");
    return 0; //just to make the compiler shut up
}

void openFiles(FILE** brightness_file,FILE** max_brightness_file,int display_controller,char* brightness_file_path,char* max_brightness_file_path){

    if(display_controller==INTEL){
        strncpy(brightness_file_path,INTEL_BRIGHTNESS_FILE_PATH,PATH_MAX); 
        strncpy(max_brightness_file_path,MAX_INTEL_BRIGHTNESS_FILE_PATH,PATH_MAX);
    }else{
        strncpy(brightness_file_path,AMD_BRIGHTNESS_FILE_PATH,PATH_MAX); 
        strncpy(max_brightness_file_path,MAX_AMD_BRIGHTNESS_FILE_PATH,PATH_MAX);
    }

    *brightness_file = fopen(brightness_file_path, "r+");
    *max_brightness_file = fopen(max_brightness_file_path,"r"); 

    if( (brightness_file==NULL) || (max_brightness_file==NULL) ){
        fprintf(
            stderr,
            "Can't access the files responsible for brightness\n"
            "fopen(): couldn't open %s\n"
            "fopen(): couldn't open %s\n"
            ,brightness_file_path,max_brightness_file_path);
        exit(EXIT_SUCCESS);
    }
}

void noArgumentsCondition(FILE* brightness_file,FILE* max_brightness_file){
    long previous_brightness_value,max_brightness;
    fscanf(brightness_file,"%ld",&previous_brightness_value);
    fscanf(max_brightness_file, "%ld",&max_brightness);

    if(printf("Current screen brightness: %ld\nBrightness range ( 0 -> %ld )\n",previous_brightness_value,max_brightness)<0)
        exit_with_sys_err("printf()");
    exit(EXIT_SUCCESS);
}

void printInFile(FILE* brightness_file,char* brightness_file_path,long current_brightness){
    if((truncate(brightness_file_path,0))==-1)
        exit_with_sys_err("truncate()");

    if(fprintf(brightness_file, "%ld",current_brightness)<0)
        exit_with_sys_err("fprintf()");
    
    if(printf("Operation successful! Screen brightness set to %ld\n",current_brightness)<0)
        exit_with_sys_err("printf()");

}

void closeFiles(FILE* brightness_file,FILE* max_brightness_file){
    //close files
    if(fclose(brightness_file)==EOF)
        exit_with_sys_err("fclose()");
    if(fclose(max_brightness_file)==EOF)
        exit_with_sys_err("fclose()");
}

int main(int argc,char** argv){

    // check if the user has run the program as "sudo"
    if(getuid()!=0)
        exit_with_err_msg("Backlight-Ctrl: This tool needs admin privileges\nmake sure to run \"sudo backlight-ctrl\"");

    //check if value is in % or value


    // select what type of display is in use
    int display_controller = displaySelector();

 
    FILE* brightness_file = NULL; 
    FILE* max_brightness_file = NULL;
    char brightness_file_path[PATH_MAX];
    char max_brightness_file_path[PATH_MAX];

    // open the appropriate sys files 
    openFiles(&brightness_file,&max_brightness_file,display_controller,brightness_file_path,max_brightness_file_path);
    
    // if the program has only one argument we just need to read the file , not write
    if(argc==1){
        noArgumentsCondition(brightness_file,max_brightness_file);
    }else{
        // load the new brighness
        char brightness_format = argumentFormatChecker(argc, argv);
        long current_brightness = brightnessSelector(argv,brightness_format,max_brightness_file);

        // set new brightness
        printInFile(brightness_file,brightness_file_path,current_brightness);
    }

    closeFiles(brightness_file,max_brightness_file);

}
