import { exercises as excersiseMuscleGroups } from '../exercises';

// Function that filters storedData by a span of dates
function filterByDate(startDate, endDate, storedData) {
    // Switch startDate and endDate if startDate is after endDate
    if (startDate > endDate) {
        const temp = startDate;
        startDate = endDate;
        endDate = temp;
    }

    startDate = startDate.toISOString();
    endDate = endDate.toISOString();

    return storedData.filter((exerciseElement) => {
        return exerciseElement.date >= startDate && exerciseElement.date <= endDate;
    });
}

export function getRecommendedExcercises() {
    const storedData = JSON.parse(localStorage.getItem('completedWorkouts')); // Trainingsdaten von key: "completedWorkouts" einlesen
    console.log('Stored Data:', storedData);
    
    const twoWeeksDiff = 14;
    
    // Sort storedData by date and then store the most current date in latestTrainingDate
    storedData.sort((a, b) => new Date(a.date) - new Date(b.date));
    const latestTrainingDate = new Date(storedData[storedData.length - 1].date);
    console.log('Latest Training Date:', latestTrainingDate);
    
    // Make endDate x days prior to today
    const priorDate = new Date(latestTrainingDate);
    priorDate.setDate(priorDate.getDate() - twoWeeksDiff);
    console.log('Prior Date:', priorDate);
    
    const latestData = filterByDate(priorDate, latestTrainingDate, storedData);
    console.log('Latest Data:', latestData);

    // Hashmap for counting exercises with exercise as key and count as value with latestData
    let recommendedMuscleGroup;    
    let muscleGroupCount = {};
    latestData.forEach((workout) => {
        workout.exercises.forEach((exercise) => {
            if (muscleGroupCount[exercise.muscleGroup]) {
                muscleGroupCount[exercise.muscleGroup]++;
            } else {
                muscleGroupCount[exercise.muscleGroup] = 1;
            }
        });
    });
    console.log('Muscle Group Count:', muscleGroupCount);

    // Get least used muscle group from hashmap and save in recommendedMuscleGroup
    const sortedMuscleGroups = Object.entries(muscleGroupCount).sort((a, b) => a[1] - b[1]); // Ascending order
    recommendedMuscleGroup = sortedMuscleGroups[0][0];
    console.log('Recommended Muscle Groups:', recommendedMuscleGroup);
      
    // Hashmap for counting exercises with exercise as key and count as value with latestData
    let recommendedExcercises = [];
    let exerciseCount = {};
    latestData.forEach((workout) => {
        workout.exercises.forEach((exercise) => {
            if (exerciseCount[exercise.title]) {
                exerciseCount[exercise.title]++;
            } else {
                exerciseCount[exercise.title] = 1;
            }
        });
    });
    console.log('Exercise Count:', exerciseCount);
    
    // Sort the exercises by count and store in recommendedExcercises for the recommendedMuscleGroup
    const sortedExercises = Object.entries(exerciseCount).sort((a, b) => a[1] - b[1]); // Ascending order

    sortedExercises.forEach((exerciseElement) => {
        for (let muscleGroup in excersiseMuscleGroups) {
            if (excersiseMuscleGroups[muscleGroup].includes(exerciseElement[0]) && muscleGroup === recommendedMuscleGroup) {
                recommendedExcercises.push(exerciseElement[0]);
            }
        }
    });


    /*for (let i = 0; i < 3 && i < sortedExercises.length; i++) {
        recommendedExcercises.push(sortedExercises[i][0]);
    }
    console.log('Recommended Exercises:', recommendedExcercises);*/
    
    // Map the recommended exercises to their respective muscle groups and store in muscleGroups array
    let recommendations = [];
    recommendedExcercises.forEach((exerciseElement) => {
        for (let muscleGroup in excersiseMuscleGroups) {
            if (excersiseMuscleGroups[muscleGroup].includes(exerciseElement)) {
                recommendations.push({ exercise: exerciseElement, muscleGroup: muscleGroup });
            }
        }
    });
    console.log('Recommendations:', recommendations);
    
    return recommendations;
}

