• Γεώργιος Χαρίτος (ΑΜ: 1115202400212)
• Παναγιώτης Βασιλάκης (ΑΜ: 1115202400014)

# Read me

Στην παρακάτω εργασία ακολουθεί η εκτέλεση ενός αυτοκινούμενου οχήματος το οποίο «διαβάζει» τον κόσμο γύρω του και παίρνει αποφάσεις έτσι ώστε να φτάσει στον προορισμό του.

Τόσο για την κατανομή των καθηκόντων αλλά και για την οργάνωση της δομής της εργασίας ο κώδικας έχει χωριστεί σε header files, των οποίων η λειτουργία μπορεί να διακριθεί από το όνομά τους. Η εργασία ακολουθεί τις αρχές του αντικειμενοστραφούς προγραμματισμού.

Ακολουθεί μια περιληπτική επεξήγηση του κάθε ξεχωριστού αρχείου.

---

## GridWorld.hpp

Στο αρχείο **GridWorld.hpp** υλοποιείται μια κλάση `GridWorld`, η οποία αναπαριστά έναν δισδιάστατο κόσμο-πλέγμα στον οποίο τοποθετούνται αντικείμενα, είτε κινούμενα (`MovingObjects.hpp`), είτε στατικά (`StaticObjects.hpp`).

Η κλάση:

* Δημιουργεί έναν κόσμο με βάση των δοσμένων διαστάσεων (πλάτος × ύψος).
* Αποθηκεύει δείκτες σε αντικείμενα μέσα σε ένα 2D vector.
* Διαχειρίζεται σωστά τη μνήμη (δέσμευση / αποδέσμευση αντικειμένων).

### Βασικές λειτουργίες

* `addObject()` : προσθέτει ένα αντικείμενο στο grid, εφόσον η θέση του είναι εντός ορίων.
* `update(tick)` : ενημερώνει όλα τα αντικείμενα ανά χρονικό βήμα (tick), χειρίζεται τις μετακινήσεις τους και διαγράφει όσα βγαίνουν εκτός κόσμου.
* `moveObjectTo()` : μετακινεί με ασφάλεια ένα αντικείμενο σε νέα θέση, ελέγχοντας όρια και συγκρούσεις.
* `isInside()` / `getObjectAt()` : έλεγχος ορίων και πρόσβαση σε αντικείμενα συγκεκριμένης θέσης.

### Οπτικοποίηση

* `visualization_full()` : εμφανίζει ολόκληρο τον κόσμο σε μορφή πλέγματος.
* `visualization_pov()` : εμφανίζει οπτική γωνία (POV) γύρω από ένα όχημα (σύμβολο `@`), είτε κεντραρισμένη είτε εμπρόσθια.

---

## MovStatObject.hpp

Στο αρχείο αυτό ορίζονται οι δύο βασικοί τύποι αντικειμένων, στατικά και κινούμενα. Ο κώδικας αυτός είναι απαραίτητος καθώς ξεχωρίζει αν ένα αντικείμενο μεταβάλλει τη θέση του στον χρόνο.

### Static Object

Η κλάση αναπαριστά αντικείμενα που δεν κινούνται. Δηλώνουν ότι είναι στατικά μέσω των `isStatic()` και `isMoving()`.

### Moving Object

Η κλάση αναπαριστά αντικείμενα που κινούνται στον κόσμο-πλέγμα.

Χαρακτηριστικά:

* Διαθέτουν κατεύθυνση (`Direction`) και ταχύτητα (`speed`).
* Η κίνηση υλοποιείται στη συνάρτηση `update()`, η οποία μεταβάλλει τη θέση του αντικειμένου ανάλογα με την κατεύθυνση και την ταχύτητα.
* Χρησιμοποιεί τη συνάρτηση `directionToVector()` για να μετατρέψει την κατεύθυνση σε διάνυσμα κίνησης.

Λειτουργικότητα:

