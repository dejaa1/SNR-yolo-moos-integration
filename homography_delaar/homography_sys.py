import cv2
import numpy as np
import sys

# Define corresponding points
pixel_coords = np.array([[1024, 0, 900, 600, 300, 512, 512, 512, 256, 768, 300, 700],
                         [720, 720, 600, 400, 500, 500, 700, 720, 720, 720, 400, 500]],
                        dtype=np.float32)

real_world_coords = np.array([[0.39, -0.39, 0.49, 0.93, -0.36, 0, 0, 0, -0.20, 0.20, -1.83, 0.39],
                              [0.14, 0.14, 0.39, 2.99, 0.94, 0.94, 0.33, 0.32, 0.27, 0.27, 2.82, 0.97]],
                             dtype=np.float32)

# Calculate the homography matrix
homography_matrix, _ = cv2.findHomography(pixel_coords.T, real_world_coords.T, method=cv2.RANSAC)

# Get user input for the point on the cover image
x, y = float(sys.argv[1]), float(sys.argv[2])

# Check if y is less than 363
if y < 364:
    print("Invalid input. Y coordinate should not be less than 364.")
    sys.exit(1)
    
cover_point = np.array([[x, y, 1]], dtype=np.float32).T

# Transform the point using homography
undistorted_point = np.dot(homography_matrix, cover_point)
undistorted_point /= undistorted_point[2]

# Print the corresponding point in meters
print("Undistorted point (meters):")
print(f"X = {undistorted_point[0, 0]:.2f} meters, Y = {undistorted_point[1, 0]:.2f} meters")
