// src/main.jsx
import * as React from 'react';
import { createRoot } from 'react-dom/client';
import CssBaseline from '@mui/material/CssBaseline';
import { ThemeProvider } from '@mui/material/styles';
import App from './App';
import { BrowserRouter, Route, Routes } from "react-router-dom";
import Routines from "./Routines";
import { Outlet } from 'react-router-dom';
import Home from './dashboard/Home';
import AddExercise from "./AddExercise";
import theme from "./theme";
import Workout from "./Workout";
import Settings from './Settings';
import Koerperdaten from './Koerperdaten';
import WorkoutDetails from './WorkoutDetails';

import { ExerciseProvider } from "./ExerciseContext";
import "./CompletedWorkouts";


const rootElement = document.getElementById('root');
const root = createRoot(rootElement);

root.render(
    <React.StrictMode>
        <ThemeProvider theme={theme}>
            <CssBaseline />
            <ExerciseProvider>
            <BrowserRouter>
                <Routes>
                    <Route path="/" element={<><App /><Outlet /></>}>
                        <Route index element={<Home />} />
                        <Route path="routinen" element={<Routines />} />
                        <Route path="workout" element={<Workout />} />
                        <Route path="add-exercise" element={<AddExercise />} />
                        <Route path="settings" element={<Settings />} />
                        <Route path="koerperdaten" element={<Koerperdaten />}  />
                        <Route path="workout/:date" element={<WorkoutDetails />} />
                    </Route>
                </Routes>
            </BrowserRouter>
            </ExerciseProvider>
        </ThemeProvider>
    </React.StrictMode>
);