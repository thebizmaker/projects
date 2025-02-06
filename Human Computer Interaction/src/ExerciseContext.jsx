import React, { createContext, useContext, useState } from "react";

const ExerciseContext = createContext();

export const ExerciseProvider = ({ children }) => {
    const [exercises, setExercises] = useState([]);
    const [startTime, setStartTime] = useState(null); // Startzeit des Workouts
    const [endTime, setEndTime] = useState(null); // Endzeit des Workouts

    const addExercise = (exercise) => {
        setExercises((prevExercises) => [
            ...prevExercises,
            {
                title: exercise.title,
                muscleGroup: exercise.muscleGroup,
                rows: [],
            },
        ]);
    };

    // Funktion, um die Startzeit explizit zu setzen
    const startWorkoutTime = () => {
        const now = new Date();
        setStartTime(now); // speichere die aktuelle Zeit
        setEndTime(null);  // reset Endzeit für neuen Workout
    };

    // Funktion, um die Endzeit zu setzen und Dauer zu berechnen
    const endWorkoutTime = () => {
        const now = new Date();
        setEndTime(now); // speichere die aktuelle Zeit
        return calculateElapsedTime(startTime, now); // Berechne und liefere Dauer zurück
    };

    // Funktion zur Berechnung der verstrichenen Zeit
    const calculateElapsedTime = (start, end) => {
        if (!start || !end) return 0; // Kein Start oder Ende -> Rückgabe 0 Stunden

        const elapsedTimeInMilliseconds = end - start; // Differenz in Millisekunden
        const elapsedTimeInHours = elapsedTimeInMilliseconds / (1000 * 60 * 60); // Millisekunden in Stunden umrechnen

        return parseFloat(elapsedTimeInHours.toFixed(4)); // Auf 2 Nachkommastellen runden und als Float zurückgeben
    };

    const toggleRowCheck = (exerciseIndex, rowId) => {
        setExercises((prevExercises) =>
            prevExercises.map((exercise, index) =>
                index === exerciseIndex
                    ? {
                        ...exercise,
                        rows: exercise.rows.map((row) =>
                            row.id === rowId
                                ? { ...row, checked: !row.checked }
                                : row
                        ),
                    }
                    : exercise
            )
        );
    };

    return (
        <ExerciseContext.Provider
            value={{
                exercises,
                setExercises,
                addExercise,
                startTime,
                startWorkoutTime,
                endWorkoutTime,
                toggleRowCheck,
            }}
        >
            {children}
        </ExerciseContext.Provider>
    );
};

export const useExerciseContext = () => useContext(ExerciseContext);