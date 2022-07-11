from cmath import e
import os
from queue import Empty
import pandas as pd
import matplotlib.pyplot as plt
from io import StringIO
import re 
import subprocess
from datetime import datetime

# Clean this up afterwards
SD_CARD_NAME = 'SANDISK16'
PATH = '/Users/vasilypiccone/Desktop/'

# Re-write this with best coding practices
def copy_file():
    # For whatever reason, this command isn't working.
    check_volumes = subprocess.Popen("cd /Volumes/")
    # open_sd_card = subprocess.Popen("cd Volumes/SANDISK16") 
    list_dir = subprocess.Popen(["ls", "-l"])  # this command lists the files/directories available in the working directory


# Clean this function
# Add an exception if the length of the data array are not equal
def clean_data(path: str):
    # Assuming that the file is separating two arrays of data
    data = []
    time = []
    dt = []
    max, avg = 0, 0
    # This function requires thorough cleaning
    with open(path, 'r') as file:
        lines = file.readlines()
        for i in range(len(lines)):
            inter = lines[i].split(',')
            data.append(float(inter[0]))
            time.append(re.sub('[^\d|\.]','', inter[1]))
            dt.append(float(re.sub('[^\d|\.]','', inter[1])))
            time[i] = float(time[i]) + float(time[i-1]) # creating a discrete time variable, as opposed to an array of delta time
            if data[i] > max:
                max, time_of_max = data[i], time[i]
        for j in range(len(time)):
            time[j] = time[j]/1000
            avg += data[j]
        avg = avg/len(data)
    print(str(max) + " kmh", str(avg) + " kmh", str(time_of_max) + " s")
    return time, data, dt, time_of_max

def displacement(dt: list, speed: list):
    t_steps, spd_steps, s0, s_tot = len(dt), len(speed), 0, 0
    disp = []
    try:
        for i in range(spd_steps):
            disp.append(s0 + dt[i]*speed[i]/1000)
            s0 = disp[i]
        s_tot = disp[-1]
    except TypeError as e:
        if t_steps != spd_steps:
            print("The array sizes are not the same." + "length of time: "+ str(t_steps) + "length of speed: "+str(spd_steps))
        else:
            print("You are experiencing some strange error.", e)
    return disp, s_tot

def accel(dt: list, time: list, speed: list):
    t_steps, spd_steps, max_accel, max_accel_time = len(dt), len(speed), 0, 0
    accel = []
    try:
        for i in range(spd_steps): # Weird issue here
            if(i > 0 and i < spd_steps):
                accel.append((speed[i]-speed[i-1])/dt[i])
            elif(i == 0):
                accel.append(0)
            else:
                accel.append(accel[-1]) #If you are out of range, just duplicate the previous array value
            # if(i < t_steps):
            #     accel.append((speed[i+1]-speed[i])/dt[i])
            # else:
            #     accel.append(accel[-1])
            # if(accel[i] > max_accel):
            #     max_accel = accel[i]
            #     max_accel_time = time[i]
    except TypeError as e:
        if t_steps != spd_steps:
            print("The array sizes are not the same." + "length of time: "+ str(t_steps) + "length of speed: "+str(spd_steps))
        else:
            print("You are experiencing some strange error.", e)
    return accel, max_accel, max_accel_time

# What's the cleanest way to write this function?
def plot_data(time: list, speed: list, displacement: list, acceleration: list):
    # Create a numpy array with the average speed and plot it
    # put speed and displacement on separate axes
    # Add points at the max acceleration and speed points
    plt.plot(time, speed, label = "Speed (kmh)")
    # plt.plot(time, displacement, label = "Distance (m)")
    plt.plot(time, acceleration, label = "Acceleration (m/s^2)")  # Do this in LaTeX such that it looks better

    now = datetime.now()
    dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
    plt.title("Data from Run at "+str(dt_string))
    plt.xlabel("Time (s)")
    plt.ylabel("Speed (kmh)")
    plt.legend()
    plt.show()

if __name__ == "__main__":
    # Get datapath (will eventually be automatically calibrated)
    data_path = '/Users/vasilypiccone/Desktop/Q/GitRepos/MtlTrike/post_processing/Test_data/DATALOG.TXT'

    # Duplicate the data file to the test_files directory
    # copy_file()
    # Open the duplicated file and clean the data

    # Perform necessary calculations, make report of test run
    time, speed, dt, max = clean_data(data_path)
    disp, s_total = displacement(dt, speed)
    acceleration, max_a, max_accel_t = accel(dt, time, speed)
    # print(disp, s_total)
    plot_data(time, speed, disp, acceleration)
    print(len(acceleration), len(time))

    # Save plot to a new folder