* Σε κάθε χρονικό βήμα, το αντικείμενο μετακινείται αυτόματα.
* Παρέχονται getters και setters για δυναμική αλλαγή κατεύθυνσης και ταχύτητας.

---

## MovingObjects.hpp

Στο αρχείο αυτό απλά διευκρινίζεται αν το κινούμενο αντικείμενο είναι ποδήλατο ή αυτοκίνητο. Και τα δύο αντικείμενα κινούνται με την ίδια ταχύτητα σε μια τυχαία κατεύθυνση που προσδιορίζεται μέσω της `Random`.

---

## StaticObjects.hpp

Αντίστοιχα στο αρχείο διευκρινίζεται η ιδιότητα του κάθε στατικού αντικειμένου (σταθμευμένο όχημα, σήμα, φανάρι). Στο κάθε αντικείμενο αντιστοιχεί και μία κλάση.

### Stationary Vehicle

Η κλάση αυτή αναπαριστά ένα σταθμευμένο όχημα. Συμβολίζεται με `P` και έχει τύπο `Parked`. Αποτελεί απλά ένα εμπόδιο.

### Street Sign

Η κλάση αυτή αναπαριστά μια πινακίδα στον δρόμο. Διαθέτει κείμενο που δίνεται από τον χρήστη, το κείμενο αυτό διαβάζεται από τη συνάρτηση `getText()`.

### TrafficLight

Η κλάση `TrafficLight` αναπαριστά φανάρι κυκλοφορίας με εναλλασσόμενες καταστάσεις.

Χαρακτηριστικά:

* Παραμένει ακίνητο στο grid.
* Διατηρεί εσωτερική κατάσταση (`state`) και μετρητή χρόνου (`counter`).
* Αλλάζει χρώμα αυτόματα με βάση τα ticks της προσομοίωσης.

Συμπεριφορά:

* Κόκκινο: 4 ticks (`R`)
* Πράσινο: 8 ticks (`G`)
* Κίτρινο: 2 ticks (`Y`)

Το σύμβολο (`R`, `G`, `Y`) ενημερώνεται δυναμικά για οπτικοποίηση.

Βοηθητικές συναρτήσεις:

* `isRed()`, `isGreen()`, `isYellow()` για έλεγχο κατάστασης.
* `getColor()` επιστρέφει το χρώμα του φαναριού σε μορφή κειμένου.

---

## Utilities.hpp

Στο αρχείο αυτό περιλαμβάνονται κάποιες γενικές συναρτήσεις και κλάσεις οι οποίες χρησιμοποιούνται σε ολόκληρη την εργασία. Ο ρόλος τους δεν συνδέεται άμεσα με το θέμα του κάθε αρχείου και αποτελεί καθαρά λειτουργικός, άρα τοποθετήθηκαν ξεχωριστά στο αρχείο `Utilities`.

---

## WorldObject.hpp

Το αρχείο **WorldObject.hpp** ορίζει την αφηρημένη (abstract) βασική κλάση `WorldObject`, από την οποία κληρονομούν όλα τα αντικείμενα που τοποθετούνται και λειτουργούν μέσα στο `GridWorld`. Αποτελεί τον κοινό πυρήνα του συστήματος αντικειμένων της προσομοίωσης.

Κάθε αντικείμενο έχει:

* Id
* Type
* Glyph
* Position

---

## NavigationSystem.hpp – Navigation System

Το αρχείο **NavigationSystem.hpp** υλοποιεί το σύστημα πλοήγησης του οχήματος μέσα στο `GridWorld`. Ο ρόλος του είναι να λαμβάνει δεδομένα αισθητήρων, να τα συνδυάζει (sensor fusion) και να παράγει αποφάσεις κίνησης, όπως επιτάχυνση, επιβράδυνση, στροφή ή στάση.

Η δομή `NavigationDecision` συγκεντρώνει όλες τις επιλογές που έχει το όχημα για κάθε tick.

