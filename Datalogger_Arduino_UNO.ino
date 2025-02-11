//datalogger con SHIELD arduino uno + RTC + SD

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

//config pines:
#define voltaje_ADC A0
#define chipSelect 10

#define SerialSpeed 9600
#define interval 10000
#define numLecturas 10      //Numero de lecturas para promediar

uint32_t prevMillis = 0;

float Voltaje_value = 0.0;
float voltajePromedio = 0.0;

RTC_DS1307 RTC;

String ano, dia, mes, horas, minutos, segundos;
String Fecha, Hora;
String NombreArchivo;

void updateTime();
String generarNombreArchivo();

void setup(){
    Serial.begin(SerialSpeed);
    delay(5000);
    Serial.println("iniciando...");

    //inicializa el RTC
    if(!RTC.begin())
    {
      Serial.println("RTC no está activado!");
      //Ajustar la fecha y hora en la que se compilo, usar esto solo la primera vez.
      //RTC.adjust(DateTime(__DATE__, __TIME__));
      return;
    }

    //Inicializa la tarjeta SD
    if(!SD.begin(chipSelect))
    {
      Serial.println("No se pudo inicializar la SD, o no está conectada");
      return;
    }
    Serial.println("SD inicializada!");
    
    //Genera un nuevo nombre de archivo
    NombreArchivo = generarNombreArchivo();
    if (NombreArchivo == "") {
      Serial.println("Error al generar el nombre del archivo");
      return;
    }

    // Crea el archivo CSV y escribe el encabezado
    File archivo = SD.open(NombreArchivo.c_str(), FILE_WRITE);
    if (archivo) {
      archivo.println("Fecha, Hora, Voltaje (V)");            //Encabezado del archivo
      archivo.close();
      Serial.print("Archivo creado: ");
      Serial.println(NombreArchivo);
    } 
    else  Serial.println("Error al crear el archivo CSV");

  prevMillis = millis();           //guarda tiempo de donde va a empezar el loop
}

void loop(){
  if (millis() - prevMillis > interval) {             //entra 1 vez cada 10 segundos
    prevMillis = millis();  

    // Lee el voltaje en el pin A0
    //Voltaje_value = analogRead(voltaje_ADC) * (4.93 / 1023.0);
    

    // Realiza varias lecturas y las promedia
    voltajePromedio = 0.0; // Reinicia el valor del promedio
    for (int i = 0; i < numLecturas; i++) {
    voltajePromedio += analogRead(voltaje_ADC) * (4.94 / 1023.0);     // "+=" va acumulando el resultado a voltajePromedio.
    delay(10);                                                        // Pequeño retardo entre lecturas
  }
  Voltaje_value = voltajePromedio / numLecturas; // Calcula el promedio
  Serial.print("Voltaje Promedio: "); Serial.println(Voltaje_value, 3);

    // Actualiza la fecha y hora
    updateTime();
    Serial.print("Fecha: "); Serial.println(Fecha);
    Serial.print("Hora: "); Serial.println(Hora);

    // Abre el archivo CSV y añade una nueva línea
    File archivo = SD.open(NombreArchivo.c_str(), FILE_WRITE);
    if (archivo) {
      archivo.print(Fecha);                     // Escribe la fecha
      archivo.print(", ");
      archivo.print(Hora);                      // Escribe la hora
      archivo.print(", ");
      archivo.println(Voltaje_value, 3);        // Escribe el voltaje con 3 decimales
      archivo.close();
      Serial.println("Datos añadidos al archivo CSV");
    } 
    else  Serial.println("Error al abrir el archivo CSV");
  }
  

}

  void updateTime()
  {
    DateTime now = RTC.now();
    ano = now.year();
    mes = now.month();
    dia = now.day();
    horas = now.hour();
    minutos = now.minute();
    segundos = now.second();
    Fecha = dia + "/" + mes + "/" + ano;
    Hora = horas + ":" + minutos + ":" + segundos;
  }

  // Función para generar un nuevo nombre de archivo
  String generarNombreArchivo() {
    uint8_t numeroArchivo = 1; // Comienza con el número 1
    String nombreBase = "datalog"; // Nombre base del archivo
    String extension = ".csv"; // Extensión del archivo

    // Busca el siguiente nombre disponible
    while (true) {
      String nombre = nombreBase + String(numeroArchivo) + extension;
      if (!SD.exists(nombre.c_str())) {
        return nombre; // Retorna el nombre si no existe
      }
      numeroArchivo++; // Incrementa el número si el archivo ya existe
    }
  }

