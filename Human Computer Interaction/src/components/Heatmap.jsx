import React from "react";
import HeatMap from "react-heatmap-grid";
import { Card, CardContent, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";

const getLast5Weeks = () => {
    const weeks = [];
    const currentDate = new Date();
    for (let i = -7; i < 28; i++) {
        const date = new Date(currentDate);
        date.setDate(currentDate.getDate() - i);
        weeks.push({
            weekday: date.toLocaleDateString("de-DE", { weekday: "short" }),
            date: date.toISOString().split("T")[0],
            muscleGroup: getMuscleGroupForDate(date),
            intensity: getIntensityForDate(date)
        });
    }
    return weeks.reverse();
};

const getMuscleGroupForDate = (date) => {
    const dateString = date.toISOString().split("T")[0];
    const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts") || []);
    const muscleGroupCount = completedWorkouts.reduce((count, workout) => {
        if (workout.date === dateString) {
            workout.exercises.forEach(exercise => {
                count[exercise.muscleGroup] = (count[exercise.muscleGroup] || 0) + 1;
            });
        }
        return count;
    }, {});
    return Object.keys(muscleGroupCount).reduce((a, b) => muscleGroupCount[a] > muscleGroupCount[b] ? a : b, "unknown");
};

const getIntensityForDate = (date) => {
    const dateString = date.toISOString().split("T")[0];
    const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts") || []);
    return completedWorkouts.reduce((total, workout) => {
        if (workout.date === dateString) {
            const totalWeight = workout.exercises.reduce((exerciseTotal, exercise) => {
                return exerciseTotal + exercise.rows.reduce((rowTotal, row) => rowTotal + row.reps * row.weight, 0);
            }, 0);
            const duration = workout.duration || 1.5; // Assume 1.5 hours if duration is missing
            total += totalWeight / duration;
        }
        return total;
    }, 0);
};

const getColorForIntensity = (intensity) => {
    const alpha = intensity > 0 ? Math.min(1, intensity / 5000) : 0;
    return `rgba(0, 0, 255, ${alpha})`;
};

export default function CustomHeatmap() {
    const xLabels = new Array(7).fill("");
    const yLabels = new Array(5).fill("");
    const navigate = useNavigate();
    const days = getLast5Weeks();
    const year = new Date(days[0].date).getFullYear();
    const data = yLabels.map((_, weekIndex) => xLabels.map((_, dayIndex) => days[weekIndex * 7 + dayIndex]));

    const handleCellClick = (x, y) => {
        const { date, muscleGroup } = data[y][x];
        if (muscleGroup !== "unknown") {
            navigate(`/workout/${date}`);
        }
    };

    return (
        <Card variant="outlined" sx={{ width: "100%" }}>
            <CardContent>
                <Typography variant="h6" gutterBottom>{year}</Typography>
                <HeatMap
                    xLabels={xLabels}
                    yLabels={yLabels}
                    xLabelsLocation="top"
                    data={data}
                    height={45}
                    onClick={handleCellClick}
                    cellStyle={(background, value) => ({
                        background: getColorForIntensity(value.intensity),
                        fontSize: "11.5px",
                        color: "#FFF",
                        cursor: value.muscleGroup !== "unknown" ? "pointer" : "default",
                    })}
                    cellRender={(value) => value && (
                        <div title={value.muscleGroup}>
                            {`${value.weekday}, ${new Date(value.date).toLocaleDateString("de-DE", { day: "2-digit", month: "2-digit" })}`}
                        </div>
                    )}
                />
            </CardContent>
        </Card>
    );
}