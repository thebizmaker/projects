import React, { useState, useEffect } from "react";

import {
    Box,
    Button,
    TextField,
    Dialog,
    DialogActions,
    DialogContent,
    DialogTitle,
    Card,
    CardContent,
    CardHeader,
    Typography,
    Checkbox
} from "@mui/material";

import { DataGrid } from "@mui/x-data-grid";
import { useExerciseContext } from "./ExerciseContext";
import { Link, useNavigate } from "react-router-dom";
import { BookmarkBorder, Bookmark } from '@mui/icons-material'; // Importiere die Icons

const Workout = () => {
    const {
        exercises,
        setExercises,
        startWorkoutTime,
        endWorkoutTime,
        elapsedTime,
        startTime,
        toggleRowCheck,
    } = useExerciseContext();
    const navigate = useNavigate();

        const [workoutDate, setWorkoutDate] = useState(
        () => localStorage.getItem("workoutDate") || new Date().toISOString().split("T")[0]
    );
    const [routineDialogOpen, setRoutineDialogOpen] = useState(false);
    const [routineName, setRoutineName] = useState("");
    const [error, setError] = useState(false);
    const [isRoutineMarked, setIsRoutineMarked] = useState(false); // Neuer Zustand für Routine-Markierung

    // Workout-Startzeit setzen beim Laden
    useEffect(() => {
        startWorkoutTime(); // Workout starten
        console.log("Workout gestartet um:", startTime);
    }, []);

    // Datum speichern bei Änderung
    useEffect(() => {
        localStorage.setItem("workoutDate", workoutDate);
    }, [workoutDate]);

    const handleAddRow = (exerciseIndex) => {
        setExercises((prevExercises) =>
            prevExercises.map((exercise, index) =>
                index === exerciseIndex
                    ? {
                        ...exercise,
                        rows: [
                            ...exercise.rows,
                            { id: exercise.rows.length + 1, reps: "", weight: "", checked: false },
                        ],
                    }
                    : exercise
            )
        );
    };

    const handleDeleteRow = (exerciseIndex) => {
        setExercises((prevExercises) =>
            prevExercises.map((exercise, index) =>
                index === exerciseIndex
                    ? { ...exercise, rows: exercise.rows.slice(0, -1) }
                    : exercise
            )
        );
    };

    const processRowUpdate = (exerciseIndex, newRow) => {
        setExercises((prevExercises) =>
            prevExercises.map((exercise, index) =>
                index === exerciseIndex
                    ? {
                        ...exercise,
                        rows: exercise.rows.map((row) =>
                            row.id === newRow.id ? newRow : row
                        ),
                    }
                    : exercise
            )
        );
        return newRow;
    };

    const handleEndWorkout = () => {
        // Workout Endzeit setzen und Dauer berechnen
        const duration = endWorkoutTime();

        // Abgeschlossenes Workout speichern
        const existingWorkouts = JSON.parse(localStorage.getItem("completedWorkouts")) || [];
        const lastWorkoutId = existingWorkouts.length > 0
            ? existingWorkouts[existingWorkouts.length - 1].id
            : 0;
        const newWorkoutId = lastWorkoutId + 1;

        const updatedExercises = exercises.map((exercise) => ({
            ...exercise,
            rows: exercise.rows.map(({ id, ...rest }) => rest),
        }));

        const updatedWorkouts = [
            ...existingWorkouts,
            {
                id: newWorkoutId,
                date: workoutDate,
                duration: duration, // Dauer speichern
                exercises: updatedExercises,
            },
        ];

        localStorage.setItem("completedWorkouts", JSON.stringify(updatedWorkouts));

        // Reset workout Kontext
        setExercises([]);
        localStorage.removeItem("workoutDate");

        if (isRoutineMarked) {
            setRoutineDialogOpen(true);
            setIsRoutineMarked(false);
        } else {
            navigate("/");
        }
    };

    const handleSaveWorkoutAsRoutine = () => {
        // Überprüfen, ob ein Name eingegeben wurde
        if (!routineName.trim()) {
            setError(true);
            return;
        }

        // Letztes Workout aus 'completedWorkouts' holen
        const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts")) || [];
        if (completedWorkouts.length === 0) {
            console.error("Keine abgeschlossenen Workouts verfügbar.");
            setError(true);
            return; // Beende die Funktion, wenn keine Workouts verfügbar sind
        }
        const lastWorkout = completedWorkouts[completedWorkouts.length - 1]; // Letztes Element

        // Routine-Objekt erstellen
        const routine = {
            id: lastWorkout.id,
            name: routineName,       // Name der Routine
            date: lastWorkout.date,    // Datum aus dem letzten Workout
            exercises: lastWorkout.exercises, // Übungen aus dem letzten Workout
        };

        // Vorhandene Routinen laden und aktualisieren
        const existingRoutines = JSON.parse(localStorage.getItem("routines")) || [];
        const updatedRoutines = [...existingRoutines, routine];

        // Routinen in localStorage speichern
        localStorage.setItem("routines", JSON.stringify(updatedRoutines));

        // Zustand zurücksetzen
        setRoutineDialogOpen(false);
        setRoutineName("");
        setError(false);

        // Navigation zurück zur Startseite
        navigate("/");
    };

    return (
        <Box sx={{ mx: 1, mt: 10, mb: 10 }}>
            <Box sx={{ textAlign: "center", mb: 3, display: "flex", alignItems: "center", justifyContent: "center", gap: 1 }}>
                <TextField
                    label="Datum"
                    type="date"
                    value={workoutDate}
                    onChange={(e) => setWorkoutDate(e.target.value)}
                />
                {isRoutineMarked ? (
                    <Bookmark
                        color="primary"
                        onClick={() => setIsRoutineMarked(false)}
                    />
                ) : (
                    <BookmarkBorder
                        onClick={() => setIsRoutineMarked(true)}
                    />
                )}
            </Box>

            {exercises.map((exercise, index) => (
                <Card key={index} sx={{ mb: 4 }}>
                    <CardHeader
                        title= {exercise.title}
                        sx={{ textAlign: "center", paddingBottom: 0 }}
                    />
                    <CardContent sx={{ p: 2 }}>
                        <DataGrid
                            rows={exercise.rows}
                            columns={[
                                {
                                    field: "checked",
                                    headerName: "",
                                    width: 62,
                                    renderCell: (params) => (
                                        <Checkbox
                                            checked={params.row.checked || false}
                                            onChange={() => toggleRowCheck(index, params.row.id)}
                                            color="primary"
                                        />
                                    ),
                                },
                                { field: "id", headerName: "Satz", width: 100 },
                                {
                                    field: "weight",
                                    headerName: "Gewicht (kg)",
                                    width: 150,
                                    editable: true,
                                },
                                {
                                    field: "reps",
                                    headerName: "Wiederholungen",
                                    width: 150,
                                    editable: true,
                                }
                            ]}
                            editMode="cell"
                            processRowUpdate={(newRow) => processRowUpdate(index, newRow)}
                            hideFooter
                        />
                        <Box sx={{ display: "flex", justifyContent: "space-between", mt: 2 }}>
                            <Button
                                variant="contained"
                                color="primary"
                                onClick={() => handleAddRow(index)}
                            >
                                Neuen Satz hinzufügen
                            </Button>
                            <Button
                                variant="outlined"
                                color="secondary"
                                onClick={() => handleDeleteRow(index)}
                            >
                                Letzten Satz entfernen
                            </Button>
                        </Box>
                    </CardContent>
                </Card>
            ))}

            {exercises.length === 0 && (
                <Box sx={{ display: "flex", justifyContent: "center", mt: 4 }} />
            )}

            <Box sx={{ display: "flex", justifyContent: "center", mt: 4, gap: 2 }}>
                <Button
                    variant="contained"
                    color="success"
                    component={Link}
                    to="/add-exercise"
                >
                    Neue Übung hinzufügen
                </Button>
                <Button variant="contained" color="error" onClick={handleEndWorkout}>
                    Workout beenden
                </Button>
            </Box>

            <Dialog open={routineDialogOpen} disableEscapeKeyDown hideBackdrop >
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
        </Box>
    );
};

export default Workout;