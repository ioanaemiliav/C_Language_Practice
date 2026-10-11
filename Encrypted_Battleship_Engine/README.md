# Encrypted Battleship Engine & Analytics

## 📌 Despre Proiect
Acest proiect implementează un motor de simulare pentru un joc de strategie navală pe grid (Battleships), completat de un modul de analiză statistică și un sistem securizat de comunicație a datelor folosind algoritmul criptografic Hill Cipher. Proiectul este dezvoltat în limbajul C și folosește exclusiv alocare dinamică a memoriei pentru gestionarea eficientă a resurselor.

## 🚀 Module Implementate

### 1. Naval Combat Simulation Engine
* **Descriere**: Un motor de joc care simulează plasarea navelor și atacurile pe o grilă bidimensională.
* **Funcționalitate**: 
  * Gestionează amplasarea validă a navelor (pe orizontală sau verticală) și verifică suprapunerile sau ieșirile din decor.
  * Procesează atacurile jucătorilor, actualizând starea tablei de joc (lovituri reușite, ratări, nave distruse complet) până la determinarea unui câștigător.

### 2. Predictive Analytics & Statistics
* **Descriere**: Un modul de monitorizare a performanței care prelucrează datele meciurilor.
* **Funcționalitate**:
  * Calculează acuratețea individuală a fiecărui meci (lovituri reușite din totalul de lovituri) și acuratețea globală ponderată.
  * Estimează performanța necesară pentru atingerea unor obiective specifice, calculând numărul minim sau maxim de lovituri/meciuri necesare pentru a at
