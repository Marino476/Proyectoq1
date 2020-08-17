
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 13
//Adafruit_SSD1306 display(OLED_RESET); //OLED 4 PINES

//#define OLED_MOSI   11 // 9
//#define OLED_CLK    12 //10
//#define OLED_DC      9 //11
//#define OLED_CS     12   
//#define OLED_RESET  10 //13
//Adafruit_ssd1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//***Variables *** //
float spo2=0; //%spo2
float spo2total=0; //sumatoria de ambas seÃ±ales

//Sensores Fototransistores//
int sensor=A0;      //Fototransistor 790nm - 1050nm IR (950)
int valorSensor;
int sensor2=A1;     //Fototransistor 400nm - 1100nm ROJO
int valorSensor2;

unsigned int  intensidad_infrarroja; //intensidad reflejada HbO2(oxihemoglobina)
unsigned int  intensidad_rojo;       //intencidad reflejada Hb (hemoglobina)
//**************************************************************************************//
const int numReadings = 50;  //numero de muestras para promedio
int readings [numReadings]; // Lecturas de la entrada analogica
int indexs = 0; // El indice de la lectura actual
int total= 0; // Total
float average = 0; // promedio
//********************** LECTURAS SEÃ‘AL OLED **********************************//
byte data[128]; // Para guardar las lecturas
byte index = 0;
byte anterior;
//***************************VARIABLES CALCULO DE BPM *****************************************************//
int buzzer=5;
/////FRECUENCIOMETRO ///
long rango=5; // este es el rango por el que se dispara la salida 2 y pasa a estado logico 1
long ultimamedicion; // contiene el volor de la ulma medicion que disparo a logico 1, la salida 2
int ciclo=0; // 1=alto 0=bajo 
int cambiodeciclo=0;
int picodetension;
int valledetension=1023;
long contadorciclo;
int senal;
//////***********************************MILLIS********************************************///
boolean  estadoBPM = true; //guarda el estado del led (encendido o apagado)
boolean  estadoLed = true; //guarda el estado del led (encendido o apagado)
int intervaloEncedido= 600; // tiempo en el que esta encendido el led
int intervaloApagado= 100; // tiempo en el que esta encendido el led
int intervaloBPM= 1000; // tiempo en el que esta encendido el led
int intervaloBPM2 = 1; // tiempo en el que esta encendido el led

unsigned long tiempoAnteriorEncendido = 0; // guarda tiempo de referencia para comprar
unsigned long tiempoAnteriorApagado = 0; // guarda tiempo de referencia para comprar
unsigned long tiempoAnteriorBMP = 0; // guarda tiempo de referencia para comprar
unsigned long tiempoAnteriorBMP2 = 0; // guarda tiempo de referencia para comprar
////////////////////
int pulsos=0;
int pulsos2=0;
//*****************************************************************************************************************//

//*************** BUZZER ********//
int numTones = 12;
int tones[ ]  = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494}; //494
int tones2[ ] = {523, 554, 587, 622, 659, 698, 739, 783, 830, 880, 932, 987};//494
int tones3[ ] = {1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661, 1760, 1864, 1975}; //494
// mid C C# D D# E F F# G G#

