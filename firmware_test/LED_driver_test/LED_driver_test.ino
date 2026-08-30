#define GATE_DELAY 20

#define SER_IN 4
#define SRCK   48
#define RCK    47
const int a_common_pin = 5;


// Accepts a 64-bit integer to safely handle all 40 bits across 5 chips
void b595_write(uint64_t data)
{

    // 1. Shift out the highest byte (Bits 39-32) -> Goes to IC5 (the furthest chip)
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 56) & 0xFF);
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 48) & 0xFF);
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 40) & 0xFF);

    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 32) & 0xFF);

    // 2. Shift out Bits 31-24 -> Goes to IC4
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 24) & 0xFF);

    // 3. Shift out Bits 23-16 -> Goes to IC3
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 16) & 0xFF);

    // 4. Shift out Bits 15-8  -> Goes to IC2
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 8) & 0xFF);
    
    // 5. Shift out the lowest byte (Bits 7-0) -> Stays in IC1 (the closest chip)
    shiftOut(SER_IN, SRCK, MSBFIRST, data & 0xFF);
    
    // 6. Pulse the latch (RCK) AFTER all 40 bits are sent
    // This updates all 5 chips simultaneously
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

    Serial.println("5-chip LED driver initialized");
}

void loop()
{

    for (unsigned int i = 1; i <= 64; i++)
    {
        // Use 1ULL (Unsigned Long Long) so it safely shifts past 32 bits
        uint64_t pattern = (1ULL << (i - 1));
        
        b595_write(pattern);
        Serial.print("Pattern: ");
        Serial.println((uint32_t)(pattern >> 32), BIN); // Print upper bits
        Serial.println((uint32_t)pattern, BIN);        // Print lower bits
        delay(20);
    }
}