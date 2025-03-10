import React, { useState, useEffect } from "react";
import axios from "axios";
import { Container, Box, Typography, IconButton } from "@mui/material";
import { Brightness4, Brightness7, Chat } from "@mui/icons-material";
import ChatWindow from "./ChatWindow";
import ChatSidebar from "./ChatSidebar";

const API_URL = import.meta.env.VITE_API_URL;

const Chatbot = ({ darkMode, toggleTheme }) => {
  const [username, setUsername] = useState("");
  const [conversations, setConversations] = useState([]);
  const [selectedConversation, setSelectedConversation] = useState(0);
  const [messages, setMessages] = useState([]);
  const [input, setInput] = useState("");
  const [typingResponse, setTypingResponse] = useState("");

  useEffect(() => {
    const storedUsername = localStorage.getItem("username");
    if (!storedUsername) {
      const user = prompt("Please enter your name") || new Date().toLocaleDateString('de-DE');
      localStorage.setItem("username", user);
      setUsername(user);
    } else {
      setUsername(storedUsername);
    }
  }, []);

  useEffect(() => {
    if (username) {
      fetchConversations(username);
    }
  }, [username]);

  const fetchConversations = async (user) => {
    try {
      const response = await axios.get(`${API_URL}/conversations?username=${user}`);
      setConversations(response.data);
      if (response.data.length > 0) {
        fetchMessages(response.data[0].id);
      }
    } catch (error) {
      console.error("Error while fetching conversations: ", error);
    }
  };

  const fetchMessages = async (conversationId) => {
    try {
      const response = await axios.get(`${API_URL}/messages?conversation_id=${conversationId}`);
      setMessages(response.data);
      setSelectedConversation(conversationId);
    } catch (error) {
      console.error("Error while loading messages: ", error);
    }
  };

  const handelSendMessage = async () => {
    if (!input.trim()) return;

    if (conversations.length === 0) {
      startNewConversation();
    }

    const userMessage = { role: "user", text: input };
    setMessages([...messages, userMessage]);
    setInput("");
    
    try {
      const response = await axios.post(`${API_URL}/chat/`, {
        message: input,
        conversation_id: selectedConversation,
        username,
      });
      displayTypingEffect(response.data.reply);
    } catch (error) {
      console.error("Error while sending message: ", error);
    }
  };

  const displayTypingEffect = (responseText) => {
    setTypingResponse("");
    let i = 0;
    const interval = setInterval(() => {
      if (i < responseText.length) {
        setTypingResponse((prev) => prev + responseText[i]);
        i++;
      } else {
        clearInterval(interval);
        setMessages([...messages, { role: "bot", text: responseText }]);
        setTypingResponse("");
        fetchMessages(selectedConversation);
      }
    }, 20);
  };

  const handelDeleteConversation = async (conversationId) => {
    try {
      const res = await axios.delete(`${API_URL}/conversation/${conversationId}`);
      if (res.status === 204) {
        setConversations(conversations.filter((conv) => conv.id !== conversationId));
        fetchConversations(username);
      }
      if (selectedConversation === conversationId) {
        setMessages([]);
        setSelectedConversation(0);
      }
    } catch (error) {
      console.error("Error while deleting conversation: ", error);
    }
  }

  const startNewConversation = async () => {
    try {
      const response = await axios.post(`${API_URL}/conversation/`, { username: username });
      setConversations([...conversations, response.data]);
      setSelectedConversation(response.data.id);
    } catch (error) {
      console.error("Error while starting new conversation: ", error);
    }
  };

  return (
    <Container maxWidth="lg" sx={{ margin: 'auto', mt: 4, width: "100%" }}>
      <Box display="flex" justifyContent="space-between" alignItems="center">
        <Typography variant="h4" align="center" gutterBottom sx={{ fontWeight: "bold" }}>
          Dester&apos;s Chatbot | chat with GPT
        </Typography>
        <IconButton onClick={toggleTheme} color="inherit">
          {darkMode ? <Brightness4 /> : <Brightness7 />}
        </IconButton>
      </Box>
      <Box display="flex" flexGrow={1} sx={{ width: "100%" }}>
        <ChatSidebar
          conversations={conversations}
          selectedChatIndex={selectedConversation}
          setSelectedConversation={setSelectedConversation}
          fetchMessages={fetchMessages}
          deleteConversation={handelDeleteConversation}
          startNewConversation={startNewConversation}
          darkMode={darkMode}
        />
        {/* <Box sx={{ width: '2px', backgroundColor: 'white' }} /> */}
        <ChatWindow
          darkMode={darkMode}
          messages={messages}
          typingResponse={typingResponse}
          input={input}
          setInput={setInput}
          handleSend={handelSendMessage}
        />
      </Box>
    </Container>
  );
};

export default Chatbot;
