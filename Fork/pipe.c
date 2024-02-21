#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#define NUM_VALUES 10

// Funksjon for å generere tilfeldige tall
void genererTilfeldigeTall(int *tall) {
	srand(time(0)); // Seed for tilfeldige tall
	for (int i = 0; i < NUM_VALUES; i++) {
		tall[i] = rand() % 100;
	}
}

// Funksjon for å beregne gjennomsnittet av tallene
float beregnGjennomsnitt(int *tall) {
	int sum = 0;
	for (int i = 0; i < NUM_VALUES; i++) {
        sum += tall[i];
	}
    return sum;
}

int main() {
	int tall[NUM_VALUES];
	int ror[2]; // Rør for kommunikasjon mellom prosessene

	if (pipe(ror) == -1) {
		perror("Feil ved oppretting av rør");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();

	if (pid < 0) {
		// Feil ved forking
		perror("Fork feilet");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		// Dette er barneprosessen
		close(ror[1]); // Lukk skriveenden av røret i barneprosessen
		// Implementer koden for å motta tallene og beregne gjennomsnittet
        int received[NUM_VALUES];
        // Read the array from the pipe
        read(ror[0], received, sizeof(int) * NUM_VALUES);
        float average = beregnGjennomsnitt(received);
        printf("Average: %f\n", average);
        close(ror[0]); // Close the read end of the pipe when done
	} else {
		// Dette er foreldreprosessen
		close(ror[0]); // Lukk leseenden av røret i foreldreprosessen
		genererTilfeldigeTall(tall);
		// Implementer koden for å sende tallene til barneprosessen
        write(ror[1], tall, sizeof(int) * NUM_VALUES);
        close(ror[1]);
	}

	return 0;
}
