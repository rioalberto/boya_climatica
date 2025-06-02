// boya_climatica.ino
// Autor: Alberto Río Martínez

#include "boya_climatica.h"

void setup() {
  pinMode(LDRPIN, INPUT); // Declaración como entrada del pin asociado a la resistencia fotosensible
  pinMode(BUTTONPIN, INPUT_PULLUP); // Declaración como entrada con resistencia de Pull Up del pin asociado al botón
  pinMode(CALENTADORPIN, OUTPUT); // Declaración como salida del pin asociado a la alimentación del calentador
  analogWrite(CALENTADORPIN, 0);  // Apagar calentador al inicio

  compuerta.attach(SERVOPIN); // Inicialización del objeto servomotor
  compuerta.write(90); // Compuerta cerrada al inicio

  dht.begin(); // Inicialización del objeto DHT

  rtc.begin(); // Inicialización del objeto RTC

  turbina.setSpeed(1000); // Define la velocidad de la turbina en 1000 rpm

  // Inicialización del objeto LCD
  lcd.init();
  lcd.backlight();

  read_dht(); // Leer temperatura y humedad iniciales
}

void loop() {

  acquisition_int();

  if (acquisition) { // Solo lee cuando al bandera de adquisición se activa cada segundo
    acquisition = false;  // Resetear bandera

    //read_dht();
    read_lux();
    read_rtc();
    read_mq2();
    temp_control();

    //lcd.clear(); // Apaga el display para actualizar los valores

    switch (mode) {
      case 0: // Modo 0: Representa en el LCD fecha y hora
        display_date_hour();
      break;
      case 1: // Modo 1: Representa en el LCD el valor de luminiscencia
        display_lux();
      break;
      case 2: // Modo 2: Representa en el LCD los valores de humedad y temperatura
        display_dht22();
      break;
      case 3: // Modo 3: Representa en el LCD si el aire está contaminado o limpio
        display_MQ2();
      break;
      case 4: // Modo 4: Representa en el LCD el estado del sistema de monitorización térmica y la temperatura actual
        display_temp_ctl();
      break;
      default:
      break;
    }

  }
  
  if (digitalRead(BUTTONPIN) == LOW) { // Lee pulsaciones en el botón
    if (millis() - lastButtonTouch > ButtonDelay) { //Evitar rebotes: si entre pulsaciones no han pasado 200ms no hace nada
      mode = mode + 1; // Cada vez que se pulsa el botón incrementa el modo en una unidad
      if (mode == 5) {
        mode = 0;
      }
      lastButtonTouch = millis(); // Guarda el último instante temporal en el que se pulsó el botón. millis() devuelve el número de milisegundos transcurridos desde que se inició el programa
    }
  }

  if (motor_flag){
    turbina.step(stepsPerRevolution); // Activa el motor de la turbina
  }
}

void acquisition_int() {
  // Simular temporizador con millis()
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    acquisition = true;
  }
}

