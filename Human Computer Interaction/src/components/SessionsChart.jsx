import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import Stack from '@mui/material/Stack';
import { useTheme } from '@mui/material/styles';
import Typography from '@mui/material/Typography';
import { LineChart } from '@mui/x-charts/LineChart';
import PropTypes from 'prop-types';

function AreaGradient({ color, id }) {
  return (
    <defs>
      <linearGradient id={id} x1="50%" y1="0%" x2="50%" y2="100%">
        <stop offset="0%" stopColor={color} stopOpacity={0.5} />
        <stop offset="100%" stopColor={color} stopOpacity={0} />
      </linearGradient>
    </defs>
  );
}

AreaGradient.propTypes = {
  color: PropTypes.string.isRequired,
  id: PropTypes.string.isRequired,
};

// function getDaysInMonth(month, year) {
//   const date = new Date(year, month, 0);
//   const monthName = date.toLocaleDateString('en-US', {
//     month: 'short',
//   });
//   const daysInMonth = date.getDate();
//   const days = [];
//   let i = 1;
//   while (days.length < daysInMonth) {
//     days.push(`${monthName} ${i}`);
//     i += 1;
//   }
//   return days;
// }

const calculatePower = (weight, reps, duration) => {
  const work = weight * reps; // approx. work
  //const timeInSeconds = parseInt(duration.slice(0, -1)); // Convert duration to seconds
  return work / (duration * 60); // Power
};

const aggregatePowerByDate = (trainingData) => {
  const powerByDate = {};

  // Aktuelles Datum und Datum von vor 30 Tagen berechnen
  const today = new Date();
  const startDate = new Date();
  startDate.setDate(today.getDate() - 60); // 30 Tage zurück

  trainingData.forEach((entry) => {
    const entryDate = new Date(entry.date); // Datum des Trainings
    if (entryDate >= startDate && entryDate <= today) {
      const date = entry.date.slice(5, 10); // MM-DD extrahieren

      entry.exercises.forEach((exercise) => {
        exercise.rows.forEach((row) => {
          const power = calculatePower(row.weight, row.reps, 1, entry.duration);
          if (!powerByDate[date]) {
            powerByDate[date] = { totalPower: 0, count: 0 };
          }
          powerByDate[date].totalPower += power;
          powerByDate[date].count += 1;
        });
      });
    }
  });

  // Daten aggregieren
  const aggregatedData = Object.keys(powerByDate).map((date) => ({
    datum: date,
    averagePower: powerByDate[date].totalPower / powerByDate[date].count,
  }));

  return aggregatedData;
};


export default function SessionsChart() {
  const theme = useTheme();
  // const { trainingData } = useTrainingData();
  // const aggregatedData = aggregatePowerByDate(trainingData);
  const completedWorkouts = JSON.parse(localStorage.getItem("completedWorkouts")) || [];
  const aggregatedData = aggregatePowerByDate(completedWorkouts);

  const data = [];
  const y_data = [];
  {aggregatedData.map((entry) => (
    data.push(entry.datum),
    y_data.push(entry.averagePower)
  ))}
  //const data = getDaysInMonth(4, 2024);

  const colorPalette = [
    theme.palette.primary.main
  ];

  return (
    <Card variant="outlined" sx={{ width: '100%' }}>
      <CardContent>
        <Typography component="h2" variant="subtitle2" gutterBottom>
          Leistung
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
            </Typography>*/}
            {/*<Chip size="small" color="success" label="+35%" />*/}
          </Stack>
          {/*<Typography variant="caption" sx={{ color: 'text.secondary' }}>
          </Typography>*/}
        </Stack>
        <LineChart
          colors={colorPalette}
          xAxis={[
            {
              scaleType: 'point',
              data,
              tickSize: 0,
              //tickInterval: (index, i) => (i + 1) % 1 === 0,
              //valueFormatter: () => '',
            },
          ]}
          series={[
            {
              id: 'direct',
              label: 'Leistung',
              showMark: false,
              curve: 'linear',
              stack: 'total',
              area: true,
              stackOrder: 'ascending',
              data: y_data //[
              //   300, 900, 600, 1200, 1500, 1800, 2400, 2100, 2700, 3000, 1800, 3300,
              //   3600, 3900, 4200, 4500, 3900, 4800, 5100, 5400, 4800, 5700, 6000,
              //   6300, 6600, 6900, 7200, 7500, 7800, 8100,
              // ],
            },
          ]}
          height={250}
          margin={{ left: 50, right: 20, top: 20, bottom: 20 }}
          grid={{ horizontal: true }}
          sx={{
            '& .MuiAreaElement-series-direct': {
              fill: "url('#direct')",
            },
          }}
          slotProps={{
            legend: {
              hidden: true,
            },
          }}
        >
          <AreaGradient color={theme.palette.primary.dark} id="organic" />
          <AreaGradient color={theme.palette.primary.main} id="referral" />
          <AreaGradient color={theme.palette.primary.light} id="direct" />
        </LineChart>
      </CardContent>
    </Card>
  );
}
