#include "NimBLEDevice.h"
#include "M5StickCPlus2.h"

#include "ssFont.h"
#include "mFont.h"
#include "hFont.h"
#include "logo.h"
M5Canvas img(&StickCP2.Display);
M5Canvas clc(&StickCP2.Display);

// The remote service we wish to connect to.
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

BLEClient *pClient;

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
std::string add[4]={"a4:c1:38:fb:64:94","a4:c1:38:0e:f9:68","a4:c1:38:e2:96:ab","a4:c1:38:6f:b8:60"};
 String rooms[4]={"KITCHEN", "FLOOR 2","OUTSIDE","BEDROOM"};
/*
Name: LYWSD03MMC, Address: a4:c1:38:0e:f9:68, rssi: -94, serviceData: gore
Name: LYWSD03MMC, Address: a4:c1:38:6f:b8:60, rssi: -72, serviceData:   soba
Name: LYWSD03MMC, Address: a4:c1:38:83:00:fa, rssi: -67, serviceData:   kupaona

Name: LYWSD03MMC, Address: a4:c1:38:e2:96:ab, rssi: -92, serviceData:  vani
Name: LYWSD03MMC, Address: a4:c1:38:fb:64:94, rssi: -98, serviceData:  dnevni
*/

  float temp[4];
  float humi[4];
  float voltage[4];
 
  bool ch=0;

  int posx[4]={5,90,5,90};
  int posy[4]={5,5,70,70};
  unsigned short blue=0x024E;
  unsigned short blue2=0x01EC; 
  unsigned short blue3=0x469F;
  unsigned short grays[13];
  int vol=0;
  int volE; 
  unsigned long tt=0;
  unsigned long tt2=0;
  int period=1000;
  int s=0;
  bool freeL[4]={1,1,1,1};

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for SENSOR 0 ");
  freeL[0]=0;
  temp[0] = (pData[0] | (pData[1] << 8)) * 0.01; //little endian
  humi[0] = pData[2];
  voltage[0] = (pData[3] | (pData[4] << 8)) * 0.001; //little endian
  Serial.printf("temp = %.1f C ; humidity = %.1f %% ; voltage = %.3f V\n", temp[0], humi[0], voltage[0]);
  Serial.println();
  freeL[0]=1;
}

static void notifyCallback2(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for SENSOR 1 ");
  
  freeL[1]=0;
  temp[1] = (pData[0] | (pData[1] << 8)) * 0.01; //little endian
  humi[1] = pData[2];
  voltage[1] = (pData[3] | (pData[4] << 8)) * 0.001; //little endian
  Serial.printf("temp = %.1f C ; humidity = %.1f %% ; voltage = %.3f V\n", temp[1], humi[1], voltage[1]);

  Serial.println();
  freeL[1]=1;
}

static void notifyCallback3(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for SENSOR 2 ");
  freeL[2]=0;
  temp[2] = (pData[0] | (pData[1] << 8)) * 0.01; //little endian
  humi[2] = pData[2];
  voltage[2] = (pData[3] | (pData[4] << 8)) * 0.001; //little endian
  Serial.printf("temp = %.1f C ; humidity = %.1f %% ; voltage = %.3f V\n", temp[2], humi[2], voltage[2]);
  
  Serial.println();
  freeL[2]=1;
}

static void notifyCallback4(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for SENSOR 3 ");
  freeL[3]=0;
  
  temp[3] = (pData[0] | (pData[1] << 8)) * 0.01; //little endian
  humi[3] = pData[2];
  voltage[3] = (pData[3] | (pData[4] << 8)) * 0.001; //little endian
  Serial.printf("temp = %.1f C ; humidity = %.1f %% ; voltage = %.3f V\n", temp[3], humi[3], voltage[3]);
 
  freeL[3]=1;
  Serial.println();
}

bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our service");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");

  // Read the value of the characteristic.
  std::string value = pRemoteCharacteristic->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());

  if(pClient->getPeerAddress().toString().c_str()==add[0])
  pRemoteCharacteristic->registerForNotify(notifyCallback);

  if(pClient->getPeerAddress().toString().c_str()==add[1])
  pRemoteCharacteristic->registerForNotify(notifyCallback2);

   if(pClient->getPeerAddress().toString().c_str()==add[2])
  pRemoteCharacteristic->registerForNotify(notifyCallback3);

     if(pClient->getPeerAddress().toString().c_str()==add[3])
  pRemoteCharacteristic->registerForNotify(notifyCallback4);

  connected = true;//added
  return true;//added
}


