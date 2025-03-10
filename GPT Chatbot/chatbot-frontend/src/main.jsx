import React, { useState } from "react";
import { createRoot } from "react-dom/client";
import ChatBot from "./App";
import { ThemeProvider, createTheme, CssBaseline } from "@mui/material";

const lightTheme = createTheme({
  palette: {
    mode: 'light',
  },
});

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
  },
});

const Root = () => {
  const [darkMode, setDarkMode] = useState(false);

  const toggleTheme = () => {
    setDarkMode(!darkMode);
  };

  return (
    <ThemeProvider theme={darkMode ? lightTheme : darkTheme}>
      <CssBaseline />
      <ChatBot darkMode={darkMode} toggleTheme={toggleTheme} />
    </ThemeProvider>
  );
};

const container = document.getElementById("root");
const root = createRoot(container);
root.render(<Root />);
