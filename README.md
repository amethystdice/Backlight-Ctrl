# Warning
this tool is still experimental so be cautious!

# Backlight-Ctrl
This tool will change the brightness of the screen via command line.
It only works if your device uses the "**intel_backlight** or **amdgpu_bl1**".

## Installation

First you will need to compile this program. To do that you need to install **gcc** and **make**.
If your distribution uses a package manager different that **apt** like **dnf** , just replace it in the following command:

```bash
sudo apt install gcc make 
```

now you can install the program

```bash
make install # compile and install
```

You can now run one of the following commands:
```bash
backlight-ctrl      # check the current brightness and it's max and min setting 
backlight-ctrl 125  # change the brightness to a value
backlight-ctrl min  #this command will """turn off""" the screen.
backlight-ctrl max  #this command will make the screen the brightest it can go
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
./bin/backlight-ctrl # run the program from the main directory
