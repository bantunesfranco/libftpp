#!/usr/bin/env python3

# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    viz.py                                             :+:    :+:             #
#                                                      +:+                     #
#    By: bfranco <bfranco@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2025/08/25 21:02:47 by bfranco       #+#    #+#                  #
#    Updated: 2025/08/25 21:07:36 by bfranco       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

import csv
import os
import numpy as np
from PIL import Image
from sys import argv

# CSV should be 100 rows x 100 columns
def create_perlin_image(filename):
	if not filename.endswith('.csv'):
		print("Input file must be a .csv")
		return

	png_dir = os.path.dirname(filename).replace("csv","png")
	os.makedirs(png_dir, exist_ok=True)

	with open(filename, newline='') as csvfile:
		reader = csv.reader(csvfile)
		data = [[float(cell) for cell in row] for row in reader]

	arr = np.array(data)

	# --- 2. Normalize to 0-255 for grayscale ---
	arr_min, arr_max = arr.min(), arr.max()
	arr_norm = ((arr - arr_min) / (arr_max - arr_min) * 255).astype(np.uint8)

	img = Image.fromarray(arr_norm, mode='L')  # 'L' = grayscale
	outfilename = os.path.join(png_dir, os.path.basename(filename).replace('csv', 'png'))
	print(f"Saving image to {outfilename}")
	img.save(outfilename)

if __name__ == "__main__":
	if len(argv) != 2:
		print("Usage: python viz.py <input_file.csv>")
		exit(1)
	create_perlin_image(argv[1])