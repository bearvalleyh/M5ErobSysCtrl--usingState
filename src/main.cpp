#include <Arduino.h>
#include <M5Core2.h>
#include <math.h>
#include "Elib.h"
#include "PinAssign.h"
#include "SystemController.h"
#include "Logging.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
// MultiTask setting
#define SENSORTASK_CORE 0
#define SENSORTASK_PRI 1
#define LOGTASK_CORE 1
#define LOGTASK_PRI 20

#define MAX_DATA_SIZE 10
int log_data[2][MAX_DATA_SIZE]; // ダブルバッファ
bool logged = true;
void prvGetSensorTask(void *pvParameters);
void prvWriteLogTask(void *pvParameters);
void logging();


int32_t enc0 = 0;
int32_t enc1 = 0;
uint16_t tof0 = 0;
uint16_t tof1 = 0;
uint16_t tof2 = 0;
uint16_t tof3 = 0;
bool photo0 = false;
bool photo1 = true;

// Bluetooth LE
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


///////////////
bool updateColor;
String lastPushedButton;
bool updateLog;
String lastLoggedFileInfo;
String lastLoggedText;
void initBLE();
void loopBLE();
void initLCDcolor();
void loopLCDcolor();
void loopSendMessage();

int double_buffer; // 切り替え用
TaskHandle_t h_WriteLogTask;


// #include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

//const char* _classname = "main";


const int M5_SHUTDOWN_DELAY = 5000; // ms

const uint8_t SCREEN_X = 0;
const uint8_t SCREEN_Y = 0;
const uint8_t LINE_HEIGHT = 20;

SystemController* systemController = NULL;

#include "BMP_UtilIcon.cpp"

void setup() {
  { /* 共通初期化 */
    M5.begin();
    M5.Lcd.clear();
    M5.Lcd.setCursor(SCREEN_X, SCREEN_Y);
    M5.Lcd.setTextSize(2);

    M5.Lcd.qrcode("https://www.fujifilm.com/fb/");

    delay(5000);
    M5.Lcd.clear();

  }

  //M5.Lcd.pushImage(0, 0, 180, 180, img);

  //while (M5.BtnA.isReleased())
  //{
  //  M5.update();
  //}
  //{ /* 制御クラス初期化 */
  //  if (!SystemController::configure()) M5.shutdown(M5_SHUTDOWN_DELAY);
  //}
    // ログ表示領域
    //M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, DARKCYAN, BLACK)); 
    M5.Lcd.fillRect(7, 22, 306, 104, M5.Lcd.alphaBlend(128, DARKCYAN, BLACK)); 
    displayInfo displayInfo {7, 22, 306, 104, M5.Lcd.alphaBlend(128, DARKCYAN, BLACK)};

    // ヘッダ
    ViewDesigner::headerView("main", DARKCYAN);

    //ボタンの枠
    ViewDesigner::buttonView(); 

    // sensor値表示領域
    ViewDesigner::initSensorValView();

    Logging::configure(INFO);
    Logging::setDisplayInfo(displayInfo);
    DEBUG_START();

    initBLE();
    initLCDcolor();

  // キーボード表示
  ViewDesigner::initKeyboardView();

  
  logged = true;

  xTaskCreatePinnedToCore(prvWriteLogTask, "WriteLogTask", 4096, (void *)log_data, LOGTASK_PRI, &h_WriteLogTask, LOGTASK_CORE);

}

#if 0
void loop() {

  M5.update();
  //INFO_LOG("loop");

  // sensor値表示領域 値更新
  ViewDesigner::updateSensorValView();
  loopBLE();
  loopLCDcolor();

  //SystemController::update();

}

#endif

#if 1


