#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int isValid(char c) { //Συνάρτηση που ελέγχει αν ένας χαρακτήρας c είναι βάση(Α, Τ, G, C)
    return (c == 'A' || c == 'T' || c == 'G' || c == 'C');
}

/*Συνάρτηση για το "φορτωμα" ενός αρχείου. Ανοιγεί το αρχείο και αποθηκεύει την αλυσίδα σε ένας πίνακα χαρακτήρων.*/
char* LoadFile(char const* path) {
    FILE* file = fopen(path, "r"); //Άνοιγμα αρχείου 
    int length; //Μέγεθος του αρχικού string
    char* buffer; //Ο πίνακας που θα αποθηκεύσει την αλυσίδα dna που περιέχει το file.
    int i = 0; //Μετρητής των έγκυρων χαρακτήρων ώστε να τοποθετηθούν στον buffer

    if (!file) { //Αν το αρχείο δεν άνοιξε επιτυχώς εμφανίζεται μήνυμα λάθους.
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file); //Υπολογισμός του αριθμού των χαρακτήρων του αρχείου.
    fseek(file, 0, SEEK_SET);

    buffer = malloc((length + 1) * sizeof(char)); // Δέσμευση μνήμης με βάση το πραγματικό μέγεθος του αρχείου
    //Προσθήκη +1 για τον τερματικό χαρακτήρα \0
    if (!buffer) { //Αν δεν πέτυχε η δέσμευση μνήμης
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(file); //Κλείσιμο του αρχείου
        exit(1);
    }

    char currentChar;//Ο τρέχων χαρακτήρας.
    while ((currentChar = fgetc(file)) != EOF) { //Όσο δεν έχουμε φτάσει στο τέλος του αρχείου
        if (isValid(currentChar)) {//Αν είναι έγκυρος ο χαρακτήρας,
            buffer[i++] = currentChar; //αποθηκεύεται στο buffer.
            //Αυξάνεται συγχρόνως ο μετρητής των έγκυρων χαρακτήρων που είναι και η θέση του κάθε χαρακτήρα στον πίνακα.
        }
    }

    buffer[i] = '\0'; //Προσθήκη του τερματικού χαρακτήρα
    fclose(file); //Κλείσιμο του αρχείου
    return buffer; //Επιστρέφεται ο πίνακας με την αλυσίδα dna.
}



/*Συνάρτηση που βρίσκει και εκτυπώνει τη μέγιστη κοινή αλυσίδα μεταξύ δύο. Δέχεται δύο πίνακες Χ[] και Υ[] (που περιέχουν τις αλυσίδες) και
τις διαστάσεις τους m και n αντίστοιχα.*/
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
            while (x < m && y < n && X[x] == Y[y])  {
               currentLength++;
                x++;
                y++;
                
                
            }

            //Έυρεση του μεγίστου. Αν το μέγεθος της τρέχουσας κοινής αλυσίδας είναι μεγαλύτερο απο το τρέχον max τοτε max γίνεται η τρέχουσα αλυσίδα
            if (currentLength > maxCommonChain) {
                maxCommonChain = currentLength;
                end = i + maxCommonChain - 1; /*Η θέση του τελευταίου στοιχείου είναι όσο είναι το i(τυχαίο, μπορουσε και j), δηλαδή από
                εκεί που ξεκινήσαμε να βρίσκουμε κοινά στοιχεία + όσο είναι το μέγεθος της μέγιστης κοινής αλυσίδας - 1 γιατι διαφορετικά
                θα είχαμε τη θέση του επόμενου στοιχείου μετά το τελευταίο.*/
            }
            
        }
    }

    if (maxCommonChain == 0) { //Αν δεν υπάρχει κοινή ακολουθία εκτυπώνεται μήνυμα.
        printf("No common substring found.\n");
        return;
    }

    long int start = end - maxCommonChain + 1; /*Ορισμός της αρχής της μέγιστης κοινής αλυσίδας που είναι όσο ήταν η θέση του
    τελευταίου στοιχείου - το μήκος της κοινης αλυσίδας + 1 γιατι διαφορετικά θα ήμασταν στη θέση του προηγούμενου απο το πρώτο στοιχείο*/
    for (long int i = start; i <= end; i++) { //Εκτύπωση της κοινής αλυσίδας.
        printf("%c", X[i]);
    }
    printf("\n");
}



int main(int argc, char **argv) {
    if (argc != 3) { //Αν τα ορίσματα δεν είναι 3 εκτυπώνεται μήνυμα λάθους
        printf("Program needs to be called as: ./dna file1 file2\n");
        return 1;
    }
    

    char* StringA = LoadFile(argv[1]); //Κληση LoadFile για το ανοιγμα των αρχείων εισόδου
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