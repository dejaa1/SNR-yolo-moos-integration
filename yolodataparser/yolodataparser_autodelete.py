import glob
import os

# Change 'your_directory_path' to your directory path
directory_path = '/Users/logandelaar/Desktop/Search And Rescue.v3i.yolov5pytorch/train/labels'  

# Use glob to get all .txt files in the directory
for filename in glob.glob(os.path.join(directory_path, '*.txt')):
    with open(filename, 'r') as file:
        # Read all lines in the file
        lines = file.readlines()

    with open(filename, 'w') as file:
        # Write back only lines that don't start with '0'
        for line in lines:
            if not line.startswith('0'):
                file.write(line)
