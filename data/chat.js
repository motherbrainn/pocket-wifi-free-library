let socket; // Declare socket variable globally
let userId;
let userColor; // Declare user color globally

function generateRandomUserId() {
  return Math.random().toString(36).substr(2, 9); // Generate a random alphanumeric user ID
}

function generateRandomColor() {
  // Generate a random color in hex format
  return "#" + Math.floor(Math.random() * 16777215).toString(16);
}

function showLoadingOverlay() {
  const loadingOverlay = document.getElementById("loading-overlay");
  loadingOverlay.style.display = "block";
}

function hideLoadingOverlay() {
  const loadingOverlay = document.getElementById("loading-overlay");
  loadingOverlay.style.display = "none";
}

function initWebSocket() {
  showLoadingOverlay(); // Show loading overlay when WebSocket is opening
  userId = generateRandomUserId(); // Generate a unique user ID
  userColor = generateRandomColor(); // Generate a unique color for the user
  console.log("User ID:", userId);
  console.log("User Color:", userColor);

  socket = new WebSocket("ws://192.168.4.1:81");

  socket.addEventListener("open", (event) => {
    console.log("WebSocket connection opened:", event);
    hideLoadingOverlay(); // Hide loading overlay when WebSocket is opened

    // Send user information to the server
    const userInfo = { type: "userInfo", userId, userColor };
    socket.send(JSON.stringify(userInfo));
  });

  socket.addEventListener("message", (event) => {
    console.log("Message received:", event.data);
    // Handle the received message
    displayMessage(event.data);
  });

  socket.addEventListener("close", (event) => {
    console.log("WebSocket connection closed:", event);
  });

  socket.addEventListener("error", (event) => {
    console.error("WebSocket error:", event);
    hideLoadingOverlay(); // Hide loading overlay in case of an error
  });
}

function displayMessage(message) {
  const chatOutput = document.getElementById("chat-output");
  const messageObj = JSON.parse(message);

  // Check if the message type is userInfo
  if (messageObj.type === "userInfo") {
    return; // Skip displaying userInfo messages
  }

  const messageContainer = document.createElement("div");
  messageContainer.classList.add("message-container");

  const userIdElement = document.createElement("span");
  userIdElement.classList.add("user-id");
  userIdElement.style.color = messageObj.userColor; // Set the user color from the message
  userIdElement.textContent = `User ${messageObj.userId}: `;
  messageContainer.appendChild(userIdElement);

  const messageElement = document.createElement("span");
  messageElement.classList.add("message");
  messageElement.textContent = messageObj.message;
  messageContainer.appendChild(messageElement);

  chatOutput.appendChild(messageContainer);
}

function sendMessage() {
  const messageInput = document.getElementById("message-input");
  const message = messageInput.value.trim();

  // Check if the message is not empty
  if (message !== "") {
    if (socket.readyState === WebSocket.OPEN) {
      const messageWithUserInfo = { userId, userColor, message };
      socket.send(JSON.stringify(messageWithUserInfo));
      console.log("Message sent successfully:", messageWithUserInfo);
    } else {
      console.error("WebSocket not open. Unable to send message.");
    }

    messageInput.value = "";
  }
}

// Initialize the WebSocket connection
initWebSocket();
