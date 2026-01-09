#include <MIDIUSB.h>

// Portas Analógicas e Digitais
const int PORT_ANALOG[6] = {A0, A1, A2, A3, A4, A5}; 
const int NUM_PIEZOS = 6;
const int PORT_DIGITAL[4] = {2, 3, 4, 5};
const int NUM_DIGITAL = 4;

// Estado dos Sensores e Switches
bool SWITCH_ON[NUM_DIGITAL] = {false};
bool PIEZO_ON[NUM_PIEZOS] = {false};

// Configurações dos Sensores piezo
const int THRESHOLD = 100;
const int SCANTIME = 50; // milliseconds
const int GAIN = 50;
const int CANAL = 1;
const int MASKTIME = 30;
const int RETRIGGER = 30;

// Mapeamento de Notas MIDI
int NOTA_VIRTUAL[PORT_DIGITAL] = {0, 0, 0, 0};
int NOTA[PORT_ANALOG] = {36, 38, 42, 46, 49, 51}; 
int NOTA_SWITCH[PORT_DIGITAL] = {4, 62, 64, 65};
