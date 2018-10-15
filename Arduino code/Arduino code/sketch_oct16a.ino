// Smart Cane for the Visually Impaired ‐ Arduino Code
const int moist = 6; // Moisture Sensor Pin
bool moisture = LOW; bool moistState = LOW;
const int buz = 9; // Buzzer Pin (PWM)
const int vib = 10; // Vibrator Pin (PWM)
// Ultrasonic Sensor Pins
const int trigPinU = 7;
const int echoPinU = 8;
const int trigPinD = 11;
const int echoPinD = 12;
int dul = 0; // delay variable
float duration; float distanceD; float distanceU;
int volts = 5; float vout = 0.0; float vin = 0.0;
float R1 = 30000.0; float R2 = 7500.0;
const int inOut = 2; // Interrupt Pin for Indoor, Outdoor Switch
float uLim = 116;
float dLim = 100;
void setup() {
pinMode(trigPinU,OUTPUT);
pinMode(echoPinU,INPUT);
pinMode(trigPinD,OUTPUT);
pinMode(echoPinD,INPUT);
pinMode(buz,OUTPUT);
pinMode(vib,OUTPUT);
pinMode(moist,INPUT);
pinMode(inOut,INPUT);
if (digitalRead(inOut) == HIGH){
uLim = 100;
dLim = 50;
}
else{
uLim = 152;
dLim = 100;
}
// InDoor, OutDoor Modes connected to Interrupts
attachInterrupt(digitalPinToInterrupt(inOut),inOutFunc,CHANGE);
Serial.begin(9600);
}
void loop() {
// put your main code here, to run repeatedly:
// Utrasonic Sensor Operation
// 1. Upper ‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
digitalWrite(trigPinU, HIGH);
delayMicroseconds(10);
digitalWrite(trigPinU, LOW);
duration = pulseIn(echoPinU, HIGH);
distanceU = duration*0.034/2;
// 2. Lower ‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
digitalWrite(trigPinD, HIGH);


delayMicroseconds(10);
digitalWrite(trigPinD, LOW);
duration = pulseIn(echoPinD, HIGH);
distanceD = duration*0.034/2;
volts = analogRead(A0);
vout = (volts * 5.0)/1024.0; // see text
vin = vout/(R2/(R1+R2));
// Loop for Battery Check ‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
// Rest of the cane won't work if battery level is insufficient
if (vin < 4){
while (vin < 4){
digitalWrite(buz,HIGH);
delay(1000);
digitalWrite(buz,LOW);
delay(1000);
volts = analogRead(A0);
vout = (volts * 5.0)/1024.0; // see text
vin = vout/(R2/(R1+R2));
}
}
// Condition for Ultrasonic Sensor (Lower) ‐‐‐‐‐‐‐‐‐‐‐‐
if (distanceD < dLim){
if (distanceD > dLim/2){
dul = 200;
}
else{
dul = 100;
}
analogWrite(buz,255);
delay(dul);
analogWrite(buz,0);
delay(dul);
analogWrite(buz,255);
delay(dul);
analogWrite(buz,0);
delay(dul);
}
else
{
digitalWrite(buz,LOW);
}
// Condition for Ultrasonic Sensor (Upper) ‐‐‐‐‐‐‐‐‐‐‐‐
if (distanceU < uLim){
if (distanceU > uLim/2){
dul = 200;
}
else{
dul = 100;
}
analogWrite(vib,123);
delay(dul);
analogWrite(vib,0);
delay(dul);
analogWrite(vib,123);
delay(dul);
analogWrite(vib,0);
delay(dul);
}
else
{
analogWrite(vib,0);
}
// Condition for Water Sensor ‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
moisture = digitalRead(moist);
while (moisture == HIGH){
analogWrite(buz,50);
delay(50);
analogWrite(buz,100);
delay(50);
analogWrite(buz,150);
delay(50);
analogWrite(buz,200);
delay(50);
analogWrite(buz,250);
delay(50);
moisture = digitalRead(moist);
}
}
// Interrupt Function for InDoor OutDoor modes ‐‐‐‐‐‐‐
void inOutFunc(){
if (digitalRead(inOut) == HIGH){
uLim = 100;
dLim = 50;
}
else{
uLim = 152;
dLim = 100;
}
}
