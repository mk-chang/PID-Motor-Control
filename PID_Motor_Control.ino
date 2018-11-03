/*
 * Encoder example sketch
 * by Andrew Kramer
 * 1/1/2016
 *
 * Records encoder ticks for each wheel
 * and prints the number of ticks for
 * each encoder every 500ms
 * https://www.pololu.com/product/1444
 * http://andrewjkramer.net/motor-encoders-arduino/
 *
 */
 
// pins for the encoder inputs
#define RH_ENCODER_A 3 
#define RH_ENCODER_B 5
#define LH_ENCODER_A 2
#define LH_ENCODER_B 4

//PID Contorl Values
#define t_interval 50
#define Kp 3
 
// variables to store the number of encoder pulses
// for each motor
volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;

unsigned int pwm_R = 150;
unsigned int pwm_L = 150;

int error_L = 0;
 
void setup() {
  pinMode(LH_ENCODER_A, INPUT);
  pinMode(LH_ENCODER_B, INPUT);
  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
  
  // initialize hardware interrupts
  attachInterrupt(0, leftEncoderEvent, CHANGE);
  attachInterrupt(1, rightEncoderEvent, CHANGE);

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel B pin
  //setup Channel A
  pinMode(12, OUTPUT); //Initiates the Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates the Brake channel A pin
  
  Serial.begin(9600);
}
 
void loop() {

  runA(pwm_R, true);
  runB(pwm_L, true);
  delay(t_interval);

  error_L = Kp*(rightCount - leftCount);
  pwm_L = pwm_L - error_L;
}
 
// encoder event for the interrupt call
void leftEncoderEvent() {
  if (digitalRead(LH_ENCODER_A) == HIGH) {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
    }
  } else {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  }

  Serial.print("Left Count: ");
  Serial.println(leftCount);
}
 
// encoder event for the interrupt call
void rightEncoderEvent() {
  if (digitalRead(RH_ENCODER_A) == HIGH) {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  } else {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
    Serial.print("Right Count: ");
    Serial.println(rightCount);
  }
}

void runA(int speed, boolean rev) {  
  if(rev) {
    //Establishes backward direction of Channel A
    digitalWrite(12, LOW);
  }else {
    //Establishes forward direction of Channel A
    digitalWrite(12, HIGH); 
  }
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed);   //Spins the motor on Channel A at full speed
}  

void runB(int speed, boolean rev) {
  if(rev) {
    digitalWrite(13, LOW); //Establishes backwards direction of Channel B
  }else{
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  }
   
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed 
}

//fullstop
void fs(){
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
}
