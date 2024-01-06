#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Συνάρτηση που ελέγχει αν η εικόνα είναι έγκυρη.
int isValidBMP(uint8_t minheader[],uint32_t width , uint32_t height, uint16_t BitsPerPixel, uint32_t headersize, uint32_t filesize, uint32_t imagesize ) {

    //Έλεγχος αν η εικόνα ξεκινάει την μαγική κεφαλίδα με τα bytes: 'B' 'M'.
    if (minheader[0] != 'B' || minheader[1] != 'M') {
         fprintf(stderr,"Not a BMP file.\n");
        return 0;
    }

    //Έλεγχος αν η επικεφαλίδα εχει τουλάχιστον 54 στοιχεία.
    if (headersize < 54) {
        fprintf(stderr,"Header < 54. Not a valid BMP image.\n");
        return 0;
    }

    //Μονο εικόνες που χρησιμοποιούν 24 bits για αναπαράσταση χρώματος είναι δεκτές.
    if (BitsPerPixel != 24) {
         fprintf(stderr,"Only 24-bit BMP files are supported.\n");
        return 0;
    }

    int bytesPerPixel = BitsPerPixel / 8; // Υπολογισμός bytes ανά pixel
    int rowSize = width * bytesPerPixel; // Μέγεθος κάθε γραμμής σε bytes χωρίς padding
    int padding = 4 - (rowSize % 4); // Υπολογισμός του padding που απαιτείται

    // Έλεγχος αν το μέγεθος της γραμμής μαζί με το padding είναι πολλαπλάσιο του 4
    if ((rowSize + padding) % 4 != 0) {
         fprintf(stderr, "Incorrect padding in the image.\n");
        return 0;
    }

    //Έλεγχος αν το μέγεθος της εικόνας συμφωνεί με τα στοιχεία του αρχείου
    if(imagesize != headersize + filesize) {
        fprintf(stderr,"File size does not match the actual size of the image\n");
        return 0;

    }

    //Έλεγχος για το αν το image size στον DIB header είναι τουλάχιστον ίσο με το πραγματικό μέγεθος των δεδομένων της εικόνας
    uint32_t realImageSize = filesize - headersize;
    if (imagesize < realImageSize) {
        fprintf(stderr, "Image size in header is smaller than actual image data size.\n");
        return 0;
    }


    //Ελεγχος για σωστο ύψος, πλάτος
    if(height == 0 || width == 0) {
        fprintf(stderr,"Invalid height or width\n");
        return 0;
    }

    return 1;
}



