#define IRPIN 3
#define BTNPIN 2

int buttonState = 0; 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(IRPIN, OUTPUT);
  pinMode(BTNPIN, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BTNPIN), sendIR, LOW);
}

//Method to send signal
void sendIR() 
{
 Serial.print("SHOT\n");
 long ms = 200000;
 cli();  // turn off any background interrupts
 while (ms > 0) 
 {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    digitalWrite(IRPIN, HIGH);  // wait 3 microseconds to happen
    delayMicroseconds(10);      // wait 10 microseconds
    digitalWrite(IRPIN, LOW);   // wait 3 microseconds
    delayMicroseconds(10);      // wait 10 microseconds
 
    ms -= 26;
  }

}
 
// the loop function runs over and over again forever
void loop() {
}
