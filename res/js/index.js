function sendToServer(inputContent) {
  // Sending the information back to the server so the bot can process it
  if(inputContent === "") {
    inputContent = "Hello!";
  }
  const data = {
    message: inputContent
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
    displayReceivedMessage(text);
  })
  .catch(error => {
  console.error('Error:', error);
  });
}

function displaySentMessage(message) {
  if (message.trim() === '') return;
  const targetDiv = document.querySelector('.chat-content');
  const userMessage = document.createElement('div');
  userMessage.className = 'chat-message sent';
  userMessage.innerHTML = `<p>${message}</p>`;
  targetDiv.append(userMessage);
  targetDiv.scrollTop = targetDiv.scrollHeight;
}

function displayReceivedMessage(message) {
  const targetDiv = document.querySelector('.chat-content');
  const botMessage = document.createElement('div');
  botMessage.className = 'chat-message received';
  botMessage.innerHTML = `<p>${message}</p>`;
  targetDiv.appendChild(botMessage);
  console.log(targetDiv);
  targetDiv.scrollTop = targetDiv.scrollHeight;
}

function process() {
  let inputContent = document.getElementById("input-text").value.trim();
  displaySentMessage(inputContent);
  sendToServer(inputContent);
}

document.addEventListener("DOMContentLoaded", function () {
    const button = document.getElementById("my-button");
    inputText = document.getElementById("input-text");

    // Add event listener to input text for Enter key
    inputText.addEventListener("keyup", function (event) {
      if (event.key === "Enter") {
        process();
        inputText.value = ""; // clear the input text after sending
      }
    });

    button.addEventListener("click", function () {
      process();
      inputText.value = ""; // clear the input text after sending
    });
});