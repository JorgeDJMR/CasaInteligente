#include <NewPing.h>
#include <ESP32Servo.h>

//Ultrasonico///////////////////////////////////////////////////////
#define TRIGGER_PIN_1 5   // Pin del trigger del sensor ultrasonido 1
#define ECHO_PIN_1 18      // Pin del echo del sensor ultrasonido 1
#define TRIGGER_PIN_2 19   // Pin del trigger del sensor ultrasonido 2
#define ECHO_PIN_2 15      // Pin del echo del sensor ultrasonido 2

#define SERVO_PIN 4       // Pin del servo motor

Servo myServo;
NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1);
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2);
////////////////////////////////////////////
//FLamas y boton//////////
#include <Arduino.h>

// Definir los pines utilizados
const int pinBuzzer = 22;         // Pin positivo del buzzer
const int pinBoton = 23;
const int pinSensorLlamas = 21;   // Pin del sensor de llamas
const int pinLED = 12;            // Pin del LED

// Variables para el estado anterior del botón y del sensor de llamas
int estadoBotonAnterior = HIGH;
int estadoLlamasAnterior = HIGH;

// Variable para controlar el estado del timbre y del LED
bool timbreActivado = false;
bool ledParpadeando = false;


/////////////////////////////////////////////////
//Fotorresistencia
// Pines donde están conectados los LEDs
const int pinLED1 = 12;

// Pin donde está conectada la fotorresistencia
const int pinFotorresistencia = 14;





//////////////////////////////////////////////////


void setup() {
//Ultrasonico////
  pinMode(SERVO_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);




//Flamas y boton//

  // Configurar los pines como salida o entrada
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinBoton, INPUT_PULLUP);
  pinMode(pinSensorLlamas, INPUT);
  pinMode(pinLED, OUTPUT);

//Fotorresistencia
  // Inicializar los pines de los LEDs como salidas
  pinMode(pinLED1, OUTPUT);


  // No es necesario configurar el pin de la fotorresistencia,
  // ya que se utilizará en modo de entrada por defecto.

}

void loop() {
//Ultrasonico//////////////////////////////////////////////////////////////////
  delay(100);  // Pequeña pausa para estabilizar las lecturas

  unsigned int distance_1 = sonar_1.ping_cm();
  if (distance_1 < 5) {  // Si el objeto está a menos de 50 cm del sensor 1
    myServo.write(90);  // Mover el servo a 100 grados (abrir el garaje)
    delay(5000);  // Esperar medio segundo
  }

  unsigned int distance_2 = sonar_2.ping_cm();
  if (distance_2 < 5) {  // Si el objeto está a menos de 50 cm del sensor 2
    myServo.write(0);  // Mover el servo a 0 grados (cerrar el garaje)
    delay(5000);  // Esperar medio segundo
  }





//Sensor de Flamas y boton//////////////////////////////////////////////////////
   // Leer el estado actual del botón
  int estadoBotonActual = digitalRead(pinBoton);

  // Leer el estado actual del sensor de llamas
  int estadoLlamasActual = digitalRead(pinSensorLlamas);

  // Verificar si el botón ha sido presionado
  if (estadoBotonActual == LOW && estadoBotonAnterior == HIGH) {
    timbreActivado = true;
    tocarTimbre();
  }

  // Verificar si se suelta el botón
  if (estadoBotonActual == HIGH && estadoBotonAnterior == LOW) {
    timbreActivado = false;
    detenerTimbre();
  }

  // Verificar si el sensor de llamas se activa
  if (estadoLlamasActual == LOW && estadoLlamasAnterior == HIGH) {
    timbreActivado = true;
    tocarTimbre();
    ledParpadeando = true;
  }
  
  // Verificar si el sensor de llamas deja de detectar calor
  if (estadoLlamasActual == HIGH && estadoLlamasAnterior == LOW) {
    timbreActivado = false;
    detenerTimbre();
    ledParpadeando = false;
    apagarLED();
  }

  // Actualizar el estado anterior del botón y del sensor de llamas
  estadoBotonAnterior = estadoBotonActual;
  estadoLlamasAnterior = estadoLlamasActual;

  // Actualizar el estado del LED
  if (ledParpadeando) {
    parpadearLED();
  }




//Fotorresistencia//////////////////////////////////////////////////////////////
 // Leer el valor de la fotorresistencia
  int valorLuz = analogRead(pinFotorresistencia);

  // Si no se detecta luz (valor alto), encender los LEDs
  if (valorLuz < 2000) {
    digitalWrite(pinLED1, HIGH);

  } 
  // Si se detecta luz (valor bajo), apagar los LEDs
  else {
    digitalWrite(pinLED1, LOW);

  }

  // Esperar un breve período antes de volver a leer el valor de la fotorresistencia
  delay(100);





//Pantalla lcd//////////////////////////////////////////////////////////////////

}
//Codigo extra//////////////////////////////////////////////////////////////////////////////////

//Flamasy boton///////

void tocarTimbre() {
  if (timbreActivado) {
    // Reproducir secuencia de tonos del timbre
    tone(pinBuzzer, 1000);  // Tono inicial
    delay(200);             // Duración del tono inicial
  }
}

void detenerTimbre() {
  noTone(pinBuzzer);  // Detener el buzzer
}

void parpadearLED() {
  static bool ledEstado = LOW;
  ledEstado = !ledEstado;      // Cambiar el estado del LED
  digitalWrite(pinLED, ledEstado);
  delay(500);                  // Tiempo de cada estado del LED
}

void apagarLED() {
  digitalWrite(pinLED, LOW);
}
/////////////////////













