//because transistors inside IC take a nonzero time to change states
#define GATE_DELAY 20

#define SER_IN 4
#define SRCK   48
#define RCK    47

void b595_write(byte bits)
{
    shiftOut(SER_IN, SRCK, LSBFIRST, bits);
    //what does shiftOut do?

    /*1. writes 8 bits to the data pin
    //2. writes clock high then low (signaling shift register to read the data)
    */
  
    // After the clock pulse, the shift register has recognized the data
    // Must pulse the latch (RCK) to output the byte
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, HIGH);
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, LOW);
}

void setup()
{
    //the clocks signals and input signals are receiving data
    pinMode(SER_IN, OUTPUT);
    pinMode(SRCK, OUTPUT);
    pinMode(RCK, OUTPUT);
}

void loop()
{
    //will light up each LED on the board for 0.5 seconds
    for (int i=0; i<16; i++)
    {
        b595_write(i);
        delay(500);
    }
}