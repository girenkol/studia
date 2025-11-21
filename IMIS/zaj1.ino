
const int ledPinClosest = 9;
const int ledPinMiddle = 10;
const int ledPinFurthest = 11;
const int buzzerpwm = 3;
const int sensortrig = 5;
const int sensorecho = 6;
const int button = 2;
const float sensor10 = 10;
const float sensor20 = 20; 
const float sensor30 = 30;

const float pwm255 = 255;
const float pwm0 = 0;

int wartosc_sensor = 0;
int wartosc_sensorout = 0;

float distance_constrain =0;
float distance = 0;
int tryb = 0;
void setup() {
  pinMode(ledPinClosest, OUTPUT);
  pinMode(ledPinMiddle, OUTPUT);
  pinMode(ledPinFurthest, OUTPUT);
  pinMode(buzzerpwm, OUTPUT);
  pinMode(sensortrig, OUTPUT);
  pinMode(sensorecho, INPUT);
  pinMode(button,INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(ledPinFurthest, LOW);
  digitalWrite(ledPinMiddle, LOW);
  digitalWrite(ledPinClosest, LOW);
  attachInterrupt(digitalPinToInterrupt(button),interrupt_button, RISING);

}

void interrupt_button(){
  Serial.print("Zmiana trybu: ");
  if(tryb >= 4)
  {
    tryb = 0;
  }
  else
  {
    tryb+=1;
  }
  Serial.println(tryb);
  delay(250);
}



void loop() {
  switch(tryb)
  {
    case 0:
    {
      tryb_0();
      break;
    }
    case 1:
      tryb_1();
      break;
    case 2:
      tryb_2();
      break;
    case 3:
      tryb_3();
      break;
    case 4:
      tryb_4();
      break;
  }
}

void tryb_0()
{

}

void tryb_1()
{
  distance_constrain = constrain(odleglosc(), 0, 30);
  if
  (distance_constrain > 20 && distance_constrain < 30)
  {
    digitalWrite(ledPinFurthest, HIGH);
    digitalWrite(ledPinMiddle, LOW);
    digitalWrite(ledPinClosest, LOW);
  }
  else if(distance_constrain > 10 && distance_constrain <= 20)
  {
    digitalWrite(ledPinFurthest, HIGH);
    digitalWrite(ledPinMiddle, HIGH);
    digitalWrite(ledPinClosest, LOW);
  }
  else if(distance_constrain <= 10)
  {
    digitalWrite(ledPinFurthest, HIGH);
    digitalWrite(ledPinMiddle, HIGH);
    digitalWrite(ledPinClosest, HIGH);
  }
  else
  {
    digitalWrite(ledPinFurthest, LOW);
    digitalWrite(ledPinMiddle, LOW);
    digitalWrite(ledPinClosest, LOW);
  }


}

void tryb_2()
{
  distance_constrain = constrain(odleglosc(), 0, 30);
  int distance_mm = distance_constrain * 10;
  if
  (distance_constrain > 20 && distance_constrain < 30)
  {
    analogWrite(ledPinFurthest, map(distance_mm,200,300,255.0,0.0));
    digitalWrite(ledPinMiddle, LOW);
    digitalWrite(ledPinClosest, LOW);
  }
  else if(distance_constrain > 10 && distance_constrain <= 20)
  {
    digitalWrite(ledPinFurthest, HIGH);
    analogWrite(ledPinMiddle,  map(distance_mm,100,200,255.0,0.0));
    digitalWrite(ledPinClosest, LOW);
  }
  else if(distance_constrain <= 10)
  {
    digitalWrite(ledPinFurthest, HIGH);
    digitalWrite(ledPinMiddle, HIGH);
    analogWrite(ledPinClosest,  map(distance_mm,0,100,255.0,0.0));
  }
  else
  {
    digitalWrite(ledPinFurthest, LOW);
    digitalWrite(ledPinMiddle, LOW);
    digitalWrite(ledPinClosest, LOW);
  }
  /*Serial.print("odleglosc: ");
  Serial.println(odleglosc());
  Serial.print("odleglosc zmpaowana: ");
  Serial.println(map(odleglosc(),100,200,255.0,0.0));*/
}

void tryb_3()
{
  distance_constrain = odleglosc();
  if (distance_constrain<30 and distance_constrain>=0){
    tone(buzzerpwm, 1000); 
    delay(50);        
    noTone(buzzerpwm);     
    delay((1.0/(31.0-distance_constrain)*1000.0));Serial.println("buzzer 1");Serial.println(distance_constrain);
  }
  else{
    noTone(buzzerpwm);
    //Serial.println("buzzer off");Serial.println(distance_constrain);
    }
  
}

void tryb_4()
{
  distance_constrain = odleglosc();
  if (distance_constrain<30 and distance_constrain>=0)
    {
    tone(buzzerpwm,(1.0/(31.0-distance_constrain)*1000),50); 
    Serial.println("Buzzer on");Serial.println(distance_constrain);
    }
  
  else{
    noTone(buzzerpwm);
    Serial.println("Buzzer off");Serial.println(distance_constrain);
  }
}

float odleglosc(){ //pomiar odległości podany w centymetrach
  digitalWrite(sensortrig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensortrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensortrig, LOW);

  distance = (pulseIn(sensorecho, HIGH)*.0343)/2;
  delay(100);
  return distance;
}  
