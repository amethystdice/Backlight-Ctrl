#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../lib/error-handler.h"

#define BUFFER_SIZE 10
#define BRIGHTNESS_FILE_PATH "/sys/class/backlight/intel_backlight/brightness"
#define MAX_BRIGHTNESS_FILE_PATH "/sys/class/backlight/intel_backlight/max_brightness"
#define GITHUB_LINK "\033]8;;https://github.com/amethystdice/intel-backlight-control\033\\\033[34mgithub\033[0m\033]8;;\033\\"


int main(int argc,char** argv){

    // check if the user has run the program as "sudo"
    if(getuid()!=0)
        exit_with_err_msg("brightness-control: This tool needs admin privileges\nmake sure to run \"sudo brightness-control\"");
    
    // check if the program supports the users computer

    if(access(BRIGHTNESS_FILE_PATH,R_OK)!=0 || true){
        fprintf(stderr, 
            "It looks like this program doesn't support your laptop!\n"
            "Make sure that your devices uses `intel backlight'.\n"
            "If it does and this message still appears make sure to report this bug on %s\n\n"
            "path to the intel-backlight directory: %s\n"
            ,GITHUB_LINK,BRIGHTNESS_FILE_PATH);
        exit(EXIT_FAILURE);
    }

    // check arguments

    if(argc>2){
        if(fprintf(stderr,"brightness-control: Number of arguments unsupported\nTry 'i-brightness-control --help' for more information\n")<0)
            exit_with_sys_err("fprintf()");
        exit(EXIT_FAILURE);
    }
    
    if(argc==2)
        if((!strncmp(argv[1], "-h", 2)) || (!strncmp(argv[1], "--help", 6))){
            if(printf(
            "brightness-control: Wrong number of arguments\n"
            "Usage: brightness-control\n"
            "or   : brightness-control [BRIGHTNESS VALUE]\n"
            "or   : brightness-control max/min\n"    
            )<0)
                exit_with_sys_err("printf()");
        }

    //open brightness and max brightness files
    
    char* brightness_file_path = BRIGHTNESS_FILE_PATH; 
    char* max_brightness_file_path = MAX_BRIGHTNESS_FILE_PATH;

    FILE* brightness_file = fopen(brightness_file_path, "r+");
    FILE* max_brightness_file = fopen(max_brightness_file_path,"r"); 

    if( (brightness_file==NULL) || (max_brightness_file==NULL) ){
        fprintf(
            stderr,
            "Can't access the files responsible for brightness\n"
            "fopen(): couldn't open %s\n"
            "fopen(): couldn't open %s\n"
            ,brightness_file_path,max_brightness_file_path);
        exit(EXIT_SUCCESS);
    }

    //load previous values and max possible brightness

    unsigned int previous_brightness_value;
    unsigned int max_brightness;

    if(fscanf(brightness_file, "%u",&previous_brightness_value)==EOF)
        exit_with_sys_err("fscanf()");
    if(fscanf(max_brightness_file, "%u",&max_brightness)==EOF)
        exit_with_sys_err("fscanf()");
    
    long current_brightness;
    bool skip_print=false;

    if(argc==1){ // value from command line
        if(printf("Current screen brightness: %u\nBrightness range ( 0 -> %u )\n",previous_brightness_value,max_brightness)<0)
            exit_with_sys_err("printf()");
    }
    else{ // value from argument
        if((!strncmp(argv[1], "max", 3)) || (!strncmp(argv[1], "MAX", 3))){
            current_brightness=max_brightness;
        }
        else
        if ((!strncmp(argv[1], "min", 3)) || (!strncmp(argv[1], "MIN", 3)))
            current_brightness=0;
        else{
            current_brightness=atol(argv[1]);
            if((current_brightness<0) || (current_brightness>max_brightness)){
                if(fprintf(stderr, "Brightness out of the permitted range (0-%u)\n",max_brightness)<0)
                    exit_with_sys_err("fprintf()");
                skip_print=true;
            }
        }

        if(skip_print==false){
            if((truncate(brightness_file_path,0))==-1)
                exit_with_sys_err("truncate()");
        
            if(fprintf(brightness_file, "%ld",current_brightness)<0)
                exit_with_sys_err("fprintf()");
            
            if(printf("Operation successful! Screen brightness set to %ld\n",current_brightness)<0)
                exit_with_sys_err("printf()");
        }
    }
    
    //close files
    if(fclose(brightness_file)==EOF)
        exit_with_sys_err("fclose()");
    if(fclose(max_brightness_file)==EOF)
        exit_with_sys_err("fclose()");
}
