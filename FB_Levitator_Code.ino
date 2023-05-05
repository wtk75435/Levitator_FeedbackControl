//reference: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
//ultrasonic: connect VCC and GND pins
//Trig and Echo Pins to digital pins...Trig should be set at 10uS for 8cycle sonic burst. Travel at speed of sound and must go forward and backward



//controller
#define mosfetPin 3 //digital pin 3 of the Arduino nano

 //variables to be input are below:
  int Totallength=100 ; //total distance between ultrasonic sensor and electromagnet in mm
  int Objlength=10 ; //in mm
  int l=Totallength-Objlength; //total distance for object to travel is total distance minus height of object
  int pwmPosition;
  const int ku=(255/100) ; //bias value to multiply the distance reading in mm to PWM's
  int v=0; //this value is increamented by 5%
  double kv=1+v; //value is set to 1, but it will increamentally change with velocity
//sensor
const int trigPin=5; //digital pin 5 and 7 of the Arduino nano
const int echoPin=7;
double duration1; //data from sensor
double distance1; //data from sensor
double duration2;
double distance2;
double tv;
int velocity=(distance2-distance1)/tv; //calculate velocity between measurements

void setup() {
  // put your setup code here, to run once:

  

  Serial.begin(9600);
  
  //controller set up
  pinMode(mosfetPin, OUTPUT);
  Serial.println("MOS activated");

  //sensor set up
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //run US code here first to apprx time for velocity
  //turn on and off the ultrasonic, to read distance1: P0
  long int t1=micros(); //measure time to apprx velocity
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //trig is set high for 10uS, echo pin will state how long it took to receive
  digitalWrite(trigPin, LOW);
  duration1=pulseIn(echoPin, HIGH); //check the time of sound wave reflection
  distance1=duration1*(.34)/2; //this value is used because speed of sound is 340m/s ->.34mm/us, divide by 2 because there and back
  //Serial.print("US Distance:"); Serial.println(distance1); //display distance reading
  long int t2=micros();

  //read distance2: P1
  long int t3=micros();
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //trig is set high for 10uS, echo pin will state how long it took to receive
  digitalWrite(trigPin, LOW);
  duration2=pulseIn(echoPin, HIGH); //check the time of sound wave reflection
  distance2=duration2*(.34)/2; //this value is used because speed of sound is 340m/s ->.34mm/us, divide by 2 because there and back
  //Serial.print("US Distance:"); Serial.println(distance2); //display distance reading
  long int t4=micros();

  long int tv=((t2-t1)+(t4-t3))/2; //average time to run velocity code
  Serial.print("average time to measure d1 to d2 in uS is"); Serial.println(tv);
  int velocity=(distance2-distance1)/tv; //calculate velocity between measurements
  Serial.print("velocity in mm/Us is"); Serial.println(velocity);

  //variables to be input are below:
  int Totallength=100 ; //total distance between ultrasonic sensor and electromagnet in mm
  int Objlength=10 ; //in mm
  int l=Totallength-Objlength; //total distance for object to travel is total distance minus height of object
  const int ku=(255/100) ; //bias value to multiply the distance reading in mm to PWM's
  float v=0; //this value is increamented by 5%
  float kv=1+v; //value is set to 1, but it will increamentally change with velocity

  

}

void loop() {
  // put your main code here, to run repeatedly:
  int n=0; //used for velocity-while loop

  
  //to read distance1: P0
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //trig is set high for 10uS, echo pin will state how long it took to receive
  digitalWrite(trigPin, LOW);
  duration1=pulseIn(echoPin, HIGH); //check the time of sound wave reflection
  distance1=duration1*(.34)/2; //this value is used because speed of sound is 340m/s ->.34mm/us, divide by 2 because there and back

  Serial.print("pwmposition is");Serial.println(pwmPosition);

  pwmPosition=distance1*ku; //holds value of PWM to mosfet based on original position

  analogWrite(mosfetPin, pwmPosition); //electromagnet turns on based on position. This prevents the code from being satisfied at original position or when attached to the electromagnet

  while(n<100000) {
    //this will check velocity 10 times before returing to position measurment
    
  //to read distance1: P0 for velocity measurements
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //trig is set high for 10uS, echo pin will state how long it took to receive
  digitalWrite(trigPin, LOW);
  duration1=pulseIn(echoPin, HIGH); //check the time of sound wave reflection
  distance1=duration1*(.34)/2; //this value is used because speed of sound is 340m/s ->.34mm/us, divide by 2 because there and back

  //to read distance2: p1 for velocity measurements
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //trig is set high for 10uS, echo pin will state how long it took to receive
  digitalWrite(trigPin, LOW);
  duration2=pulseIn(echoPin, HIGH); //check the time of sound wave reflection
  distance2=duration2*(.34)/2; //this value is used because speed of sound is 340m/s ->.34mm/us, divide by 2 because there and back
  if (velocity>1) { //object is moving upward
    v=v-1; //this will be used to slow object down. original value of v is zero
  } else{ 
    v=v+1; //vis versa
  }

  //below will adjust the PWM% output to MosFET based on velocity
  kv=kv+(v*.05); //the 5% can be adjusted for how much "power" velocity has over loop
  pwmPosition=pwmPosition*kv; //the pwm% will go up or down a percentage based on its velocity and original position
  analogWrite(mosfetPin,pwmPosition);

  n++; //loop will close when it hits 10 rotations
  }

  serial.print("velocity loop exited after"); serial.print(millis()); serial.println("milliseconds");
  

}
