# Runic: L-System & Image Processing Engine

## 📌 Despre Proiect
Acest proiect este un motor grafic interactiv dezvoltat în C, specializat în manipularea imaginilor (format PPM), generarea de geometrii fractale și randarea tipografică. Arhitectura se bazează pe procesarea de comenzi în linia de comandă, cu o gestionare avansată a memoriei pentru a susține un sistem complex de stocare a stărilor (Undo/Redo).

## 🚀 Module Implementate

### 1. State Management & Command Interpreter
* Interpretorul analizează și execută secvențial comenzi dintr-un buffer, alocând dinamic memorie pentru structurile de istoric al stărilor (`comanda_undo` și `comanda_redo`).
* Oferă funcționalitate completă de anulare și refacere a acțiunilor (Undo/Redo), operând la nivel de "deep copy" pentru datele imaginii și structurile sistemului la fiecare operațiune modificatoare.

### 2. L-System (Fractal) Processor
* Analizează fișiere structurate pentru a extrage axiome și reguli de producție specifice sistemelor Lindenmayer.
* Generează șiruri fractale complexe prin derivări recursive ale axiomei, realocând dinamic memoria pe parcursul extinderii gramaticii.

### 3. Turtle Graphics & Bresenham Renderer
* Încarcă și prelucrează imagini binare în format PPM (P6) și translatează șirurile fractale în instrucțiuni de mișcare spațială.
* Folosește algoritmul lui Bresenham pentru a calcula și desena linii discrete (rasterizare), gestionând stiva de stări, orientarea carteziană, pașii de deplasare și cei unghiulari.

### 4. Bitmap Typography Engine
* Analizează fișiere de fonturi în format BDF (Glyph Bitmap Distribution Format) și extrage metadatele necesare reprezentării binare (glyph-uri, dimensiuni, offset-uri, bounding box-uri).
* Randează mesaje text direct peste matricea de pixeli a imaginii, mapând fiecare bit al structurii tipografice în spațiul 2D la culoarea RGB cerută.

### 5. Bitstream Error Simulator
* Analizează fluxul brut de date binare (octeții) la nivel de bit pentru a simula atenuări și interferențe de citire specifice hardware-ului de gravură CNC.
* Detectează vulnerabilitățile la nivelul biților (tiparele `0010` și `1101`) direct în memoria alocată și recalculează/raportează coordonatele pixelilor și anomaliile de culoare rezultate.
