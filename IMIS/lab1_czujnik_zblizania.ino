// C++ code
//
const int led_1 = 11; //zmienic na: #define led_1 11
const int led_2 = 10;
const int led_3 = 9;
const int trigPin = 6;
const int echoPin = 5;
float distance;  
const int buzzer = 3;
const int button = 2;
int mode = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), interrupt_button, FALLING);
}

float distance_a(){ //pomiar odległości podany w centymetrach
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  distance = (pulseIn(echoPin, HIGH)*.0343)/2;
  delay(10);
  return distance;
}  


void interrupt_button(){
  Serial.print("Interrupt");
  if(mode == 3){mode = 0;}
  else{mode+=1;}
  //Serial.println(mode);
  //delay(250);
}

void mode_0(){
  noTone(buzzer);
  if (distance_a()<30 and distance_a()>20){
    analogWrite(led_3,255);
    analogWrite(led_2,0);
    analogWrite(led_1,0);Serial.println("led1");
    Serial.println(distance_a());}
  else if (distance_a()<20 and distance_a()>10){
    analogWrite(led_3,255);
    analogWrite(led_2,255);
    analogWrite(led_1,0);Serial.println("led2");Serial.println(distance_a());}
  else if(distance_a()<10 ){
    analogWrite(led_3,255);
    analogWrite(led_2,255);
    analogWrite(led_1,255);Serial.println("led3");Serial.println(distance_a());}
  else{
    analogWrite(led_3,0);
    analogWrite(led_2,0);
    analogWrite(led_1,0);Serial.println("led4");Serial.println(distance_a());}
}

void mode_1(){
  noTone(buzzer);
  if (distance_a()<=30 and distance_a()>=20){
    analogWrite(led_3,map(distance_a(),20,30,255,0));
    analogWrite(led_2,0);
    analogWrite(led_1,0);Serial.println("2led1");Serial.println(distance_a());}
    
  else if (distance_a()<=20 and distance_a()>=10){
    analogWrite(led_3,255);
    analogWrite(led_2,map(distance_a(),10,20,255,0));
    analogWrite(led_1,0);Serial.println("2led2");Serial.println(distance_a());}

  else if(distance_a()<=10 ){
    analogWrite(led_3,255);
    analogWrite(led_2,255);
    analogWrite(led_1,map(distance_a(),0,10,255,0));Serial.println("2led3");Serial.println(distance_a());}

  else{
    analogWrite(led_3,0);
    analogWrite(led_2,0);
    analogWrite(led_1,0);Serial.println("2led4");Serial.println(distance_a());}
}
void mode__1(){
  noTone(buzzer);
  if (distance_a()<=30 and distance_a()>=20){
    analogWrite(led_3,map(distance_a(),20,30,255,0));
    analogWrite(led_2,map(distance_a(),10,20,255,0));
    analogWrite(led_1,map(distance_a(),0,10,255,0));Serial.println("2led1");Serial.println(distance_a());}
  else{
    analogWrite(led_3,0);
    analogWrite(led_2,0);
    analogWrite(led_1,0);Serial.println("2led4");Serial.println(distance_a());}
}

void mode_2(){
  
  if (distance_a()<30 and distance_a()>=0){
    tone(buzzer, 1000); 
    delay(50);        
    noTone(buzzer);     
    delay((1/(31-distance_a())*1000));Serial.println("buzzer 1");Serial.println(distance_a());
  }
  else{
    noTone(buzzer);
    Serial.println("buzzer 2");Serial.println(distance_a());}
}

void mode_3(){
  
  if (distance_a()<30 and distance_a()>=0){
    tone(buzzer,(1/(31-distance_a())*1000),50); 
    Serial.println("2buzzer 1");Serial.println(distance_a());}
  
  else{
    noTone(buzzer);
    Serial.println("2buzzer 2");Serial.println(distance_a());}
}

void loop()
{
  Serial.println(mode);
  if(mode==3){
   mode_3();
  } 
  else if(mode==2){
   mode_2();
  }
  else if(mode==1){
   mode_1();
  }
  else if(mode==0){
   mode_0();
  }
}