void setup() {

 auto cfg = M5.config();
    StickCP2.begin(cfg);
    //StickCP2.Rtc.setDateTime( { { 2023, 12, 30 }, { 22, 43, 0 } } );
   
    StickCP2.Display.setBrightness(38);
    StickCP2.Display.setRotation(3);
    StickCP2.Display.fillScreen(RED);
    img.createSprite(170,135);
    clc.createSprite(60,125);
    clc.setSwapBytes(true);
  
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

     int co=220;
     for(int i=0;i<13;i++)
     {grays[i]=StickCP2.Display.color565(co, co, co);
     co=co-20;}

  connectToServer(BLEAddress(add[0]));
  connectToServer(BLEAddress(add[1]));
  connectToServer(BLEAddress(add[2]));
  connectToServer(BLEAddress(add[3]));
  StickCP2.Display.fillScreen(BLACK);

} // End of setup.


const uint8_t notificationOff[] = {0x0, 0x0};
const uint8_t notificationOn[] = {0x1, 0x0};
bool onoff = true;

void drawCL()
{
clc.fillSprite(BLACK);
clc.setTextDatum(0);
clc.setTextColor(grays[0],BLACK);
auto dt = StickCP2.Rtc.getDateTime();
clc.loadFont(hFont);
clc.drawString(String(dt.time.hours)+":"+String(dt.time.minutes),0,0);
clc.unloadFont();
clc.pushImage(0,65,60,60,logo);
clc.fillSmoothRoundRect(36,26,24,20,2,grays[8]);
clc.drawRect(0,26,29,20,grays[1]);
clc.fillRect(29,30,3,12,grays[1]);

for(int i=0;i<volE;i++)
clc.fillRect(3+(i*5),29,3,14,GREEN);

clc.loadFont(ssFont);
clc.setTextColor(grays[0],grays[8]);
clc.setTextDatum(middle_center);
clc.drawString(String(dt.time.seconds),48,36);
clc.setTextDatum(0);
clc.setTextColor(RED,BLACK);
clc.drawString("VOLOS",0,48);

clc.unloadFont();
clc.pushSprite(175,5);
delay(5);
}

void draw()
{
  img.fillSprite(BLACK);
 
  for(int i=0;i<4;i++)
  {
    img.fillSmoothRoundRect(posx[i],posy[i],80,60,3,blue);
    
    img.fillSmoothCircle(posx[i]+10,posy[i]+25,2,ORANGE);
    img.setTextDatum(middle_center);
    img.setTextColor(WHITE,blue);
    img.loadFont(mFont);
    img.drawString(String(temp[i]),posx[i]+50,posy[i]+32);
    img.unloadFont();
    img.setTextDatum(0);
    
    img.loadFont(ssFont);
    img.setTextColor(blue3,blue);
    img.drawString(String((int)humi[i])+"%",posx[i]+4,posy[i]+45);
    img.setTextColor(grays[1],blue);
    img.drawString(rooms[i],posx[i]+4,posy[i]+2);
    img.unloadFont();
    
    img.fillSmoothRoundRect(posx[i]+42,posy[i]+46,38,14,2,BLACK);
    img.setTextColor(grays[1],BLACK);
    img.drawString(String(voltage[i])+"V",posx[i]+46,posy[i]+50);
    img.drawFastHLine(posx[i]+4,posy[i]+18,60,0x66DF);
  }
  img.pushSprite(0,0);
  delay(10);
}


void loop() {

   vTaskDelay(20);

    if(millis()>tt2+period){
    s++;
    if(s==60) s=0; 
    drawCL();
    tt2=millis();  
    }
    
    if(millis()>tt+period){
    vol = StickCP2.Power.getBatteryVoltage();
    volE=map(vol,3000,4180,0,5); 
    if(freeL[0]==1 && freeL[1]==1 && freeL[0]==1 && freeL[3]==1)
    draw();
    tt=millis();
    }
}