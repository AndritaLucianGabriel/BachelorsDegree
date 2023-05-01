document.addEventListener("DOMContentLoaded", function () {
    const button = document.getElementById("myButton");
    const content = document.getElementById("content");
    var a = 0;

    button.addEventListener("click", function () {
        content.innerHTML = "This content was updated " + a +" times by the button click.";
        a++;
    });
});

function sendData() {
    const data = {
      message: 'Hello from JavaScript!'
    };

    fetch('http://localhost:9090', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(data)
    })
    .then(response => response.text())
    .then(text => {
    console.log('Data received from the server:', text);
    })
    .catch(error => {
    console.error('Error:', error);
    });
}