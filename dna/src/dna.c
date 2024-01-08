#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Η συνάρτηση load_file διαβάζει το περιεχόμενο ενός αρχείου και το επιστρέφει ως δυναμικά δεσμευμένη συμβολοσειρά.
char* LoadFile(char const* path) {
    char* buffer;  //Δήλωση πίνακα buffer
    long  length;
    
    FILE * file = fopen(path, "r"); // Άνοιγμα του αρχείου για ανάγνωση

    if (file) { //Αν το αρχείο άνοιξε επιτυχώς.
        fseek(file, 0, SEEK_END);
        length = ftell(file); // Υπολογισμός του μεγέθους του αρχείου.
        fseek(file, 0, SEEK_SET);
        buffer = malloc((length + 1) * sizeof(char)); // Δέσμευση μνήμης με βάση το πραγματικό μέγεθος του αρχείου
        //Προσθήκη +1 για τον τερματικό χαρακτήρα \0
        if(!buffer) { // Έλεγχος για την επιτυχία της δέσμευσης μνήμης.
            fprintf(stderr,"Failed to allocate memory for buffer\n");
            exit(1);
        }
        
        if (buffer) {
            size_t bytesread = fread(buffer, sizeof(char), length, file);//Διάβασμα του αρχείου
            if(bytesread != (size_t)length) {
                fprintf(stderr, "Failed to read file: %s\n", path);
                exit(1);
            }
            buffer[length] = '\0'; // Προσθήκη τερματισμού συμβολοσειράς
        }else { //Αν το διάβασμα του αρχείου απέτυχε
        fprintf(stderr,"Failed to read file: %s\n", path);
        }
        fclose(file);
    }else { //Αν το αρχείο δεν άνοιξε επιτυχώς.
        fprintf(stderr,"Failed to open file: %s\n", path);
        exit(1);
    }

    return buffer;  // Επιστροφή του πίνακα buffer με τα στοιχεία της αλυσίδας.
    

}


void CommonSubStr(char* X, char* Y, long int m, long int n) {
    long int maxCommonChain = 0; //Το μέγιστο μήκος της κοινής αλυσίδας μεταξύ δύο αλυσίδων. 
    long int end = 0; //Η θέση του τελευταίου στοιχείου της κοινής αλυσίδας

    //For loop για κάθε χαρακτήρα του πίνακα Χ[]
    for (long int i = 0; i < m; i++) {
        //For loop για κάθε χαρακτήρα του πίνακα Υ[]
        for (long int j = 0; j < n; j++) {
            long int currentLength = 0;  //Το τρέχον μέγεθος μιας κοινής συμβολοσειράς
            long int x = i, y = j; //Κρατάω τις θέσεις των στοιχείων του πινακα Χ και του Υ αντίστοιχα

            /*Έλεγχος για κοινή αλυσίδα ξεκινώντας απο τις θέσεις i και j. Όσο δεν είμαστε στο τέλος κάποιας αλυσίδας και τα δύο στοιχεία
            ταυτίζονται αυξάνεται το μέγεθος της τρέχουσας κοινής αλυσίδας και οι θέσεις των στοιχείων ωστε να πάμε στα επόμενα*/
            while (x < m && y < n && X[x] == Y[y]) {
                currentLength++;
                x++;
                y++;
            }

            //Έυρεση του μεγίστου. Αν το μέγεθος της τρέχους κοινής αλτσίδας είναι μεγαλύτερο απο το τρέχον max τοτε max γίνεται η τρέχουσα αλυσίδα
            if (currentLength > maxCommonChain) {
                maxCommonChain = currentLength;
                end = i + maxCommonChain - 1; /*Η θέση του τελευταίου στοιχείου είναι όσο είναι το i(τυχαίο, μπορουσε και j), δηλαδή
                η θέση (της μιας αλυσίδας) στην οποία βρισκόμαστε  + όσο είσαι το μέγεθος της μέγιστης κοινής αλυσίδας - 1 γιατι διαφορετικά
                θα είχα τη θέση του επόμενου στοιχείου μετά το τελικό.*/
            }
        }
    }

    if (maxCommonChain == 0) { //Αν δεν υπάρχει κοινή ακολουθίας εκτυπώνεται μήνυμα.
        printf("No common substring found.\n");
        return;
    }

    long int start = end - maxCommonChain + 1; /*Ορισμός της αρχής της μέγιστης κοινής αλυσίδας που είναι όσο ήταν η θέση του
    τελευταίου στοιχείου - το μήκος της κοινης αλυσίδας + 1 γιατι διαφορετικά θα ήμασταν στη θέση του προηγούμενου απο το πρώτο στοιχείο*/
    for (long int i = start; i <= end; i++) { //Εκτύπωση μόνο των έγκυτων χαρακατήρων.
        if(X[i] == 'A' || X[i] == 'C' || X[i] == 'G' || X[i] == 'T'){
        printf("%c", X[i]);
        }
    }
    printf("\n");
}



int main(int argc, char **argv) {
    if (argc != 3) { //Αν τα ορίσματα δεν είναι 3 εκτυπώνεται μήνυμα λάθους
        printf("Program needs to be called as: ./dna <file1> <file2>\n");
        return 1;
    }
    

    char* StringA = LoadFile(argv[1]); //Κληση load_file για το ανοιγμα των αρχείων εισόδου
    char* StringB = LoadFile(argv[2]);

    if (!StringA || !StringB) { //Έλεγχος αν πέτυχε το φόρτωμα των αρχείων
        fprintf(stderr, "Error loading file contents\n");
        return 1;
    }

    long int length1 = strlen(StringA); //Υπολογισμός μεγέθους των δύο ακολουθιών dna
    long int length2 = strlen(StringB);
    if(length1 > 100000 || length2 > 100000) { //Έλεγχος αν οι αλυσίδες ξεπερνούν τις 100.000 βάσεις.
        printf("Maximum chain size allowed is 100.000\n");
        free(StringA);
        free(StringB);
        return 1;
    }

    CommonSubStr(StringA, StringB, length1, length2); //Κλήση της συνάρτησης για την εύρεση της μέγιστης κοινής αλυσίδας μεταξύ των 2.

    
    free(StringA); //Αποδέσμευση των πινάκων με τις ακολουθίες dna
    free(StringB); 
    


    return 0;
}
