const int MCTLA = 5;
const int MCTLB = 6;
const int FADERS = 8;
const byte MApins[] = {7,8,9};
const byte MBEN = 10;

void setup(){
  Serial.begin(9600);
  pinMode(MCTLA,OUTPUT);
  pinMode(MCTLB,OUTPUT);
  pinMode(MBEN,OUTPUT);
  digitalWrite(MBEN, HIGH);
  for( int bt=0; bt<sizeof(MApins); bt++ ) {
    pinMode(MApins[bt], OUTPUT);
    Serial.print("pin ");
    Serial.print(MApins[bt]);
    Serial.print("=OUTPUT,");
  }
  Serial.println();
  Serial.println("START");
  
}

void loop(){
  for( int fader=0; fader<FADERS; fader++ ) { 
    Serial.print("Fader ");
    Serial.println(fader);
    for( int bt=0; bt<sizeof(MApins); bt++ ) {
      byte state = bitRead(fader, bt);
      digitalWrite(MApins[bt], state);
      Serial.print("MA");
      Serial.print(bt);
      Serial.print("=");
      Serial.println(state);
    }
    
    digitalWrite(MCTLA,HIGH);   //Pin#7 as High
    digitalWrite(MCTLB,LOW);    //Pin#8 as Low
    delay(400);               //Delay 4 seconds Motor rotates in clock wise direction
    digitalWrite(MCTLA,LOW);    //Pin#7 as Low
    digitalWrite(MCTLB,LOW);    //Pin#8 as Low 
    digitalWrite(MCTLB,HIGH);   //Pin#8 as High
    digitalWrite(MCTLA,LOW);    //Pin#7 as Low
    delay(400);               //Delay for 4 seconds Motor rotates in anti clock wise direction
    digitalWrite(MCTLA,LOW);    //Pin#7 as Low
    digitalWrite(MCTLB,LOW);    //Pin#8 as Low
  }
}