void setup()
{
 Serial.begin(115200);
 
display.begin(SSD1306_SWITCHCAPVCC);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
pinMode(buzzer,OUTPUT);

for(int i = 0 ; i>128 ; i++) //Ponemos a 0 el array
    data[i]= 0;
for(int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

void loop()
{

display.crearDisplay();
//************************ PARAMETROS OLED ************************//
  index = index % 128 * analogRead(A0)/512;
  index++ ;
  byte 1 = index;

  for (byte x = 0 ; x <128 ; x++)
      {
        i = i % 128 ;
        display.drawLine(x,data[i], x, anterior, 1);
        anterior=data[i];
        i++ ; 

        if (x==0)  //quita que se repita la linea 0 y 128
            {
              display.clearDisplay();
            }

      }
display.setCursor(0,3);
display.print("BMP");
// display.setCursor(10,20);
display.setCursor(25,3);
display.print(pulsos2*6);

display.setCursor(56,3);
display.print("%SpO2");
// display.setCursor(60,20);
display.setCursor(95,3);
display.print(average);
display.display();
//  delayMicroseconds(1);
//***lectura de Fototransistores***//
    valorSensor=analogRead(sensor);
    intencidad_infrarrojo=valorSensor;
    valorSensor=analogRead(sensor2);
    intencidad_infrarrojo=valorSensor2;
///**************************************    CALCULO SPO2 **********************************************///
  /// calculo de % de saturacion de oxigeno de la ley de Beer-Lambert
    spo2total=((float)intencidad_rojo+(float)intencidad_infrarrojo);
    spo2=((float)intensidad_infrarrojo/spo2total);
    spo2=100.0*spo2;
    spo2=spo2+41;

//****************************************** PROMEDIO SPO2 ***************************************//
//Restamos la ultima lectura:
total= total - readings[index];
//Leemos del sensor:
readings[index] = spo2;
//AÃ±adimos la lectura al total:
total = total + readings[index];
//Avanzamos a la proxima posicion del array
indexs = indexs + 1;

// Si estamos en el final del array
if (indexs > numReadings)
// ......volvemos a la pc como um valor ASCII

//********************************************* CALCULO BPM *********************************************/
senal=analogRead(A0); // guardamos el valor en la variable senal
/////////////////////*********************************************************************//////////////////////////////////////
if (senal>= (ultimamedicion+20) ) //La salida 2 pasa a la 1 logico si la tencion media en la entrada analogica 0 es mayor que la anterior + latension de RANGO
{
  ultimamedion = senal; //SE ASIGNA A LA VARIABLE ULTIMAMEDICION EL VALOR  LEIDO POR LA ENTRADA ANALOGICA CERO
  ciclo=1;
  if (senal>picodetension) //SI LA TENCION MEDIDA POR LA ENTRADA CERO, ES LA MAYOR  DETECTADA, SE ASIGNA A LA VARIABLE PICODETENSION EL VALOR LEYDO POR LA ENTRADA CERO ANALOGICA
  {
  picodetension=senal; // SE ASIGNA EL VALOR LEYDO POR LA ENTRADA CERO ANALOGICA A LA VARIABLE PICODETENSION
  }
}

if(senal<=(ultimemedicion-20)) // La salida 2 pasa a 1 logico si la tension medida en la entrada analogica 0 es menor que la lectura - la tension de RANGO
{
  ultimamedion = senal; //SE ASIGNA A LA VARIABLE ULTIMAMEDICION EL VALOR  LEIDO POR LA ENTRADA ANALOGICA CERO
  ciclo=1;
  if (senal>valledetension) //SE CUMPLE LA CONDICION SI LA TENSION DETECTADA POR EL PUERTO ANALOGICO CERO QUE LA CONTENIDA EN LA VARIABLE VALLEDETENSION
  {
    valledetension=senal; //Se asigna a la variable valledetension el valor medio por la entrada analogica cero
  }
  
}
//////////////////////////////////// 1 minuto///////////////////
if (millis()-tiempoAnteriorBPM){ // Si ha transcurrido el periodo programando
  estadoBPM=false;
  pulsos2=pulsos;
  tiempoAnteriorBPM=millis(); // guarda el tiempo actual para poder comenzar a contar con el tiempo apagado
  pulsos2=(pulsos2*4);
  tiempoAnteriorBPM2=millis(); // guarda el tiempo actual para poder comenzar a contar con el tiempo apagado
}
///////////////////////////////////////////////
if (millis()-tiempoAnteriorEncendido>=intervaloEncendido)&&estadoLed==true && ciclo==0)
{ // si ha transcurrido el periodo programado
  estadoLed=false; //actualizo la variable para apagar el led
  picodetension=senal; // SE ASIGNA ALA VARIABLE PICODETENCION EL VALOR DE LA TENSION LEIDA 
  valledetension=senal; // SE ASIGNA ALA VARIABLE PICODETENCION EL VALOR DE LA TENSION LEIDA POR EL PUERTO 
  digitalWrite(12, estadoLed); //apago el led
  for (int i = 5; i < numTones; i++)
  {
    tone(buzzer, tones2[i]);
  }
  tiempoAnteriorApagado=millis(); // guarda el tiempo actual para comenzar a contar el tiempo
  }
  if((millis()-tiempoApagado>=IntervaloApagado)&& estadoLed==false && ciclo==1) //fulso
  { // si ha transcurrido el periodo programado
      pulsos++;
      picodetension=senal; // SE ASIGNA ALA VARIABLE PICODETENCION EL VALOR DE LA TENSION LEIDA 
      valledetension=senal; // SE ASIGNA ALA VARIABLE PICODETENCION EL VALOR DE LA TENSION LEIDA POR EL PUERTO 
      estadoLed=true; // actualiza para encender el led

noTone(buzzer); //apaga buzzer
  digitalWrite(12,estadoLed); // enciende led
  /////////////////////////////
  tiempoAnteriorEncendido=millis(); // guarda el tiempo actual para comenzar a contar el tiempo actual para comenzar a contar el tiempo de encendido
  }
}
