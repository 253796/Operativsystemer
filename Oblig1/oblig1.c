#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_VALUE 1000

int pickRandomValue() {
    // pick a random value between 0 and MAX_VALUE
    printf("Picking random value\n");
    return rand() % MAX_VALUE;
}

int isHigherLower(int value, int guess) {
    printf("Value: %d, Guess: %d\n", value, guess);
    if (guess == value) {
        // guess is correct
        return 0;
    } else if (guess > value) {
        // guess is higher than the value
        return -1;
    } else {
        // guess is lower than the value
        return 1;
    }
}

int main() {
    srand(time(0));
    int randomValue = pickRandomValue();
    int pipeParentToChild[2], pipeChildToParent[2];

    if (pipe(pipeParentToChild) == -1 || pipe(pipeChildToParent) == -1) {
        perror("Feil ved oppretting av rør");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // child process
        close(pipeParentToChild[1]); // close write end of parent to child pipe
        close(pipeChildToParent[0]); // close read end of child to parent pipe

        int guess;
        while (1) {
            read(pipeParentToChild[0], &guess, sizeof(guess)); // read the guess from the parent process

            int response = isHigherLower(randomValue, guess);
            write(pipeChildToParent[1], &response, sizeof(response)); // send the response to the parent process

            if (response == 0) {
                break;
            }
        }
        close(pipeParentToChild[0]);
        close(pipeChildToParent[1]);
        exit(0);
    } else {
        // parent process
        int low = 0, high = MAX_VALUE, guess, response, attempts = 0;
        close(pipeParentToChild[0]); // close read end of parent to child pipe
        close(pipeChildToParent[1]); // close write end of child to parent pipe

        while (low <= high) {
            guess = low + (high - low) / 2;
            write(pipeParentToChild[1], &guess, sizeof(guess));
            read(pipeChildToParent[0], &response, sizeof(response));

            attempts++;

            if (response == 0) {
                printf("The right value is %d, and it took %d attempts to guess it\n", guess, attempts);
                break;
            } else if (response == -1) {
                high = guess - 1;
            } else {
                low = guess + 1;
            }
        }
        close(pipeParentToChild[1]);
        close(pipeChildToParent[0]);
        wait(NULL);
    }
    return 0;
}