#include <EEPROM.h>
//#include <SD.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <avr/pgmspace.h>
HardwareSerial Uart = HardwareSerial();

unsigned long currTimestamp; //used to store current timstamp for timing purposes
unsigned long timestamps[17];  //tracks next check of various pins or functions for polling timing
//#define btStat digitalRead(25)  // 0=connected, 1=disconnected (hardware GPIO connect status from BT module)


# define btInitElements 9
char btstring_0[] PROGMEM =  "BC:PF=00,01,00";
char btstring_1[] PROGMEM =  "BC:FT=00,00,0A,0A";
char btstring_2[] PROGMEM =  "BC:BP=00,00";             
char btstring_3[] PROGMEM =  "BC:NM=BTmodule"; 
char btstring_4[] PROGMEM =  "IO=03"; 
char btstring_5[] PROGMEM =  "BC:MM=00";  
char btstring_6[] PROGMEM =  "BC:SN=01";  
char btstring_7[] PROGMEM =  "BC:UI=01"; 
char btstring_8[] PROGMEM =  "BC:MD=01"; 
/*
// Old module commands:
# define btInitElements 6
char btstring_0[] PROGMEM =  "BC:PF=00,01,00";
char btstring_1[] PROGMEM =  "BC:FT=00,00,0A,0A";
char btstring_2[] PROGMEM =  "BC:BP=00,00";             
char btstring_3[] PROGMEM =  "BC:NM=MyBTold"; 
char btstring_4[] PROGMEM =  "BC:PN=0000"; 
char btstring_5[] PROGMEM =  "BC:MM=00";  
*/
//char btstring_7[] PROGMEM =  ""; 
//char btstring_8[] PROGMEM =  "";             
char btstring_9[] PROGMEM =  "";           
char btstring_10[] PROGMEM =  ""; 
char btstring_11[] PROGMEM =  ""; 
char btstring_12[] PROGMEM =  ""; 
char btstring_13[] PROGMEM =  ""; 
char btstring_14[] PROGMEM =  ""; 
char btstring_15[] PROGMEM =  ""; 
char btstring_16[] PROGMEM =  ""; 
char btstring_17[] PROGMEM =  ""; 

PROGMEM const char *btstring_table[] =
{
btstring_0,
btstring_1,
btstring_2,
btstring_3,
btstring_4,
btstring_5,
btstring_6,
btstring_7,
btstring_8,
btstring_9,
btstring_10,
btstring_11,
btstring_12,
btstring_13,
btstring_14,
btstring_15,
btstring_16,
btstring_17
};

void setup(){
  Serial.begin(38400);
  Uart.begin(9600);
  digitalWrite(5,HIGH); 
  digitalWrite(6,HIGH); 
}


void readUart(void){
  byte infofound=0; 
  byte bctr=0;
  byte fieldptr=0;
  byte byteread;
  uint8_t uartBuffer[4][30];
  unsigned long temptime = millis() + 150;

  while( temptime > millis() && 13!=byte(byteread) ){
    if(Uart.available()){
      byteread =  Uart.read() ;
      infofound=1;
      Serial.print(char(byteread));
       
      if(byteread==44 || byteread==58 || byteread==61 || byteread==13){ //comma, colon, equal or <CR>
        uartBuffer[fieldptr][bctr]='\0';
        if(fieldptr < 4) fieldptr++;
        bctr=0;
        }
      else if(byteread!=32 && byteread!=10){    //  << Filter out spaces and Line Feed characters

        uartBuffer[fieldptr][bctr] = byteread; 
        bctr++;
        }

      temptime = millis() + 150;
      }
    }
  if(infofound) Serial.println("");

/*

  //If we have received a new successful connection, we need to
  //"push" the remote host address this onto the host stack and stop looking for other devices
  if(fieldptr > 1){
    if( uartBuffer[0][0]==73 && uartBuffer[0][1]==83 &&     // IS
        uartBuffer[1][0]==48 && uartBuffer[1][1]==50 &&     // 02
        btDiscoverableStat == 1){                     // only remember if BT was discoverable
      ledDelay[1]=0;
      serviceLED(1);
      Serial.println("Successful Connect");
      btConnectID = 8;  // stop looking for additional hosts to connect to
      storeBTaddr(&uartBuffer[2][0]);


      btenable=1;
      timestamps[16] = temptime + 3000; //schedule a bluetooth disconnect
      }
    }
*/
  }


void InitBT(void){
  
  byte cmdptr=0;
  char buffer[20];

  for(cmdptr=0;cmdptr<=btInitElements;cmdptr++){
    strcpy_P(buffer, (char*)pgm_read_word(&(btstring_table[cmdptr])));

    if( !strcmp("WAIT",buffer) ){
      delay(2000);
      }

    else {
      Serial.println( buffer);
      Uart.println( buffer );
      }

    delay(100);
    readUart();
    }
  Serial.println("");
  Serial.println("Bluetooth Initialization Complete.");
  }




void loop() {
  delay(2000);
  InitBT();
  Uart.println("BC:IQ");
  
  while(1){
    currTimestamp = millis();



  //see if any new events are waiting on the UART interface from the bluetooth module to be read
//  if(currTimestamp > timestamps[10]){
//    timestamps[10] = currTimestamp + 100;
    readUart();
//    }



  }
}