Η κλάση `NavigationSystem` είναι η βασική λειτουργία του αρχείου και παίρνει αποφάσεις. Δέχεται και επεξεργάζεται τα στοιχεία από τους αισθητήρες μέσω του `SensorFusionEngine`.

Η βασική συνάρτηση του συστήματος πλοήγησης είναι η `MakeDecision`. Υπολογίζει την κατεύθυνση προς τον στόχο, ελέγχει την απόσταση και προσαρμόζει την ταχύτητα με βάση τον στόχο, εμπόδια, φανάρια κτλπ.

Άλλες συναρτήσεις:

* `setCurrentTarget()` : ορίζει τον προορισμό.
* `updateCurrentData()` : δέχεται τις μετρήσεις αισθητήρων και τις συγχωνεύει.
* `checkEnvironment()` : ελέγχει το γύρω περιβάλλον και προειδοποιεί τον χρήστη για αντικείμενα που μπορεί να βρίσκονται στον δρόμο του οχήματος.
* `calculateDirectionToTarget()` : υπολογίζει την ιδανική κατεύθυνση προς τον στόχο.
* `checkForHazards()` : ελέγχει μπροστά από το όχημα για εμπόδια, φανάρια και όρια του κόσμου.
* `hasStopSignAhead()` : ανιχνεύει πινακίδα STOP μπροστά στο όχημα.

---

## SelfDrivingCar.hpp

Το self driving car συνεργάζεται με το αρχείο navigation system. Ορίζει GPS στόχους (διαδρομή). Στέλνει συγχωνευμένα δεδομένα αισθητήρων. Λαμβάνει αποφάσεις όπως επιτάχυνση, επιβράδυνση, στροφή ή στάση.

Η πλοήγηση βασίζεται:

* στην απόσταση από τον στόχο (Manhattan distance),
* στην ύπαρξη εμποδίων,
* σε φανάρια και πινακίδες STOP.

### Κύκλος λειτουργίας (Σε κάθε tick της προσομοίωσης)

* Συλλέγονται δεδομένα από όλους τους αισθητήρες.
* Τα δεδομένα συγχωνεύονται (sensor fusion).
* Το σύστημα πλοήγησης ενημερώνεται.
* Λαμβάνεται απόφαση κίνησης.
* Το όχημα επιταχύνει, επιβραδύνει, στρίβει ή σταματά.
* Εκτελείται μετακίνηση στο GridWorld.
* Ελέγχεται αν επιτεύχθηκε GPS στόχος.
* Εμφανίζεται αναλυτική κατάσταση (debug / status).

---

## SensorFusionEngine.hpp

Το `SensorFusionEngine` παίρνει μετρήσεις από όλους τους αισθητήρες και τις συνδυάζει παίρνοντας ως δεδομένη τη μέτρηση με το υψηλότερο confidence, συμπληρώνοντας πληροφορίες στην περίπτωση που λείπουν αντικείμενα.

---

## SensorReading.hpp

Το `SensorReading` είναι μία δομή δεδομένων που αποθηκεύει τις πληροφορίες του αισθητήρα (objectType, objectId, position, distance, confidence) και άλλες πληροφορίες όπως κατεύθυνση και κείμενο του κάθε σήματος στην περίμετρο «ανάγνωσης».

---

## Sensors.hpp

Η αντίληψη του αυτοκινήτου εξαρτάται από το συγκεκριμένο αρχείο. Οι αισθητήρες που χρησιμοποιούνται είναι τρεις (lidar, radar, camera).

Για τον κάθε αισθητήρα υπάρχει η αντίστοιχη κλάση. Η πρώτη γενικότερη κλάση (`Sensor`) καθορίζει τι είναι ένας αισθητήρας έτσι ώστε να δημιουργηθούν οι ακόλουθες υποκλάσεις για τον κάθε αισθητήρα.

