import React, { useState } from "react";
import { Link, useNavigate } from "react-router-dom";
import CssBaseline from "@mui/material/CssBaseline";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import Stack from "@mui/material/Stack";
import Button from "@mui/material/Button";
import Card from "@mui/material/Card";
import CardContent from "@mui/material/CardContent";
import { useExerciseContext } from "./ExerciseContext";

import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import DialogContent from "@mui/material/DialogContent";
import DialogTitle from "@mui/material/DialogTitle";
import TextField from "@mui/material/TextField";

import Accordion from "@mui/material/Accordion";
import AccordionSummary from "@mui/material/AccordionSummary";
import AccordionDetails from "@mui/material/AccordionDetails";
import ExpandMoreIcon from "@mui/icons-material/ExpandMore";

import BookmarkBorderIcon from "@mui/icons-material/BookmarkBorder"; // Bookmark Icon (Outline)
import BookmarkIcon from "@mui/icons-material/Bookmark"; // Bookmark Icon (Filled)
import Snackbar from "@mui/material/Snackbar";

export default function Routines() {
    const navigate = useNavigate();
    const { exercises, setExercises } = useExerciseContext();

    // Load routines from localStorage
    const [routines, setRoutines] = useState(JSON.parse(localStorage.getItem("routines")) || []);

    // Fetch completed workouts from localStorage
    const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts")) || [];
    const last7Workouts = completedWorkouts
        .sort((a, b) => new Date(b.date) - new Date(a.date)) // Sort by date (descending)
        .slice(0, 7); // Get the most recent 7 workouts

    // Snackbar state for feedback
    const [snackbarOpen, setSnackbarOpen] = useState(false);
    const [snackbarMessage, setSnackbarMessage] = useState("");

    // Dialog-specific states
    const [routineDialogOpen, setRoutineDialogOpen] = useState(false);
    const [routineName, setRoutineName] = useState("");
    const [selectedWorkout, setSelectedWorkout] = useState(null);
    const [error, setError] = useState(false);

    const handleSnackbarClose = () => {
        setSnackbarOpen(false);
    };

    // Function to start a selected routine
    const startRoutine = (routine) => {
        setExercises(
            routine.exercises.map((exercise) => ({
                ...exercise,
                rows: exercise.rows.map((row, index) => ({
                    id: index, // Set a new ID based on the current index
                    ...row,
                })),
            }))
        );
        navigate("/workout");
    };

    const handleSaveWorkoutAsRoutine = () => {
        // Make sure a name is entered
        if (!routineName.trim()) {
            setError(true);
            return;
        }

        const routine = {
            ...selectedWorkout,
            name: routineName, // Use the provided name
        };

        // Update routines and save to localStorage
        const updatedRoutines = [...routines, routine];
        setRoutines(updatedRoutines);
        localStorage.setItem("routines", JSON.stringify(updatedRoutines));

        setRoutineDialogOpen(false); // Close dialog
        setRoutineName(""); // Reset dialog input
        setSelectedWorkout(null); // Clear selected workout
        setSnackbarMessage("Workout erfolgreich zur Routine gespeichert");
        setSnackbarOpen(true); // Show feedback
    };


    const addToRoutine = (workout) => {
        // Check if workout is already in routines
        const isAlreadyInRoutines = routines.some((routine) => routine.id === workout.id);

        if (isAlreadyInRoutines) {
            // Remove the workout from routines
            const updatedRoutines = routines.filter((routine) => routine.id !== workout.id);
            setRoutines(updatedRoutines);
            localStorage.setItem("routines", JSON.stringify(updatedRoutines));
            setSnackbarMessage("Workout aus Routinen entfernt");
            setSnackbarOpen(true);
        } else {
            // Open the dialog to save the workout as a routine
            setSelectedWorkout(workout);
            setRoutineDialogOpen(true); // Open the dialog
        }
    };

    return (
        <>
            <CssBaseline enableColorScheme />
            <Box
                sx={{
                    mx: "auto",
                    mt: 15,
                    mb: 10,
                    px: 2,
                }}
                display="flex"
                justifyContent="center"
            >
                <Stack
                    sx={{
                        width: "100%",
                        maxWidth: "500px",
                        height: "100%",
                    }}
                >
                    {/* Button to start a blank workout */}
                    <Button
                        sx={{
                            width: "50%",
                            alignSelf: "center",
                            mb: 5,
                        }}
                        variant="contained"
                        component={Link}
                        to="/workout"
                    >
                        Leeres Workout
                    </Button>

                    <Accordion
                        sx={{
                            background: "transparent",
                            boxShadow: "none",
                        }}
                        defaultExpanded
                    >
                        <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                            <Typography variant="h5" sx={{ fontWeight: "bold" }}>
                                Deine Routinen
                            </Typography>
                        </AccordionSummary>
                        <AccordionDetails>
                            {routines.length === 0 ? (
                                <Typography variant="body2" color="text.secondary">
                                    Keine gespeicherten Routinen gefunden.
                                </Typography>
                            ) : (
                                routines.map((routine) => (
                                    <Card
                                        key={routine.id}
                                        sx={{
                                            display: "flex",
                                            flexDirection: "column",
                                            mb: 2,
                                        }}
                                    >
                                        <CardContent
                                            sx={{
                                                display: "flex",
                                                flexDirection: "column",
                                            }}
                                        >
                                            <Typography variant="h6" gutterBottom>
                                                {routine.name}
                                            </Typography>
                                            <Typography variant="body2" color="text.secondary">
                                                {routine.exercises.map((exercise) => exercise.title).join(", ")}
                                            </Typography>
                                            <Button
                                                variant="contained"
                                                color="primary"
                                                sx={{ mt: 2 }}
                                                onClick={() => startRoutine(routine)}
                                            >
                                                Routine starten
                                            </Button>
                                        </CardContent>
                                    </Card>
                                ))
                            )}
                        </AccordionDetails>
                    </Accordion>

                    {/* Completed Workouts Accordion */}
                    <Accordion
                        sx={{
                            background: "transparent",
                            boxShadow: "none",
                        }}
                    >
                        <AccordionSummary
                            expandIcon={<ExpandMoreIcon />}
                            sx={{
                                backgroundColor: "transparent",
                            }}
                        >
                            <Typography variant="h5" sx={{ fontWeight: "bold" }}>
                                Letzte Workouts
                            </Typography>
                        </AccordionSummary>
                        <AccordionDetails>
                            {last7Workouts.length === 0 ? (
                                <Typography variant="body2" color="text.secondary">
                                    Keine abgeschlossenen Workouts gefunden.
                                </Typography>
                            ) : (
                                last7Workouts.map((workout) => (
                                    <Card
                                        key={workout.id}
                                        sx={{
                                            display: "flex",
                                            flexDirection: "column",
                                            mb: 2,
                                            position: "relative",
                                        }}
                                    >
                                        <CardContent
                                            sx={{
                                                display: "flex",
                                                flexDirection: "column",
                                            }}
                                        >
                                            <Box
                                                sx={{
                                                    position: "absolute",
                                                    top: 8,
                                                    right: 8,
                                                    cursor: "pointer",
                                                }}
                                                onClick={() => addToRoutine(workout)}
                                            >
                                                {/* Bookmark toggled based on routine existence */}
                                                {routines.some((routine) => routine.id === workout.id) ? (
                                                    <BookmarkIcon color="primary" />
                                                ) : (
                                                    <BookmarkBorderIcon />
                                                )}
                                            </Box>
                                            <Typography variant="h6" gutterBottom>
                                                {workout.date}
                                            </Typography>
                                            <Typography variant="body2" color="text.secondary">
                                                {workout.exercises.map((exercise) => exercise.title).join(", ")}
                                            </Typography>
                                        </CardContent>
                                    </Card>
                                ))
                            )}
                        </AccordionDetails>
                    </Accordion>
                </Stack>
            </Box>

            {/* Snackbar for feedback */}
            <Snackbar
                open={snackbarOpen}
                autoHideDuration={3000}
                onClose={handleSnackbarClose}
                message={snackbarMessage}
            />

            {/* Routine Dialog */}
            <Dialog open={routineDialogOpen} disableEscapeKeyDown hideBackdrop>
                <DialogTitle>Routine speichern</DialogTitle>
                <DialogContent>
                    <TextField
                        label="Name der Routine"
                        value={routineName}
                        onChange={(e) => {
                            setRoutineName(e.target.value);
                            setError(false);
                        }}
                        error={error}
                        helperText={error ? "Bitte einen Namen eingeben!" : ""}
                        fullWidth
                        sx={{ mb: 2 }}
                    />
                </DialogContent>
                <DialogActions>
                    <Button onClick={handleSaveWorkoutAsRoutine} color="primary">
                        OK
                    </Button>
                </DialogActions>
            </Dialog>
        </>
    );
}