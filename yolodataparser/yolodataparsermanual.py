import cv2
import os
import glob
import shutil

# Directories for images and labels
image_dir = "/Path/To/trained/images"
label_dir = "/path/to/trained/labels"
removed_txt_dir = "/path/to/where/you/want/empty/txt/files/to/go"
removed_image_dir = "/path/to/where/you/want/empty/txt/file/images/to/go"

# Ensure the directories exist
os.makedirs(removed_txt_dir, exist_ok=True)
os.makedirs(removed_image_dir, exist_ok=True)

# A dictionary to store filename and new lines
new_file_contents = {}

# Get a list of all .txt files
file_list = glob.glob(os.path.join(label_dir, '*.txt'))

# Initialize index and action
i = 0
last_action = None
annotated_count = 0

# Filter files to count only those starting with '0' (excluding 'CBC' files)
total_files = sum(1 for filename in file_list if any(line.startswith('0') for line in open(filename)) and not os.path.basename(filename).startswith('CBC'))

# Loop over the list
while i < len(file_list):

    filename = file_list[i]
    base = os.path.basename(filename)
    base_name = os.path.splitext(base)[0]
    image_filepath = os.path.join(image_dir, base_name + '.jpg')

    with open(filename, 'r') as file:
        # Read all lines in the file
        lines = file.readlines()

    # Check if any line starts with '0'
    if any(line.startswith('0') for line in lines):
        # If filename starts with 'CBC', automatically delete lines starting with '0' and continue
        if "CBC" in filename:
            new_file_contents[filename] = [line for line in lines if not line.startswith('0')]
            i += 1
            continue

        # Clear the terminal
        os.system('cls' if os.name == 'nt' else 'clear')

        # Print progress
        print(f"Annotating file: {filename}")
        print(f"Corresponding image: {image_filepath}")
        print(f"Files annotated: {annotated_count}")
        print(f"Files remaining: {total_files - annotated_count}")
        if last_action is not None:
            print(f"Last image was: {last_action}")

        # Check if corresponding image exists
        if not os.path.exists(image_filepath):
            i += 1
            continue

        # Open the image file with OpenCV
        img = cv2.imread(image_filepath)

        # Show the image in a window
        cv2.imshow('Image', img)

        # Wait for keypress
        key = cv2.waitKey(0) & 0xFF

        # If 'd' is pressed, store lines that don't start with '0'
        # If space is pressed, store all lines
        new_lines = [line for line in lines if key != ord('d') or not line.startswith('0')]

        # Store new lines in dictionary
        new_file_contents[filename] = new_lines

        # Set last action based on key
        last_action = "deleted" if key == ord('d') else "saved"

        
        i += 1
        annotated_count += 1

        # Close the window
        cv2.destroyAllWindows()
    else:
        i += 1

# After all images are processed, write new lines to files and move empty ones
for filename, new_lines in new_file_contents.items():
    with open(filename, 'w') as file:
        file.writelines(new_lines)

    # If file is empty, move .txt and corresponding .jpg to removed folders
    if os.stat(filename).st_size == 0:
        base = os.path.basename(filename)
        base_name = os.path.splitext(base)[0]
        image_filepath = os.path.join(image_dir, base_name + '.jpg')
        shutil.move(filename, os.path.join(removed_txt_dir, base))
        if os.path.exists(image_filepath):
            shutil.move(image_filepath, os.path.join(removed_image_dir, base_name + '.jpg'))

print("All .txt files have been processed.")
