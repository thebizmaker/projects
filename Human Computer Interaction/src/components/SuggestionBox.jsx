import React from 'react';
import { Box, Typography, Paper } from '@mui/material';
import { getRecommendedExcercises } from '../utils/decisionSupport.js';

export default function SuggestionBox() {
    const recommendations = getRecommendedExcercises();

    return (
        <Box
            sx={{
                display: 'flex',
                justifyContent: 'center',
                width: '100%',
                mt: 4,
            }}
        >
            <Box
                sx={{
                    display: 'flex',
                    flexDirection: 'column',
                    alignItems: 'center',
                    justifyContent: 'center',
                    borderRadius: 2,
                    p: 2,
                    boxShadow: 3,
                    width: '100%',
                    maxWidth: '500px', 
                    margin: 'auto',
                }}
            >
                <Typography 
                    variant="h6" 
                    component="div" 
                    gutterBottom
                    sx={{ color: '#90ee90' }} 
                >
                    Empfehlungen
                </Typography>
                {recommendations.map((recommendation, index) => (
                    <Paper
                        key={index}
                        sx={{
                            p: 2,
                            mb: 2,
                            width: '100%',
                            borderRadius: 2,
                        }}
                    >
                        <Typography variant="body1">
                            <strong>Übung:</strong> {recommendation.exercise}
                        </Typography>
                        <Typography variant="body1">
                            <strong>Muskelgruppe:</strong> {recommendation.muscleGroup}
                        </Typography>
                    </Paper>
                ))}
            </Box>
        </Box>
    );
}


