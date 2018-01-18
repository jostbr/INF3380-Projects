# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# define DATA_POINTS 25
# define CHAR_LENGTH 6

int main(){
    // Checking if data file can be opened.
    FILE *rf;
    rf = fopen("temperature_data.txt", "r");

    if (rf == NULL){
        printf("Can't open data file!\n");
        exit(1);
    }

    // Define variables for getting data.
    char time_of_day[CHAR_LENGTH];
    char TOD_storage[DATA_POINTS][CHAR_LENGTH];
    float temp;
    float temp_storage[DATA_POINTS];
    int i = 0;

    // Getting data from file.
    while (fscanf(rf, "%s %f", time_of_day, &temp) != EOF){
        //printf("%s %d\n", time_of_day, temp);
        strcpy(TOD_storage[i], time_of_day);
        temp_storage[i] = temp;
        i = i + 1;
    }

    fclose(rf);

    float temp_average = 0.0;
    float temp_min = temp_storage[0] + 1; // Safety.
    float temp_max = temp_storage[0] - 1; // Safety.
    int index_min;
    char index_max;

    // Finding min/max and and average temperatures.
    for (i = 0; i < DATA_POINTS; i++){
        temp_average += temp_storage[i];
        if (temp_storage[i] < temp_min){
            temp_min = temp_storage[i];
            index_min = i;
        }

        if (temp_storage[i] > temp_max){
            temp_max = temp_storage[i];
            index_max = i;
        }
    }

    temp_average = temp_average / DATA_POINTS;

    // Computing standard deviation from variance.
    float temp_variance = 0;
    float standard_deviation;

    for (i = 0; i < DATA_POINTS; i++){
        temp_variance += pow((temp_storage[i] - temp_average), 2);
    }

    temp_variance = temp_variance / DATA_POINTS;
    standard_deviation = sqrt(temp_variance);

    // Printing results about temperature.
    printf("The average temperature was %.2f\n", temp_average);
    printf("The standard deviation was %.2f\n", standard_deviation);
    printf("Min temp of %.1f occurred at %s\n",
           temp_min, TOD_storage[index_min]);
    printf("Max temp of %.1f occurred at %s\n",
           temp_max, TOD_storage[index_max]);

    return 0;
}



