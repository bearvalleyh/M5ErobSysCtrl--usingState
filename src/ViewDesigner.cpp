#include <ViewDesigner.h>


int ViewDesigner::inputVal = 0;
int ViewDesigner::updateVal = 0;
bool ViewDesigner::keyPressed = false;
const char ViewDesigner::num[] = {'0','1','2','3','4','5','6','7','8','9'};
const String ViewDesigner::boolean[] = {"false","true"};

// 通知
void ViewDesigner::notifyView(const char* classname, int backGroundColor, int textColor)
{
    M5.Lcd.fillScreen(backGroundColor);
    M5.Lcd.setTextColor(textColor);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.drawString(classname, 160, 120, 2); 
}

void ViewDesigner::notifyLED()
{
    // Lチカで通知
    for (int i=0; i<5; i++)
    {
        M5.Axp.SetLed(0);
        delay(100);
        M5.Axp.SetLed(1);
        delay(100);
    }
}

void ViewDesigner::buttonView(const char* btnA, const char* btnB, const char* btnC)
{
    //ボタンの枠
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.fillRect(7, 216, 100, 18, DARKGREY); 
    M5.Lcd.drawRect(7, 216, 100, 18, WHITE); 
    M5.Lcd.drawString(btnA, 57, 225, 2); 
    M5.Lcd.fillRect(110, 216, 100, 18, DARKGREY); 
    M5.Lcd.drawRect(110, 216, 100, 18, WHITE); 
    M5.Lcd.drawString(btnB, 160, 225, 2); 
    M5.Lcd.fillRect(213, 216, 100, 18, DARKGREY); 
    M5.Lcd.drawRect(213, 216, 100, 18, WHITE); 
    M5.Lcd.drawString(btnC, 263, 225, 2); 
}

void ViewDesigner::buttonExplainView(String btnA, String btnB, String btnC)
{
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.drawString("BtnA... " + btnA, 160, 160, 2); 
    M5.Lcd.drawString("BtnB... "+ btnB, 160, 180, 2); 
    M5.Lcd.drawString("BtnC... "+ btnC, 160, 200, 2); 
}

void ViewDesigner::buttonPressedView(short btn, int backGroundColor)
{
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, backGroundColor, BLACK)); 
    if (btn==0) M5.Lcd.drawString("BtnA was Pressed.", 160, 86, 2); 
    if (btn==1) M5.Lcd.drawString("BtnB was Pressed.", 160, 86, 2); 
    if (btn==2) M5.Lcd.drawString("BtnC was Pressed.", 160, 86, 2); 
}


