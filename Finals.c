/** Doxygen-Info
 * @file Finals.c
 * @author Oshini Jayaweera
 * @Matriculation No: 2757043
 * @author Dr.-Ing. Kolja Pikora
 * @date SS20240625
 * @version 20240401
 * @brief HAW SS2024 SOL1 Lab5
 * Detailed description:
 *   Practise the following skills:
 *   - from mathematical formulas to algorithms
 *   - Using structures and command line input
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // Defining structures
typedef struct {
    int goals_winner;
    int goals_runnerup;
    char note[20];
} result_t;

typedef struct {
    int day;
    int month;
    int year;
} date_t;

typedef struct {
    char location[50];
    date_t date;
    int visitors;
    char winner[50];
    char runnerup[50];
    char thirdplace[50];
    result_t result;
} final_t;

//Function to store and print final data (used pointers as arguments)
void print_finals(final_t* em_finals, int count) {
    printf("%-10s %-10s %-10s %-40s\n", "Date", "Location", "Visitors", "Final");
    for (int i = 0; i < count; i++) {
        printf("%02d.%02d.%04d %-10s %-10d %-15s - %-15s %d:%d %s\n",
            em_finals[i].date.day,
            em_finals[i].date.month,
            em_finals[i].date.year,
            em_finals[i].location,
            em_finals[i].visitors,
            em_finals[i].winner,
            em_finals[i].runnerup,
            em_finals[i].result.goals_winner,
            em_finals[i].result.goals_runnerup,
            em_finals[i].result.note);
    }
}
// Function to read data from file
void read_finals(const char* filename, int* n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d\n", n) != 1) {
        fprintf(stderr, "Error: Could not read the number of finals\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    final_t* em_finals = (final_t*)malloc(*n * sizeof(final_t));
    if (!em_finals) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *n; ++i) {
        int day, month, year;
        int goals_winner, goals_runnerup;
        char location[50], winner[50], runnerup[50], thirdplace[50], note[20];
        int visitors;

        int result = fscanf(file, "%49s %d.%d.%d %49s %49s %49s %d:%d %19s %d\n",
            location, &day, &month, &year,
            winner, runnerup, thirdplace,
            &goals_winner, &goals_runnerup, note, &visitors);

        if (result != 11) {
            fprintf(stderr, "Error: Could not read data for final %d, result: %d\n", i + 1, result);
            free(em_finals);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        em_finals[i].date.day = day;
        em_finals[i].date.month = month;
        em_finals[i].date.year = year;
        strcpy(em_finals[i].location, location);
        em_finals[i].visitors = visitors;
        strcpy(em_finals[i].winner, winner);
        strcpy(em_finals[i].runnerup, runnerup);
        strcpy(em_finals[i].thirdplace, thirdplace);
        em_finals[i].result.goals_winner = goals_winner;
        em_finals[i].result.goals_runnerup = goals_runnerup;
        strcpy(em_finals[i].result.note, note);

       
    }

    fclose(file);
    printf("Reading data from file %s\n", filename);
    printf("Data successfully read\n");
}



// Main function
int main() {
    FILE* file = fopen("previous_em_finals.txt", "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    final_t* em_finals = (final_t*)malloc(n * sizeof(final_t));
    if (!em_finals) {
        printf("Memory allocation error\n");
        fclose(file);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d.%d.%d %s %s %s %d:%d %s %d",
            em_finals[i].location,
            &em_finals[i].date.day,
            &em_finals[i].date.month,
            &em_finals[i].date.year,

            em_finals[i].winner,
            em_finals[i].runnerup,
            em_finals[i].thirdplace,
            &em_finals[i].result.goals_winner,
            &em_finals[i].result.goals_runnerup,
            em_finals[i].result.note,
            &em_finals[i].visitors);
    }

    printf("Reading data from file previous_em_finals.txt\nData successfully read\n");
    print_finals(em_finals, n);//calling function to print finals data table
    
    double avg_goals, avg_visitors;
    fscanf(file, "%lf %lf", &avg_goals, &avg_visitors);//Getting average og goals & visitors from the text file data


    //Calculating average goals and visitors
    double calculated_avg_goals = 0.0;
    double calculated_avg_visitors = 0.0;

    for (int i = 0; i < n; i++) {
        calculated_avg_goals += (em_finals[i].result.goals_winner + em_finals[i].result.goals_runnerup);
        calculated_avg_visitors += em_finals[i].visitors;
    }
    calculated_avg_goals /= n;
    calculated_avg_visitors /= n;

    printf("Average Goals: %lf (file) vs %lf (calculated)\n", avg_goals, calculated_avg_goals);
    printf("Average Visitors: %lf (file) vs %lf (calculated)\n", avg_visitors, calculated_avg_visitors);

    if (fabs(avg_goals - calculated_avg_goals) < 0.01 && abs(avg_visitors - calculated_avg_visitors) < 1.0) {
        printf("Averages are correct.\n");
    }
    else {
        printf("Averages are incorrect.\n");
    }

    // Adding the current championship data
    n++;
    em_finals = (final_t*)realloc(em_finals, n * sizeof(final_t));
    if (!em_finals) {
        printf("Memory allocation error\n");
        fclose(file);
        return 1;
    }

    // Prompting the user for current championship data
    printf("Enter data for the current championship final:\n");
    strcpy(em_finals[n - 1].location, "Berlin");
    em_finals[n - 1].date.day = 14;
    em_finals[n - 1].date.month = 7;
    em_finals[n - 1].date.year = 2024;
    printf("Visitors: ");
    scanf("%d", &em_finals[n - 1].visitors);
    printf("Winner: ");
    scanf("%s", em_finals[n - 1].winner);
    printf("Runner-up: ");
    scanf("%s", em_finals[n - 1].runnerup);
    strcpy(em_finals[n - 1].thirdplace, "-");
    printf("Goals of Winner: ");
    scanf("%d", &em_finals[n - 1].result.goals_winner);
    printf("Goals of Runner-up: ");
    scanf("%d", &em_finals[n - 1].result.goals_runnerup);
    printf("Note: ");
    scanf("%s", em_finals[n - 1].result.note);

    // Print the updated data
    print_finals(em_finals, n);

    // Recalculate the averages
    calculated_avg_goals = 0.0;
    calculated_avg_visitors = 0.0;

    for (int i = 0; i < n; i++) {
        calculated_avg_goals += (em_finals[i].result.goals_winner + em_finals[i].result.goals_runnerup);
        calculated_avg_visitors += em_finals[i].visitors;
    }
    calculated_avg_goals /= n;
    calculated_avg_visitors /= n;

    FILE* output_file = fopen("em_finals_2024.txt", "w");
    if (!output_file) {
        printf("Error opening output file\n");
        free(em_finals);
        fclose(file);
        return 1;
    }

    fprintf(output_file, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(output_file, "%s %02d.%02d.%d %d %s %s %s %d:%d %s\n",
            em_finals[i].location,
            em_finals[i].date.day,
            em_finals[i].date.month,
            em_finals[i].date.year,
            em_finals[i].visitors,
            em_finals[i].winner,
            em_finals[i].runnerup,
            em_finals[i].thirdplace,
            em_finals[i].result.goals_winner,
            em_finals[i].result.goals_runnerup,
            em_finals[i].result.note);
    }
    fprintf(output_file, "%.2lf\n%.2lf\n", calculated_avg_goals, calculated_avg_visitors);

    fclose(output_file);

    // Freeing allocated memory
    free(em_finals);
    fclose(file);
    return 0;
}







