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

def generate_style_tag():
	return """
	<link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
	<style>
		* {
			margin: 0;
			padding: 0;
			box-sizing: border-box;
			background-color: rgb(52, 73, 85);
		}

		.flex-containner {
			display: flex;
			justify-content: space-between;
		}

		ul {
			list-style-type:disc;
			width: 80%;
		}


		.file {
			margin-left: 10%;
		}


		.delete-butt{
			font-size: 1rem;
			color: red;
		}

		.delete-butt:hover
		{
			font-size: 2rem;
			cursor: pointer;
		}

		a{
			font-size: 1.5rem;
			text-decoration: none;
			color: #78A083;
			padding-left: 0.5rem;
		}

		.normal-font-color {
			color: #78A083;
		}

		a:hover{
			font-size: 2rem;
			color: #35374B;
		}

		li {
		    border-bottom: rgb(120, 160, 131) solid 1px;
			height: 3rem;
			align-items:center;
		}

		h1{
			background-color: rgb(80, 114, 123);
			padding: 1rem 1rem 1rem 1rem;
			color:rgb(52, 73, 85);
		}

		.space-size{
			width: 50%;
		}

		.middle-text {
		}

		.upload-item {
			margin-top: 5rem;
			margin-left:4rem;
			font-size: 1.5rem;
		}
		input {
			color:rgb(53, 55, 75);
			border-radius:4px;
			background:rgb(120, 160, 131);
			padding: 0.5rem;
			margin: 0.3rem;
		}
		button {
			background:rgb(120, 160, 131);
			color:rgb(53, 55, 75);
			padding: 0.5rem;
			border-radius:4px;
		}
		button:hover {
			background:rgb(80, 114, 123);
			color:rgb(53, 55, 75);
			padding: 0.5rem;
			font-size: 1.5rem;
			border-radius:4px;
			cursor: pointer;
		}
	</style>
	"""


def generate_html(directory):
	files, folders = list_files_and_folders(directory)
	html = "<html><head>"
	html += generate_style_tag()
	html += "</head>"
	html += f"<h1>From: {directory}</h1><br><ul>"  # Add this line
	for folder in folders:
		folder_path = "./" + folder + "/"
		html += f"""
		<li class="file flex-containner normal-font-color">
			<div class="space-size" >
				<i class="material-icons">&#xe2c8;</i>
				<a class="middle-text" href='{folder_path}'>{folder}</a>
			</div>
			<div >
				Folder
			</div>
			<div class="delete-butt material-icons">
				&#xe92b;
			</div>
		</li>
		"""
	for file in files:
		file_path = "./" + file
		html += f"""
			<li class="file flex-containner normal-font-color">
				<div class="space-size">
					<i class="material-icons">&#xe2c4;</i>
					<a class="middle-text" href='{file_path}' download>{file}</a>
				</div>
				<div>
					File
				</div>
				<div class="delete-butt material-icons">&#xe92b;
				</div>
			</li>
		"""
	html += "</ul>"
	html += generate_upload_file()
	html += generate_script_tag()
	html += generate_script_upload()
	html += "</body>\n</html>"
	return html

def generate_upload_file():
	return """
	<div class="upload-item">
		<div style="color:rgb(120, 160, 131)">File Name</div>
		<input type="text" id="filename" value=""><br>
		<input type="file" id="fileInput" value=null>
		<button onclick="uploadFile()">Upload</button>
	</div>
	"""

def generate_script_tag():
	return """
	<script>
			// Add event listener to all delete buttons
			document.querySelectorAll('.delete-butt').forEach(button => {
				//console.log(button.parentElement.childNodes[1].childNodes[3].href);
				const node_href = button.parentElement.childNodes[1].childNodes[3].href;
				
				button.addEventListener('click', function() {
					fetch( node_href, {
						method: 'DELETE',
					})
					.then(response => {
						if (!response.ok) {
							throw new Error('Not OK na');
						}
						button.parentElement.remove();
					})
					.catch(errpr=> {
						console.error("ariva");
					})
				});
			});
	</script>
	"""

def generate_script_upload():
	return """
	<script>
	function uploadFile() {
		const input = document.getElementById('fileInput');
		const fname = document.getElementById('filename');
		const file = input.files[0];

		if (file) {
			const formData = new FormData();
			formData.append(fname.value, file);

			fetch('http://localhost:8089/upload/', {
			method: 'POST',
			body: formData
			})
			.then(response => {
			if (!response.ok) {
				throw new Error('Network response was not ok error code : ' + response.status);
			}
			return response;
			})
			.then(data => {
				console.log('Upload successful:');
				// Handle success response from the server
				alert("Success Upload :" + data.status);
				location.reload()
			})
			.catch(error => {
				console.error('Error uploading file:', error);
				alert(error);
			// Handle error
			});
		} else {
			alert('Please select a file to upload.');
		}
	}
	</script>
	"""



if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Please provide a directory path as a command-line argument.")
		sys.exit(1)
	directory = sys.argv[1]
	html = generate_html(directory)
	print(html)
