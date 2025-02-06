import { exercises } from "./exercises";
import dayjs from "dayjs";

const getRandomInRange = (min, max) => Math.random() * (max - min) + min;

const getMuscleGroup = (exerciseName) => {
    for (const [muscleGroup, groupExercises] of Object.entries(exercises)) {
        if (groupExercises.includes(exerciseName)) {
            return muscleGroup;
        }
    }
    return "unknown";
};

const generateExerciseDataProgressive = (exerciseName, dayIndex) => {
    const workouts = [];
    const sets = Math.floor(getRandomInRange(3, 5)); // 3 bis 5 Sätze
    let baseWeight = 20 + dayIndex * 0.3; // Gewicht steigt progressiv (0.3 pro Tag)
    for (let i = 0; i < sets; i++) {
        const reps = Math.floor(getRandomInRange(6, 12) + dayIndex * 0.05); // Wiederholungen steigen leicht (max +3 Reps über 2 Monate)
        const weight = Math.floor(baseWeight + i * 2); // Gewicht pro Set etwas höher
        workouts.push({ reps, weight });
    }
    return {
        title: exerciseName,
        muscleGroup: getMuscleGroup(exerciseName),
        rows: workouts,
    };
};

const generateCompletedWorkoutsProgressive = () => {
    const completedWorkouts = [];
    const today = dayjs();
    let startDate = today.subtract(2, "month");
    let workoutId = 1; // Initialize workout ID counter

    for (let i = 0; i < 60; i++) { // Nur 60 Tage generieren
        const workoutDate = startDate.format("YYYY-MM-DD");

        // 3-4 zufällige Übungen pro Tag wählen
        const availableExercises = Object.values(exercises).flat();
        const numExercises = Math.floor(getRandomInRange(3, 4));
        const dailyExercises = [];
        for (let j = 0; j < numExercises; j++) {
            const randomExerciseIndex = Math.floor(getRandomInRange(0, availableExercises.length - 1));
            const exercise = availableExercises.splice(randomExerciseIndex, 1)[0];
            dailyExercises.push(generateExerciseDataProgressive(exercise, i)); // Progressiver Tag i
        }

        // Trainingsdauer zwischen 1-2 Stunden (leichter Fokus auf Effizienz)
        const duration = parseFloat(getRandomInRange(1, 2).toFixed(2));

        // Add workout object with unique ID
        completedWorkouts.push({
            id: workoutId++, // Assign unique ID and increment counter
            date: workoutDate,
            duration,
            exercises: dailyExercises,
        });

        startDate = startDate.add(1, "day");
    }

    return completedWorkouts;
};

// Generiere realistische Daten
const completedWorkouts = generateCompletedWorkoutsProgressive();
localStorage.setItem("completedWorkouts", JSON.stringify(completedWorkouts));

export default completedWorkouts;