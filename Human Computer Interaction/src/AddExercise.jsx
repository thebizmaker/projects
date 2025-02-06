// Importiere die Übungen-Daten aus exercises.jsx
import Autocomplete from "@mui/material/Autocomplete";
import Box from "@mui/material/Box";
import Button from "@mui/material/Button";
import Stack from "@mui/material/Stack";
import TextField from "@mui/material/TextField";
import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import SuggestionBox from "./components/SuggestionBox";
import { useExerciseContext } from "./ExerciseContext";
import { exercises } from "./exercises"; // Passe den Pfad entsprechend an

import Chip from "@mui/material/Chip";

function MusclegroupSearch({ selectedGroup, setSelectedGroup }) {
    const options = Object.keys(exercises); // Muskelgruppen aus den Schlüsseln der Daten

    return (
        <Autocomplete
            options={options}
            value={selectedGroup}
            onChange={(event, newValue) => {
                setSelectedGroup(newValue); // Setze die ausgewählte Muskelgruppe
            }}
            renderOption={(props, option) => (
                <Box
                    {...props}
                    sx={{
                        display: "flex",
                        alignItems: "center",
                        paddingX: 1,
                        paddingY: 0.5,
                        gap: 1, // Abstand zwischen Elementen
                    }}
                >
                    {/* Muskelgruppe als Chip */}
                    <Chip
                        label={option} // Muskelgruppe wird als Chip angezeigt
                        color="primary" // Optional: Hintergrundfarbe des Chips

                        sx={{
                            fontWeight: "bold",
                            textTransform: "capitalize", // Erster Buchstabe groß
                        }}
                    />
                </Box>
            )}
            getOptionLabel={(option) => option} // Zeigt die Muskelgruppe als Namen an
            renderInput={(params) => (
                <TextField
                    {...params}
                    label="Muskelgruppe (optional)"
                    variant="outlined"
                />
            )}
        />
    );
}

function ExerciseSearch({ selectedGroup, selectedExercise, setSelectedExercise }) {
    // Sammle die Übungen mit Muskelgruppen
    const options = selectedGroup
        ? (exercises[selectedGroup] || []).map((exercise) => ({ exercise, muscleGroup: selectedGroup }))
        : Object.entries(exercises).flatMap(([group, exerciseList]) =>
            exerciseList.map((exercise) => ({ exercise, muscleGroup: group }))
        );

    return (
        <Autocomplete
            options={options}
            getOptionLabel={(option) => option.exercise} // Zeigt nur den Übungsnamen an
            value={
                selectedExercise
                    ? options.find((option) => option.exercise === selectedExercise) || null
                    : null
            }
            onChange={(event, newValue) => {
                // Speichere nur den Namen der Übung
                setSelectedExercise(newValue ? newValue.exercise : "");
            }}
            renderOption={(props, option) => (
                <Box
                    {...props}
                    sx={{
                        display: "flex",
                        alignItems: "center",
                        justifyContent: "space-between",
                        paddingX: 1,
                        paddingY: 0.5,
                        gap: 1
                    }}
                >
                    {/* Übungsname */}
                    <span>{option.exercise}</span>
                    {/* Muskelgruppe als Chip */}
                    <Chip
                        label={option.muscleGroup} // Muskelgruppe als Text
                        color="primary" // Optionale Hintergrundfarbe aus MUI-Theme
                        size="small" // Kleine Größe des Chips
                        sx={{
                            fontWeight: "bold",
                            textTransform: "capitalize", // Erster Buchstabe groß
                        }}
                    />
                </Box>
            )}
            renderInput={(params) => (
                <TextField
                    {...params}
                    label="Übung"
                    variant="outlined"
                />
            )}
        />
    );
}

export default function AddExercise() {
    const [selectedGroup, setSelectedGroup] = useState(""); // Ausgewählte Muskelgruppe
    const [selectedExercise, setSelectedExercise] = useState(""); // Ausgewählte Übung
    const { addExercise } = useExerciseContext(); // Kontext verwenden
    const navigate = useNavigate(); // Navigations-Hook

    const handleAddExercise = () => {
        if (selectedExercise) {
            // Suche die Muskelgruppe basierend auf der `selectedExercise`
            const muscleGroup = Object.keys(exercises).find(group =>
                exercises[group].includes(selectedExercise)
            );

            // Speichere die Übung und Muskelgruppe im Kontext
            addExercise({ title: selectedExercise, muscleGroup });
            alert(`Übung "${selectedExercise}" in der Muskelgruppe "${muscleGroup}" wurde hinzugefügt!`);
            navigate("/workout"); // Weiterleiten zur Route "/workout"
        } else {
            alert("Bitte wähle eine Übung aus!"); // Sicherstellen, dass eine Übung ausgewählt ist
        }
    };

    return (
        <Stack
            spacing={5} // Abstand zwischen den Komponenten (16px)
            sx={{
                justifyContent: "flex-start",
                mx: 5,
                mt: 10,
                mb: 10,
                position: "relative",
            }}
        >
            {<SuggestionBox />}
            {/* Optional: Muskelgruppen-Textfeld */}
            <MusclegroupSearch
                selectedGroup={selectedGroup}
                setSelectedGroup={setSelectedGroup}
            />
            {/* Übungen-Textfeld */}
            <ExerciseSearch
                selectedGroup={selectedGroup}
                selectedExercise={selectedExercise}
                setSelectedExercise={setSelectedExercise}
            />
            {/* Hinzufügen-Button */}
            <Box
                sx={{
                    display: "flex",
                    justifyContent: "center", // Zentrieren des Buttons
                    mt: 2, // Abstand nach oben
                }}
            >
                <Button
                    variant="contained"
                    color="primary"
                    onClick={handleAddExercise}
                >
                    Hinzufügen
                </Button>
            </Box>
        </Stack>
    );
}