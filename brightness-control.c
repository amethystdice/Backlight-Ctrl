#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#define BUFFER_SIZE 10

int main(int argc,char** argv){
    
    if(getuid()!=0){
        fprintf(stderr,"brightness-control: This tool needs admin privileges, make sure to run \"sudo brightness-control\"\n");
        exit(EXIT_FAILURE);
    }
    // check arguments

    if(argc<1 || argc>2)
        fprintf(stderr,
        "brightness-control: Wrong number of arguments\n"
        "Usage: brightness-control\n"
        "or   : brightness-control [BRIGHTNESS VALUE]\n"
        );
    
    //open the files
    
    char* brightness_file_path = "/sys/class/backlight/intel_backlight/brightness"; 
    char* max_brightness_file_path = "/sys/class/backlight/intel_backlight/max_brightness";

    FILE* brightness_file = fopen(brightness_file_path, "r+");
    FILE* max_brightness_file = fopen(max_brightness_file_path,"r"); 

    if( (brightness_file==NULL) || (max_brightness_file==NULL) ){
        fprintf(
            stderr,
            "the files responsible for brightness control couldn't be found, make sure that your devices uses \"intel backlight\"\n"
            "fopen(): couldn't open %s\n"
            "fopen(): couldn't open %s\n"
            ,brightness_file_path,max_brightness_file_path);
        exit(EXIT_FAILURE);
    }

    //load previous values and max possible brightness

    unsigned int previous_brightness_value;
    unsigned int max_brightness;

    if(fscanf(brightness_file, "%u",&previous_brightness_value)==EOF){
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
    if(fscanf(max_brightness_file, "%u",&max_brightness)==EOF){
        perror("fscanf()");
        exit(EXIT_FAILURE);
    }
    
    long current_brightness;

    if(argc==1){ // selezione manuale da command line
        
        if(printf("Current screen brightness value: %u\nChoose a new value between 0 and %u\nNew value: ",previous_brightness_value,max_brightness)<0){
            perror("fprintf()");
            exit(EXIT_FAILURE);
        }
        char buffer[BUFFER_SIZE];
        fgets(buffer, BUFFER_SIZE,stdin);
        current_brightness = atol(buffer);
    
        while( (current_brightness<0) || (current_brightness>max_brightness)){
            if(fprintf(stderr, "brightness value outside permitted range(0-%u)\n",max_brightness)<0){
                perror("fprintf()");
                exit(EXIT_FAILURE);
            }
        }
    }
    else{ //selezione da argomento
        current_brightness = atol(argv[1]);
        if(current_brightness<0 || current_brightness>max_brightness){
            if(fprintf(stderr, "Brightness out of the permitted range (0-%u)\n",max_brightness)<0){
                perror("fprintf()");
                exit(EXIT_FAILURE);
            }
        }
    }
  
    if(truncate(brightness_file_path,0)==-1){
        perror("truncate()");
        exit(EXIT_FAILURE);
    }

    if(fprintf(brightness_file, "%ld",current_brightness)<0){
        perror("fprintf() (brightness_file)");
        exit(EXIT_FAILURE);
    }
    
    if(fclose(brightness_file)==EOF){
        perror("fclose()");
        exit(EXIT_FAILURE);
    }
    if(fclose(max_brightness_file)==EOF){
        perror("fclose()");
        exit(EXIT_FAILURE);
    }


}
