long rango=5;
long ultmedicion;
int ciclo=0;
int changeciclo=0;
int picotension;
int valleten;
long contvisual;
long contciclo;

//estado led
boolean estBPM= true;
boolean estled= true;
int intervaloON= 200;
int intervaloOFF= 50;

int intervaloBPM=15000; //TIEMPO OFF DEL LED
int intervaloBPM2= 1;

unsigned long tanteriorON=0;
unsigned long tanteriorOFF=0;
unsigned long tanteriorBPM=0;
unsigned long tanteriorBPM2=0;

int lectura=A0; //cambia por el pin que estes utilizando
int senal=0;

int pulsos=0;
int pulsos2=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(lectura, INPUT);
  //pinMode(12,OUTPUT); //cambiar por el ping que estes utilizando para el led
  //digitalWrite(12,estled); //enciende
  

}

void loop() {
  // put your main code here, to run repeatedly:
  senal=analogRead(lectura);
  if(senal >= (ultmedicion+200)){
    ultmedicion=senal;
    Serial.println("medicion: " + ultmedicion);
    ciclo=1;
    if(senal>picotension){
      picotension=senal;
      Serial.println("pico de tension: " + picotension);
    }
  }

  if(senal <=(ultmedicion-200)){
    ultmedicion=senal;
    Serial.println("medicion: " + ultmedicion);
    ciclo=0;
    if(senal<valleten){
      valleten=senal;
      Serial.println("valle: "+ valleten);
    }  
  }

  //
  if(millis()-tanteriorBPM>=intervaloBPM){
    estBPM=false;
    pulsos2=pulsos;
    tanteriorBPM=millis();
    pulsos=0;
  }

  if(millis()-tanteriorBPM2>=intervaloBPM2){
    estBPM=true;
    pulsos2=(pulsos2+pulsos2)/2;
    Serial.println(pulsos2*4);
    tanteriorBPM2=millis();
    }
//esto es del led
    /*if((millis()-tanteriorON>=intervaloON)&&estled==true && ciclo==0){
      estled=false;
      picotension=senal;
      valleten=senal;
      digitalWrite(12,estled); // colocar el pin del led
      tanteriorOFF=millis();
      
    }

    if((millis()-tanteriorOFF>=intervaloOFF)&&estled==false && ciclo==1){
      pulsos++;
      picotension=senal;
      valleten=senal;
      estled=true;
      digitalWrite(12, estled); //cambiar por el numero de ping que uses 
      tanteriorON=millis();
    }*/
  

}
