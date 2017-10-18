#include <SoftwareSerial.h>
SoftwareSerial A7board(10,11);// RX, TX

void setup() {
Serial.begin(9600);
A7board.begin(9600);
GPS();
}

void loop() {

ShowSerialData();
}
void GPS()
{//GPS()
//Serial.println("AT+GPS turn on"); 
A7board.println("AT+GPS=1"); 
delay(500);
A7board.println("AT+GPSRD=2"); 
delay(500);
ShowSerialData();
}//GPS()

void ShowSerialData()
{
while( A7board.available()!=0){
Serial.write( A7board.read());}

//Serial.println();
}
