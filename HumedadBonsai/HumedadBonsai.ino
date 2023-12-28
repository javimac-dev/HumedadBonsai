#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C, 16 columnas y 2 filas
const int pinSensor = A0;  // Pin analógico donde está conectado el sensor de humedad

bool retroiluminacion = true;
int lecturaHumedad; // variable global
int porcentajeHumedad;

int horas = 0;
int minutos = 0;
int segundos = 0;

// Valores para el cálculo del porcentaje de humedad
//const int valorMinimo = 40; // Valor correspondiente al 0% de humedad
const int valorMaximo = 1023; // Valor (DADO POR EL SENSOR) correspondiente al 100% de humedad 

//********VALORES CONFIGURABLES*******//
const int valorAlarma = 100; // Pide agua al 10& (redondeado) de humedad
int tiempoCiclo = 3600; // (1 hora) //los segundos totales que tarda en hacer una nueva lectura mientras está húmedo.
//int tiempoCiclo = 5;//5 segundos 
//************************************//
int contador = tiempoCiclo;

void setup() 
{
  pinMode(pinSensor, INPUT);  // Configura el pin del sensor de humedad como entrada
  lcd.init();        // Inicializa la pantalla
  lcd.backlight();   // Enciende la retroiluminación
  Serial.begin(9600); // Para visualizar datos en el PC

  lecturaHumedad = analogRead(pinSensor); // Lectura del sensor de humedad
  porcentajeHumedad = lecturaHumedad*100/valorMaximo;
}

void loop() 
{
  if (lecturaHumedad < valorAlarma) // entra cuando baja del 10%.  En el primer ciclo siempre entra.
  {
    porcentajeHumedad = lecturaHumedad*100/valorMaximo;
    lcd.clear();  // Borra ambas líneas de la pantalla
    lcd.print("AGREGA AGUA 1Lt");
    lcd.setCursor(0, 1); // Ubica el cursor en la segunda línea
    lcd.print("Humedad " + String(porcentajeHumedad) + "%  " + String(lecturaHumedad));
    lecturaHumedad = analogRead(pinSensor); //lectura del sensor de humedad

    if (retroiluminacion) 
    {
      lcd.backlight();
    } 
    else 
    {
      lcd.noBacklight();
    }
    retroiluminacion = !retroiluminacion;   
  }
  else
  {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);

    horas = contador / 3600;
    minutos = (contador % 3600) / 60;
    segundos = contador % 60;
    lcd.print("SOY KIZ  " + String(horas) + ":" + String(minutos) + ":" + String(segundos));

    if(contador == 0)
    {
      lecturaHumedad = analogRead(pinSensor);
      //porcentajeHumedad = map(lecturaHumedad, valorMaximo, valorMinimo, 0, 100);
       porcentajeHumedad = lecturaHumedad*100/valorMaximo;
       contador = tiempoCiclo;
    }
    lcd.setCursor(0, 1);
    lcd.print("Humedad " + String(porcentajeHumedad) + "%  " + String(lecturaHumedad));
    contador = contador - 1;
  }
  Serial.println("Humedad: " + String(porcentajeHumedad) + "% - Lectura: " + String(lecturaHumedad));
  delay(1000); // demora antes de cerrar el loop principal
}