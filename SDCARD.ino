#include <SPI.h>
#include <SD.h>


String fileName = "";
String baseFileName = "F";


void initSDCard()
{
  
   //Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(SPI_CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
      SET_LED(LED_RED);
      SET_LED(LED_GREEN);
    //while (1);
  }
  else
  {
      createNewFile();

  }
 // Serial.println("card initialized.");

}

void createNewFileIfButtonPressed()
{
  bool isButtonPressed = false;
  static int buttonCounter =0;
  if(IS_BUTTON1_PRESSED())
  {
    buttonCounter++;
    if(buttonCounter > 20)
    {   buttonCounter = 0;
       isButtonPressed = true;
    }
  }
  else
  {
    buttonCounter = 0;
  }
  
   if(isButtonPressed )
  {
    createNewFile();
  }

  
}


void createNewFile()
{
    int index = 0;
    index = getLastFileIndex();

   /* Serial.print("Qnt of files:");
    Serial.println(index);
    */
    fileName = getFileName(index);

  for(int i=0;i<4;i++)
    {
      SET_LED(LED_RED);
      SET_LED(LED_GREEN);
      delay(500);
      CLR_LED(LED_RED);
      CLR_LED(LED_GREEN);
       delay(500);
    }
    
}
File dataFile;
bool openSDCard()
{
   bool ret = false;
   dataFile = SD.open(fileName, FILE_WRITE);
   if(dataFile) ret = true;
   return ret;
}

void closeSDCard()
{
      dataFile.close();
}

void writeLine(String &line)
{
  dataFile.print(line);
  Serial.print(line);
}

int writeDataToFile(String writeDataToFile)
{
  //char __dataFileName[fileName.length()];
    //fileName.toCharArray(__dataFileName, fileName.length());
    //Serial.println("NEw file created: " + fileName);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(writeDataToFile);
    dataFile.close();

    CLR_LED(LED_RED);
    CLR_LED(LED_GREEN);

   }
   else
   {
   // Serial.println("ERROR 2 Write");
    SET_LED(LED_RED);
    SET_LED(LED_GREEN);
    initSDCard();
   }
}


String getFileName(int index)
{
  String newName = baseFileName;
  newName += String(index);
  newName += ".cow";
  
  return newName;
}


int getLastFileIndex()
{
   File dir = SD.open("/");
// Begin at the start of the directory
  dir.rewindDirectory();
  int count =0;
  while(true) {
     File entry =  dir.openNextFile();
      //    Serial.print(entry.name());

     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     count++;
   
     entry.close();
   }
  dir.close();
   return count;
}
