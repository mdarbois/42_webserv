

function uploadFile() {
	// Display the "Uploading file..." message
	document.getElementById("message").style.display = "block";
	// Get the file input element
	var fileInput = document.getElementById('file');
	//console.log(fileInput.files[0].name);
	// Check if a file is selected
	if (fileInput.files.length > 0) {
		// Create a new XMLHttpRequest object

		var xhr = new XMLHttpRequest();
		//xhr.setRequestHeader('Content-Type', 'undefined');
		
		// Configure it for a POST request to the dynamic host and port
		var host = window.location.hostname; // Get the current hostname dynamically
		var port = window.location.port; // Get the current port dynamically
		xhr.open('POST', 'http://' + host + ':' + port + '/uploads/', true);
		//console.log(host, port);
		// Define a callback function to handle the response
	
		xhr.onload = function() {
			// Hide the "Uploading file..." message
			document.getElementById("message").style.display = "none";
			if (xhr.status === 201 || xhr.status == 200) {
				alert('File uploaded successfully!');
				window.location.reload();
			} else {
				// Display the HTML response from the server in the current window				console.log(xhr.responseText);
				document.open(); // Open a new document
				document.write(xhr.responseText); // Write the HTML response to the document
				document.close(); // Close the document
			}
		};

		// Define an error event handler to handle ERR_ACCESS_DENIED error
		xhr.onerror = function() {
            // Hide the "Uploading file..." message
            document.getElementById("message").style.display = "none";
            alert('Error: Access denied. You do not have permission to upload this file.');
        };

		// Create a FormData object
		var formData = new FormData();
		
		// Append the file to the FormData object
		formData.append('file', fileInput.files[0], fileInput.files[0].name.replace(/ /g, "%20"));
		// Send the FormData object containing the file
		xhr.send(formData);
		//console.log(fileInput.files[0]);
		
	} else {
		 // Hide the "Uploading file..." message if no file is selected
		 document.getElementById("message").style.display = "none";
		alert('Please select a file before uploading.');
	}
}

const sendGetRequest = (fileName) => {
	
	fetch(`/uploads/${fileName}`, {
		method: 'GET',
	})
	.then(response => {
		if (!response.ok) {
			// Handle non-successful responses here
			return response.text().then(html => {
				throw new Error(`${html}`);
			});
		}
		return response.blob();
	})
	.then(blob => {
		// Create a link element and trigger the download
		const link = document.createElement('a');
		link.href = URL.createObjectURL(blob);
		link.download = fileName;
		document.body.appendChild(link);
		link.click();
		document.body.removeChild(link);
	})
	.catch(error => {
		// Handle errors here
		const errorMessage = document.createElement('div');
		errorMessage.textContent = error.message;
		// Display the error HTML in the browser
		document.body.innerHTML = error.message;
	});
};


function addPercentage(fileName)
{
	return fileName.replace(/ /g, "%20");
}

const sendDeleteRequest = (fileName) => {

	const encodedFileName = addPercentage(fileName);
	return new Promise((resolve, reject) => {
		fetch(`/uploads/${encodedFileName}`, {
			method: 'DELETE',
		})
		.then(response => {
			if (!response.ok) {
				// Handle non-successful responses here
				return response.text().then(html => {
					throw new Error(`${html}`);
				});
			}
			else if (response.ok) {
				console.log(`File ${encodedFileName} deleted successfully`);
				resolve();
			} else {
				console.error(`Failed to delete file ${encodedFileName}`);
				reject();
			}
		})
		.catch(error => {
			// Handle errors here
			const errorMessage = document.createElement('p');
			errorMessage.textContent = error.message;
			console.error(error.message); // Log the error
			// Display the error HTML in the browser
			document.body.innerHTML = error.message;
			reject();
		});
	});
};




fetch('/list.php')
.then(response => response.json())
.then(files => {
				
				const fileList = document.getElementById('fileList');
				// Clear existing list
				fileList.innerHTML = "";
				// Populate the list with fetched files
				files.forEach(file => {
					const listItem = document.createElement('li');
					listItem.textContent = file;
					const getButton = document.createElement('button');
					getButton.textContent = 'GET';
					getButton.addEventListener('click', () => sendGetRequest(file));

					const deleteButton = document.createElement('button');
					deleteButton.textContent = 'DELETE';
					deleteButton.addEventListener('click', () => {
						console.log(file);
						sendDeleteRequest(file)
										.then(() => location.reload())
										.catch(error => console.error('Error deleting file:', error));
									});
									listItem.appendChild(getButton);
									listItem.appendChild(deleteButton);
									fileList.appendChild(listItem);
								});
								
							})
			.catch(error => console.error('Error fetching file list:', error));




function listFiles() {
	const fileList = document.getElementById("fileList");
	if (fileList.style.display === "none") {
		
		fetch('/list.php')
		.then(response => response.json())
		.then(files => {
						document.getElementById("listButton").innerText = "Hide Files";
						
						//const fileList = document.getElementById('fileList');
						// Clear existing list
						fileList.innerHTML = "";
						// Populate the list with fetched files
						files.forEach(file => {
							const listItem = document.createElement('li');
							listItem.textContent = file;
							const getButton = document.createElement('button');
							getButton.textContent = 'GET';
							getButton.addEventListener('click', () => sendGetRequest(file));

							const deleteButton = document.createElement('button');
							deleteButton.textContent = 'DELETE';
							deleteButton.addEventListener('click', () => {
								sendDeleteRequest(file)
												.then(() => location.reload())
												.catch(error => console.error('Error deleting file:', error));
											});
											listItem.appendChild(getButton);
											fileList.appendChild(deleteButton);
											fileList.appendChild(listItem);
										});
										
									})
					.catch(error => console.error('Error fetching file list:', error));
		fileList.style.display = "block";
	} else {
				// If list is displayed, hide it and update the button text
				document.getElementById("fileList").innerHTML = "";
				document.getElementById("listButton").innerText = "List Files";
				fileList.style.display = "none";

	} 
}