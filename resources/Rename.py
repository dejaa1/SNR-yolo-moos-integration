import os
import shutil

# Source folder containing the original .jpg photos
source_folder = "/Path/To/Photo/Folder"

# Destination folder to save the renamed .jpg photos
destination_folder = "/Path/To/Save/Folder"

# Create the destination folder if it doesn't exist
os.makedirs(destination_folder, exist_ok=True)

# Get a list of all .jpg files in the source folder
jpg_files = [file for file in os.listdir(source_folder) if file.endswith(".jpg")]

# Sort the files alphabetically
jpg_files.sort()

#Get a name for begining of file
name_of_file = input("What should it be called? ")
#Get persons name

persons_name = input("What is your last name? ")
# Counter to keep track of the numerical order
counter = 1

# Iterate through the .jpg files
for file in jpg_files:
    # Create the new filename with the counter value
    new_filename = f"{name_of_file}_{persons_name}_{counter}.jpg"

    # Build the full paths of the source and destination files
    source_path = os.path.join(source_folder, file)
    destination_path = os.path.join(destination_folder, new_filename)

    # Rename and move the file to the destination folder
    shutil.move(source_path, destination_path)

    # Increment the counter
    counter += 1

print("Renaming and moving complete.")
