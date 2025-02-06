import * as React from 'react';
import AppBar from '@mui/material/AppBar';
import Box from '@mui/material/Box';
import Toolbar from '@mui/material/Toolbar';
import FitnessCenterIcon from '@mui/icons-material/FitnessCenter';


export default function AppNavbar(props) {
    return (
        <AppBar
            position="fixed"
            sx={{
                bgcolor: 'background.paper',
                backgroundImage: 'none',
                borderBottom: '1px solid',
                borderColor: 'divider',
            }}
        >
            <Toolbar>
                {/* Linksbündiges Icon */}
                <FitnessCenterIcon
                    fontSize="large"
                    sx={{ color: 'primary.main' }}
                />

                {/* Platzhalter, um beide Seiten auseinander zu ziehen */}
                <Box sx={{ flexGrow: 1 }} />

                {/* Rechtsbündiges Dropdown */}
               {/*<ColorModeIconDropdown />*/}
            </Toolbar>
        </AppBar>
    );
}