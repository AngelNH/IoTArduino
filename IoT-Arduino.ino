/*-------------VARIABLES-------------*/
//Volumen
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion= 7.5;//7.11; //para convertir de frecuencia a caudal
float volumen=0;
long dt=0; //variación de tiempo por cada bucle
long t0=0; //millis() del bucle anterior

// VOLUMEN FUNCTION
//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()  
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuecia() 
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}

//PH Sensor
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

//COLOR
//
// Cableado de TCS3200 a Arduino
//
#define S0 8
#define S1 9
#define S2 12
#define S3 11
#define salidaSensor 10
 
// Para guardar las frecuencias de los fotodiodos
int frecuenciaRojo = 0;
int frecuenciaVerde = 0;
int frecuenciaAzul = 0;
int colorRojo;
int colorVerde;
int colorAzul;
 

void setup() 
{ 
  
  Serial.begin(9600); 
  pinMode(PinSensor, INPUT); 
  attachInterrupt(0,ContarPulsos,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  //Serial.println ("Envie 'r' para restablecer el volumen a 0 Litros"); 
  t0=millis();
  //COLOR SETUP
  // Definiendo las Salidas
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Definiendo salidaSensor como entrada
  pinMode(salidaSensor, INPUT);
  
  // Definiendo la escala de frecuencia a 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  
} 
float prom = 0.0;
void loop ()    
{
  /*-----------------------------------------------------------*/
  /*----------------------VOLUME-------------------------------*/
  /*-----------------------------------------------------------*/
  if (Serial.available()) {
    //if(Serial.read()=='r')volumen=0;//restablecemos el volumen si recibimos 'r'
  }
  float frecuencia=ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  prom = (prom + caudal_L_m)/2;
  //prom = sqrt(prom + caudal_L_m);
  dt=millis()-t0; //calculamos la variación de tiempo
  t0=millis();
  //Serial.print ("Caudal PROMEDIO: "); 
  //Serial.print (prom,3);
  volumen=volumen+(prom/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)
  
  //volumen=volumen+(caudal_L_m/60)*(dt/1000); // volumen(L)=caudal(L/s)*tiempo(s)

   //-----Enviamos por el puerto serie---------------
//  Serial.print ("\tCaudal: "); 
//  Serial.print (caudal_L_m,3);
//  Serial.print ("L/min \tMilitros/s ");
float caudal_Ml_s= caudal_L_m / 0.06;
//  Serial.print (caudal_Ml_s,3);
//  Serial.print ("\tVolumen: "); 
//  Serial.print (volumen,3); 
//  Serial.println (" L");
  /*-----------------------------------------------*/
  /*-------------------------PH--------------------*/
  /*------------------------------------------------*/
  for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
// Serial.print("sensor = ");
// Serial.println(phValue);

// delay(20);
 /*------------------------------------------------*/
 /*--------------------COLOR-----------------------*/
 /*------------------------------------------------*/
 // Definiendo la lectura de los fotodiodos con filtro rojo
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Leyendo la frecuencia de salida del sensor
  frecuenciaRojo = pulseIn(salidaSensor, LOW);
 
  // Mapeando el valor de la frecuencia del ROJO (RED = R) de 0 a 255
  // Usted debe colocar los valores que registro. Este es un ejemplo: 
  // colorRojo = map(frecuenciaRojo, 70, 120, 255,0);
  colorRojo = map(frecuenciaRojo, 70, 120, 255,0);
  
  // Mostrando por serie el valor para el rojo (R = Red)
//  Serial.print("R = ");
//  Serial.print(colorRojo);
  delay(100);
  
  // Definiendo la lectura de los fotodiodos con filtro verde
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Leyendo la frecuencia de salida del sensor
  frecuenciaVerde = pulseIn(salidaSensor, LOW);
 
  // Mapeando el valor de la frecuencia del VERDE (GREEN = G) de 0 a 255
  // Usted debe colocar los valores que registro. Este es un ejemplo: 
  // colorVerde = map(frecuenciaVerde, 100, 199, 255,0);
  colorVerde = map(frecuenciaVerde, 100, 199, 255,0);
 
  // Mostrando por serie el valor para el verde (G = Green)
//  Serial.print("G = ");
//  Serial.print(colorVerde);
  delay(100);
 
  // Definiendo la lectura de los fotodiodos con filtro azul
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Leyendo la frecuencia de salida del sensor
  frecuenciaAzul = pulseIn(salidaSensor, LOW);
 
  // Mapeando el valor de la frecuencia del AZUL (AZUL = B) de 0 a 255
  // Usted debe colocar los valores que registro. Este es un ejemplo: 
  // colorAzul = map(frecuenciaAzul, 38, 84, 255, 0);
  colorAzul = map(frecuenciaAzul, 37, 85, 255, 0);
  
//  // Mostrando por serie el valor para el azul (B = Blue)
//  Serial.print("B = ");
//  Serial.print(colorAzul);
  delay(100);

  //Variables finales a enviar
  // SEND VOLUMEN: volumen
  // SEND PH: phValue
  // SEND COLOR RGB: (colorRojo, colorVerde, colorAzul)
  Serial.print("VolumenSEND: ");
  Serial.print(volumen,3); 
  Serial.print("PhSEND: ");
  Serial.print(phValue);
  Serial.print("\tRGBSEND: (");
  Serial.print(colorRojo);
  Serial.print(" , ");
  Serial.print(colorVerde);
  Serial.print(" , ");
  Serial.print(colorAzul);
  Serial.print(" )");
  Serial.print("\n");
  delay(100);
  
  // Comprobar cual es el color detectado y mostrarlo
  // con un mensaje en el monitor serie
  // ESTO NO
//  if(colorRojo > colorVerde && colorRojo > colorAzul){
//      Serial.println(" - Detectado ROJO");
//  }
//  if(colorVerde > colorRojo && colorVerde > colorAzul){
//    Serial.println(" - Detectado VERDE");
//  }
//  if(colorAzul > colorRojo && colorAzul > colorVerde){
//    Serial.println(" - Detectado AZUL");
//  }
  
}
