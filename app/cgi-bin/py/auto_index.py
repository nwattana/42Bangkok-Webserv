#!/usr/bin/env python3
import os
import sys


def list_files_and_folders(directory):
	files_and_folders = os.listdir(directory)
	file_list = []
	folder_list = []
	for item in files_and_folders:
		item_path = os.path.join(directory, item)
		if os.path.isfile(item_path):
			file_list.append(item)
		elif os.path.isdir(item_path):
			folder_list.append(item)
	return file_list, folder_list

def generate_html(directory):
	files, folders = list_files_and_folders(directory)
	html = "<html>\n<body>\n"
	html += f"<h1>From: {directory}</h1>\n"  # Add this line
	for folder in folders:
		folder_path = "./" + folder + "/"
		html += f"\t<a href='{folder_path}'>{folder} --- Folder </a><br>\n"
	for file in files:
		file_path = "./" + file
		html += f"\t<a href='{file_path}'>{file} --- File </a><br>\n"
	html += "</body>\n</html>"
	return html

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Please provide a directory path as a command-line argument.")
		sys.exit(1)
	directory = sys.argv[1]
	html = generate_html(directory)
	print(html)
