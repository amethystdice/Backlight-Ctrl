# Warning
Do not install or try this tool, it's a work in progress and most of all it's my first project so be warned.
This tool modifies basic system components and it may cause damage if used improprely or even proprely.

Also the brightness resets after each boot, this is by design and kind of a safety feature

# About
This tool will change the brightness of the screen via command line.
It only works if your device uses the "**intel backlight**".

## Installation

Navigate to the root directory of the program *intel-backlight-control/*.
First you will need to compile it. To do that you need to install **gcc** and **make**.
If your distribution uses a package manager different that **apt** like **dnf** use it's equivalent command:

```bash
sudo apt install gcc make 
```

now you can install the program

```bash
make install # compile and install
```

You can now run one of the following commands:
```bash
brightness-control      # check the current brightness and it's max and min setting 
brightness-control 125  # change the brightness to a value
brightness-control min  #this command will """turn off""" the screen.
brightness-control max  #this command will make the screen the brightest it can go
```

## How to uninstall

If you wish to uninstall the program you just need to run:

```bash
make uninstall
```

## Other use cases

If you wish to use the program without installing it into your system you can run this command and access the executable directly instide the bin/ directory.

```bash
make #compile the program
```
```bash
./bin/brightness-control # run the program from the main directory
