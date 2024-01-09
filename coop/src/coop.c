#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void coop() {
    char move;

    putchar('C'); //Το πρόγραμμα ξεκινάει με συνεργασία 
    putchar('\n');
    fflush(stdout);

    long int roundCounter = 0; //Μετρητης των γύρων.
    int opponentCooperationCount = 0; //Μετρητής των φορών που ο αντίπαλος συνεργάστηκε 
    int opponentDefectionCount = 0; //Μετρητής των φορών που ο αντίπαλος δεν συνεργάστηκε
    int consecutiveDefectionCount =0;
  

    while ((move = getchar()) != EOF && roundCounter < 1e6) { //Όσο η είσοδος δεν είναι EOF και δεν έχουν ξεπεραστεί 10^6 επαναλήψεις
        if (move == 'C' || move == 'D') {
            roundCounter++; //Αύξηση μετρητή των γύρων

             if (move == 'C') {
                opponentCooperationCount++; //Αυξηση μετρητή των συνεργασιων (C)
                consecutiveDefectionCount =0;
                

            } else { 
                opponentDefectionCount++; //Αυξηση μετρητή των εκδικήσεων (D)
                consecutiveDefectionCount++;
                

            }

            if (roundCounter % 10 == 0) {  //Κάθε 10 γύρους
                
                if (opponentCooperationCount > opponentDefectionCount) { //Αν έχει συνεργαστεί περισσότερες απο ότι δεν συνεργάστηκε
                    
                    putchar('D'); //το πρόγραμμα δεν συνεργάζεται
                } else {
                    
                    putchar('C'); //αλλιώς συνεργάζεται
                }

                
                opponentCooperationCount = 0; //επαναφορά των μετρητών στο 0 για του επόμενους 10 γύρους
                opponentDefectionCount = 0;
            }else { //Αν δεν είναι πολλαπλάσιο του 10, τοτε το πρόγραμμα αντιγράφει τη κίνηση του αντιπάλου
                putchar(move);  
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