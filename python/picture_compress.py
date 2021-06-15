#!/usr/bin/env python3

#**********************************************
#* name:    FRACTATOR 2077                    *
#* author:  STEPAN MAROUSEK                   *
#* date:    2021/6/15                         *
#**********************************************

import psutil
import os
import time
from PIL import Image

def checkIfProcessRunning(processName):
    for proc in psutil.process_iter():
        try:
            if processName.lower() in proc.name().lower():
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return False;

if (checkIfProcessRunning('Fractator_2077')):
	print("Python found running process")
	picture_num = 1
	filename = "fractal-%d"%(picture_num)
	while (checkIfProcessRunning('Fractator_2077') or os.path.isfile(filename+".ppm")):
		if os.path.isfile(filename+".ppm"):
			im = Image.open(filename+".ppm")
			im.save(filename+".png")
			os.remove(filename+".ppm")
			picture_num+=1
			filename = "fractal-%d"%(picture_num)
		else:
			time.sleep(0.2)


else:
	print("Python did not found Fractator_2077 process")

print("Python script terminated")
