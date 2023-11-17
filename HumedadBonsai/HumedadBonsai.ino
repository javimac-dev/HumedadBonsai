#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C, 16 columnas y 2 filas
const int pinSensor = A0;  // Pin analógico donde está conectado el sensor de humedad
bool retroiluminacionEncendida = true;

int lecturaHumedad; // variable global
int porcentajeHumedad;
int delaySeco = 1000;  // lecturas de 1 segundo cuando seco 
int delayHumedo = 18000000; // lecturas de 5 horas cuando seco
//int delayHumedo = 1000; // lecturas de 1 seg cuando seco

// seco=1023
// agua=420
// media=721

// Valores para el cálculo del porcentaje de humedad
const int valorMinimo = 420; // Valor correspondiente al 100% de humedad
const int valorMaximo = 1023; // Valor correspondiente al 0% de humedad
// Declara una variable booleana para rastrear el estado de la retroiluminación

void setup() 
{
  pinMode(pinSensor, INPUT);  // Configura el pin del sensor de humedad como entrada
  lcd.init();        // Inicializa la pantalla
  lcd.backlight();   // Enciende la retroiluminación
  Serial.begin(9600); //Para visualizar datos en el PC
}

void loop() 
{
  lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad

  while (lecturaHumedad > 721) //entra cuando baja del 50%
  {
    // Enciende o apaga la retroiluminación según el estado actual
    if (retroiluminacionEncendida) 
    {
      lcd.backlight();
    }
     else 
    {
      lcd.noBacklight();
    }
   
    lcd.clear();  // Borra ambas líneas de la pantalla
    lcd.print("AGREGA AGUA-1Lt");
    porcentajeHumedad = static_cast<int>((1 - static_cast<float>(lecturaHumedad - valorMinimo) / (valorMaximo - valorMinimo)) * 100);
    lcd.setCursor(0, 1); // Ubica el cursor en la segunda línea
    lcd.print("Humedad " + String(porcentajeHumedad)+ "%  "+ String(lecturaHumedad));
    lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad

    // Alterna la retroiluminación en cada ciclo del while
    retroiluminacionEncendida = !retroiluminacionEncendida;   
    delay(delaySeco);
  }

  lcd.backlight();//Por si en el último ciclo while queda apagada la pantalla.

  /*La siguiente impresión se ubica después del while para que se actualice inmediatamente después de agregar agua al bonsai*/
  lcd.clear();  // Borra ambas líneas de la pantalla
  //  lcd.setCursor(0, 0); // Ubica el cursor en la primera línea
  lcd.print("  KARATE KIZZ");
  porcentajeHumedad = static_cast<int>((1 - static_cast<float>(lecturaHumedad - valorMinimo) / (valorMaximo - valorMinimo)) * 100);
  lcd.setCursor(0, 1); // Ubica el cursor en la segunda línea
  lcd.print("Humedad " + String(porcentajeHumedad)+ "%  "+ String(lecturaHumedad));

  delay(delayHumedo); //demora antes de cerrar el loop principal
}