//Συνάρτηση που περιστρέφει μια εικόνα bmp 90 μοίρες με τη φορά του ρολογιού
void rotateBMP90degrees(FILE *input, FILE *output) {
        uint8_t minheader[54]; //Δήλωση ενος πίνακα για την αποθήκευση των στοιχείων της κεφαλίδας

    //Διάβασμα της κεφαλίδας του αρχείου και έλεγχος οτι τα στοιχεία της κεφαλίδας ειναι 54.
    if (fread(minheader, sizeof(uint8_t), 54, input) != 54) {
        fprintf(stderr,"Error reading BMP header.\n");
        exit(1);
    }

    //Πληροφορίες για το πλάτος, το ύψος και τα bits για την αναπαράσταση του χρώματος και το offset της εικόνας, το μέγεθος του αρχείου,
    //και το συνολικό μέγεθος της εικόνας από τα στοιχεία της κεφαλίδας. Υπολογισμός μεγέθους κεφαλίδας και otherdata
    uint32_t width = *(uint32_t*)&minheader[18];
    uint32_t height = *(uint32_t*)&minheader[22];
    uint16_t BitsPerPixel = *(uint16_t*)&minheader[28]; 
    uint32_t offset = *(uint32_t*)&minheader[10]; 
    uint32_t imagesize = *(uint32_t*)&minheader[2];
    uint32_t filesize = *(uint32_t*)&minheader[34];

    uint32_t headersize = imagesize - filesize; //Το μέγεθος του header είναι το συνολικό μέγεθος της εικόνας - το μέγεθος του αρχείου

    uint32_t otherdata = offset - headersize; //Τα other data θα είναι από εκει που τελειώνει το header εως το offset της εικόνας.
    
    //Κλήση της isValidBMP για να ελέγξω αν η εικόνα ειναι έγκυρη
    int valid = isValidBMP(minheader,width,height, BitsPerPixel, headersize, filesize, imagesize); 

    if(!valid) { //Αν δεν ειναι εγκυρη το προγραμμα τερματίζεται με κωδικό εξόδου 1.
        exit(1);
    }

    //Δημιουργια πινακα header με ολα τα στοιχεια του.
    uint8_t *header = malloc(headersize * sizeof(uint8_t));
    if(!header) { //Έλεγχος επιτυχίας της malloc
        fprintf(stderr,"Failed to allocate memory for header.\n");
        exit(1);
    } 

    //Αντιγραφή του παλιού minheader στο καινούργιο πινακα header.
    for(int i = 0; i < 54; i++) {
        header[i] = minheader[i];
    }

    //Διάβασμα των υπολοιπων στοιχείων απο το header
    if (fread(header + 54, sizeof(uint8_t), headersize - 54, input) != headersize - 54) {
    fprintf(stderr,"Error reading complete header.\n");
    free(header);
    exit(1);
    }
    
    int originalRawSize = 3 * width; //Το μέγεθος της γραμμής είναι το πλάτος επί 3 γιατί η εικόνα έχει 3 bytes ανα pixel
    int padding = (4 - (originalRawSize % 4)) % 4; //Υπολογισμός του padding
    originalRawSize += padding; //Το τελικό μέγεθος κάθε γραμμής


    //Δυναμική δέσμευση μνήμης για τα otherdata.
    uint8_t *OtherData = malloc(otherdata * sizeof(uint8_t));
    if(!OtherData) {
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }

    //Διάβασμα των otherdata στον πινακα OtherData.
    if(fread(OtherData, sizeof(uint8_t), otherdata, input) != otherdata) {
        fprintf(stderr,"Error reading other data.\n");
        free(OtherData);
        exit(1);
        
    }


    //Δυναμική δέσμευση μνήμης για τα pixels της αρχικής εικόνας
    uint8_t* pixels = malloc(height * originalRawSize * sizeof(uint8_t));
    if(!pixels) { //Έλεγχος επιτυχίας της malloc.
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }

    //Διάβασμα της εικόνας και έλεγχος αν η fread πέτυχε
    if (fread(pixels, sizeof(uint8_t), height * originalRawSize, input) != height * originalRawSize) {
        fprintf(stderr,"Error reading pixel data.\n");
        free(pixels);
        exit(1);
    }

    //Το νέο πλάτος της εικόνας είναι το παλιό ύψος
    uint32_t new_width = height;

    //Το νέο ύψος της εικόνας είναι το παλιό πλάτος
    uint32_t new_height = width;

    //Το μέγεθος της εικόνας λαμβάνοντας υπόψη το padding
    int newRawSize = 3 * new_width;
    int new_padding = (4 -(newRawSize % 4)) % 4;
    newRawSize += new_padding;


    //Δυναμική δέσμευση μνήμης για την αποθήκευση των νέων ανεστραμμενων pixels
    uint8_t* rotatedPixels = malloc(new_height * newRawSize * sizeof(uint8_t));
    if(!rotatedPixels) { //Έλεγχος επιτυχίας της malloc.
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }


    /*Για τη περιστροφή: Η πρώτη γραμμη της εικόνας θα γίνει η τελευταία στήλης της νέας εικόνας, η τελευταία γραμμή της εικόνας
    θα γίνει η πρώτη στήλη της νέας εικόνας κοκ. H πρώτη στήλη της εικόνας θα γίνει η πρώτη γραμμη της νέας κοκ.*/
    for (uint32_t i = 0; i < height; ++i) { //Αφορά το ύψος της εικόνας, τις γραμμες

        uint32_t new_j = i; //οι στήλες της νέας εικόνας θα γίνουν οι γραμμες της αρχικής εικόνας.

        for (uint32_t j = 0; j < width; ++j) { //Αφορά το πλάτος της εικόνας, τις στήλες
            uint32_t new_i = width - 1 - j;//Οι γραμμες της νέας εικόνας θα γίνουν οι στήλες της αρχικής εικόνας ξεκινώντας απο το τελος.

            //Αντιγράφονται το κοκκινο, το πράσινο και το μπλε στοιχείο των pixels της αρχικής εικόνας στη κατάλληλη θέση στο νέο πινακα με τα pixels
            /*Ο πολλαπλασιασμός του new_i με το new_row_size υπολογίζει τη γραμμή της νέας εικόνας,
              Ο πολλαπλασιασμός του new_j με το 3 (το οποίο αντιστοιχεί στον αριθμό των χρωματικών στοιχείων RGB - κόκκινο, πράσινο, μπλε)
              υπολογίζει τη στήλη της νέας εικόνας.*/
            rotatedPixels[(new_i * newRawSize) + (new_j * 3)] = pixels[(i * originalRawSize) + (j * 3)]; //κοκκινο
            rotatedPixels[(new_i * newRawSize) + (new_j * 3) + 1] = pixels[(i * originalRawSize) + (j * 3) + 1]; //πράσινο
            rotatedPixels[(new_i * newRawSize) + (new_j * 3) + 2] = pixels[(i * originalRawSize) + (j * 3) + 2]; //μπλε
        }

        // Γέμισμα του padding με μηδέν
        for (int p = 0; p < new_padding; ++p) {
        rotatedPixels[(i + 1) * newRawSize - new_padding + p] = 0;
        }
    }

    // Ενημέρωση των στοιχείων της κεφαλίδας για την ανεστραμμενη εικόνα
    *(uint32_t*)&header[18] = new_width;
    *(uint32_t*)&header[22] = new_height;
    uint32_t newimagesize = newRawSize * new_height;
    *(uint32_t*)&header[34] = newimagesize;
    uint32_t newfilesize = newimagesize + headersize;
    *(uint32_t*)&header[2] = newfilesize;
    


    //Τα στοιχεία της κεφαλίδας γράφονται στην νέα εικόνα (output).
    fwrite(header, sizeof(uint8_t), headersize, output);

    //Τα otherdata αντιγράφονται στη νέα εικόνα(output).
    fwrite(OtherData,sizeof(uint8_t), otherdata, output);

    //Τα νεα pixels γράφονται στη νέα εικόνα(output).
    fwrite(rotatedPixels, sizeof(uint8_t), new_height * newRawSize, output);


    //Αποδέσμευση των πινάκων
    free(pixels); 
    free(rotatedPixels);
    free(header);
    free(OtherData);
}



int main() {
     
    rotateBMP90degrees(stdin, stdout); // Κλήση συνάρτησης για περιστροφή 
    return 0;
} 
