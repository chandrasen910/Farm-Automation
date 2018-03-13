#include <SoftwareSerial.h>
int moisture = A0 , i , temp , k=0;
int MoistureTreshold1 = 700;
int MoistureTreshold2 = 500;
char str[15],inChar;
  uint32_t ts1,ts2,ts3 =0;
SoftwareSerial mySerial(9,10);


void setup()
{ pinMode(moisture, INPUT);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(3, LOW);
}



void loop()
{
  int analogSensor = analogRead(moisture);
                                                   RecieveMessage();
                                                    if(temp==1)
                                                      {
                                                        check();
                                                        temp=0;
                                                        i=0;
                                                        delay(1000);
                                                      }  
 if (analogSensor > MoistureTreshold1 && k==0)
  SendMessage1();
   if (analogSensor > MoistureTreshold1 && k==1 && ts3 >600000)
  SendMessage1();
 ts1 = millis();  
 if (analogSensor < MoistureTreshold2 && k==1 )
  SendMessage2();
  ts3 = ts2-ts1;
}

                                                    void RecieveMessage()
                                                                        {
                                                                          mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
                                                                          delay(500);
                                                                           if(mySerial.available() >0)
                                                                           {
                                                                            inChar=mySerial.read();
                                                                            Serial.println(mySerial.read());
                                                                                                            if(Serial.find("#A."))
                                                                                                              {
                                                                                                                digitalWrite(13, HIGH);
                                                                                                                delay(500);
                                                                                                                digitalWrite(13, LOW);
                                                                                                                 while (Serial.available()) 
                                                                                                                 {
                                                                                                                   char inChar=Serial.read();
                                                                                                                  str[i++]=inChar;
                                                                                                                   if(inChar=='*')
                                                                                                                     {
                                                                                                                       temp=1;
                                                                                                                       return;
                                                                                                                     } 
                                                                                                                  } 
                                                                                                                }
                                                                             }
                                                                         }
 
                                    void check()
                                                 {  if(!(strncmp(str,"all on",6)))
                                                      {
                                                       digitalWrite(3 , HIGH);
                                                       delay(200);
                                                        
                                                        }
                                                   else if(!(strncmp(str,"all off",7)))
                                                      {
                                                         digitalWrite(3 , LOW);
                                                         delay(200);
                                   
                                                       }
                                                  }

void SendMessage1()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917330880274\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("the water level is too low!");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  k=1;
  uint32_t ts2 = millis();
}
void SendMessage2()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917330880274\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("the water level is adequate");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  k=0;
}
