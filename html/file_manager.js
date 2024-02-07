function uploadFile() {
    // Get the file input element
    var fileInput = document.getElementById('file');
    
    // Check if a file is selected
    if (fileInput.files.length > 0) {
        // Create a new XMLHttpRequest object
        var xhr = new XMLHttpRequest();
        //xhr.setRequestHeader('Content-Type', 'undefined');
        
        // Configure it for a POST request to the dynamic host and port
        var host = window.location.hostname; // Get the current hostname dynamically
        var port = window.location.port; // Get the current port dynamically
        xhr.open('POST', 'http://' + host + ':' + port + '/uploads/', true);
        console.log(host, port);
        // Define a callback function to handle the response
        xhr.onload = function() {
            if (xhr.status === 201 || xhr.status == 200) {
                alert('File uploaded successfully!');
            } else {
                // Display the HTML response from the server in the current window
                console.log(xhr.responseText);
                document.open(); // Open a new document
                document.write(xhr.responseText); // Write the HTML response to the document
                document.close(); // Close the document
            }
        };

        // Create a FormData object
        var formData = new FormData();
        
        // Append the file to the FormData object
        formData.append('file', fileInput.files[0]);

        // Send the FormData object containing the file
        xhr.send(formData);
    } else {
        alert('Please select a file before uploading.');
    }
}

const sendDeleteRequest = (fileName) => {

	return new Promise((resolve, reject) => {
		fetch(`/html/uploads/${fileName}`, {
			method: 'DELETE',
		})
		.then(response => {
			if (response.ok) {
				console.log(`File ${fileName} deleted successfully`);
				resolve();
			} else {
				console.error(`Failed to delete file ${fileName}`);
				reject();
			}
		})
		.catch(error => {
			console.error('Error deleting file:', error);
			reject();
		});
	});
};
function listFiles() {
    const fileList = document.getElementById("fileList");
    if (fileList.style.display === "none") {
        fileList.innerHTML = "coucou";
                    fetch('list_files.php')
                    .then(response => response.json())
                    .then(files => {
                        //const fileList = document.getElementById("fileList");
                        // Clear existing list
                        //fileList.innerHTML = "";
                        // Populate the list with fetched files
                        files.forEach(file => {
                            const listItem = document.createElement("li");
                            listItem.textContent = file;
                            const deleteButton = document.createElement('button');
                            deleteButton.textContent = 'DELETE';
                            deleteButton.addEventListener('click', () => {
                                sendDeleteRequest(file)
                                                .then(() => location.reload())
                                                .catch(error => console.error('Error deleting file:', error));
                                            });
                                            fileList.appendChild(deleteButton);
                                            fileList.appendChild(listItem);
                                        });
                                        
                                    })
                    .catch(error => console.error('Error fetching file list:', error));
        document.getElementById("listButton").innerText = "Hide Files";
        fileList.style.display = "block";
    } else {
                // If list is displayed, hide it and update the button text
                document.getElementById("fileList").innerHTML = "";
                document.getElementById("listButton").innerText = "List Files";
                fileList.style.display = "none"

    } 
}