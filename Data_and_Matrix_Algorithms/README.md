# Algoritmi de Procesare a Datelor și Analiză pe Matrici

## 📌 Despre Proiect
Acest proiect conține un set de trei aplicații scrise în limbajul C, axate pe manipularea fluxurilor de date, validarea regulilor pe grid-uri 2D și analiza geometrică a matricilor. Scopul este implementarea unor soluții optimizate fără utilizarea bibliotecilor externe complexe, folosind exclusiv funcții, vectori și matrici.

## 🚀 Module Implementate

### 1. Data Stream Processor (Window Join)
* **Descriere**: Un modul care analizează un flux continuu de date numerice asociate cu marcaje de timp (timestamps).
* **Funcționalitate**: Grupează datele într-o fereastră de timp glisantă (`window`) și extrage relații matematice calculând Cel Mai Mic Multiplu Comun (CMMMC) și Cel Mai Mare Divizor Comun (CMMDC) pentru perechile valide.
* **Implementare**: CMMDC este calculat prin algoritmul lui Euclid, iar CMMMC derivat din formula standard.

### 2. Akari Logic Engine (Grid Validator)
* **Descriere**: Un motor de validare și deducție logică pentru puzzle-ul japonez *Light Up* (Akari).
* **Funcționalitate**:
  * **Auto-completare parțială**: Evaluează un grid 2D și propagă raza de acțiune a surselor de lumină ('L') pe linii și coloane. Blochează automat celulele adiacente blocurilor numerotate care și-au atins capacitatea maximă.
  * **Validare strictă**: Verifică dacă starea curentă a grid-ului respectă constrângerile jocului (sursele de lumină nu se intersectează direct, iar blocurile numerotate au numărul exact de vecini permiși).

### 3. Spatial Collision Detector (Matrix Geometry)
* **Descriere**: Un algoritm de analiză spațială care validează și calculează suprapunerea unor forme geometrice pe o matrice binară.
* **Funcționalitate**:
  * Proiectează triunghiuri dreptunghice isoscele pe o matrice (reprezentând, de exemplu, amprenta la sol a unor elicoptere).
  * Validează orientarea și geometria formelor (respingând ipotenuze invalide).
  * Calculează aria de suprapunere pe zone marcate specific (0/1) pentru a determina dacă forma geometrică este valid poziționată (mai puțin de 50% suprapunere pe zone restricționate).
