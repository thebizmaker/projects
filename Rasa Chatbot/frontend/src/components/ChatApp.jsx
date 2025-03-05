import React, { useState, useEffect } from "react";
import axios from "axios";
import ChatSidebar from "./ChatSidebar";
import ChatWindow from "./ChatWindow";
// import Container from "react-bootstrap/Container";
import { Box } from "@mui/material";
// import Row from "react-bootstrap/Row";
// import Col from "react-bootstrap/Col";

const ChatApp = () => {
    const [chats, setChats] = useState([]);
    const [selectedChatIndex, setSelectedChatIndex] = useState(0);
    const [conversations, setConversations] = useState([]);
    const userId = "user1234";

    useEffect(() => {
        const fetchChatHistory = async () => {
            try {
                const currentConversationId = conversations[selectedChatIndex]?.conversationId;
                if (currentConversationId) {
                    const response = await axios.get(`http://localhost:5001/history?user_id=${userId}&conversation_id=${currentConversationId}`);
                    const data = response.data;

                    const formattedData = data.map(chat => ({
                        user: chat.user_message,
                        bot: chat.bot_response,
                    }));

                    const updatedChats = [...chats];
                    updatedChats[selectedChatIndex] = formattedData;
                    setChats(updatedChats);
                    localStorage.setItem("chats", JSON.stringify(updatedChats));
                }
            } catch (error) {
                console.error("Error fetching chat history", error);
            }
        };
        fetchChatHistory();
    }, [userId, conversations, selectedChatIndex]);

    const handleSendMessage = async (message) => {
        const updatedChats = [...chats];
        if (!updatedChats[selectedChatIndex]) {
            updatedChats[selectedChatIndex] = [];
        }
        updatedChats[selectedChatIndex].push({ user: message });
        setChats(updatedChats);
        localStorage.setItem("chats", JSON.stringify(updatedChats));
        const currentConversationId = conversations[selectedChatIndex]?.conversationId;
        if (!currentConversationId) {
            console.error("No conversation Id found");
            return;
        }
        try {
            const response = await axios.post("http://localhost:5001/chat",
                {
                    message: message,
                    user_id: userId,
                    conversation_id: currentConversationId,
                }, {
                headers: {
                    "Content-Type": "application/json",
                },
            });
            const botResponse = response.data[0]?.text || 'No response';
            updatedChats[selectedChatIndex].push({ bot: botResponse });
            setChats([...updatedChats]);
            localStorage.setItem("chats", JSON.stringify(updatedChats));
        } catch (error) {
            console.error("Error sending message", error);
        }
    };

    const startNewConversation = async () => {
        try {
            const response = await axios.post("http://localhost:5001/new_conversation",
                {
                    user_id: userId,
                });
            const newConversationId = response.data.conversation_id;
            const newConversations = [...conversations, { conversationId: newConversationId }];
            setConversations(newConversations);
            localStorage.setItem("conversations", JSON.stringify(newConversations));
            const newChats = [...chats, []];
            setChats(newChats);
            localStorage.setItem("chats", JSON.stringify(newChats));
            const newChatIndex = newChats.length - 1;
            setSelectedChatIndex(newChatIndex);
            localStorage.setItem("selectedChatIndex", newChatIndex);
            console.log("New conversation started", newConversationId);
        } catch (error) {
            console.error("Error starting new conversation", error);
        }
    };

    const deleteConversation = async (index) => {
        const conversationId = conversations[index]?.conversationId;
        if (!conversationId) {
            console.error("No conversation Id found");
            return;
        }
        try {
            await axios.delete(`http://localhost:5001/conversation`,
                {
                    data: {
                        user_id: userId,
                        conversation_id: conversationId,
                    },
                }
            );
            const updatedConversations = conversations.filter((_, i) => i !== index);
            setConversations(updatedConversations);
            localStorage.setItem("conversations", JSON.stringify(updatedConversations));
            const updatedChats = chats.filter((_, i) => i !== index);
            setChats(updatedChats);
            localStorage.setItem("chats", JSON.stringify(updatedChats));
            if (selectedChatIndex >= updatedConversations.length) {
                setSelectedChatIndex(updatedConversations.length - 1);
                localStorage.setItem("selectedChatIndex", updatedConversations.length - 1);
            }
        } catch (error) {
            console.error("Error deleting conversation", error);
        }
    };

    const selectChat = (index) => {
        setSelectedChatIndex(index);
        localStorage.setItem("selectedChatIndex", index);
    };

    return (
        <Box sx={{ display: 'flex', height: '800px' }}>
            <ChatSidebar
                chats={chats}
                selectChat={selectChat}
                startNewConversation={startNewConversation}
                deleteConversation={deleteConversation}
                selectedChatIndex={selectedChatIndex}
            />
            <Box sx={{ width: '2px', backgroundColor: 'white' }} />
            <ChatWindow
                chat={chats[selectedChatIndex] || []}
                onSendMessage={handleSendMessage}
                conversationId={conversations[selectedChatIndex]?.conversationId}
            />
        </Box>
    );
};
export default ChatApp;