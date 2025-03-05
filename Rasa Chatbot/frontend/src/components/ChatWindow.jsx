import React, { useState, useRef, useEffect } from "react";
import { Box, TextField, Button, Typography, Paper } from '@mui/material';

const ChatWindow = ({ chat = [], onSendMessage, conversationId }) => {
    const [message, setMessage] = useState("");
    const messagesEndRef = useRef(null);

    const handleSubmit = (e) => {
        e.preventDefault();
        if (!conversationId) {
            console.error("No conversation Id provided");
            return;
        }
        if (message.trim()) {
            onSendMessage(message);
            setMessage("");
        }
    };

    useEffect(() => {
        messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
    }, [chat, message]);

    return (
        <Box display="flex" flexDirection="column" flexGrow={1} p={4} bgcolor="gray" sx={{ width: 800 }}>
            <Box flexGrow={1} overflow="auto" border={1} p={3}>
                {chat.length === 0 ? (
                    <Typography align="center" color="white" variant="h6">How can I help you?</Typography>
                ) : (
                    chat.map((msg, index) => (
                        <Box key={index} my={2}>
                            {msg.user && (
                                <Box textAlign="right">
                                    <Paper elevation={3} style={{ display: 'inline-block', padding: '8px', backgroundColor: '#3C3D37', color: 'white' }}>
                                        <Typography variant="body1"><strong>User:</strong> {msg.user}</Typography>
                                    </Paper>
                                </Box>
                            )}
                            {msg.bot && (
                                <Box textAlign="left">
                                    <Paper elevation={3} style={{ display: 'inline-block', padding: '8px', backgroundColor: '#f0f0f0' }}>
                                        <Typography variant="body1"><strong>Bot:</strong> {msg.bot}</Typography>
                                    </Paper>
                                </Box>
                            )}
                        </Box>
                    ))
                )}
                <div ref={messagesEndRef} />
            </Box>
            <Box component="form" onSubmit={handleSubmit} display="flex" mt={3}>
                <TextField
                    variant="outlined"
                    fullWidth
                    value={message}
                    onChange={(e) => setMessage(e.target.value)}
                    placeholder="Type a message"
                    sx={{ backgroundColor: 'white', borderRadius: '5px' }}
                />
                <Button type="submit" variant="contained" color="info" style={{ marginLeft: '8px' }}>
                    Send
                </Button>
            </Box>
        </Box>
    );
};

export default ChatWindow;