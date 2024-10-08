# End-to-end-WSN-Project-2023-2024

## Περιγραφή Project

Το παρόν project υλοποιήθηκε από δύο φοιτητές στα πλαίσια του μαθήματος Αλγοριθμικές Θεμελιώσεις
Δικτύων Αισθητήρων και Διαδικτύου των Αντικειμένων. Το project περιλαμβάνει τη συλλογή δεδομένων
από αισθητήρες, την αποθήκευση αυτών σε βάση δεδομένων MongoDB, και την οπτικοποίησή τους σε
πραγματικό χρόνο μέσω μιας web εφαρμογής με χρήση Flask. Τέλος, δείχνουμε την χρήση διάφορων μοντέλων (πχ ARIMA), όπου προβλέπουν επόμενες τιμές των μετρήσεων.

## Ερώτημα 1: Συλλογή Δεδομένων

Για την επικοινωνία των κόμβων και τη συλλογή δεδομένων από τους αισθητήρες TelosB,
χρησιμοποιήθηκε το πρωτόκολλο NullNet του Contiki-NG. Τα δεδομένα (θερμοκρασία και υγρασία) που
συλλέγονται, μεταφέρονται από τους αισθητήρες-φύλλα στον αισθητήρα-πατέρα μέσω unicast
μηνυμάτων.

## Ερώτημα 2: Αποθήκευση Δεδομένων

Για την αποθήκευση των δεδομένων, δημιουργήθηκε μια MongoDB βάση δεδομένων. Ένας named pipe
(out_pipe) δημιουργείται για την ανάγνωση δεδομένων από τον κόμβο πατέρα, ενώ ένα Python script
αποθηκεύει τα δεδομένα αυτά σε MongoDB. Το store_data.py script διαβάζει τα δεδομένα από τον pipe
και τα αποθηκεύει με τη σωστή μορφή στη βάση δεδομένων.

## Ερώτημα 3: Οπτικοποίηση Δεδομένων

Για την οπτικοποίηση των δεδομένων χρησιμοποιήθηκε Flask και το Plotly.js για γραφικές παραστάσεις.
Η εφαρμογή επιτρέπει την προβολή των δεδομένων σε πραγματικό χρόνο, καθώς και την ανάκτηση
ιστορικών δεδομένων με βάση χρονικά διαστήματα που εισάγουν οι χρήστες. Το αρχείο app.py περιέχει
τον κώδικα του server Flask, ενώ το αρχείο index.html περιέχει το frontend της εφαρμογής.

## Ερώτημα 4: Εκπαίδευση Μοντέλων

Για την εκπαίδευση μοντέλων πρόβλεψης χρησιμοποιήθηκαν τα δεδομένα που συλλέχθηκαν και
αποθηκεύτηκαν σε MongoDB. Τα δεδομένα προετοιμάστηκαν σε pandas DataFrame και
χρησιμοποιήθηκαν μοντέλα όπως ARIMA και LSTM για την πρόβλεψη μελλοντικών τιμών
θερμοκρασίας και υγρασίας. Περισσότερες λεπτομέρεις υπάρχουν στο αρχείο model_training.ipynb και screenshots στην αναφορά.

## Οδηγίες Εκτέλεσης

- 1. Εγκατάσταση Contiki-NG και Ρυθμίσεις

- 2. Εκκίνηση Συλλογής Δεδομένων

Στον κόμβο πατέρα, εκτελέστε την εφαρμογή για να συλλέξετε δεδομένα από τους αισθητήρες και να τα
στείλετε στον named pipe.

 #### make TARGET=sky PORT=/dev/ttyUSB0 serialdump > out_pipe

- 3. Αποθήκευση Δεδομένων σε MongoDB

Εκτελέστε το Python script store_data.py για την αποθήκευση των δεδομένων από τον named pipe στη
MongoDB.

 #### python3 store_data.py

- 4. Εκκίνηση Flask Server

Εκτελέστε την εφαρμογή Flask για την οπτικοποίηση των δεδομένων.

 #### python3 app.py

Η εφαρμογή θα τρέξει στο http://127.0.0.1:5000.

- 5. Εκπαίδευση Μοντέλων και Πρόβλεψη Τιμών

Με τα δεδομένα που συλλέχθηκαν, μπορούμε να εκπαιδεύσουμε μοντέλα για να προβλέψουμε τις μελλοντικές τιμές της θερμοκρασίας και της υγρασίας.

Για να ξεκινήσετε την εκπαίδευση των μοντέλων ARIMA, SARIMAX, LSTM και SimpleRNN:

Ανοίξτε το αρχείο model_training.ipynb στο Jupyter Notebook.
Εκτελέστε τα βήματα για την επεξεργασία των δεδομένων και την εκπαίδευση των μοντέλων.
Τα αποτελέσματα των μοντέλων θα περιλαμβάνουν την πρόβλεψη των μελλοντικών τιμών καθώς και την ακρίβεια των μοντέλων.
Για να εκτελέσετε το Jupyter Notebook:

#### jupyter notebook model_training.ipynb

Αφού εκτελέσετε το notebook, τα μοντέλα θα εκπαιδευτούν με τα δεδομένα και θα γίνει η πρόβλεψη των τιμών θερμοκρασίας και υγρασίας.
