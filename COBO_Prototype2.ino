#include "LowPower.h"
const int SPI_CS = A1;

const int BUTTON2 = 8;

const int BUTTON1 = 9;
const int LED_GREEN = 5;
const int LED_RED = 6;
#define AVERAGE_SIZE_SEC  8

#define IS_BUTTON2_PRESSED() (digitalRead(BUTTON2) == 0)

#define IS_BUTTON1_PRESSED() (digitalRead(BUTTON1) == 0)
#define SET_LED(LED) (digitalWrite(LED, HIGH))
#define CLR_LED(LED) (digitalWrite(LED, LOW))
struct sensor_s
{
  float aX;
  float aY;
  float aZ;
  float gX;
  float gY;
  float gZ;
  float temp;
  float   mic;
};
bool isDebugEnable = false;
#define SENSOR_ARRAY_SIZE 1
int sensorArrayCounter =0;
struct sensor_s sensorsArray[SENSOR_ARRAY_SIZE];
void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.setTimeout(10);
//  Serial.println("Started!");
  // put your setup code here, to run once:ç.l,
  initSDCard();

initCowSensors();

}
int count2LedIndicate = 0;
void loop() {
  //createNewFileIfButtonPressed();
  if(IS_BUTTON1_PRESSED())
    {
      for(int i=0;i<10;i++)
      {
        SET_LED(LED_RED);
        CLR_LED(LED_GREEN);
        delay(500);
        CLR_LED(LED_RED);
        SET_LED(LED_GREEN);
         delay(500);
      }
    }
    
  if(readSensors())
    {
      sensorArrayCounter =0 ;
           
          if(count2LedIndicate >9)
          {
          SET_LED(LED_GREEN);
          count2LedIndicate =0;
          }
          else
          {
            count2LedIndicate++;
          }
         for(int i=0;i<SENSOR_ARRAY_SIZE;i++)
         {
            String line = getSensorString(i);
            if(IS_BUTTON2_PRESSED())
            {
              SET_LED(LED_RED);
               SET_LED(LED_GREEN);
              Serial.print(line);
                Serial.flush();
                isDebugEnable = true;
            }

            writeDataToFile(line);

         }
       
          isDebugEnable = false;
          CLR_LED(LED_RED);
          CLR_LED(LED_GREEN);
    }

      LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);  

 

  // put your main code here, to run repeatedly:

}
