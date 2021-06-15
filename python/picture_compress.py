#!/usr/bin/env python3

import psutil
import os
import time
from PIL import Image

def checkIfProcessRunning(processName):
    '''
    Check if there is any running process that contains the given name processName.
    '''
    #Iterate over the all the running process
    for proc in psutil.process_iter():
        try:
            # Check if process name contains the given name string.
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
	print("Terminating python script...")

print("Python script terminated")
