#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int prime( long int n){ //Συνάρτηση που ελέγχει αν ένας αριθμός ειναι πρώτος
   if(!(n % 2)){
    return 0;
   }
   for(int i = 3 ; i <= sqrt(n); i+=2){
    if(!(n % i)){
        return 0;
    }
   }
   return 1;
}


void coop() {
    char move;

    putchar('C'); //Το πρόγραμμα ξεκινάει με συνεργασία 
    putchar('\n');
    fflush(stdout);

    long int roundCounter = 0; //Μετρητης των γύρων.
    int opponentCooperationCount = 0; //Μετρητής των φορών που ο αντίπαλος συνεργάστηκε 
    int opponentDefectionCount = 0; //Μετρητής των φορών που ο αντίπαλος δεν συνεργάστηκε

    while ((move = getchar()) != EOF && roundCounter < 1e6) { //Όσο η είσοδος δεν είναι EOF και δεν έχουν ξεπεραστεί 1000 επαναλήψεις
        if (move == 'C' || move == 'D') {
            roundCounter++; //Αύξηση μετρητή των γύρων

             if (move == 'C') {
                opponentCooperationCount++; //Αυξηση μετρητή των συνεργασιων (C)
            } else { 
                opponentDefectionCount++; //Αυξηση μετρητή των εκδικήσεων (D)
            }

            if (roundCounter % 10 == 0) {  //Κάθε 10 γύρους
                
                if (opponentCooperationCount > opponentDefectionCount) { //Αν έχει συνεργαστεί περισσότερες απο ότι δεν συνεργάστηκε
                    
                    putchar('D'); //το πτόγραμμα δεν συνεργάζεται
                } else {
                    
                    putchar('C'); //αλλιώς συνεργάζεται
                }

                
                opponentCooperationCount = 0; //επαναφορά των μετρητών στο 0 για του επόμενους 10 γύρους
                opponentDefectionCount = 0;
            }else { //Αν οι γύροι δεν ειναι πολλαπλασιο του 10
                if(prime(roundCounter)){ //Αν ο αριθμός των γύρων είναι πρώτος
                    putchar('D'); //Δεν συνεργάζεται
                }else {
                    putchar('C'); //αλλιως συνεργάζεται
                }
            }
            putchar('\n');
            fflush(stdout);

        }
    }
}

int main() {
    coop(); //κλήση της coop
    return 0;
}