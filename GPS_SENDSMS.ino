#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial A7board(10,11);// RX, TX

unsigned long   start           = 0;

String          GPSfix    ="";
String          GPSmentah ="";
boolean         GPS_ON    =false;
void setup()
{
 A7board.begin(9600);   // the GPRS baud rate   
 Serial.begin(9600);    // the GPRS baud rate 

 A7board.println("AT+CMGF=1\r"); // set SMS mode to text
 delay(1000);
 ShowSerialData();

 A7board.println("AT+CPIN?");  //get the signal Quality
 delay(1000);
 ShowSerialData();
}
 
void loop()
{ 
  Serial.println("GET DATA GPS");
  GPS_ON=true;
  GPS();
    
  /*Serial.println("Cek Sim");
  pengecekan ();*/

  /*Serial.println("SEND SMS");
  SendTextMessage();*/
}

void pengecekan ()
{
  A7board.println("AT+GPS=0");//tutup dulu GPS nya
  delay(500); 
    
  A7board.println("AT+CPIN?");  //get the signal Quality
  delay(500);
  ShowSerialData();
   
  A7board.println("AT+CREG?");
  delay(500);
  ShowSerialData();
   
  A7board.println("AT+CSQ");
  delay(500);
  ShowSerialData();  
}

void SendTextMessage()
{  
  A7board.println("AT+GPS=0");//tutup dulu GPS nya
  delay(500); 
  
  A7board.println("AT+CMGS=\"+62xxxxxxxxx\"");//send sms message, masukkan nomor telpon
  //A7board.println("AT + CMGS = \"" + sender_phone +"\"");
  delay(2000);
  ShowSerialData();
  
  A7board.println("GSM test message!");//the content of the message
  delay(500);
  ShowSerialData();
  
  A7board.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(500);
  ShowSerialData();
  A7board.println();

}//SendTextMessage()

void GPS()
{
  if (GPS_ON){
    //Serial.println("GPS turn on");  
    A7board.println("AT+GPS=1");//0 buat matikan
    delay(500); 
  
    A7board.println("AT+GPSRD=5");
    delay(500);//5000
    ShowSerialData();
    
    GPS_ON=false;
  }
  ShowDataGPS();
}//GPS()


void ShowDataGPS(){   
  int       awal              =0;
  int       akhir             =0;
  int       buffervalidasi    =0;
  int       indeksvalidasi    =0;
  uint16_t  latitude1         =0;
  float     latitude2         =0.0000;  
  float     latitude3         =0.0000; 
  uint16_t  longitude1        =0;
  float     longitude2        =0.0000;
  float     longitude3        =0.0000;

  String DataGPS          ="";
  String validasi         ="";
  String bufferstr        ="";
  String LatM             ="";
  String Lat1             ="";
  String Lat2             ="";
  String LongM            ="";
  String Long1            ="";
  String Long2            =""; 
  
  GPSfix                  ="";
  GPSmentah               ="";
  
  start = millis();
  do{
     while (A7board.available()>0)
     DataGPS+=(String) (char)A7board.read();
    }while (millis() - start <5000);//6000

  Serial.print("Start DataGPS");Serial.print(DataGPS);Serial.println("END DataGPS");
  
  awal  = DataGPS.indexOf("$GPRMC");
  //strGPRMC=DataGPS.substring(awal,awal+6); //Serial.print("strGPRMC=");Serial.println(strGPRMC);
  //Serial.print("index_awal=");Serial.println(awal);
  
  buffervalidasi=awal+6;
  indeksvalidasi=DataGPS.indexOf("000",buffervalidasi);
  validasi=DataGPS.substring(indeksvalidasi+4,indeksvalidasi+5); //Serial.print("validasi=");Serial.println(validasi);
  //Serial.print("index_validasi=");Serial.println(indeksvalidasi);
  

  akhir = DataGPS.indexOf(",,,",indeksvalidasi);
  
  if (validasi=="A")
  {
    Serial.println("GPS_VALID");

    GPSmentah=DataGPS.substring(awal,akhir+3);//kalo yg dikirim data mentah cukup (awal,akhir)
    //Serial.print("GPSmentah=");Serial.println(GPSmentah);

    //tanggal
    awal  =0;
    bufferstr="";
    awal  = GPSmentah.indexOf(",,,");
    bufferstr=GPSmentah.substring(awal-6,awal);//Serial.print("Tanggal=");Serial.println(bufferstr);
    GPSfix +="["+bufferstr+"," ;

    //jam
    akhir =0;
    bufferstr="";
    akhir  = GPSmentah.indexOf("$GPRMC");
    bufferstr=GPSmentah.substring(akhir+7,akhir+13);//Serial.print("Jam=");Serial.println(bufferstr);
    GPSfix +=bufferstr+"]" ;

    //lattitude
    awal  =0;
    akhir =0; 
    awal  = GPSmentah.indexOf('A');
    akhir = GPSmentah.indexOf('S');
    LatM=GPSmentah.substring(awal+2,akhir-1); //Serial.print("LatM=");Serial.println(LatM);
    awal  =0;
    awal  = LatM.indexOf('.');    
    Lat1=LatM.substring(0,awal-2);                       //Serial.print("Lat1=");Serial.println(Lat1);    
    Lat2=LatM.substring(awal-2,LatM.length());           //Serial.print("Lat2=");Serial.println(Lat2);
    latitude1=Lat1.toInt();                              //Serial.print("latitude1=");Serial.println(latitude1);
    latitude2=Lat2.toFloat();                 //Serial.print("latitude2=");Serial.println(latitude2); 
    latitude2 /=60.00;                        //Serial.print("latitudehasilbagi=");Serial.println(latitude2);
    latitude3 = latitude1+latitude2;          //Serial.print("latitude3=");Serial.println(latitude3);

    //Longitude
    awal  =0;
    awal  = GPSmentah.indexOf('E');
    LongM=GPSmentah.substring(akhir+2,awal-1);          //Serial.print("LongM=");Serial.println(LongM);
    akhir=0;
    akhir=LongM.indexOf('.');
    Long1=LongM.substring(0,akhir-2);                   //Serial.print("Long1=");Serial.println(Long1);
    Long2=LongM.substring(akhir-2,LongM.length());      //Serial.print("Long2=");Serial.println(Long2);
    longitude1=Long1.toInt();                           //Serial.print("longitude1=");Serial.println(longitude1);
    longitude2=Long2.toFloat();                         //Serial.print("longitude2=");Serial.println(longitude2);
    longitude2 /=60.00;
    longitude3 = longitude1+longitude2;                 //Serial.print("longitude3=");Serial.println(longitude3);
    GPSfix += "<-"+String (latitude3,6)+","+String(longitude3,6)+">";
    
    Serial.print("GPSfix=");Serial.println(GPSfix);   
  }
  else{
    Serial.println("GPS_NOT_VALID");
    //ShowSerialData();
  }

}//showdataGPS

void ShowSerialData()
{
  while( A7board.available()!=0){
    Serial.write( A7board.read());}

    //Serial.println();
}
