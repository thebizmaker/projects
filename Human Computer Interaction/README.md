# Gym-App

## Übersicht
Dieses Projekt ist eine Webanwendung, die mit React und Material-UI entwickelt wurde. Es umfasst verschiedene Komponenten für die Benutzerinteraktion, Datenvisualisierung und Zustandsverwaltung.

## Repository klonen
```bash
  git clone git@code.fbi.h-da.de:hci_group7/gym_app.git
   ```

# Abhängigkeiten installieren und Starten
```bash
  npm install
  npm run dev
   ```

## Nutzung
### Anwendung ausführen
Öffnen Sie Ihren Browser und navigieren Sie zu http://localhost:5173/.

## Systemarchitektur
### Frontend

#### React
- Die Benutzeroberfläche wird mit React entwickelt, einer JavaScript-Bibliothek für den Aufbau von Benutzeroberflächen.

#### Material-UI (MUI)
- Für das Styling der Komponenten wird Material-UI verwendet.

### Zustandsverwaltung

#### Context API
- Die Anwendung verwendet die Context API von React, um den Zustand von Übungen und anderen Daten über verschiedene Komponenten hinweg zu verwalten.
- Die `ExerciseProvider`-Komponente umschließt die Anwendung, um die Zustandsverwaltung bereitzustellen.

### Datenspeicherung

#### localStorage
- Trainingsdaten werden in `localStorage` gespeichert, um über Sitzungen hinweg erhalten zu bleiben.
- Die Daten umfassen abgeschlossene Workouts, die in verschiedenen Komponenten abgerufen und manipuliert werden.

### Theming

#### ThemeProvider
- Das Thema wird in `src/theme.js` definiert und mit `ThemeProvider` angewendet, um ein konsistentes Styling in der gesamten Anwendung zu gewährleisten.

### Routing

#### React Router
- Für die Navigation innerhalb der Anwendung wird React Router verwendet.

### Zustandsverwaltung
Die Anwendung verwendet die Context API von React, um den Zustand von Übungen und anderen Daten über verschiedene Komponenten hinweg zu verwalten. Die ExerciseProvider-Komponente umschließt die Anwendung, um die Zustandsverwaltung bereitzustellen.

### Styling
Material-UI (MUI) wird für das Styling der Komponenten verwendet. Das Thema wird in src/theme.ts definiert und mit ThemeProvider angewendet.

### Datenspeicherung
Trainingsdaten werden in localStorage gespeichert, um über Sitzungen hinweg erhalten zu bleiben. Die Daten umfassen abgeschlossene Workouts, die in verschiedenen Komponenten abgerufen und manipuliert werden.


# **Release Notes - Gym App**

### Version 2.0.0 - Initial Release

- **Date:** 2025-02-04
- **Responsible:** Multiple Contributors

#### Changes:
- Aktualisierung der README-Datei *(Jan Beckmann)*
- Entfernen der Ticks auf dem Power-Chart *(Auriand Dester Ngouateu Kemma)*
- Refactoring des `SuggestionBox`-Layouts und Aktualisierung des `localStorage`-Keys *(Dogukan Tanir)*
- Verbesserung der Heatmap mit Intensitätsberechnung und Anzeige *(Jan Beckmann)*
- Verschiedene Änderungen *(Joshua Schmidt)*

### Version 1.3.0- Feature Update

- **Date:** 2025-02-03
- **Responsible:** Joshua Schmidt

#### Changes:
- Hinzufügen einer Checkbox zu Sets und Ergänzung der Workout-Dauer *(Joshua Schmidt)*

### Version 1.2.0 - Bug Fixes and Improvements

- **Date:** 2025-01-31
- **Responsible:** Multiple Contributors

#### Changes:
- Echtzeit-Datenbindung für Diagramme abgeschlossen *(Auriand Dester Ngouateu Kemma)*
- Dauer in Stunden zu Dummy-Daten hinzugefügt *(Joshua Schmidt)*
- Fehlerbehebungen an zusammengeführtem Code *(Joshua Schmidt)*
- Behebung von Merge-Konflikten *(Joshua Schmidt)*
- Speicherung von `completedWorkouts` gefixt *(Joshua Schmidt)*
- Aktualisierung der Routinen-Seite mit Favoriten *(Blasius Fornge Afanyi)*
- Refactoring des Statistik-Moduls und Implementierung der Entscheidungsunterstützung *(Dogukan Tanir)*

### Version 1.1.0- Additional Enhancements

