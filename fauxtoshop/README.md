> Για την επίλυση της άσκησης 1: **Fauxtoshop** 

* Φτιαχνω τη συνάρτηση **isValidBMP** η οποία ελέγχει αν η εικόνα απο το stdin είναι έγκυρη(ειναι BMP εικόνα)
    - Αν ξεκινάει με BM
    - Αν το μέγεθος του header ειναι τουλάχιστον 54.
    - Αν χρησιμοποιούνται 24 bits για την αναπαράσταση του χρώματος
    - Αν είναι σωστό το padding
    - Αν είναι σωστό το μέγεθος του αρχείου.
    - Αν είναι έγκυρο το μήκος και το πλάτος.


* Έπειτα τη συνάρτηση **rotateBMP90degrees**, η οποία δέχεται 2 αρχεία ως ορίσματα (τα οποία θα προέλθουν από το stdin):
    * Ορίζει έναν πίνακα 54 θέσεων για το ελάχιστο μέγεθος του header
    * "Διαβάζει" τα πρώτα 54 στοιχεία του αρχείου από το stdin με τη χρήση της fread(). Διαβάζει σημαντικά στοιχεία απο τη κεφαλίδα όπως:

    ```uint32_t width = *(uint32_t*)&minheader[18];```

    ```uint32_t height = *(uint32_t*)&minheader[22];```

    ```uint16_t BitsPerPixel= *(uint16_t*)&minheader[28];```

    ```uint32_t headersize = *(uint32_t*)&minheader[10];```


    * Το 18ο byte της κεφαλίδας (18 - 22) περιέχει το πλάτος της εικόνας άρα αφορά 4*8 = 32 bits αναπαράσταση του πλάτους.
    * Το 22ο byte της κεφαλίδας (22 - 26) περιέχει το ύψος της εικόνας άρα αφορά 4*8 = 32 bits αναπαράσταση του ύψους.
    * Το 28ο byte της κεφαλίδας (28 - 29) περιέχει τον αριθμό των bits ανά pixels της εικόνας άρα αφορά 2*8 = 16 bits αναπαράσταση.
    * Το 10ο byte της κεφαλίδας (10 - 14) περιέχει την αρχή των pixels της εικόνας (το offset) αρα αφορά 4*8 = 32 bits για την αναπαράσταση.
    * (Ομοίως για τα υπόλοιπα).

    - Η έκφραση ```(*(uint32_t*)&header[18]);```:  
        * Το ```&header[18]```  παίρνει τη διεύθυνση της μνήμης του 18 στοιχείου του πίνακα header.
        * Το ```(uint32_t*)``` κάνει cast τη διεύθυνση μνήμης του 18ου στοιχείου του πίνακα απο δεικτη σε uint8_t σε δεικτη σε uint32_t.
        * To ```*``` στην αρχή παραπέμπει στο δείκτη που λαμβάνεται μετα το cast. Παίρνει την τιμή που είναι αποθηκευμένη στη θέση μνήμης που δείχνει η διεύθυνση που υπολογίζεται ερμηνεύοντας το &header[18] ως uint32_t*. Αυτό σημαίνει ότι ερμηνεύει τα 4 byte ξεκινώντας από το 18ο byte στον πίνακα κεφαλίδων ως μια ακέραια τιμή.
        * (ομοίως για τα άλλα 2 με τη μόνη διαφορά το uint16_t στο 3ο)

    * Καλείται η συνάρτηση isValidBMP.
    * Δημιουργείται νέος πίνακας header με βάση το μέγεθος του header όπως υπολογίστηκε από τα στοιχεία του αρχικού minheader και διαβάζονται τυχόν καινούργια στοιχεία, πέρα απο τα 54
        - ```fread(header + 54, sizeof(uint8_t), headersize - 54, input) != headersize - 54```
            * Θα διαβαστούν headersize - 54 στοιχεία επιπλέον(μπορεί να είναι και 0 στη περίπτωση αυτή δεν θα αλλάξει κάτι).
            * Το headersize + 54 στην αρχή προσαρμόζει τον δείκτη μιας και ήδη έχουν διαβαστεί 54 στοιχεία.
    * Υπολογίζεται το μέγεθος της εικόνας λαμβάνοντας υπόψη το padding ως εξής: 

        * ```(4 - (originalRowSize % 4)) % 4;```: 
            * ```originalRowSize % 4```: Δείχνει πόσα bytes λείπουν για να είναι πολλαπλάστιο του 4 η γραμμή
            * ```4 - (originalRowSize % 4)```: Αφού γνωρίζω πόσα bytes λείπουν για να γίνει η γραμμή πολλαπλάσιο του 4,υπολογίζεται πόσα bytes πρέπει να προστεθούν για να συμπληρωθεί το padding.
            * ```(4 - (originalRowSize % 4)) % 4;```: Το τελευταίο % 4 εξασφαλίζει ότι το αποτέλεσμα δεν θα είναι 4 καθώς αν είναι τότε αυτό σημαίνει οτι η γραμμή είναι πολλαπλάσιο του 4 και άρα δεν θέλω να προσθέσω κι άλλο padding. Οπότε σε περίπτωση που είναι 4 μηδενίζεται, ενώ σε κάθε άλλη περίπτωση δεν αλλοιώνεται.

    * Δημιουργείται ο πίνακας OtherData που θα αποθηκεύσει τα στοιχεία (αν υπάρχουν) μεταξύ του header και των pixels και διαβάζονται με τη χρήση της fread().

    * Δημιουργείται ο πίνακας pixels που θα αποθηκεύσει τα pixels της εικόνας (μεγέθους ύψος επί πλάτος εικόνας). Ορίζεται το νέο υψος, πλάτος και μέγεθος της ανεστραμμένης εικόνας, δεσμεύεται χώρος για τα νέα pixels (μεγέθους νέο ύψος επί νέο πλάτος) και γίνεται η περιστροφή με τη χρήση 2 for loops.(Κάθε pixel της εικόνας αναπαριστάται απο 3 θέσεις στον πίνακα, μία για κάθε ένα απο τα 3 χρώματα RGB). 
        - Ανανεώνοται σε κάθε for οι μεταβλητές που αντιπροσωπεύουν τις γραμμές και τις στήλες.
        - Στη θέση new_i * newRowSize + new_j * 3 αποθηκεύεται το πρώτο byte που αφορά το χρώμα κόκκινο, από τον αρχικό πίνακα με τα pixels και συγκεκριμένα από τη θέση i * RowSize + j * 3.
        - Με τη προσθήκη του +1 αντιπροσωπεύεται το πράσινο και με τη χρήση του +2 το μπλε (μιας και κάθε pixel αντιπροσωπεύεται από 3 διαδοχικές θέσεις στον πίνακα).
        - Με αυτές τις εκφράσεις που χρησιμοποιούνται ως δείκτες στους δύο πίνακες καθορίζεται η θέση στην οποία θέλω να αποθηκεύσω τα bytes κάθε pixel και φυσικά απο ποιo σημείο του αρχικού πίνακα θα τα αντλήσω. Πολλαπλασιάζοντας τη θέση γραμμής(new_i) στην οποία βρισκόμαστε με το μέγεθος της γραμμής και τη θέση της στήλης που βρισκόμαστε (new_j) με 3 (λόγω των 3 χρωμάτων) φτάνουμε κάθε φορά στο επιθυμητό σημείο αφού το i και το j ανανεώνονται με τα loops και τα new_j και new_i προσαρμόζονται κατάλληλα κάθε φορά που αλλάζουν τα i και j. Ειδικότερα:
            -  Το ```new_i * newRowSize``` καθορίζει την αρχή κάθε γραμμής στον νέο πίνακα των pixels.
            - Το ```new_j * 3``` καθορίζει τη συγκεκριμένη στήλη στην οποία θέλω να βρεθώ.
            - Έτσι, συνδυασμένα τα 2 παραπάνω μας μεταφέρουν στον σημείο του πίνακα που θέλω να αποθηκεύσω τα pixels απο την αρχή μέχρι το τέλος.(Φυσικά το ίδιο ισχύει και για τον αρχικό πίνακα pixels απλά ο σκοπός είναι αντλήσουμε τα στοιχεία).
    * Τοποθετούνται μηδενικά στις θέσεις του padding.
    
        ```for (int p = 0; p < new_padding; ++p) {```

        ```rotatedPixels[(new_i + 1) * newRowSize - new_padding +p] =0;```

        ```}```

        Με την έκφραση που χρησιμοποιείται για δείκτης του πίνακα ουσιαστικά μεταφερόμαστε για κάθε γραμμή(new_i) στο σημείο αμέσως μετά το τέλος των pixels (new_i+1) ωστε να τοποθετηθούν μηδενικά σε όλο το χώρο που έχει δεσμευθεί για το padding.

    * Καλείται η **isValidBMP()**, για έλεγχο της ανεστραμμένης εικόνας.
    * Mεταφέρονται τα νέα δεδομένα(header,otherdata(άθικτα), pixels) στο καινούριο αρχείο.

* Στη main καλείται η συνάρτηση για περιστροφή της εικόνας με ορίσματα: stdin, stdout, γιατι από εκεί δέχεται το πρόγραμμα τα δεδομένα.