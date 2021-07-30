#!/usr/bin/env python3

#**********************************************
#* name:    FRACTATOR 2077                    *
#* author:  STEPAN MAROUSEK                   *
#* date:    2021/07/30                        *
#**********************************************

import psutil
import os
import time

def checkIfProcessRunning(processName):
    for proc in psutil.process_iter():
        try:
            if processName.lower() in proc.name().lower():
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return False;

if (checkIfProcessRunning('Fractator_2077')):
    print("Python 2 found running process")
    picture_num = 1
    while (checkIfProcessRunning('Fractator_2077')):
        if os.path.isfile("make_animation_666.yeah"):
            f = open("make_animation_666.yeah", "r")
            first_picture = f.readline()
            last_picture = f.readline()
            print("First picture is %d    Last picture is %d"%(int(first_picture),int(last_picture)))
            while os.path.isfile("fractal-%d.ppm"%(int(last_picture))):
                time.sleep(1)
            print("Video animation creation started...")
            os.system("cat $(find . -maxdepth 1 -name \"*.png\" | sort -V) | ffmpeg -hide_banner -loglevel error -r:v 30 -i - -codec:v libx264 -preset veryslow animation.avi")
            os.system("rm make_animation_666.yeah")
            print("Animation video created")
        else:
            time.sleep(0.2)
else:
	print("Python 2 did not found Fractator_2077 process")
print("Python script 2 terminated")