### Lidar

* Έχει εμβέλεια εννιά κελιών.
* Ελέγχει τα κελιά από -4 έως 4 στους άξονες x και y γύρω από τη θέση του αυτοκινήτου.
* Υπολογίζει Manhattan Distance.
* Υπολογίζει τη βεβαιότητα και καταγράφει τη θέση και τον τύπο των αντικειμένων που ανιχνεύει.

### Radar

* Έχει εμβέλεια 12 κελιών μόνο προς την κατεύθυνση του αυτοκινήτου.
* Αν εντοπίσει κάποιο αντικείμενο υπολογίζει την απόσταση.
* Μεταβάλλει τη βεβαιότητα καλώντας τη συνάρτηση `calculateConfidence`.
* Διαβάζει την κατεύθυνση και την ταχύτητα του εντοπισμένου αντικειμένου.

### Camera

* Έχει μικρότερη εμβέλεια από τους δύο προηγούμενους αισθητήρες.
* Χρησιμοποιείται για την ανάγνωση πινακίδων και αναγνώριση χρωμάτων των φαναριών.

---

## main.cpp

Η `main` στήνει όλη την προσομοίωση. Αρχικά γίνεται το parsing των arguments και σε περίπτωση που δεν δίνονται οι συντεταγμένες το πρόγραμμα σταματά.

Δημιουργεί το GridWorld, τοποθετεί τα αντικείμενα, τοποθετεί το αυτοκίνητο στη θέση εκκίνησης, αρχίζει τη διαδρομή και εκτυπώνει τον κόσμο όταν φτάσει στον προορισμό του το αυτοκίνητο.

