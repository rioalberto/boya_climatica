// boya_climatica.h
// Autor: Alberto Río Martínez

#include <LiquidCrystal_I2C.h> // Librería para usar Display
#include <DHT.h> // Librería para usar DHT22
#include "RTClib.h" // Librería para usar RTC
#include <Servo.h> // Librería para usar servomotor
#include <Stepper.h> // Librería para usar el motor de la turbina

#define LDRPIN A0 // Pin asociado a la resistencia fotosensible
#define DHTPIN 12 // Pin asociado al DHT22
#define DHTTYPE DHT22 // Tipo de sensor de la familia DHT
#define MQ2PIN 7 // Pin asociado al MQ-2
#define CALENTADORPIN 4 // Pin asociado al calentador de las baterías
#define SERVOPIN 3 // Pin asociado al servo que abre la compuerta
#define BUTTONPIN 13 // Pin asociado al botón
#define MOTORPIN1 8 // Pines asociados al motor de la turbina
#define MOTORPIN2 9
#define MOTORPIN3 10
#define MOTORPIN4 11

String lastLine0 = "";
String lastLine1 = "";
String line0 = "";
String line1 = "";

uint8_t mode = 0; // Variable que indica el modo de operación
uint8_t lastMode = 0;
volatile bool acquisition = false; // Bandera que marcará lectura
volatile bool motor_flag = false; // Bandera que marcará caundo el motor se debe activar

// Constantes del LDR
const float GAMMA = 0.7; // Constante de la relación logarítimica entre la impedancia del LDR y la luminiscencia (lux)
const float RL10 = 50; // Impedancia del LDR a 10 lux

unsigned long lastButtonTouch = 0; // Variable que almacena la última vez que se pulsó el botón
const unsigned long ButtonDelay = 200; // Intervalo temporal que se debe cumplir entre pulsaciones

unsigned long previousMillis = 0; // // Variable que almacena la última vez que se levó a cabo una interrupción
const unsigned long interval = 1000; // Intervalo temporal de 1s que se debe cumplir entre interrupciones

bool MQ2Value; // Variable que almacena el valor digital de salida del MQ2

DateTime now;  // Variable de tipo DateTime (clase incluida en la librería RTClib.h que representa una fecha y una hora completas)

float humidity; // Variable de punto flotante que alamacena el valor de humedad medido por el DHT22
float temperature; // Variable de punto flotante que alamacena el valor de temperatura medido por el DHT22
const float limiteInferior = 22.00; // Límite inferior del margen de temperatura deseado en la boya
const float limiteSuperior = 28.00; // Límite superior del margen de temperatura deseado en la boya
// Valores de cambio de temperatura por segundo
const float cambioCalor = 0.04;   // °C/s al calentar
const float cambioFrio  = -0.28;  // °C/s al enfriar
const float dt = 1.0;             // intervalo en segundos entre mediciones
float delta = 0.0;

float lux; // Variable de punto flotante que alamacena el valor de luminiscencia medido por la resistencia fotosensible

const int stepsPerRevolution = 200; // Número de pasos por revolución del motor de la turbina

DHT dht(DHTPIN, DHTTYPE); // Declaración del objeto DHT
LiquidCrystal_I2C lcd(0x27, 16, 2); // Declaración del objeto LCD
RTC_DS1307 rtc; // Declaración del objeto RTC
Servo compuerta; // Declaración del objeto asociado al servo que abre la compuerta
Stepper turbina(stepsPerRevolution, MOTORPIN1, MOTORPIN2, MOTORPIN3, MOTORPIN4); // Declaración del objeto asociado al motor de la turbina

// Función que lee los valores de humedad y temperatura medidos por el DHT22
void read_dht() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}

// Función que lee los valores de luminiscencia medido por la resistencia fotosensible
void read_lux() {
  int analogValue = analogRead(LDRPIN);
  //Conversión del valor analógico a unidades de lux
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
}

// Función que lee los valores de fecha y hora medidos por el RTC DS1317
void read_rtc() {
  now = rtc.now();
}

// Función que lee el valor digital medido por el MQ2
void read_mq2() {
  MQ2Value = digitalRead(MQ2PIN);
}

// Función que controla que la temperatura de la boya esté en los margenes deseados
void temp_control() {
 // Lógica de control con simulación térmica
    if (temperature > limiteSuperior) {
      // Enfriar
      compuerta.write(0);
      motor_flag = true;
      temperature += cambioFrio * dt;
    } else if (temperature < limiteInferior) {
      // Calentar
      digitalWrite(CALENTADORPIN, HIGH);
      temperature += cambioCalor * dt;
    } else {
      // Estable: apagar todo
      compuerta.write(90);
      analogWrite(CALENTADORPIN, 0);
      motor_flag = false;
      read_dht();
    }
}

// Función que limpia la consola y actualiza los valores
void clear_lcd(String &line0, String &lastLine0, String &line1, String &lastLine1) {
  // Solo actualizar si algo ha cambiado
  if (line0 != lastLine0 || line1 != lastLine1 || mode != lastMode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line0);
    lcd.setCursor(0, 1);
    lcd.print(line1);
    lastLine0 = line0;
    lastLine1 = line1;
    lastMode = mode;
  }
}

// Función que representa los valores de fecha y hora medidos por el RTC DS1317
void display_date_hour() {
  line0 = "Fecha: " + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
  line1 = "Hora: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  clear_lcd(line0, lastLine0, line1, lastLine1);
}

// Función que representa los valores de luminiscencia medido por la resistencia fotosensible
void display_lux() {
  line0 = "Lux: " + String(lux, 2);
  line1 = "                            ";
  clear_lcd(line0, lastLine0, line1, lastLine1);
}

// Función que representa los valores de humedad y temperatura medidos por el DHT22
void display_dht22() {
  line0 = "H(%): " + String(humidity, 2);
  line1 = "T(\xDF""C): " + String(temperature, 2);
  clear_lcd(line0, lastLine0, line1, lastLine1);
}

// Función que representa el valor digital medido por el MQ2
void display_MQ2() {
  line0 = "Calidad aire: ";
  line1 = MQ2Value ? "Limpio" : "Contaminado";
  clear_lcd(line0, lastLine0, line1, lastLine1);
}

// Función que representa el control de la temperatura de la boya
void display_temp_ctl() {
  if (temperature > limiteSuperior) {
    // Enfriar
    line0 = "Enfriando";
    line1 = "T(\xDF""C): " + String(temperature, 2);
    clear_lcd(line0, lastLine0, line1, lastLine1);
  } else if (temperature < limiteInferior) {
    // Calentar
    line0 = "Calentando";
    line1 = "T(\xDF""C): " + String(temperature, 2);
    clear_lcd(line0, lastLine0, line1, lastLine1);
  } else {
    // Estable
    line0 = "Estable";
    line1 = "T(\xDF""C): " + String(temperature, 2);
    clear_lcd(line0, lastLine0, line1, lastLine1);
  }
}
