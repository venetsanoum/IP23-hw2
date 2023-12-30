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
    long int result = 0; 
    long int end = 0; 

    
    for (long int i = 0; i < m; i++) {
        for (long int j = 0; j < n; j++) {
            long int len = 0; 
            long int x = i, y = j; 

           
            while (x < m && y < n && X[x] == Y[y]) {
                len++;
                x++;
                y++;
            }

            
            if (len > result) {
                result = len;
                end = i + len - 1;
            }
        }
    }

    if (result == 0) {
        printf("No common substring found.\n");
        return;
    }

    printf("%ld\n" ,result);
    for (long int i = end - result + 1; i <= end; i++) {
        if(X[i] == 'A' || X[i] == 'C' || X[i] == 'G' || X[i] == 'T'){
        printf("%c", X[i]);
        }
    }
    printf("\n");
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
