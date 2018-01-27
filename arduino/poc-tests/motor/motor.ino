int m1 = 6;           //Direction control pins
int m2 = 7;           //Direction control pins

void setup(){
  pinMode(m1,OUTPUT); //Arduino Pin#7 as Output 
  pinMode(m2,OUTPUT); //Arduino Pin#8 as Output 
}

void loop(){
  digitalWrite(m1,HIGH);   //Pin#7 as High
  digitalWrite(m2,LOW);    //Pin#8 as Low
  delay(400);               //Delay 4 seconds Motor rotates in clock wise direction
  digitalWrite(m1,LOW);    //Pin#7 as Low
  digitalWrite(m2,LOW);    //Pin#8 as Low 
  digitalWrite(m2,HIGH);   //Pin#8 as High
  digitalWrite(m1,LOW);    //Pin#7 as Low
  delay(400);               //Delay for 4 seconds Motor rotates in anti clock wise direction
  digitalWrite(m1,LOW);    //Pin#7 as Low
  digitalWrite(m2,LOW);    //Pin#8 as Low
}
