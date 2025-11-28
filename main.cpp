#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Highscore {
    char name[50];
    int attempts;
    double time_taken;
};

void loadHighscore(struct Highscore *hs) {
    FILE *file = fopen("highscores.txt", "r");
    if (file == NULL) {
        strcpy(hs->name, "None");
        hs->attempts = 9999;
        hs->time_taken = 9999;
        return;
    }
    fscanf(file, "%s %d %lf", hs->name, &hs->attempts, &hs->time_taken);
    fclose(file);
}

void saveHighscore(struct Highscore hs) {
    FILE *file = fopen("highscores.txt", "w");
    fprintf(file, "%s %d %.2f", hs.name, hs.attempts, hs.time_taken);
    fclose(file);
}

int main() {
    int secret_number, guess, attempts = 0, max_attempts, range;
    int mode, choice;
    char player_name[50];

    struct Highscore best;
    loadHighscore(&best);

    printf("Enter your player name: ");
    scanf("%s", player_name);

    printf("\n===== CURRENT HIGHSCORE =====\n");
    printf("Player : %s\nAttempts : %d\nTime : %.2f seconds\n\n",
           best.name, best.attempts, best.time_taken);

    printf("Highest score player (runtime): %s\n\n", best.name);

    // Difficulty selection
    printf("Choose Difficulty Level:\n");
    printf("1. Easy (1 - 10)\n");
    printf("2. Medium (1 - 50)\n");
    printf("3. Hard (1 - 100)\n");
    printf("4. Impossible (1 - 500)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    int time_limit = 0;

    if (choice == 1) { range = 10; max_attempts = 5; time_limit = 10; }
    else if (choice == 2) { range = 50; max_attempts = 7; time_limit = 20; }
    else if (choice == 3) { range = 100; max_attempts = 8; time_limit = 25; }
    else if (choice == 4) { range = 500; max_attempts = 5; time_limit = 30; }
    else { printf("Invalid choice! Exiting.\n"); return 0; }

    printf("\nChoose Game Mode:\n");
    printf("1. Normal\n");
    printf("2. Time Attack\n");
    printf("3. Blindfold\n");
    printf("Enter your choice: ");
    scanf("%d", &mode);

    srand(time(NULL));
    secret_number = (rand() % range) + 1;

    time_t start_time = time(NULL);
    time_t current_time;

    if (mode == 2)
        printf("\nTime Attack! You must finish in %d seconds!\n", time_limit);
    if (mode == 3)
        printf("\nBlindfold Mode! No hints!\n");

    printf("I have chosen a number between 1 and %d.\n", range);

    while (attempts < max_attempts) {

        if (mode == 2) {
            current_time = time(NULL);
            if (current_time - start_time > time_limit) {
                printf("\n⏱ Time's up! Number was %d.\n", secret_number);
                return 0;
            }
        }

        printf("Enter guess: ");
        scanf("%d", &guess);
        attempts++;

        if (guess == secret_number) {
            double time_taken = difftime(time(NULL), start_time);

            printf("\n🎉 Correct! Number: %d\n", secret_number);
            printf("Attempts: %d\n", attempts);
            printf("Time Taken: %.2f seconds\n", time_taken);

            if (attempts < best.attempts ||
                (attempts == best.attempts && time_taken < best.time_taken)) {

                struct Highscore newScore;
                strcpy(newScore.name, player_name);
                newScore.attempts = attempts;
                newScore.time_taken = time_taken;

                saveHighscore(newScore);

                printf("\n🏆 NEW HIGHSCORE ACHIEVED!\n");
            }

            return 0;
        }

        if (mode == 3)
            printf("Wrong! Try again.\n");
        else if (guess < secret_number)
            printf("Too low!\n");
        else
            printf("Too high!\n");

        if (attempts < max_attempts)
            printf("Attempts left: %d\n\n", max_attempts - attempts);
    }

    printf("\n❌ Out of attempts! The number was %d.\n", secret_number);

    return 0;
}
