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
});

window.addEventListener("load", function () {
  const themeSwitch = document.getElementById('themeSwitch');
  //  Verific tema
  if (localStorage.getItem('theme') === 'lightTheme' || localStorage.getItem('theme') === '') {
      setTheme('lightTheme');
      themeSwitch.checked = false;
  } else if (localStorage.getItem('theme') === 'darkTheme') {
      setTheme('darkTheme');
      themeSwitch.checked = true;
  }
  function setTheme(themeName) {
    localStorage.setItem('theme', themeName);
    if (themeName == "lightTheme") {
        document.body.style.setProperty("--primary-color", "#007bff");
        document.body.style.setProperty("--secondary-color", "#F0F2F5");
        document.body.style.setProperty("--text-color", "#444");
        document.body.style.setProperty("--chat-background-color", "#DCD7C9");
    }
    else if (themeName == "darkTheme") {
        document.body.style.setProperty("--primary-color", "#2C3639");
        document.body.style.setProperty("--secondary-color", "#3F4E4F");
        document.body.style.setProperty("--text-color", "#DCD7C9");
        document.body.style.setProperty("--chat-background-color", "#A27B5C");
    }
  }
  themeSwitch.addEventListener('change', () => {
    if (themeSwitch.checked) {
      setTheme("darkTheme");
    } else {
      setTheme("lightTheme");
    }
  });
});