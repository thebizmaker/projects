import { createTheme } from '@mui/material/styles';
import { red } from '@mui/material/colors';

// Create a theme instance.
const theme = createTheme({
  cssVariables: true,
  palette: {
    mode: 'dark', // Aktiviert den Dark Mode
  },
});

export default theme;
