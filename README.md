# Estación Climática Marina Autónoma

Este proyecto implementa el control de una boya climática inteligente equipada con sensores ambientales, actuadores de control térmico y un sistema de visualización. La estación está diseñada para operar en entornos marinos y supervisar condiciones atmosféricas y ambientales de forma autónoma.

---

## Descripción General

La boya está controlada por **Arduino Uno** y cuenta con sensores para medir:

- **Temperatura** y **humedad relativa** mediante el **DHT22**
- **Luminiscencia ambiental** con un **LDR**
- **Fecha y hora** con un **RTC DS1307**
- **Calidad del aire** con un sensor de gases **MQ-2**

Además, implementa un sistema de regulación térmica que mantiene la temperatura interna de la boya entre **22 °C y 28 °C** mediante:

- Un **calentador PTC (FIT0845)** de 5V / 8W para aumentar la temperatura 
- Un **sistema de refrigeración pasiva**, que introduce agua de mar (de una temperatura de en torno a 20 °C) mediante:
  - Un **servo** que abre una compuerta
  - Una **turbina** (motor paso a paso) que distribuye el agua internamente

---

## Hardware Requerido

### - DHT22 
Sensor digital que mide temperatura y humedad relativa con buena precisión y fiabilidad. Entrega directamente una señal digital calibrada sin necesidad de conversión analógica ni procesamiento adicional, ya que internamente incluye un microcontrolador de 8 bits. Su consumo es bajo, tiene un formato compacto de cuatro pines y permite transmisión de datos hasta 20 metros, lo que lo hace adecuado incluso para entornos exigentes como una boya climática flotante.
En cuanto a precisión, el DHT22 tiene un error relativo de ±2 % en humedad y ±0.5 °C en temperatura, lo cual es perfectamente aceptable para aplicaciones de monitoreo ambiental, ya que estas pequeñas desviaciones no afectan significativamente el análisis climático, donde las variaciones relevantes suelen ser mayores.

### - LDR 
Resistencia fotosensible que permite estimar la cantidad de luz presente en el entorno en unidades de lux. Puede ser usada con fines de monitorización climática o para determinar un umbral de luminiscencia a partir del cual la alimentación del sistema se lleve a cabo con una batería solar.

### - MQ-2 
Sensor de gases que destaca por su alta sensibilidad, bajo coste, larga vida útil y facilidad de uso. En este proyecto se utilizará su salida digital, la cual se activa (nivel bajo) cuando la concentración de gases supera un umbral de aproximadamente 350 ppm. Su inclusión en una boya climática es útil porque puede alertar de la presencia de vapores inflamables o contaminantes en ambientes cercanos al agua, como zonas portuarias, industriales o áreas donde puedan producirse derrames de combustibles.

### - DS1307 
Módulo RTC (Real Time Clock) económico que proporciona fecha y hora incluso en caso de que el Arduino se apague o reinicie, ya que cuenta con una batería de respaldo. Resulta fundamental para registrar eventos o realizar medidas temporizadas de manera autónoma y dispone de una interfaz I2C, lo que facilita la comunicación.

