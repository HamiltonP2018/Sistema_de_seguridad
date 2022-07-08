/*
  LiquidCrystal Library - display() and noDisplay()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 display() and noDisplay() functions to turn on and off
 the display.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

*/

// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdio.h>
#include <string.h>
#define ROJO 38
#define AZUL 39
#define VERDE 40

void leerPassword();
int validarPassword();
void sigueIntentando();
void passwordCorrecta();
void passwordIncorrecta();
void limpiarPass_usuario();

//contraseña establecidad
const char password[5] ={'1','2','3','4'};
//contraseña digitadad por el usuario
char pass_usuario[5];
//caracter digitado en el teclado
unsigned char key = 0;
unsigned char nulo = 0;

//contadores
int contador = 0;
int intentos = 1; 
int fallos = 3;

//bandera
int bandera = 0;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 34, d5 = 35, d6 = 35, d7 = 37;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("SISTEMA DE SEGURIDAD");
  pinMode(ROJO, OUTPUT);
  pinMode(AZUL, OUTPUT);
  pinMode(VERDE, OUTPUT);
  digitalWrite(AZUL, HIGH);
  

}



void loop() {

  if( intentos <= fallos && bandera != 1){
       lcd.print("SISTEMA DE SEGURIDAD");
       leerPassword();
       if((validarPassword() == 1)){
           
            passwordCorrecta();
            bandera = 1;
        }else
        {
            sigueIntentando();
           
        }
  }else if(bandera == 1){
          passwordCorrecta();
  }
  else{
        passwordIncorrecta();
    }
  
}


 /**
     * Lee la contraseñar digitada por el usuario <br>
     * <b>post: </b> primero limpia la pantalla de LCD luego se imprimie un texto con el numero de intentos, luego entra a un 
     * ciclo repetitivo donde se digita caracter por caracter y se va guardando en arreglo de caracteres, el ciclo se repetira hasta 
     * ser menor a 4, porque es la longitud del arreglo.
     * @param key variable de tipo caracter que guarda el caracter diigitado por el usuario.
     * @param  pass_usuario[] arreglo que guarad los 4 caracteres digitados por el usuario.
 */

void leerPassword()
{
      lcd.clear();
      lcd.print("INTENTO " + intentos);
     for(int i =0; i <4; i++){
            key = keypad.getKey();
            pass_usuario[i] = key;
      }    
}
 /**
     * validad si la contraseña digitada por el usuario <br>
     * <b>post: </b> Se inicializa un contadores, luego se inicia un ciclo que contiene un condicional que permite evaluar      
     * si los caracteres del arreglo con la contraseña digitada por el usuario estan en la misma posicion que el 
     * arreglo que contiene la contraseña ya preestablecidad, cuando el caracter es correcto se incrementa un contador que 
     * permite identificar cuantos caractes correctos hay, si el numero de caractes es 3, signiica que la contraseña es 
     * correcta de lo contrario es incorrecta 
     * @return el numero 1 si el numero de caracteres es correcto
     * @return el numero 0 si el numero de caracteres es incorrecto
 */
int validarPassword(){
 int i = 0;
 int j = 0;
 do{
      if(pass_usuario[i] == password[i]){
          j++;
        }
        i++;
    }while(i < 4);

    if(i == 3){
      return 1;
      }else{
        return 0;
      }
}




 /**
     * Limpia todos los caractes del arreglo que contiene la contraseña digitada por el usuario
     * <b>post: </b> en un ciclo repetitivo se igualan  todas las posiciones del arreglo a un valor 
     * nulo
 */


void limpiarPass_usuario(){

    for(int i =0; i < 4; i++){
        pass_usuario[i] = nulo;
      }
}


 /**
     * Muestra que la contraseña es correcta
     * <b>post: </b> hace el llamado al metodo que limpia la contraseña, luego imprime en el LCD
     * un texto que avisa que la contraseña es correcta, enciende el led verde y apaga los demas leds.
     * se muestra durante 5 segundos
     * @see limpiarPass_usuario()
 */
 
void passwordCorrecta(){
   limpiarPass_usuario();
     lcd.clear();
        lcd.print("CLAVE CORRECTA");
        digitalWrite(AZUL, LOW);
        digitalWrite(ROJO, LOW);
        digitalWrite(VERDE, HIGH);
        delay(5000);
  }

/**
     * Muestra que la contraseña es incorrecta, pero que puede seguir intentando
     * <b>post: </b> hace el llamado al metodo que limpiar la contraseña, luego imprime en el LCD
     * un texto que avisa que la contraseña es incorrecta y que puede seguir intentando, enciende el led azul, apaga los demas leds
     * e incrementa el numero de intentos.
     * se muestra durante 5 segundos
     * @see limpiarPass_usuario()
 */
void sigueIntentando(){
        limpiarPass_usuario();
        lcd.clear();
        lcd.print("CLAVE INCORRECTA");
        lcd.print("SIGUE INTENTANDO");
        digitalWrite(VERDE, LOW);
        digitalWrite(ROJO, LOW);
        digitalWrite(AZUL, HIGH);
        intentos++;
        delay(3000);
}

/**
     * Muestra que la contraseña es incorrecta
     * <b>post: </b> hace el llamado al metodo que limpiar la contraseña, luego imprime en el LCD
     * un texto que avisa que la contraseña es incorrecta y que ha alcanzado el numero maximo de intentos, 
     * enciende el led rojo, apaga los demas leds
     * se muestra durante 5 segundos
     * @see limpiarPass_usuario()
 */
 void passwordIncorrecta(){
        limpiarPass_usuario();
        lcd.clear();
        lcd.print("CLAVE INCORRECTA");
        lcd.print("MAXIMO NUMERO DE INTENTOS");
        digitalWrite(VERDE, LOW);
        digitalWrite(ROJO, HIGH);
        digitalWrite(AZUL, LOW);
        delay(5000);
}
