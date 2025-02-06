import React from 'react';
import { useParams } from 'react-router-dom';
import { Card, CardContent, Typography, List, ListItem, ListItemText, ListItemAvatar, Avatar, Divider, Accordion, AccordionSummary, AccordionDetails } from '@mui/material';
import ExpandMoreIcon from '@mui/icons-material/ExpandMore';

const WorkoutDetails = () => {
    const { date } = useParams();
    const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts") || []);
    const workout = completedWorkouts.find(workout => workout.date === date);

    if (!workout) {
        return <Typography variant="h6" color="error">No workout found for this date.</Typography>;
    }

    const groupedExercises = workout.exercises.reduce((groups, { muscleGroup, ...exercise }) => {
        groups[muscleGroup] = groups[muscleGroup] || [];
        groups[muscleGroup].push(exercise);
        return groups;
    }, {});

    return (
        <Card variant="outlined" sx={{ margin: 2, marginTop: '70px' }}>
            <CardContent>
                <Typography variant="h4" gutterBottom>
                    Training von dem {new Date(date).toLocaleDateString('de-DE')}
                </Typography>
                <Divider />
                {Object.entries(groupedExercises).map(([muscleGroup, exercises], index) => (
                    <div key={index}>
                        {exercises.map((exercise, idx) => (
                            <div key={idx}>
                                <ListItem>
                                    <ListItemAvatar>
                                        <Avatar>
                                            {exercise.rows.reduce((sum, row) => sum + parseInt(row.reps || 0, 10), 0)}
                                        </Avatar>
                                    </ListItemAvatar>
                                    <ListItemText
                                        primary={exercise.title}
                                        secondary={`Muscle Group: ${muscleGroup}`}
                                    />
                                </ListItem>
                                <Accordion>
                                    <AccordionSummary expandIcon={<ExpandMoreIcon />}>
                                        <Typography>Details</Typography>
                                    </AccordionSummary>
                                    <AccordionDetails>
                                        <List>
                                            {exercise.rows.map((row, rowIndex) => (
                                                <React.Fragment key={rowIndex}>
                                                    <ListItem>
                                                        <ListItemText primary={`Set ${rowIndex + 1}: Reps: ${row.reps}, Weight: ${row.weight}kg`} />
                                                    </ListItem>
                                                    {rowIndex < exercise.rows.length - 1 && <Divider />}
                                                </React.Fragment>
                                            ))}
                                        </List>
                                    </AccordionDetails>
                                </Accordion>
                                {idx < exercises.length - 1 && <Divider />}
                            </div>
                        ))}
                        {index < Object.keys(groupedExercises).length - 1 && <Divider />}
                    </div>
                ))}
            </CardContent>
        </Card>
    );
};

export default WorkoutDetails;