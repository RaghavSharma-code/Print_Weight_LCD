#include <HX711_ADC.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const int HX711_dout = 6;
const int HX711_sck = 7;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

const int calVal_eepromAdress = 0;
unsigned long t = 0;

HX711_ADC LoadCell(HX711_dout, HX711_sck);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Starting");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Namaskar");

  float CalibrationValue;
  CalibrationValue = 396.24;

  LoadCell.begin();
  unsigned long Stablizing_Time = 2000;
  boolean _Tare = true;

  LoadCell.start(Stablizing_Time, _Tare);

  if (LoadCell.getTareTimeoutFlag())
  {
    Serial.print("Timeout , Check Wiring");
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("Wiring Issue");
    // delay(100);
    while (1)
      ;
  }

  else
  {
    LoadCell.setCalFactor(CalibrationValue);
    Serial.println("Startup is complete");
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Startup");
    lcd.setCursor(5, 1);
    lcd.print("Complete");
    delay(100);
    lcd.clear();
  }
}

void loop()
{
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;

  if (LoadCell.update())
    newDataReady = true;
  if (newDataReady)
  {
    if (millis() > t + serialPrintInterval)
    {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      lcd.setCursor(0,0);
      lcd.print("Weight");
      lcd.setCursor(0,1);
      lcd.println(i);
      newDataReady = 0;
      t = millis();
    }
  }
}