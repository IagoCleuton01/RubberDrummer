#include <MIDIUSB.h>

// Piezo sensor pins (Analog)
const int PIEZO_PINS[6] = {A0, A1, A2, A3, A4, A5};
const int NUM_PIEZOS = 6;

// Switch pins (Digital)
const int SWITCH_PINS[4] = {2, 3, 4, 5};
const int NUM_SWITCHES = 4;

// MIDI settings
const int MIDI_CHANNEL = 1;
const int MIDI_VELOCITY = 100;

// Piezo threshold to trigger note
const int THRESHOLD = 100;
const int SCANTIME = 50; // milliseconds

// Note mapping for piezos (MIDI notes)
int piezoNotes[NUM_PIEZOS] = {36, 38, 42, 46, 49, 51}; // C1, D1#, F#1, A#1, C#2, D#2

// Note mapping for switches
int switchNotes[NUM_SWITCHES] = {60, 62, 64, 65}; // C4, D4, E4, F#4

// State tracking
bool piezoPreviouslyHit[NUM_PIEZOS] = {false};
bool switchPreviouslyPressed[NUM_SWITCHES] = {false};
unsigned long lastPiezoTime[NUM_PIEZOS] = {0};
unsigned long lastSwitchTime[NUM_SWITCHES] = {0};

void setup() {
    Serial.begin(115200);
    
    // Initialize switch pins as input with pull-up
    for (int i = 0; i < NUM_SWITCHES; i++) {
        pinMode(SWITCH_PINS[i], INPUT_PULLUP);
    }
    
    Serial.println("Electronic Drum Pad - MIDI Mode Initialized");
}

void loop() {
    // Check piezo sensors
    checkPiezos();
    
    // Check switches
    checkSwitches();
    
    delay(10);
}

void checkPiezos() {
    for (int i = 0; i < NUM_PIEZOS; i++) {
        int sensorValue = analogRead(PIEZO_PINS[i]);
        
        // Debounce check
        if (millis() - lastPiezoTime[i] < DEBOUNCE_DELAY) {
            continue;
        }
        
        // Detect hit
        if (sensorValue > PIEZO_THRESHOLD && !piezoPreviouslyHit[i]) {
            piezoPreviouslyHit[i] = true;
            lastPiezoTime[i] = millis();
            
            // Send MIDI Note On
            noteOn(MIDI_CHANNEL, piezoNotes[i], MIDI_VELOCITY);
            MidiUSB.flush();
            
            Serial.print("Piezo");
            Serial.print(i);
            Serial.print(" hit - Note: ");
            Serial.println(piezoNotes[i]);
        }
        
        // Release detection
        if (sensorValue < PIEZO_THRESHOLD && piezoPreviouslyHit[i]) {
            piezoPreviouslyHit[i] = false;
            
            // Send MIDI Note Off
            noteOff(MIDI_CHANNEL, piezoNotes[i], 0);
            MidiUSB.flush();
        }
    }
}

void checkSwitches() {
    for (int i = 0; i < NUM_SWITCHES; i++) {
        bool buttonPressed = digitalRead(SWITCH_PINS[i]) == LOW;
        
        // Debounce check
        if (millis() - lastSwitchTime[i] < DEBOUNCE_DELAY) {
            continue;
        }
        
        // Button press detection
        if (buttonPressed && !switchPreviouslyPressed[i]) {
            switchPreviouslyPressed[i] = true;
            lastSwitchTime[i] = millis();
            
            noteOn(MIDI_CHANNEL, switchNotes[i], MIDI_VELOCITY);
            MidiUSB.flush();
            
            Serial.print("Switch ");
            Serial.print(i);
            Serial.print(" pressed - Note: ");
            Serial.println(switchNotes[i]);
        }
        
        // Button release detection
        if (!buttonPressed && switchPreviouslyPressed[i]) {
            switchPreviouslyPressed[i] = false;
            
            noteOff(MIDI_CHANNEL, switchNotes[i], 0);
            MidiUSB.flush();
        }
    }
}

void noteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, 0x90 | (channel - 1), pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, 0x80 | (channel - 1), pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}