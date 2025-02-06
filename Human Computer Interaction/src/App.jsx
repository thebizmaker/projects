import * as React from 'react';
import * as statistics from './utils/decisionSupport.js'

import CssBaseline from '@mui/material/CssBaseline';

import AppNavbar from './components/AppNavbar.jsx';

import FixedBottomNavigation from './components/CustomBottomNavBar.jsx';


export default function App(props) {
    return (
<>

    <CssBaseline enableColorScheme />
    <AppNavbar title="Home"/>
    <FixedBottomNavigation />

</>

    );
}
