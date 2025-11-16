# SmartSensorSystem
Det här är mitt projekt i C++, där jag har skapat ett system för att samla in, spara och analysera mätvärden från flera olika sensorer. Projektet gjorde jag som en del av kursen i objektorienterad programmering, med fokus på arv, polymorfi, datastrukturer och programflöde.

# Funktioner i programmet

Programmet kan utföra följande:

✔ Samla in mätdata automatiskt från tre sensorer:
 • Temperatur
 • Luftfuktighet
 • Lufttryck

✔ Visa statistik för valfri sensor (min, max, medelvärde)
✔ Visa alla insamlade mätvärden
✔ Konfigurera tröskelvärden för sensorer och skapa larm
✔ Visa larmhistorik
✔ Spara mätvärden till fil (CSV-liknande format)
✔ Läsa in mätvärden från fil
✔ 🔹 Visa histogram för sensorvärden (VG-nivå)
✔ 🔹 Söka mätvärden efter sensor och datumintervall (VG-nivå)

# Arkitektur och design

Projektet använder arv och polymorfi där alla sensorer delar samma abstrakta gränssnitt.

Klass	Ansvar
Sensor	Abstrakt basklass för alla sensorer
TemperatureSensor	Genererar simulerade temperaturvärden
HumiditySensor	Genererar simulerade luftfuktighetsvärden
PressureSensor	Genererar simulerade lufttrycksvärden
Measurement	Struct som lagrar mätvärde och tidsstämpel
Threshold	Struct som innehåller gränsvärde och larmvillkor
Alert	Struct som sparar triggat larm
SystemController	Samordnar hela systemet: insamling, statistik, filhantering & sökning
SmartSensorSystem.cpp	Main – meny och användarinteraktion

Sensorobjekten lagras i SystemController genom:

std::vector<std::unique_ptr<Sensor>>


vilket möjliggör polymorf hantering av alla sensorer.


# Projektstruktur
.
├── sensor.h
├── temperature_sensor.h / .cpp
├── humidity_sensor.h / .cpp
├── pressure_sensor.h / .cpp
├── measurement.h
├── system_controller.h / .cpp
├── SmartSensorSystem.cpp
└── README.md   ← den här filen

# Så körs programmet
1️⃣ Visual Studio

Öppna projektet

Bygg med Ctrl + Shift + B

Kör programmet med F5

2️⃣ Kompilator (g++)
g++ -std=c++17 *.cpp -o SmartSensorSystem
./SmartSensorSystem

# Så används programmet

När programmet startar visas en meny:

1. Sample all
2. Show stats for sensor
3. Show all measurements
4. Configure threshold
5. Show alerts
6. Save to file
7. Load from file
8. Show histogram
9. Search measurements
0. Exit


Du väljer en funktion genom att skriva en siffra och trycka Enter.

# Exempel på användning
Choice: 1   ← läser in mätvärden från alla sensorer
Choice: 1
Choice: 3   ← skriver ut alla tidigare mätningar
2025-11-16 14:20:45 | 21.4 | Temp1
2025-11-16 14:20:45 | 46.8 | Hum1
2025-11-16 14:20:45 | 1002.7 | Press1

# Statistik som beräknas
Statistik	Beskrivning
Min	Lägsta uppmätta värdet
Max	Högsta uppmätta värdet
Medel	Medelvärde av alla mätningar

Histogramfunktion (menyval 8) visar hur många värden som ligger inom olika intervall.

Sökningsfunktionen (menyval 9) filtrerar mätningar baserat på:

sensor

startdatum

slutdatum

# Lärdomar

Det här projektet har lärt mig mycket om:

🔹 hur arv och polymorfi kan användas för att skapa flexibla system
🔹 hur man lagrar och analyserar data i C++ med STL-containrar
🔹 hur system kan struktureras med ansvar i separata klasser
🔹 hur man arbetar med tid, slumpvärden och filhantering
🔹 hur viktig robust menylogik och användarvänlighet är

Jag upplevde att när programmet växte blev organisationsprinciper som “separation of concerns” och tydlig klassdesign väldigt viktiga.

# Sammanfattning

Smart Sensor System är ett fullständigt C++-program som kombinerar:

objektorienterad programmering

datainsamling

filhantering

statistik

menyer och interaktiv konsol

# Mitt GitHub-repo
https://github.com/hazaralmasri98-tech/SmartSensorSystem.git

# Avslutande 
Jag är nöjd med resultatet och tycker att projektet hjälpte mig att fördjupa min förståelse för C++ och objektorienterad systemutveckling.
Tveka inte att kontakta mig om du har frågor eller feedback!


# Avslutande 
Jag är nöjd med resultatet och tycker att projektet hjälp mig att fördjupa min förståelse för C++ och objektorienterad systemutveckling.
Tveka inte att kontakta mig om du har frågor eller feedback!
