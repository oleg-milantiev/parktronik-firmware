#include "MPU9250.h"
#include "eeprom_utils.h"

// http://go-hiking.ru/tools/declination/
// 7 градусов для Краснодара, 10 для Москвы
// http://www.magnetic-declination.com/ ещё источник

MPU9250 mpu;

//#define DEBUG

void setup() {
    Serial.begin(115200);

    #ifdef DEBUG
      Serial.println("Start");
    #endif
    
    pinMode(2, INPUT_PULLUP);
    Wire.begin();
    delay(1000);
    mpu.setup(0x68);  // change to your own address

    #ifdef DEBUG
      Serial.println("MPU init done");
    #endif

//    if (digitalRead(2) == 0) {
    if (false) {
//    if (true) {
      // Кнопка нажата, ушли в калибровку
      
      #ifdef DEBUG
        Serial.println("Start GYRO calibration");
      #endif
      mpu.calibrateAccelGyro();
      #ifdef DEBUG
        Serial.println("Done GYRO calibration. Start MAG calibration");
      #endif
      mpu.calibrateMag();
      #ifdef DEBUG
        Serial.println("Done MAG calibration");
      #endif
  
      saveCalibration();
    }

    loadCalibration();
    mpu.setMagneticDeclination(7);
}

uint32_t myTimer1;


void loop() {
    if (mpu.update()) {
      if (millis() - myTimer1 >= 1000) { 
        myTimer1 = millis();

        Serial.print("t=");
        Serial.print(mpu.getTemperature());
        Serial.print(",r=");
        Serial.print(mpu.getRoll());
        Serial.print(",p=");
        Serial.print(mpu.getPitch());
        Serial.print(",y=");
        Serial.println(mpu.getYaw());
      }
    }
}