void loop() {
  ViewDesigner::updateSensorValView();
  loopBLE();
  loopLCDcolor();
  delay(100);
  if (!logged)
  {
    ViewDesigner::updateSensorValView(enc0,enc1,tof0,tof1,tof2,tof3,photo0,photo1);
    logged = !logged;
    delay(100);
    //INFO_LOG("sensor update");
  }

  //ViewDesigner::updateSensorValView(0,0,0,0,0,0,true,false);
#if 0
  M5.Lcd.fillRect(7, 22, 306, 104, M5.Lcd.alphaBlend(128, DARKCYAN, BLACK)); 
  M5.Lcd.setCursor(10,24);
  M5.Lcd.println("1: UturnSceneInfo(5, 10,15)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("2: BackSceneInfo(5)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("3: ToFSceneEndJudgeInfo(30)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("4: BackStraghtMotionInfo(-30, -30)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("5: GoDiagonalLRMotionInfo(20, 30)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("6: TurnMotionInfo(0, 45)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("7: GoStraghtMotionInfo(30, 30)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("8: ToFSceneEndJudgeInfo(30)");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
  M5.Lcd.println("9: EXIT");
  M5.Lcd.setCursor(10,M5.Lcd.getCursorY()+2);
#endif
  //ViewDesigner::updateKeyboardView();
  //int infoNum = ViewDesigner::updateVal;
  //switch (infoNum)
  //{
  //case 1:
  //  ViewDesigner::updateKeyboardView();
  //  break;
  //
  //default:
  //  break;
  //}
  //char buf[256];
  //snprintf(buf, sizeof(buf), "val: %d", ViewDesigner::updateVal);
  //Logging::record(NONE, __FILENAME__, __LINE__, buf);
  //M5.Lcd.println(buf);

  //delay(2000);


}
#endif

/*****  ログ記録  *****/


void prvWriteLogTask(void *pvParameters)
{
  int *ld;
  while (1)
  {
    //loopBLE();
    loopSendMessage();
    if (logged)
    {
      //ViewDesigner::updateSensorValView();
      //ViewDesigner::updateSensorValView(0,0,0,0,0,0,true,false);
      enc0 = 100;
      enc1 = 100;
      tof0 = 100;
      tof1 = 100;
      tof2 = 100;
      tof3 = 100;
      photo0 = !photo0;
      photo1 = !photo1;
    }
    delay(1);  // これ大事
    logged = !logged;
  }
  //logged = !logged;
}

///////////////////
// Bluetooth LE  //
///////////////////
BLEServer *pServer = NULL;
BLECharacteristic * pNotifyCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

#define LOCAL_NAME                  "M5Stack-Color"
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                "39b1ae62-bfcd-11ed-afa1-0242ac120002"
#define CHARACTERISTIC_UUID_RX      "39b1b2fe-bfcd-11ed-afa1-0242ac120002"
#define CHARACTERISTIC_UUID_NOTIFY  "39b1b448-bfcd-11ed-afa1-0242ac120002"


// Bluetooth LE Change Connect State
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


// Bluetooth LE Recive
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        // iPhoneからデータ受信
        String cmd = String(rxValue.c_str());
        Serial.print("Received Value: ");
        Serial.println(cmd);
        if (cmd == "Button A")
        {
          // ToDo: ボタンAが押された時の処理を追加
          lastPushedButton = "Button A";
          updateColor = true;
        }
        if (cmd == "Button B")
        {
          // ToDo: ボタンBが押された時の処理を追加
          lastPushedButton = "Button B";
          updateColor = true;
        }
        if (cmd == "Button C")
        {
          // ToDo: ボタンCが押された時の処理を追加
          lastPushedButton = "Button C";
          updateColor = true;
        }
      }
    }
};


// Bluetooth LE initialize
void initBLE() {
  // Create the BLE Device
  BLEDevice::init(LOCAL_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pNotifyCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_NOTIFY,
                        BLECharacteristic::PROPERTY_NOTIFY
                        );
  
  pNotifyCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

// Bluetooth LE loop
void loopBLE() {
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("startAdvertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

///////////////
// LCD Color //
///////////////
void initLCDcolor() {
  lastPushedButton = "NONE";
}

void loopLCDcolor() {
  if (M5.BtnA.wasPressed())
  {
    lastPushedButton = "Button A";
    updateColor = true;
  }
  if (M5.BtnB.wasPressed())
  {
    lastPushedButton = "Button B";
    updateColor = true;
  }
  if (M5.BtnC.wasPressed())
  {
    lastPushedButton = "Button C";
    updateColor = true;
  }
  
  if (updateColor) {
    if (lastPushedButton == "Button A")
    {
      // RED
      //M5.Lcd.fillScreen(RED);
      INFO_LOG("BtnA was pressed");
    }
    if (lastPushedButton == "Button B")
    {
      // YELLOW
      //M5.Lcd.fillScreen(YELLOW);
      INFO_LOG("BtnB was pressed");
    }
    if (lastPushedButton == "Button C")
    {
      // BLUE
      //M5.Lcd.fillScreen(BLUE);
      INFO_LOG("BtnC was pressed");
    }
    // ToDo: iPhone側に必要なデータを送信する
    if (deviceConnected) {
      char sendMessage[256];
      lastPushedButton.toCharArray(sendMessage, 256);
      pNotifyCharacteristic->setValue(sendMessage);
      pNotifyCharacteristic->notify();
    }
    updateColor = false;
  }
}

void loopSendMessage() {
  if (updateLog)
  {
    // ToDo: iPhone側に必要なデータを送信する
    if (deviceConnected) {
      char sendMessage1[256];
      char sendMessage2[256];
      if (sizeof(lastLoggedFileInfo) > 0)
      {
        lastLoggedFileInfo.toCharArray(sendMessage1, 256);
        pNotifyCharacteristic->setValue(sendMessage1);
        pNotifyCharacteristic->notify();
      }
      if (sizeof(lastLoggedText))
      {
        lastLoggedText.toCharArray(sendMessage2, 256);
        pNotifyCharacteristic->setValue(sendMessage2);
        pNotifyCharacteristic->notify();
      }
    }
    lastLoggedFileInfo = "";
    lastLoggedText = "";
    updateLog = false;
  }
}
