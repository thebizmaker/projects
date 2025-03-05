import React, { useState } from "react";
import { Button, Drawer, List, ListItem, ListItemText, IconButton, Typography, Box } from '@mui/material';
import { FaTrash, FaBars } from "react-icons/fa";
// import { light } from "@mui/material/styles/createPalette";

const ChatSidebar = ({ chats, selectChat, deleteConversation, startNewConversation, selectedChatIndex }) => {
    // const [show, setShow] = useState(false);

    return (
        <>
            {/* <Button variant="contained" onClick={() => setShow(!show)} style={{ position: 'fixed', top: '16px', right: '16px' }}>
                <FaBars />
            </Button> */}
            {/* <Drawer anchor="left" open={show} onClose={() => setShow(false)}> */}
                <Box sx={{ width: 250 }} bgcolor="#1e201e">
                    <Typography variant="h6" sx={{ padding: '16px', color: "white", fontWeight: 'bold' }}>Rasa ChatBot</Typography>
                    <List>
                        {chats.length === 0 ? (
                            <ListItem  sx={{ textAlign: 'center', color: "white", backgroundColor: "#6b7069", width: '80%', margin: 'auto' }}>
                                <ListItemText primary="No conversations yet" />
                            </ListItem>
                        ) : (
                            chats.map((chat, index) => (
                                <ListItem button key={index} selected={index === selectedChatIndex} onClick={() => selectChat(index)}  sx={{ textAlign: 'center', color: "white", backgroundColor: "#6b7069", width: '80%', margin: 'auto', marginBottom: '10px' }}>
                                    <ListItemText primary={`Conversation ${index + 1}`}/>
                                    <Box ml={2}>
                                        <IconButton edge="end" aria-label="delete" onClick={(e) => {
                                            e.stopPropagation();
                                            if (window.confirm("Are you sure you want to delete this conversation?")) {
                                                deleteConversation(index);
                                            }
                                        }} sx={{ color: "white" }}>
                                            <FaTrash />
                                        </IconButton>
                                    </Box>
                                </ListItem>
                            ))
                        )}
                    </List>
                    <Button variant="contained" color="primary" onClick={startNewConversation} sx={{ margin: 'auto', marginTop: '16px', width: '80%', backgroundColor: "#ECDFCC", color: "#1e201e" }}>
                        New Chat
                    </Button>
                </Box>
            {/* </Drawer> */}
        </>
    );
};

export default ChatSidebar;