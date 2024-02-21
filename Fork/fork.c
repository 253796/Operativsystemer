#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Funksjon for å beregne summen av de første 10 naturlige tallene
int beregnSum() {
  int top = 10;
  int sum = 0;
  for (int i = 0; i < top; i++) {
    sum += i;
  }
  return sum;
}

int isPrime(int num) {
  if (num <= 1)
    return 0;
  if (num % 2 == 0 && num > 2)
    return 0;
  for (int i = 3; i <= sqrt(num); i += 2) {
    if (num % i == 0)
      return 0;
  }
  return 1;
}

// Funksjon for å beregne produktet av de første 5 primtallene
int beregnProdukt() {
  int foundPrimes = 0;
  int sum = 1;
  int num = 0;
  while (foundPrimes < 5) {
    if (isPrime(num)) {
      sum *= num;
      foundPrimes++;
    }
    num++;
  }
  return sum;
}

int main() {

  pid_t pid = fork();

  if (pid < 0) {
    // Feil ved forking
    fprintf(stderr, "Fork feilet\n");
    return 1;
  } else if (pid == 0) {
    // Dette er barneprosessen
    printf("Barneprosess: Summen av de første 10 naturlige tallene er %d\n",
           beregnSum());
  } else {
    // Dette er foreldreprosessen
    printf("Foreldreprosess: Produktet av de første 5 primtallene er %d\n",
           beregnProdukt());
  }

  return 0;
}