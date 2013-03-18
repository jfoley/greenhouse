#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define INTERIOR_SENSOR 0
#define EXTERIOR_SENSOR 1

#define ONE_WIRE_BUS 2
#define FAN_PIN 3
#define AC_PIN 4
#define HEAT_PIN 5

#define HEATING_THRESHOLD 62.0
#define COOLING_THRESHOLD 82.0
#define EXTERIOR_THRESHOLD 79.0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup(void)
{
  pinMode(FAN_PIN, OUTPUT);
  pinMode(AC_PIN, OUTPUT);
  pinMode(HEAT_PIN, OUTPUT);
  lcd.begin(16, 2);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  float interior_temp = sensors.getTempFByIndex(INTERIOR_SENSOR);
  float exterior_temp = sensors.getTempFByIndex(EXTERIOR_SENSOR);

  lcd.setCursor(0, 0);
  lcd.print("Int: ");
  lcd.print(interior_temp);

  lcd.setCursor(0, 1);
  lcd.print("Ext: ");
  lcd.print(exterior_temp);

  if(interior_temp < HEATING_THRESHOLD) {
    lcd.setCursor(12, 0);
    lcd.print("HEAT");
    digitalWrite(HEAT_PIN, HIGH);
  }

  if(interior_temp < COOLING_THRESHOLD && interior_temp > HEATING_THRESHOLD) {
    lcd.setCursor(12, 0);
    lcd.print("    ");
    lcd.setCursor(12, 1);
    lcd.print("    ");

    digitalWrite(HEAT_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(AC_PIN, LOW);
  }

  if (interior_temp > COOLING_THRESHOLD) {
    lcd.setCursor(12, 0);
    lcd.print("COOL");
    digitalWrite(HEAT_PIN, LOW);

    if(exterior_temp < EXTERIOR_THRESHOLD) {
      lcd.setCursor(12, 1);
      lcd.print("FAN ");

      digitalWrite(FAN_PIN, HIGH);
      digitalWrite(AC_PIN, LOW);
    }
    else {
      lcd.setCursor(12, 1);
      lcd.print("AC  ");
      digitalWrite(FAN_PIN, LOW);
      digitalWrite(AC_PIN, HIGH);
    }
  }

  delay(3000);
}
