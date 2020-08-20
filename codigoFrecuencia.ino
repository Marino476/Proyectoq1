

//codigo que lee los latidos en un minuto
int lectura=A0;
int senal=0;
int a=0;
int cont=0;
int b=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(lectura,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
 if(cont==900){
  Serial.println("frecuencia cardiaca");
  delay(2000);
  Serial.print("                       Latidos ");
  delay(800);
  Serial.print("por ");
  delay(800);
  Serial.println("minuto ");
  delay(800);
  Serial.println(a);
  delay(1000);
  Serial.println("------------------------------");
  delay(7000);
  cont=0;
  a=0;
 }
 if(b==1 || cont==0 ||b==0 ||senal>0)
 {
  cont=cont+1;
  Serial.print("contador: ");
  Serial.print(cont);
  
 }

 senal=analogRead(lectura);
 Serial.print("    valor: ");
 Serial.print(senal);

if(senal >=660){
  a=a+1;
  b=1;
}
Serial.print("                  latidos: ");
Serial.println(a);
delay(50);

}
