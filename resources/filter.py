import cv2
import os
import shutil

# Specify the source directory and target directory
src_dir = '/Path/To/Source/Photo/Folder'
target_dir = '/Path/To/Save/Folder'

# Make sure the target directory exists 
os.makedirs(target_dir, exist_ok=True)

# Walk through all files in the source directory
for filename in os.listdir(src_dir):
    if filename.endswith('.jpg'):
        # Create the full file path by joining the source directory and the filename
        filepath = os.path.join(src_dir, filename)

        # Open the image file with OpenCV
        img = cv2.imread(filepath)

        # Show the image in a window
        cv2.imshow('Image', img)

        # Wait for keypress
        key = cv2.waitKey(0) & 0xFF

        # If 'd' is pressed, delete the file
        if key == ord('d'):
            os.remove(filepath)

        # If space bar is pressed, move the image to the target directory
        elif key == 32:  # ASCII for space is 32
            # Create the target file path
            target_filepath = os.path.join(target_dir, filename)

            # Move the file to the target directory
            shutil.move(filepath, target_filepath)

        # Close the window
        cv2.destroyAllWindows()

print("All .jpg files have been processed.")
