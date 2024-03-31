import os
import sys

def delete_file(file_path):
	try:
		os.remove(file_path)
		response = "<html><body><h1>File '{file_path}' deleted successfully.</h1></body></html>"
		http_response = f"HTTP/1.1 200 OK\nContent-Type: text/html\n\n{response}"
		print(http_response)
	except FileNotFoundError:
		response = "<html><body><h1>File '{file_path}' not found.</h1></body></html>"
		http_response = f"HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n{response}"
		print(http_response)
	except PermissionError:
		response = "<html><body><h1>Permission denied to delete file '{file_path}'.</h1></body></html>"
		http_response = f"HTTP/1.1 403 Forbidden\nContent-Type: text/html\n\n{response}"
		print(http_response)
	except Exception as e:
		response = f"<html><body><h1>An error occurred while deleting file '{file_path}': {str(e)}</h1></body></html>"
		http_response = f"HTTP/1.1 500 Internal Server Error\nContent-Type: text/html\n\n{response}"
		print(http_response)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		response = "<html><body><h1>Please provide the file path as a command-line argument.</h1></body></html>"
		http_response = f"HTTP/1.1 400 Bad Request\nContent-Type: text/html\n\n{response}"
		print(http_response)
	else:
		file_path = sys.argv[1]
		delete_file(file_path)
