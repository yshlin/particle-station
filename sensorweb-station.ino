// This #include statement was automatically added by the Particle IDE.
#include "HttpClient/HttpClient.h"
#define INTERVAL 5000
#define BUFLENG 32
char buf[BUFLENG];

int PM1Value=0;          //define PM1.0 value of the air detector module
int PM2_5Value=0;         //define PM2.5 value of the air detector module
int PM10Value=0;          //define PM10 value of the air detector module

unsigned int nextTime = 0;    // Next time to contact the server
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;


void setup() {
//   pinMode(led1, OUTPUT);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  Serial1.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // send data only when you receive data:
  if(Serial1.available()) {
    Serial1.readBytes(buf,BUFLENG);
    if(buf[0] == 0x42 && buf[1] == 0x4d){
      if(checkValue(buf,BUFLENG)){
        PM1Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
        PM10Value=transmitPM10(buf); //count PM10 value of the air detector module 
        unsigned long now = millis();
        if (nextTime > millis()) {
          return;
        }
        Particle.variable("PM1", PM1Value);
        Particle.variable("PM2_5", PM2_5Value);
        Particle.variable("PM10", PM10Value);
        // Serial.print("PM1.0: ");  //send PM1.0 data to bluetooth
        // Serial.print(PM01Value);
        // Serial.println("  ug/m3");            
        
        // Serial.print("PM2.5: ");  //send PM1.0 data to bluetooth
        // Serial.print(PM2_5Value);
        // Serial.println("  ug/m3");     

        // Serial.print("PM10:  ");  //send PM1.0 data to bluetooth
        // Serial.print(PM10Value);
        // Serial.println("  ug/m3"); 
        nextTime = millis() + 10000;
      }           
    } 
  }
}

char checkValue(char *thebuf, char leng)
{  
  char receiveflag=0;
  int receiveSum=0;
  char i=0;
 
  for(i=0;i<leng;i++)
  {
  receiveSum=receiveSum+thebuf[i];
  }
    
  if(receiveSum==((thebuf[leng-2]<<8)+thebuf[leng-1]+thebuf[leng-2]+thebuf[leng-1]))  //check the serial data 
  {
    receiveSum=0;
    receiveflag=1;
  }
  return receiveflag;
}

int transmitPM01(char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[4]<<8) + thebuf[5]); //count PM1.0 value of the air detector module
  return PM01Val;
}
 
//transmit PM Value to PC
int transmitPM2_5(char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[6]<<8) + thebuf[7]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }
 
//transmit PM Value to PC
int transmitPM10(char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[8]<<8) + thebuf[9]); //count PM10 value of the air detector module  
  return PM10Val;
}

// For httpclient usage reference
/*
void loop() {
    if (nextTime > millis()) {
        return;
    }

    Serial.println();
    Serial.println("Application>\tStart of Loop.");
    // Request path and body can be set at runtime or at setup.
    request.hostname = "www.timeapi.org";
    request.port = 80;
    request.path = "/utc/now";

    // The library also supports sending a body with your request:
    //request.body = "{\"key\":\"value\"}";

    // Get request
    http.get(request, response, headers);
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);

    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);

    nextTime = millis() + 10000;
}
*/
