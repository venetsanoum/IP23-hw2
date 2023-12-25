#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Η συνάρτηση load_file διαβάζει το περιεχόμενο ενός αρχείου και το επιστρέφει ως δυναμικά δεσμευμένη συμβολοσειρά.
char* load_file(char const* path) {
    char* buffer = NULL;  //Αρχικοποίηση με NULL για να δεσμευτεί μνήμη αργότερα με βάση το μέγεθος του αρχείου
    long  length;
    
    FILE * file = fopen(path, "r"); // Άνοιγμα του αρχείου για ανάγνωση

    if (file) { //Αν το αρχείο άνοιξε επιτυχώς.
        fseek(file, 0, SEEK_END);
        length = ftell(file); // Υπολογισμός του μεγέθους του αρχείου.
        fseek(file, 0, SEEK_SET);
        buffer = (char*)malloc((length + 1) * sizeof(char)); // Δέσμευση μνήμης με βάση το πραγματικό μέγεθος του αρχείου
        //Προσθήκη +1 για τον τερματικό χαρακτήρα \0
        if(!buffer) { // Έλεγχος για την επιτυχία της δέσμευσης μνήμης.
            printf("Failed to allocate memory for buffer\n");
            exit(1);
        }
        
        if (buffer) {
            size_t bytesread = fread(buffer, sizeof(char), length, file);//Διάβασμα του αρχείου
            if(bytesread != (size_t)length) {
                printf("Failed to read file: %s\n", path);
                exit(1);
            }
            buffer[length] = '\0';
        }else { //Αν η δυναμική δέσμευση μνήμης απέτυχε
        printf("Failed to open file: %s\n", path);
    }
        fclose(file);
    }else { //Αν το αρχείο δεν άνοιξε επιτυχώς.
        printf("Failed to open file: %s\n", path);
        exit(1);
    }

    return buffer;  // Επιστροφή του δείκτη προς την αρχή του buffer που περιέχει τα δεδομένα του αρχείου.
    

}

int max(long int a, long int b) { //Συνάρτηση για την εύρεση του μεγίστου μεταξύ δύο αριθμών
    return (a > b) ? a : b;
}

void CommonSubStr(char* X, char* Y, long int m, long int n) {
    // Δήλωση του πίνακα CommonStrCounter για αποθήκευση των μηκών των κοινών υποσυμβολοσειρών
    unsigned char **CommonStrCounter = malloc((m + 1) * sizeof(unsigned char *));
    //Προσθήκη +1 για τον τερματικό χαρακτήρα \0
    if (!CommonStrCounter) {
        printf("Failed to allocate memory1\n");
        exit(1);
    }
    // Δέσμευση μνήμης για κάθε σειρά του πίνακα CommonStrCounter
    //Προσθήκη +1 για τον τερματικό χαρακτήρα \0
    for (long int k = 0; k < m + 1; k++) {
        CommonStrCounter[k] = malloc((n + 1) * sizeof(unsigned char));
        if (!CommonStrCounter[k]) {
            printf("Failed to allocate memory2\n");
            exit(1);
        }
    }

    
    // Αρχικοποίηση μεταβλητών για τον εντοπισμό της μεγαλύτερης κοινής υποσυμβολοσειράς
    long int result = 0;
    long int end = 0;

    // Δημιουργία πίνακα για αποθήκευση των μήκων των μεγαλύτερων κοινών συμβόλων των string.
    // Σημειώστε ότι η CommonStrCounter[i][j] περιέχει το μήκος της μεγαλύτερης κοινής κατάληξης
    // των X[0..i-1] και Y[0..j-1].

    for (long int i = 0; i <= m; i++) {
        for (long int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
            // Οι πρώτες εγγραφές στην πρώτη σειρά και στην πρώτη στήλη
            // δεν έχουν λογική σημασία, 
            // χρησιμοποιούνται μόνο για την απλοποίηση του προγράμματος.
                CommonStrCounter[i][j] = 0;
            else if (X[i - 1] == Y[j - 1]) {
                // Αν τα σύμβολα στις θέσεις i-1 και j-1 είναι ίδια, αυξάνουμε το μήκος της κοινής υποσυμβολοσειράς
                CommonStrCounter[i][j] = CommonStrCounter[i - 1][j - 1] + 1;
                if (CommonStrCounter[i][j] > result) {
                    // Αν το νέο μήκος είναι μεγαλύτερο από το προηγούμενο, ενημερώνουμε τα αποτελέσματα
                    result = CommonStrCounter[i][j];
                    end = i - 1;
                }
            } else {
                // Σε περίπτωση μη ταυτότητας των συμβόλων, το μήκος της κοινής υποσυμβολοσειράς γίνεται 0
                CommonStrCounter[i][j] = 0;
            }
        }
    }

    if (result == 0) {
        printf("No common substring found.\n");
        return;
    }

     // Υπολογισμός των δεικτών για την αρχή και το τέλος της μεγαλύτερης κοινής υποσυμβολοσειράς
    long int start = end - result + 1;
    
    // Εκτύπωση της μεγαλύτερης κοινής υποσυμβολοσειράς
    for (long int i = start; i <= start + result - 1; i++) {
        // Εκτύπωση μόνο των έγκυρων χαρακτήρων
        if (X[i] == 'A' || X[i] == 'G' || X[i] == 'T' || X[i] == 'C') {
            printf("%c", X[i]);
        }
    }
    printf("\n");
    //Αποδέσμευση της μνήμης για τον πίνακα CommonStrCounter
    for (long int k = 0; k < m + 1; k++) {
        free(CommonStrCounter[k]);
    }
    free(CommonStrCounter);
}



int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Program needs to be called as: ./dna <file1> <file2>\n");
        return 1;
    }
    

    char* StringA = load_file(argv[1]); //Κληση load_file για το ανοιγμα των αρχείων εισόδου
    char* StringB = load_file(argv[2]);

    if (!StringA || !StringB) {
        perror("Error loading file contents\n");
        return 1;
    }
 

    long int length1 = strlen(StringA); //Υπολογισμός μεγέθους των δύο ακολουθιών dna
    long int length2 = strlen(StringB);


    StringA[length1] = '\0'; // Προσθήκη τερματισμού συμβολοσειράς
    StringB[length2] = '\0'; // Προσθήκη τερματισμού συμβολοσειράς

    

    CommonSubStr(StringA, StringB, length1, length2);

    
    free(StringA); //Αποδέσμευση των πινάκων με τις ακολουθίες dna
    free(StringB); 
    


    return 0;
}
