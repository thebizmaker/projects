import { Box, Button, TextField, Typography } from "@mui/material";
import { FaUser, FaRobot } from "react-icons/fa";
import Avatar from '@mui/material/Avatar';
// import { motion } from "framer-motion";
import React from "react";



function ChatWindow({darkMode, messages, typingResponse, input, setInput, handleSend}) {
    return (
    <Box ml={0.5} sx={{ height: "100%", display: "flex", flexDirection: "column", width: "80%" }}>
        <Box
        sx={{
          minHeight: 600,
          overflowY: "auto",
          border: "1px solid #ccc",
          borderRadius: 2,
          p: 2,
          mb: 2,
          width: "100%",
          backgroundColor: darkMode ? "#fff" : "#333",
        }}
      >
        {messages.map((msg, index) => (
          <Typography
            key={index}
            sx={{
              // textAlign: msg.sender === "user" ? "left" : "right",
              backgroundColor: msg.role === "user" ? (darkMode ? "skyblue" : "#1e201e") : (darkMode ? "lightgray" : "#6b7069"),
              borderRadius: 2,
              p: 1,
              my: 1,
              maxWidth: "70%",
              display: "flex",
              alignItems: "center",
              ml: msg.role === "user" ? 0 : "auto",
              fontWeight: darkMode ? "bold" : "normal",
            }}
          >
            {msg.role === "user" ? (
                <Avatar sx={{ bgcolor: "primary.main", marginRight: "5px" }}><FaUser /></Avatar>
                ) : (
                <Avatar sx={{ bgcolor: "secondary.main", marginRight: "5px" }}><FaRobot /></Avatar>
                )}
            {msg.text}
          </Typography>
        ))}
        {typingResponse && (
        //   <motion.p
        //     animate={{ opacity: [0, 1, 0] }}
        //     transition={{ duration: 1, times: [0, 0.5, 1], loop: Infinity }}
        //   >
            <Typography
              sx={{
                backgroundColor: darkMode ? "ligthgray" : "#6b7069",
                borderRadius: 2,
                p: 1,
                my: 1,
                maxWidth: "70%",
                display: "flex",
                alignItems: "center",
                ml: "auto",
                fontWeight: darkMode ? "bold" : "normal",
              }}
            >
              <Avatar sx={{ bgcolor: "secondary.main", marginRight: "5px" }}><FaRobot /></Avatar>
              {typingResponse}
            </Typography>
        //   </motion.p>
        )}
      </Box>
      <Box display="flex" sx={{ width: "100%" }}>
        <TextField
          fullWidth
          variant="outlined"
          placeholder="Frage stellen..."
          value={input}
          onChange={(e) => setInput(e.target.value)}
          onKeyDown={(e) => e.key === "Enter" && handleSend()}
        />
        <Button
          variant="contained"
          color="primary"
          onClick={handleSend}
          sx={{ ml: 1 }}
        >
          Senden
        </Button>
      </Box>
    </Box>
    );
}

export default ChatWindow;