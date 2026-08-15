#define GATE_DELAY 20

#define SER_IN 4
#define SRCK   48
#define RCK    47

// Updated to accept an unsigned int (16 bits) so it handles both ICs
void b595_write(unsigned int data)
{
    // 1. Shift out the high byte first (this goes past IC1 and ends up in IC2)
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 8) & 0xFF);
    
    // 2. Shift out the low byte (this stays in IC1)
    shiftOut(SER_IN, SRCK, MSBFIRST, data & 0xFF);
    
    // 3. Pulse the latch (RCK) AFTER all 16 bits are sent
    // This makes both ICs update their outputs at the exact same time
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, HIGH);
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, LOW);
}

void setup()
{
    Serial.begin(115200);    
    pinMode(SER_IN, OUTPUT);
    pinMode(SRCK, OUTPUT);
    pinMode(RCK, OUTPUT);

    Serial.println("LED driver initialized");

}

void loop()
{
    for (unsigned int i = 0; i <= 256; i++)
    {
        b595_write(i);
        Serial.print("Pattern: ");
        Serial.println(i);
        delay(1000);

        
    }
}