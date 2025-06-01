# Estación Climática Marina Autónoma

Este proyecto implementa el control de una boya climática inteligente equipada con sensores ambientales, actuadores de control térmico y un sistema de visualización. La estación está diseñada para operar en entornos marinos y supervisar condiciones atmosféricas y ambientales de forma autónoma.

---

## Descripción General

La boya cuenta con sensores para medir:

- **Temperatura** y **humedad relativa** mediante el **DHT22**
- **Luminiscencia ambiental** con un **LDR**
- **Fecha y hora** con un **RTC DS1307**
- **Calidad del aire** con un sensor de gases **MQ-2**

Además, implementa un sistema de regulación térmica que mantiene la temperatura interna de la boya entre **22 °C y 28 °C** mediante:

- Un **calentador PTC (FIT0845)** de 5V / 8W para aumentar la temperatura [https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/6648/FIT0845.pdf](#)
- Un **sistema de refrigeración pasiva**, que introduce agua de mar (~20 °C) mediante:
  - Un **servo** que abre una compuerta
  - Una **turbina** (motor paso a paso) que distribuye el agua internamente

---

## Hardware Requerido

- DHT22 – Sensor de temperatura y humedad
- LDR – Sensor de luz (resistencia fotosensible)
- MQ-2 – Sensor de gases
- DS1307 – Reloj en tiempo real
- FIT0845 – Calentador PTC 5V 8W
- Servo 5V – Para apertura de compuerta
- Motor paso a paso (28BYJ-48 o similar)
- Pantalla LCD I2C 16x2
- Microcontrolador compatible con Arduino (ej. UNO, Mega)
- Pulsador para cambio de modo
- Resistencias, cables y estructura de soporte

---

## Esquema de Conexiones

> Próximamente se incluirá un diagrama de conexión (Fritzing o similar).

---

## Funcionalidad del Sistema

### Sensorial

Lectura cada segundo de:

- Temperatura y humedad (DHT22)
- Luminiscencia (LDR)
- Calidad del aire (MQ-2)
- Fecha y hora (RTC)

### Control Térmico

- Si la **temperatura < 22 °C**:
  - Se activa el calentador
  - Se simula un aumento de temperatura de **+0.04 °C/s**
- Si la **temperatura > 28 °C**:
  - Se abre la compuerta con el servo
  - Se activa la turbina para distribuir agua marina (~20 °C)
  - Se simula un enfriamiento de **–0.28 °C/s**
- Si está **dentro del rango**:
  - Se apaga el calentador
  - Se cierra la compuerta
  - Se detiene la turbina

### Visualización en LCD (controlado por botón):

- Fecha y hora actual
- Nivel de luminiscencia (lux)
- Temperatura y humedad
- Estado del aire (contaminado/limpio)
- Estado térmico y temperatura actual

---

## Estructura del Código

El código está organizado en varias funciones que separan claramente:

- La adquisición de datos
- La lógica de control térmico
- La visualización en pantalla
- El manejo de eventos como el pulsador

El temporizador interno se implementa mediante `millis()` para simular una lectura cíclica cada 1 segundo (`interval = 1000 ms`).

---

## Cómo Usarlo

1. Conecta todos los sensores y actuadores al microcontrolador como se indica en las definiciones de pines (`#define ...`).
2. Carga el código en el microcontrolador desde el IDE de Arduino.
3. Al iniciar, la boya se encontrará en modo de visualización 0 (fecha y hora).
4. Usa el botón para alternar entre los **5 modos de visualización**.

---

## Notas Técnicas

- El valor de temperatura es actualizado con lógica de simulación térmica dentro del código:  
  `temperature += cambioFrio * dt` o `cambioCalor * dt`
- Se asume que el sensor MQ-2 está configurado en modo digital:  
  `0 = limpio`, `1 = contaminado`
- La simulación **ignora la dinámica real del entorno marino** y sirve para pruebas en laboratorio o demostraciones educativas.

---

## Posibles Mejoras

- Añadir datalogging a SD o comunicación LoRa/WiFi
- Usar sensores analógicos con calibración real (MQ-2 en modo analógico)
- Integrar energía solar para operación prolongada
- Medir temperatura del agua marina directamente

---

## Vídeos de Pruebas de Funcionamiento

Puedes ver vídeos demostrativos del sistema en acción en los siguientes enlaces:

- [Video 1 – Funcionamiento general y control térmico](#)  
- [Video 2 – Ciclo completo de lectura y visualización](#)  
- [Video 3 – Simulación de ambiente contaminado y respuesta del sistema](#)

> *Los vídeos ilustran el sistema en entorno controlado con simulación de variables físicas.*

---

## Licencia

Este proyecto es de código abierto y se publica bajo la **MIT License**.
