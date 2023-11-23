/*
Control de humedad que hace lecturas cada 5 horas mientras la tierra está sobre el 50% de humedad.  Por debajo del 50% hace lecturas cada segundo.
Elementos:
- Arduino uno
- Sensor de humedad YL-69
- Módulo receptor YL-38
- Pantalla LCD 16x2
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C, 16 columnas y 2 filas
const int pinSensor = A0;  // Pin analógico donde está conectado el sensor de humedad

bool retroiluminacion = true;
int lecturaHumedad; // variable global
int porcentajeHumedad;

int tiempoCiclo=3600; // (1 hora) //los segundos totales que tarda en hacer una nueva lectura mientra está húmedo.
//int tiempoCiclo=7200; // (2 hora) //los segundos totales que tarda en hacer una nueva lectura mientra está húmedo.
//int tiempoCiclo=7200; // (3 hora) //los segundos totales que tarda en hacer una nueva lectura mientra está húmedo.
//int tiempoCiclo=60; // (1 minuto) //los segundos totales que tarda en hacer una nueva lectura mientra está húmedo.

int horas = 0;
int minutos = 0;
int segundos = 0;

int contador=tiempoCiclo;

// Valores para el cálculo del porcentaje de humedad
const int valorMinimo = 40; // Valor correspondiente al 100% de humedad
const int valorMaximo = 1023; // Valor correspondiente al 0% de humedad
// media=491

void setup() 
{
  pinMode(pinSensor, INPUT);  // Configura el pin del sensor de humedad como entrada
  lcd.init();        // Inicializa la pantalla
  lcd.backlight();   // Enciende la retroiluminación
  Serial.begin(9600); //Para visualizar datos en el PC

  lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad
  porcentajeHumedad = static_cast<int>((1 - static_cast<float>(lecturaHumedad - valorMinimo) / (valorMaximo - valorMinimo)) * 100);
}

void loop() 
{
  if (lecturaHumedad > 721) //entra cuando baja del 50%.  En el primer ciclo siempre entra.
  {
    lcd.clear();  // Borra ambas líneas de la pantalla
    lcd.print("AGREGA AGUA 1/2L");
    porcentajeHumedad = static_cast<int>((1 - static_cast<float>(lecturaHumedad - valorMinimo) / (valorMaximo - valorMinimo)) * 100);
    lcd.setCursor(0, 1); // Ubica el cursor en la segunda línea
    lcd.print("Humedad " + String(porcentajeHumedad)+ "%  "+ String(lecturaHumedad));
    lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad

    // Enciende o apaga la retroiluminación según el estado actual
    if (retroiluminacion) 
    {
      lcd.backlight();
    }
     else 
    {
      lcd.noBacklight();
    }
    // Alterna la retroiluminación en cada ciclo del while
    retroiluminacion = !retroiluminacion;   
  }
  else
  {
    lcd.backlight();//Por si en el último ciclo queda apagada la pantalla.
    lcd.clear();  // Borra ambas líneas de la pantalla
    lcd.setCursor(0, 0); // Ubica el cursor en la primera línea
    horas = contador / 3600;
    minutos = (contador % 3600) / 60;
    segundos = contador % 60;
    lcd.print("SOY KIZ  " + String(horas) + ":" + String(minutos) + ":" + String(segundos)); // EL QUE ME VENDIÓ EL BONSAI DIJO:  "VEA... ESE ES EL DE KARATE KIZZ"
    if(contador==0)
    {
      lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad
      porcentajeHumedad = static_cast<int>((1 - static_cast<float>(lecturaHumedad - valorMinimo) / (valorMaximo - valorMinimo)) * 100);
      contador=tiempoCiclo;
    }
    lcd.setCursor(0, 1); // Ubica el cursor en la segunda línea
    lcd.print("Humedad " + String(porcentajeHumedad)+ "%  "+ String(lecturaHumedad));
    contador=contador-1;
  }
  delay(1000); //demora antes de cerrar el loop principal
}
