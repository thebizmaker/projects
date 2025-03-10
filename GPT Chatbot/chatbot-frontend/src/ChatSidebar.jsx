import React from 'react';
import { Box, Button, IconButton, List, ListItem, ListItemText, Typography } from '@mui/material';
import { FaTrash } from 'react-icons/fa';




function ChatSidebar({ conversations, selectedChatIndex, setSelectedConversation, fetchMessages, deleteConversation, startNewConversation, darkMode }) {
  return (
    <>
        <Box
        sx={{ width: "300px", backgroundColor: darkMode ? "#fff" : "#333", border: "1px solid #ccc", borderRadius: 2, }}
        textAlign={'center'}
        mr={0.5}
        >
            <List sx={{ paddingTop: '15px'}}>
                {conversations.length === 0 ? (
                    <ListItem 
                    sx={{ textAlign: 'center',
                    color: darkMode ? "black" : "white",
                    backgroundColor: darkMode ? "lightgray" : "#6b7069",
                    width: '80%',
                    height: '40px',
                    borderRadius: 1,
                    margin: 'auto' }}>
                        <ListItemText primary="No conversations yet" />
                    </ListItem>
                ) : (
                    conversations.map((conv, index) => (
                        <ListItem
                        button
                        key={conv.id}
                        selected={index === (selectedChatIndex === conv.id ? parseInt(conv.name[conv.name.length - 1])-1 : selectedChatIndex)}
                        onClick={function () {
                            setSelectedConversation(conv.id);
                            fetchMessages(conv.id);
                        }} 
                        sx={{ textAlign: 'center',
                            color: darkMode ? "black" : "white",
                            backgroundColor: index === (selectedChatIndex === conv.id ? parseInt(conv.name[conv.name.length - 1])-1 : selectedChatIndex) ? (darkMode ? "#90caf9" : "#1e88e5") : (darkMode ? "lightgray" : "#6b7069"),
                            width: '80%',
                            height: '40px',
                            borderRadius: 1,
                            margin: 'auto',
                            marginBottom: '10px',
                            cursor: 'pointer',
                        }}
                        >
                            <ListItemText primary={conv.name}/>
                            <Box ml={2}>
                                <IconButton edge="end" aria-label="delete" onClick={(e) => {
                                    e.stopPropagation();
                                    if (window.confirm("Are you sure you want to delete this conversation?")) {
                                        deleteConversation(conv.id);
                                    }
                                }} sx={{ color: "red" }}>
                                    <FaTrash />
                                </IconButton>
                            </Box>
                        </ListItem>
                    ))
                )}
            </List>
            <Button
            variant="contained"
            color="success"
            onClick={startNewConversation}
            sx={{ margin: 'auto', marginTop: '16px', width: '80%' }}
            >
                New Chat
            </Button>
        </Box>
    </>
  );
}

export default ChatSidebar;