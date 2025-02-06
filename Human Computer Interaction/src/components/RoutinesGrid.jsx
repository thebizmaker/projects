import Box from '@mui/material/Box';
import Button from '@mui/material/Button';
import Stack from '@mui/material/Stack';
import * as React from 'react';
import { Link } from "react-router-dom";

export default function RoutinesGrid() {

    return (
        <>
            <Box display="flex"
                justifyContent="center"
                alignItems="center">
                <Stack spacing={2} direction="row">
                    <Button variant="contained" component={Link} to="/workout">
                        Leeres Workout
                    </Button>
                </Stack>
            </Box>
        </>



    );
}
