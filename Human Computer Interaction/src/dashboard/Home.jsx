import * as React from 'react';

import Box from '@mui/material/Box';
import CssBaseline from '@mui/material/CssBaseline';
import MainGrid from '../components/MainGrid';




export default function Home(props) {
  return (

<>
      <CssBaseline enableColorScheme />



      {/* <AppNavbar title="Home"/>
          <FixedBottomNavigation />*/}

        <Box sx={{
            mx: 1,
            mt: 10,
            mb: 10,
        }}>
            <MainGrid />
        </Box>
</>
  );
}
