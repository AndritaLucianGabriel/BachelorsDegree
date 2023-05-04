/* Copyright 2023 Andrita Lucian-Gabriel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

function sendToServer(inputContent) {
  // Sending the information back to the server so the bot can process it
  displayBubbleUntillResponse();
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

function displayBubbleUntillResponse() {
  const targetDiv = document.querySelector('.chat-content');
  const bubble = document.createElement('div');
  bubble.className='typing';
  bubble.id="bubble";

  const placheholder = document.createElement('span');
  placheholder.className = 'circle';
  bubble.appendChild(placheholder.cloneNode(true));
  bubble.appendChild(placheholder.cloneNode(true));
  bubble.appendChild(placheholder.cloneNode(true));

  targetDiv.appendChild(bubble);
  targetDiv.scrollTop = targetDiv.scrollHeight;
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
  let bubble = document.getElementById("bubble");

  while (bubble.firstChild) {
    bubble.removeChild(bubble.firstChild);
  }
  if (bubble.parentNode) {
    bubble.parentNode.removeChild(bubble);
  }

  const botMessage = document.createElement('div');
  botMessage.className = 'chat-message received';
  botMessage.innerHTML = `<p>${message}</p>`;
  targetDiv.appendChild(botMessage);

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

    const content = document.getElementsByClassName("chat-content")[0];
    const input = document.getElementById("input-text");

    content.addEventListener("click", function() {
      input.focus();
    });
});