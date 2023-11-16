#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C, 16 columnas y 2 filas
const int pinSensor = A0;  // Pin analógico donde está conectado el sensor de humedad

int lecturaHumedad; // variable global
int porcentajeHumedad;
const int lecturaSeco = 1015; // Valor de lectura en condición de suelo seco
const int lecturaAgua = 524;  // Valor de lectura en condición de suelo húmedo

void setup() 
{
  pinMode(pinSensor, INPUT);  // Configura el pin del sensor de humedad como entrada
  lcd.init();        // Inicializa la pantalla
  lcd.backlight();   // Enciende la retroiluminación
}

void loop() 
{
  // seco=1015
  // agua=524
  // media=769

  lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad


  while (lecturaHumedad > 769)
  {
    lcd.clear();  // Borra ambas líneas de la pantalla
    lcd.print("eche agua " + String(lecturaHumedad));
    lecturaHumedad = analogRead(pinSensor);
    delay(1000);
  }

  /*La siguiente impresión se ubica después del while para que se actualice inmediatamente después de echar agua al bonsai*/
  lcd.clear();  // Borra ambas líneas de la pantalla
  lcd.setCursor(0, 0); // Ubica el cursor en la primera línea
  lcd.print("HOLA, SOY KIS");
  lcd.setCursor(0, 1); // Ubica el cursor en la segunda línea


  lcd.print("tierra húmeda");


//  lcd.print("Humedo = " + String(lecturaHumedad)); //la lectura e impresión de humedad también está dentro del while, **toca hacer una función**

  delay(5000); //demora un segundo antes de cerrar el loop
}

