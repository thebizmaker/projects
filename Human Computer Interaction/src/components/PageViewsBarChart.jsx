import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import Stack from '@mui/material/Stack';
import { useTheme } from '@mui/material/styles';
import Typography from '@mui/material/Typography';
import { BarChart } from '@mui/x-charts/BarChart';
import * as React from 'react';
// import { exercises } from '../exercises';
// import { useTrainingData } from '../TrainingDataContext';

// function searchExerciseGroup(exercise) {
//   for (const category in exercises) {
//     if (exercises[category].includes(exercise)) {
//       return category;
//     }
//   }
// }

function aggregateFrequencyByMuscle(trainingData) {
  const frequenciesByMuscle = {
    beine: 0,
    arme: 0,
    rücken: 0,
    brust: 0,
    bauch: 0,
    schultern: 0,
  };

  trainingData.forEach((entry) => {
    entry.exercises.forEach((exercise) => {
      const category = exercise.muscleGroup;     
      frequenciesByMuscle[category] += 1;
    });
    
  });

  return frequenciesByMuscle;
}

export default function PageViewsBarChart() {
  const theme = useTheme();
  const colorPalette = [
    (theme.vars || theme).palette.primary.dark,
    (theme.vars || theme).palette.primary.main,
    (theme.vars || theme).palette.primary.light,
  ];

  // const { trainingData } = useTrainingData();
  const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts")) || [];
  const frequenciesByMuscle = aggregateFrequencyByMuscle(completedWorkouts);
  

  return (
    <Card variant="outlined" sx={{ width: '100%' }}>
      <CardContent>
        <Typography component="h2" variant="subtitle2" gutterBottom>
          Muskel Trainingshäufigkeit
        </Typography>
        <Stack sx={{ justifyContent: 'space-between' }}>
          <Stack
            direction="row"
            sx={{
              alignContent: { xs: 'center', sm: 'flex-start' },
              alignItems: 'center',
              gap: 1,
            }}
          >
            {/*<Typography variant="h4" component="p">
            </Typography>
            <Chip size="small" color="error" label="-8%" />*/}
          </Stack>
          {/*<Typography variant="caption" sx={{ color: 'text.secondary' }}>
          </Typography>*/}
        </Stack>
        <BarChart
          borderRadius={8}
          colors={colorPalette}
          xAxis={[
            {
              scaleType: 'band',
              categoryGapRatio: 0.5,
              data: ['Bein', 'Arm', 'Rücken', 'Brust', 'Bauch', 'Schulter'],
            },
          ]}
          series={[
            {
              id: 'frequency',
              label: 'Häufigkeit',
              data: [
                frequenciesByMuscle.beine,
                frequenciesByMuscle.arme,
                frequenciesByMuscle.rücken,
                frequenciesByMuscle.brust,
                frequenciesByMuscle.bauch,
                frequenciesByMuscle.schultern,
              ],
              stack: 'A',
            },
          ]}
          height={250}
          margin={{ left: 50, right: 0, top: 20, bottom: 20 }}
          grid={{ horizontal: true }}
          slotProps={{
            legend: {
              hidden: true,
            },
          }}
        />
      </CardContent>
    </Card>
  );
}