### - FIT0845 – Calentador PTC 5V 8W
Calentador cerámico tipo PTC (Coeficiente de Temperatura Positivo). Este tipo de calentadores tiene la ventaja de autorregular su temperatura, ya que su resistencia eléctrica aumenta conforme se calienta, limitando la corriente que consume. Esto lo hace más seguro y eficiente que resistencias tradicionales. Tiene una tensión nominal de 5 VDC, una potencia de 8 W y una temperatura de operación típica entre 50 °C y 70 °C. [https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/6648/FIT0845.pdf](#)

### - Servo 5V – Para apertura de compuerta
Un servo motor es un actuador rotativo que permite controlar de manera precisa el ángulo de giro. En este caso se usa un servo de 5 V (como el SG90 o similar) para abrir una compuerta por la que puede entrar agua marina para refrigerar la boya.Tiene una tensión de operación de 5 VDC, un ángulo típico de operación de 0° a 180° y su control se lleva a cabo mediante señal PWM.
Uso en este proyecto: Abre la compuerta solo si la temperatura interna supera los 28 °C. El ángulo se ajusta para permitir el paso de agua y luego se cierra automáticamente cuando la temperatura baja al rango normal.

### - Motor paso a paso (28BYJ-48 o similar)
Motor paso a paso unipolar o bipolar empleado para accionar una turbina o sistema de distribución interna de agua que ayuda a enfriar el interior de la boya. Cuando la compuerta se abre (por temperatura > 28 °C), se activa el motor para distribuir el agua que entra y acelerar el enfriamiento, simulando una reducción térmica de -0.28 °C/s.

### - Pantalla LCD I2C 16x2
Display LCD de 16 columnas y 2 líneas que permite mostrar los datos capturados de forma clara y compacta. Al contar con comunicación I2C, se minimiza el número de pines requeridos en el Arduino, dejando más disponibles para otros sensores o funciones futuras.

### - Microcontrolador compatible con Arduino (ej. UNO, Mega)
En este proyecto se utiliza una placa Arduino UNO, basada en el microcontrolador ATmega328P. Es ideal para sistemas embebidos de pequeña escala por su simplicidad, robustez y ecosistema de librerías.

### - Pulsador para cambio de modo
Un pulsador simple (push button) es utilizado para alternar entre distintos modos de visualización en la pantalla LCD (como temperatura, fecha/hora, calidad del aire, etc.).

### - Resistencias, cables y estructura de soporte

---

## Esquema de Conexiones



| Color del cable  | Pin en Arduino | Conectado a                      | Función                                      |
|------------------|----------------|----------------------------------|----------------------------------------------|
|    Rojo          | 5V             | DHT22, Pulsador, ULN2003, MQ-2, Pantalla LCD I2C, RTC DS1307 | Alimentación (VCC)                          |
|    Negro         | GND            | Todos los módulos (común)        | Tierra (GND)                                 |
|    Blanco        | 12             | DHT22                            | Señal de temperatura/humedad                 |
|    Amarillo      | 13             | Pulsador                         | Entrada de botón                             |
|    Naranja       | A0             | LDR                              | Sensor LDR                                   |
|    Gris          | 4              | FIT0845                          | Calentador                                   |
|    Verde claro   | 8              | Motor A+                         | Motor paso a paso                            |
|    Rosa          | 9              | Motor A-                         | Motor paso a paso                            |
|    Morado        | 10             | Motor B+                         | Motor paso a paso                            |
|    Marrón        | 11             | Motor B-                         | Motor paso a paso                            |
|    Verde         | 3              | Servo motor                      | PWM de control                               |
|    Violeta       | 7              | MQ-2                             | Lectura analógica del gas                    |
|    Verde         | A4 (SDA)       | RTC DS1307, Pantalla LCD I2C     | Comunicación I2C – datos                     |
|    Azul          | A5 (SCL)       | RTC DS1307, Pantalla LCD I2C     | Comunicación I2C – reloj                     |


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

### Cálculos térmicos

Estos cálculos permiten estimar los efectos térmicos del calentador y del sistema de enfriamiento por convección en una masa de referencia de **200 g (0.2 kg)**, como podría ser una batería o compartimento.

#### 1. Estimación del calentamiento

**Supuestos:**

- Masa afectada:  
  `m = 0.2 kg`

- Calor específico (ej. batería de litio):  
  `c = 900 J/(kg·°C)`

- Potencia del PTC (valor práctico):  
  `Q = 8 J/s = 8 W`

**Fórmula aplicada:**

`ΔT = Q / (m · c) = 8 / (0.2 × 900) = 8 / 180 ≈ 0.044 °C/s`

**Resultado:**  
En condiciones ideales, el sistema podría incrementar la temperatura en **~0.044 °C por segundo**.

*En la práctica, este valor será menor por pérdidas térmicas. Además, el PTC se autorregula y limita su potencia conforme sube la temperatura, protegiendo el sistema de sobrecalentamientos.*

---

#### 2. Estimación del enfriamiento

Se modela como una **convección forzada**, al abrir la compuerta e introducir agua marina más fría con ayuda de una turbina.

##### Paso 1: Cálculo del flujo de calor

**Fórmula:**

`Q = h · A · ΔT`

**Parámetros usados:**

- Coeficiente de convección:  
  `h = 500 W/(m²·°C)`  
  *(valor razonable para agua con flujo moderado)*

- Área de intercambio térmico:  
  `A = 0.01 m²` *(10 cm × 10 cm)*

- Diferencia de temperatura:  
  `ΔT = 10 °C` *(Ejemplo: batería a 30 °C, agua a 20 °C)*

**Cálculo:**

`Q = 500 × 0.01 × 10 = 50 W`

##### Paso 2: Cálculo del descenso térmico

**Fórmula de calorimetría:**

`ΔT = Q / (m · c) = 50 / (0.2 × 900) = 50 / 180 ≈ 0.278 °C/s`

**Resultado:**  
La temperatura descendería idealmente **~0.28 °C por segundo** al enfriar la carcasa con agua.

*Este valor también depende del flujo, turbulencia, conductividad de los materiales, etc.*

---

#### Conclusión

- **Calentamiento simulado:**  
  `ΔT_calor ≈ +0.04 °C/s` *(valor ajustado en software)*

- **Enfriamiento simulado:**  
  `ΔT_frío ≈ –0.28 °C/s`

Estas tasas son implementadas en el software del Arduino para simular la evolución térmica del sistema, de forma que el comportamiento general sea realista en un entorno controlado.

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