void ViewDesigner::headerView(const char* classname, int backGroundColor)
{
    // ヘッダ的な
    M5.Lcd.fillRect(0, 0, 320, 16, backGroundColor); 
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(TL_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.setCursor(0,0);
    #ifdef M5_DEBUG
    M5.Lcd.print("Debug Mode: "); 
    #else
    M5.Lcd.print("Release Mode: "); 
    #endif
    M5.Lcd.print(classname); 
    M5.Lcd.setTextDatum(TR_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.drawString(__DATE__, 312, 0, 2); 
}

/* テスト用入力キーボード表示 */
void ViewDesigner::initKeyboardView()
{
    //M5.Lcd.setCursor(10, 40);
    //M5.Lcd.print("Enter a number:");

    // display 0-9 numeric keyboard
    //for(int i=0; i<10; i++){
    //  M5.Lcd.drawRect(20 + (i%5)*45, 90 + (i/5)*45, 40, 40, WHITE);
    //  M5.Lcd.setCursor(35 + (i%5)*45, 103 + (i/5)*45);
    //  M5.Lcd.print(i);
    //}
    for(int i=0; i<10; i++){
      M5.Lcd.drawRect(7 + (i%5)*(36+5), (126+7+3) + (i/5)*(36+5), 36, 36, WHITE);
      M5.Lcd.setCursor(7+15 + (i%5)*(36+5), (126+7+3)+(18-5) + (i/5)*(36+5));
      M5.Lcd.print(i);
    }
}

/* テスト用入力キーボード表示 */
void ViewDesigner::updateKeyboardView()
{
    while(1)
    {
    //INFO_LOG("Enter a number:");
    
    // wait for user input
    while(!keyPressed){
    //while(!M5.BtnC.wasPressed()){
      if(M5.Touch.ispressed()){
        TouchPoint_t touch = M5.Touch.getPressPoint();
        int touchX = touch.x;
        int touchY = touch.y;
        for(int i=0; i<10; i++){
          //if(touchX > 20 + (i%5)*45 && touchX < 60 + (i%5)*45 && touchY > 90 + (i/5)*45 && touchY < 130 + (i/5)*45){
          if(touchX > 7 + (i%5)*(36+5) && touchX < (7+36) + (i%5)*(36+5) && touchY > (126+7+3) + (i/5)*(36+5) && touchY < (126+7+3)+18+7 + (i/5)*(36+5)){
            if (inputVal > 0) inputVal = inputVal*10 + i;
            else inputVal = i;
            keyPressed = true;
            break;
          }
        }
      }
      //delay(100);
    }

    // clear screen and display input value
    //M5.Lcd.clear();
    M5.Lcd.fillRect(7, 126, 206, 10, BLACK); // clear inputVal area; 

    M5.Lcd.setCursor(7, 126+2);
    M5.Lcd.printf("Input Val: %d ", inputVal);
    //M5.Lcd.setCursor(10, 100);
    //M5.Lcd.print(inputVal);

    //char buf[256];
    //sprintf(buf, "Input Value: %d", inputVal);
    //INFO_LOG(buf);

    // reset flag and wait for user input again
    M5.Lcd.printf("Please push btn.");
    buttonView("continue", "done", "clear");
    while(1)
    {
        M5.update();
        if (M5.BtnA.wasPressed())
        {
            M5.Lcd.fillRect(7, 126, 206, 10, BLACK); // clear inputVal area; 
            M5.Lcd.setCursor(7, 126+2);
            M5.Lcd.printf("continue");
            delay(100);
            M5.update();
            break;
        }
        if (M5.BtnB.wasPressed())
        {
            M5.Lcd.fillRect(7, 126, 206, 10, BLACK); // clear inputVal area; 
            M5.Lcd.setCursor(7, 126+2);
            M5.Lcd.printf("done");
            updateVal = inputVal;
            inputVal = 0;
            keyPressed = false;
            buttonView();
            M5.Lcd.fillRect(7, 126, 206, 10, BLACK); // clear inputVal area; 
            delay(100);
            M5.update();
            return;
        }
        if (M5.BtnC.wasPressed()) 
        {
            inputVal = 0;
            M5.Lcd.fillRect(7, 126, 206, 10, BLACK); // clear inputVal area; 
            M5.Lcd.setCursor(7, 126+2);
            M5.Lcd.printf("clear");
            delay(100);
            M5.update();
            break;
        }

        delay(100);
    }
    
    keyPressed = false;
    buttonView();
    delay(1000);
    M5.Lcd.fillRect(7, 126, 206, 10, BLACK); // clear inputVal area; 
    M5.Lcd.setCursor(7, 126+2);
    M5.Lcd.printf("Current Val: %d ", inputVal);
    }
}

/* センサ出力値表示 */
void ViewDesigner::initSensorValView()
{
    // 枠
    //M5.Lcd.fillRect(213, 126, 100, 90, M5.Lcd.alphaBlend(128, WHITE, BLACK)); 
    //M5.Lcd.drawRect(213, 126, 100, 90, WHITE); 
    M5.Lcd.drawFastHLine(213, 135, 100, DARKGREY);
    M5.Lcd.drawFastVLine(213, 126, 90, DARKGREY);
    M5.Lcd.drawFastVLine(263, 126, 90, DARKGREY);
    M5.Lcd.drawFastVLine(313, 126, 90, DARKGREY);

    // 左ペイン
    M5.Lcd.setCursor(213, 126);
    M5.Lcd.setTextFont(0);
    M5.Lcd.setTextSize(0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    int16_t y = 126;
    M5.Lcd.drawString("sensor", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("encoder0", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("encoder1", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("tof0", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("tof1", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("tof2", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("tof3", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("photo0", 213+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("photo1", 213+25, y+5, 1); // txt,x,y,font 

    // 右ペイン
    M5.Lcd.setCursor(213, 126);
    M5.Lcd.setTextFont(0);
    M5.Lcd.setTextSize(0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    y = 126;
    M5.Lcd.drawString("value", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("0000", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("0000", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("0000", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("0000", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("0000", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("0000", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("false", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("true", 263+25, y+5, 1); // txt,x,y,font 
}

/* センサ出力値表示の更新 */
void ViewDesigner::updateSensorValView()
{
    // 右ペインの初期化
    M5.Lcd.fillRect(264, 137, 48, 79, BLACK); 

    // 右ペイン
    M5.Lcd.setCursor(263, 126);
    M5.Lcd.setTextFont(0);
    M5.Lcd.setTextSize(0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    int16_t y = 126;
    //M5.Lcd.drawString("value", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(String(0000), 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(String(0000), 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(String(0000), 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(String(0000), 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(String(0000), 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(String(0000), 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("false", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString("true", 263+25, y+5, 1); // txt,x,y,font 
}

void ViewDesigner::updateSensorValView(int32_t enc0, int32_t enc1, uint16_t tof0, uint16_t tof1, uint16_t tof2, uint16_t tof3, bool photo0, bool photo1)
{
     // 右ペインの初期化
    M5.Lcd.fillRect(264, 137, 48, 79, BLACK); 

    // 右ペイン
    int16_t x = 263+13, y = 126+2;
    M5.Lcd.setCursor(x, y);
    M5.Lcd.setTextFont(0);
    M5.Lcd.setTextSize(0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(TL_DATUM);    //Set text alignment to center-aligned
    //M5.Lcd.drawString("value", 263+25, y+5, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.setCursor(x, y);
    ViewDesigner::printSensorVal(enc0);
    y += 10;
    M5.Lcd.setCursor(x, y);
    ViewDesigner::printSensorVal(enc1);
    y += 10;
    M5.Lcd.setCursor(x, y);
    ViewDesigner::printSensorVal(tof0);
    y += 10;
    M5.Lcd.setCursor(x, y);
    ViewDesigner::printSensorVal(tof1);
    y += 10;
    M5.Lcd.setCursor(x, y);
    ViewDesigner::printSensorVal(tof2);
    y += 10;
    M5.Lcd.setCursor(x, y);
    ViewDesigner::printSensorVal(tof3);
    y += 10;
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.drawString(boolean[photo0], 263+25, y+2, 1); // txt,x,y,font 
    y += 10;
    M5.Lcd.drawString(boolean[photo1], 263+25, y+2, 1); // txt,x,y,font 

}


void ViewDesigner::printSensorVal(int32_t val)
{
    M5.Lcd.printf("%c",num[val%1000]);
    M5.Lcd.printf("%c",num[val%100]);
    M5.Lcd.printf("%c",num[val%10]);
    M5.Lcd.printf("%c",num[val]);
}

void ViewDesigner::printSensorVal(uint16_t val)
{
    M5.Lcd.printf("%c",num[val%1000]);
    M5.Lcd.printf("%c",num[val%100 - val%1000*10]);
    M5.Lcd.printf("%c",num[val%10 - val%1000*100 - val%100*10]);
    M5.Lcd.printf("%c",num[val] - val%1000*1000 - val%100*100 - val%10*10);
}



void ViewDesigner::waitingView()
{

}

void ViewDesigner::runningView()
{
    
}

void ViewDesigner::gohomeView()
{
    
}

void ViewDesigner::errorView()
{
    
}