## Μεταγλώττιση και εκτέλεση
Για την μεταγλώττιση υπάρχει κατάλληλο Makefile και χρησιμοποιούμε την εντολές  `make clean` και έπειτα `make`.
```
$ make clean
rm -f main.o oopproj
$ make
g++ -std=c++11 -Wall -I. -c main.cpp -o main.o
g++ -std=c++11 -Wall -I. -o oopproj main.o
```
Στην συνέχεια τρέχοντας `./oopproj` θα εμφανιστεί το usage message.
```
$ ./oopproj 
Error: GPS coordinates required! Use --gps x1 y1 x2 y2
Self-Driving Car Simulation
Usage:
  --seed <n>                 Random seed (default: current time)
  --dimX <n>                 World width (default: 40)
  --dimY <n>                 World height (default: 40)
  --numMovingCars <n>        Number of moving cars (default: 3)
  --numMovingBikes <n>       Number of moving bikes (default: 4)
  --numParkedCars <n>        Number of parked cars (default: 5)
  --numStopSigns <n>         Number of stop signs (default: 2)
  --numTrafficLights <n>     Number of traffic lights (default: 2)
  --simulationTicks <n>      Maximum simulation ticks (default: 100)
  --viewRadius <n>           POV view radius (default: 3)
  --frontView                Use front view instead of centered
  --gps <x1> <y1> <x2> <y2>  GPS target coordinates (required)
  --help                     Show this help message

Example: ./oopproj --seed 12 --viewRadius 5 --gps 10 20 32 15
```
Έτσι τρέχοντας σωστά το πρόγραμμα με για παράδειγμα την εντολή ` ./oopproj --seed 11 --dimX 15 --dimY 15 --gps 1 1 5 5`. Θα έχουμε το εξής αποτέλεσμα:
```
$ ./oopproj --seed 11 --dimX 15 --dimY 15 --gps 1 1 5 5
=== Self Driving Car Simulation ===
World: 15x15 | Seed: 11
GPS: (1,1) -> (5,5)
Ticks: 100 | View: Centered (radius: 3)
==================================

Creating objects...
[PARKED] at (8,14)
[PARKED] at (12,5)
[PARKED] at (3,10)
[PARKED] at (1,1)
[PARKED] at (14,13)
[STOP SIGN] at (5,6)
[STOP SIGN] at (11,4)
[TRAFFIC LIGHT] at (1,14)
[TRAFFIC LIGHT] at (13,3)
[MOVING CAR] at (3,4)
[MOVING CAR] at (1,5)
[MOVING CAR] at (7,8)
[MOVING BIKE] at (13,11)
[MOVING BIKE] at (4,8)
[MOVING BIKE] at (0,3)
[MOVING BIKE] at (1,10)

=== INITIAL STATE (Full Visualization) ===

=== FULL WORLD (15x15) ===
. R . . . . . . P . . . . . . 
. . . . . . . . . . . . . . P 
. . . . . . . . . . . . . . . 
. . . . . . . . . . . . . B . 
. B . P . . . . . . . . . . . 
. . . . . . . . . . . . . . . 
. . . . B . . C . . . . . . . 
. . . . . . . . . . . . . . . 
. . . . . S . . . . . . . . . 
. C . . . . . . . . . . P . . 
. . . C . . . . . . . S . . . 
B . . . . . . . . . . . . R . 
. . . . . . . . . . . . . . . 
. P . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . 
=============================

=== CREATING AUTONOMOUS CAR ===
Warning: Starting position occupied! Finding alternative...
New starting position: (0,0)
[+VEHICLE:CAR17] Self driving car initialized at (0,0), heading EAST
[+LDAR:CAR17_LIDAR] Lidar sensor ready - Sensing with pew pews!
[+RADAR:CAR17_RADAR] Radar sensor ready - I'm a Radio star!
[+CAMERA:CAR17_CAMERA] Camera sensor ready - Say cheese!
[SYSTEM] Total sensors: 3
[SYSTEM] Car equipped with 3 sensors and navigation system
[GPS:CAR17] GPS route set with 1 targets
  Target 1: (5,5)
[NAV] Target set to: (5,5)

=== STARTING SIMULATION ===
Autonomous car ID: CAR17
Starting at: (0,0)
Target: (5,5)

--- Tick 0 ---

[TICK 0] Car CAR17 updating...
[SENSOR] LIDAR detected 3 objects
[SENSOR] RADAR detected 0 objects
[SENSOR] CAMERA detected 2 objects
[SENSOR] Total raw detections: 5 from 3 sensors
[FUSION] Processing object: BIKE15
[FUSION] Processing object: PARKED4
[FUSION] Fused 5 readings into 2 objects
[FUSION] Fused 5 readings into 2 objects
  High confidence: PARKED at (1,1) (conf: 0.8547)
[FUSION] Processing object: BIKE15
[FUSION] Processing object: PARKED4
[FUSION] Fused 2 readings into 2 objects
[NAV] Memory updated with 2 fused objects
  - Reading[PARKED4]: type->PARKED, pos->(1,1), dist->2.000000, conf->0.854700

=== ENVIRONMENT CHECK ===
Nearby: BIKE at distance 4, confidence 0.6336
Nearby: PARKED at distance 2, confidence 0.8547
========================

[NAV] Distance to target: 10
[NAV] Decision: ACCELERATE 
[DECISION] Decision: ACCELERATE 
[CAR:CAR17] Accelerating to HALF_SPEED
[MOVEMENT] Attempting to move 1 step(s)
[MOVEMENT] Successfully moved to (1,0)
[GPS] Distance to target 1: 9 units
[GPS] Distance to target 1: 9 units
[STATUS] Car CAR17: Pos->(1,0), Dir->EAST, Speed=HALF, Target=(5,5) (1/1)
[SPEED] [HALF]
[PROGRESS] Target 1 (9 units)

=== POV VIEW ===
Car: @ at (1,0)
Χ Χ . B . . . 
Χ Χ . . . . . 
Χ Χ . P . . . 
Χ Χ . @ . . . 
Χ Χ Χ Χ Χ Χ Χ 
Χ Χ Χ Χ Χ Χ Χ 
Χ Χ Χ Χ Χ Χ Χ 
===============

--- Tick 1 ---

[TICK 1] Car CAR17 updating...
[SENSOR] LIDAR detected 3 objects
[SENSOR] RADAR detected 0 objects
[SENSOR] CAMERA detected 2 objects
[SENSOR] Total raw detections: 5 from 3 sensors
[FUSION] Processing object: BIKE15
[FUSION] Processing object: PARKED4
[FUSION] Fused 5 readings into 2 objects
[FUSION] Fused 5 readings into 2 objects
  High confidence: PARKED at (1,1) (conf: 0.9878)
[FUSION] Processing object: BIKE15
[FUSION] Processing object: PARKED4
[FUSION] Fused 2 readings into 2 objects
[NAV] Memory updated with 2 fused objects
  - Reading[PARKED4]: type->PARKED, pos->(1,1), dist->1.000000, conf->0.987800

=== ENVIRONMENT CHECK ===
Nearby: PARKED at distance 1, confidence 0.9878
========================

[NAV] Distance to target: 9
[NAV] Decision: ACCELERATE TURN to NORTH 
[DECISION] Decision: ACCELERATE TURN to NORTH 
[NAV] Cannot turn NORTH - obstacle at (1,1)
[NAV] Trying alternative direction: EAST
[CAR:CAR17] Accelerating to FULL_SPEED
[MOVEMENT] Attempting to move 2 step(s)
[MOVEMENT] Successfully moved to (2,0)
[GPS] Distance to target 1: 8 units
[MOVEMENT] Successfully moved to (3,0)
[GPS] Distance to target 1: 7 units
[GPS] Distance to target 1: 7 units
[STATUS] Car CAR17: Pos->(3,0), Dir->EAST, Speed=FULL, Target=(5,5) (1/1)
[SPEED] [FULL]
[PROGRESS] Target 1 (7 units)

=== POV VIEW ===
Car: @ at (3,0)
. . B . . . . 
. . . . . . . 
. P . . . . . 
. . . @ . . . 
Χ Χ Χ Χ Χ Χ Χ 
Χ Χ Χ Χ Χ Χ Χ 
Χ Χ Χ Χ Χ Χ Χ 
===============

--- Tick 2 ---

[TICK 2] Car CAR17 updating...
[SENSOR] LIDAR detected 3 objects
[SENSOR] RADAR detected 0 objects
[SENSOR] CAMERA detected 2 objects
[SENSOR] Total raw detections: 5 from 3 sensors
[FUSION] Processing object: BIKE15
[FUSION] Processing object: PARKED4
[FUSION] Fused 5 readings into 2 objects
[FUSION] Fused 5 readings into 2 objects
  High confidence: BIKE at (3,3) (conf: 0.7667)
  High confidence: PARKED at (1,1) (conf: 0.7777)
[FUSION] Processing object: BIKE15
[FUSION] Processing object: PARKED4
[FUSION] Fused 2 readings into 2 objects
[NAV] Memory updated with 2 fused objects
  - Reading[BIKE15]: type->BIKE, pos->(3,3), dist->3.000000, conf->0.766700
  - Reading[PARKED4]: type->PARKED, pos->(1,1), dist->3.000000, conf->0.777700

=== ENVIRONMENT CHECK ===
Nearby: BIKE at distance 3, confidence 0.7667
  CAUTION: Bike nearby!
Nearby: PARKED at distance 3, confidence 0.7777
========================

[NAV] Distance to target: 7
[NAV] Decision: ACCELERATE TURN to NORTH 
[DECISION] Decision: ACCELERATE TURN to NORTH 
[CAR:CAR17] Turning from EAST to NORTH
[MOVEMENT] Attempting to move 2 step(s)
[MOVEMENT] Successfully moved to (3,1)
[GPS] Distance to target 1: 6 units
[MOVEMENT] Successfully moved to (3,2)
[GPS] Distance to target 1: 5 units
[GPS] Distance to target 1: 5 units
[STATUS] Car CAR17: Pos->(3,2), Dir->NORTH, Speed=FULL, Target=(5,5) (1/1)
[SPEED] [FULL]
[PROGRESS] Target 1 (5 units)

=== POV VIEW ===
Car: @ at (3,2)
. . . . . . . 
. . . . . . C 
. . . B . . . 
. . . @ . . . 
. P . . . . . 
. . . . . . . 
Χ Χ Χ Χ Χ Χ Χ 
===============

--- Tick 3 ---

[TICK 3] Car CAR17 updating...
[SENSOR] LIDAR detected 4 objects
[SENSOR] RADAR detected 1 objects
[SENSOR] CAMERA detected 2 objects
[SENSOR] Total raw detections: 7 from 3 sensors
[FUSION] Processing object: BIKE15
[FUSION] Processing object: CAR12
[FUSION] Processing object: PARKED4
[FUSION] Fused 7 readings into 3 objects
[FUSION] Fused 7 readings into 3 objects
  High confidence: BIKE at (4,3) (conf: 0.8822)
  High confidence: PARKED at (1,1) (conf: 0.7799)
[FUSION] Processing object: BIKE15
[FUSION] Processing object: CAR12
[FUSION] Processing object: PARKED4
[FUSION] Fused 3 readings into 3 objects
[NAV] Memory updated with 3 fused objects
  - Reading[BIKE15]: type->BIKE, pos->(4,3), dist->2.000000, conf->0.882200
  - Reading[PARKED4]: type->PARKED, pos->(1,1), dist->3.000000, conf->0.779900

=== ENVIRONMENT CHECK ===
Nearby: BIKE at distance 2, confidence 0.8822
  CAUTION: Bike nearby!
Nearby: PARKED at distance 3, confidence 0.7799
========================

[NAV] Distance to target: 5
[NAV] Decision: ACCELERATE 
[DECISION] Decision: ACCELERATE 
[MOVEMENT] Attempting to move 2 step(s)
[MOVEMENT] Successfully moved to (3,3)
[GPS] Distance to target 1: 4 units
[MOVEMENT] Successfully moved to (3,4)
[GPS] Distance to target 1: 3 units
[GPS] Distance to target 1: 3 units
[STATUS] Car CAR17: Pos->(3,4), Dir->NORTH, Speed=FULL, Target=(5,5) (1/1)
[SPEED] [FULL]
[PROGRESS] Target 1 (3 units)

=== POV VIEW ===
Car: @ at (3,4)
. . . . . . . 
. . . . . S . 
. . . . . . . 
. . . @ . . . 
. . . . B . . 
. . . . . . . 
. P . . . . . 
===============

--- Tick 4 ---

[TICK 4] Car CAR17 updating...
[SENSOR] LIDAR detected 4 objects
[SENSOR] RADAR detected 0 objects
[SENSOR] CAMERA detected 3 objects
[SENSOR] Total raw detections: 7 from 3 sensors
[FUSION] Processing object: BIKE15
[FUSION] Processing object: CAR12
[FUSION] Processing object: PARKED4
[FUSION] Processing object: SIGN6
[FUSION] Fused 7 readings into 4 objects
[FUSION] Fused 7 readings into 4 objects
[FUSION] Processing object: BIKE15
[FUSION] Processing object: CAR12
[FUSION] Processing object: PARKED4
[FUSION] Processing object: SIGN6
[FUSION] Fused 4 readings into 4 objects
[NAV] Memory updated with 4 fused objects

=== ENVIRONMENT CHECK ===
Nearby: BIKE at distance 3, confidence 0.6721
  CAUTION: Bike nearby!
========================

[NAV] Distance to target: 3
[NAV] Decelerating: approaching target (distance: 3)
[NAV] Decision: DECELERATE TURN to EAST 
[DECISION] Decision: DECELERATE TURN to EAST 
[CAR:CAR17] Decelerating to HALF_SPEED
[CAR:CAR17] Turning from NORTH to EAST
[MOVEMENT] Attempting to move 1 step(s)
[MOVEMENT] Successfully moved to (4,4)
[GPS] Distance to target 1: 2 units
[GPS] Distance to target 1: 2 units
[STATUS] Car CAR17: Pos->(4,4), Dir->EAST, Speed=HALF, Target=(5,5) (1/1)
[SPEED] [HALF]
[PROGRESS] Target 1 (2 units)

=== POV VIEW ===
Car: @ at (4,4)
. . . . . . . 
. . . . S . . 
. . . . . . . 
. . . @ . . . 
. . . . B . . 
. . . . . . . 
P . . . . . . 
===============

--- Tick 5 ---

[TICK 5] Car CAR17 updating...
[SENSOR] LIDAR detected 4 objects
[SENSOR] RADAR detected 1 objects
[SENSOR] CAMERA detected 3 objects
[SENSOR] Total raw detections: 8 from 3 sensors
[FUSION] Processing object: BIKE15
[FUSION] Processing object: CAR10
[FUSION] Processing object: SIGN6
[FUSION] Fused 8 readings into 3 objects
[FUSION] Fused 8 readings into 3 objects
  High confidence: BIKE at (6,3) (conf: 0.7282)
  High confidence: SIGN at (5,6) (conf: 0.7447)
[FUSION] Processing object: BIKE15
[FUSION] Processing object: CAR10
[FUSION] Processing object: SIGN6
[FUSION] Fused 3 readings into 3 objects
[NAV] Memory updated with 3 fused objects
  - Reading[BIKE15]: type->BIKE, pos->(6,3), dist->3.000000, conf->0.728200
  - Reading[SIGN6]: type->SIGN, pos->(5,6), dist->3.000000, conf->0.744700

=== ENVIRONMENT CHECK ===
Nearby: BIKE at distance 3, confidence 0.7282
  CAUTION: Bike nearby!
Nearby: SIGN at distance 3, confidence 0.7447
========================

[NAV] Distance to target: 2
[NAV] Decelerating: approaching target (distance: 2)
[NAV] Very close to target - maintaining speed
[NAV] Decision: 
[DECISION] Decision: 
[MOVEMENT] Attempting to move 1 step(s)
[MOVEMENT] Successfully moved to (5,4)
[GPS] Distance to target 1: 1 units
[GPS] Reached target 1 at (5,5)
[GPS] All 1 targets reached!
[GPS] All targets completed
[STATUS] Car CAR17: Pos->(5,4), Dir->EAST, Speed=STOPPED, Target=(5,4) (2/1)
[SPEED] [STOPPED]

=== POV VIEW ===
Car: @ at (5,4)
. . . . . . . 
. . . S . . . 
. . . . . . . 
. . . @ . . . 
. . . . B . . 
. . . . . . . 
. . . . . . . 
===============

✓ ALL TARGETS REACHED! ✓

=== FINAL STATE (Full Visualization) ===

=== FULL WORLD (15x15) ===
. . . . B . . . P . . . . . . 
. . . . . . . . . . . . . . P 
. . . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . 
. . . P . . . . . . . . . . . 
. . . . . . . . . . . . . . . 
. C . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . 
. . . . . S . . . . . . . . . 
. . . . . . . . . . . . P B . 
. . . . . @ . . . C . S . . . 
. . . . . . B . . . . . . G . 
. . . . . . . . . . . . . . . 
. P . . . . . . . . . . . . . 
. . . . . . . . . . . . . . . 
=============================

=== SIMULATION COMPLETE ===
Final position: (5,4)
Target: (5,5)
Distance: 1 cells
```
