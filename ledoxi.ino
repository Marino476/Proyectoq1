int lectura=A0;
int senal=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(lectura,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  senal=analogRead(lectura);
  Serial.println(senal);
  if(senal>=700){
    digitalWrite(9,HIGH);
  }
  if(senal<=699){
    digitalWrite(9,LOW);
  }
  delay(30);

}