- **Date:** 2025-01-28
- **Responsible:** Multiple Contributors

#### Changes:
- Speicherung von `completedWorkouts` und Hinzufügen eines Datums zu Workouts *(Joshua Schmidt)*
- Fertigstellung der Statistik-Bindung *(Auriand Dester Ngouateu Kemma)*
- Erstellung einer Einstellungsseite *(Auriand Dester Ngouateu Kemma)*
- Berechnung des durchschnittlichen Gewichts und Einführung einer persönlichen Vorstellung *(Dogukan Tanir)*
- Generierung von Trainingsdaten und Einführung der sitzungsbasierten Speicherung *(Jan Beckmann)*

### Version 1.0.0 - Initial Commit

- **Date:** 2025-01-11
- **Responsible:** Joshua Schmidt



## **Commits**

### **Februar 04, 2025**
- **[Jan Beckmann]**
    - Aktualisierung der README-Datei
    - Commit: `a8a7c367`

- **[Auriand Dester Ngouateu Kemma]**
    - Entfernen der Ticks auf dem Power-Chart
    - Commit: `7b66ac47`

- **[Jan Beckmann]**
    - Merge des Branches `heatmapfix` in `main`
    - Commit: `50f0245f`

- **[Dogukan Tanir]**
    - Refactoring des `SuggestionBox`-Layouts
    - Aktualisierung des `localStorage`-Keys für Trainingsdaten
    - Commit: `f9f85715`

- **[Jan Beckmann]**
    - Verbesserung der Heatmap mit Intensitätsberechnung und optimierter Anzeige
    - Commit: `bac93f65`

- **[Joshua Schmidt]**
    - Verschiedene Änderungen
    - Commit: `9faca415`

### **Februar 03, 2025**
- **[Joshua Schmidt]**
    - Merge-Vorgang durchgeführt
    - Commit: `531fa14f`
    - Hinzufügen einer Checkbox zu Sets
    - Ergänzung der Workout-Dauer
    - Commit: `e1cdb396`

### **Januar 31, 2025**
- **[Auriand Dester Ngouateu Kemma]**
    - Echtzeit-Datenbindung für Diagramme abgeschlossen
    - Commit: `c2629ae1`

- **[Joshua Schmidt]**
    - Dauer in Stunden zu Dummy-Daten hinzugefügt
    - Commit: `974bf2b2`
    - Fehlerbehebungen an zusammengeführtem Code
    - Commit: `e924e855`

### **Januar 29, 2025**
- **[Joshua Schmidt]**
    - Behebung von Merge-Konflikten
    - Commit: `c5812ad7`
    - Speicherung von `completedWorkouts` gefixt
    - Commit: `bd33d378`

- **[Blasius Fornge Afanyi]**
    - Aktualisierung der Routinen-Seite mit Favoriten
    - Commit: `5c7c8a63`
    - Weitere Änderungen an der Routinen-Seite
    - Commit: `b161b0b4`

- **[Jan Beckmann]**
    - Merge des `heatmap`-Branches in `main`
    - Commit: `7fed446f`

- **[Jan Beckmann]**
    - Hinzufügen des `Heatmap`-Moduls und Integration in `WorkoutDetails`
    - Commit: `41a2c0cb`

- **[Dogukan Tanir]**
    - Refactoring des Statistik-Moduls
    - Implementierung einer Entscheidungsunterstützung mit Empfehlungen
    - Commit: `9e5be2d0`

### **Januar 28, 2025**
- **[Joshua Schmidt]**
    - Speicherung von `completedWorkouts`
    - Hinzufügen eines Datums zu Workouts
    - Commit: `614b0fd6`

### **Januar 25, 2025**
- **[Auriand Dester Ngouateu Kemma]**
    - Fertigstellung der Statistik-Bindung
    - Erstellung einer Einstellungsseite (ohne Design)
    - Commit: `4aaa51da`

### **Januar 24, 2025**
- **[Dogukan Tanir]**
    - Berechnung des durchschnittlichen Gewichts
    - Einführung einer persönlichen Vorstellung
    - Commit: `0d3e3788`

### **Januar 12, 2025**
- **[Jan Beckmann]**
    - Generierung von Trainingsdaten
    - Einführung von Sitzungsbasierter Speicherung
    - Commit: `9a0af807`

### **Januar 11, 2025**
- **[Joshua Schmidt]**
    - Erster Commit des Projekts
    - Commit: `52f59860` 