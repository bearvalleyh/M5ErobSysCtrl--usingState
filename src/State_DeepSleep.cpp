#include "SystemController.h"
#include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class DeepSleep
--------------*/


/* static member */
DeepSleep* DeepSleep::_singleton = NULL;
const char* DeepSleep::_classname = "DeepSleep";
//SystemController* _systemController = NULL;



/* private method */
DeepSleep::DeepSleep()
{
    //_systemController = SystemController::getInstance();
    //if (_systemController == NULL) M5.shutdown();
}


/* static private method */
DeepSleep* DeepSleep::getInstance()
{
    if (_singleton == NULL) {
        /*
        ERRORLOG
            内容：シングルトンがまだ生成されていない
        */
        return NULL;
    }
    return _singleton;
}


/* public method */
DeepSleep::~DeepSleep()
{
    // @ToDo: 明示的にメモリ解放しなくてはならない処理があれば追加する
}


/* static public method */
bool DeepSleep::configure()
{
    if (_singleton == NULL) {
        _singleton = new DeepSleep();
        if (_singleton == NULL) {
            /*
            ERRORLOG
                内容：メモリ確保に失敗した 
            */
            return false;
        }
    }
    return true;
}


void DeepSleep::show()
{
    M5.Lcd.clear();
    M5.Axp.SetLed(1);

    #ifdef M5_DEBUG
    // ログ表示領域
    M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, DARKCYAN, BLACK)); 

    // ヘッダ
    ViewDesigner::headerView(_classname, DARKCYAN);

    // ボタンの説明
    ViewDesigner::buttonExplainView("transition to Setup ", "transition to Waiting");

    //ボタンの枠
    ViewDesigner::buttonView(); 
    #endif

}


void DeepSleep::write()
{

}


void DeepSleep::notify()
{
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.drawString(_classname, 160, 120, 2); 

    // Lチカで通知
    for (int i=0; i<5; i++)
    {
        M5.Axp.SetLed(0);
        delay(100);
        M5.Axp.SetLed(1);
        delay(100);
    }
}


void DeepSleep::update()
{   
    #ifdef M5_DEBUG
    M5.update();
    if (M5.BtnA.wasPressed())
    {
        ViewDesigner::buttonPressedView(0, DARKCYAN);
        delay(1000);
        SystemController::transitionTo(Setup::getInstance());
        return;
    }

    if (M5.BtnB.wasPressed())
    {
        ViewDesigner::buttonPressedView(1, DARKCYAN);
        delay(1000);
        SystemController::transitionTo(Waiting::getInstance());
        return;

    }
    
    if (M5.BtnC.wasPressed())
    {
        M5.Axp.SetLed(0);
        M5.update();
        while (!M5.BtnC.wasPressed()) {
            M5.update();
        }
        M5.Axp.SetLed(1);
        ViewDesigner::buttonPressedView(2, DARKCYAN);
    }


    #else
    // どこかのボタンを押されたらSetupへ
    if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed())
    {
        M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
        M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, CYAN, BLACK)); 
        M5.Lcd.drawString("Wake up.", 160, 86, 2); 
        ViewDesigner::notifyLED();
        SystemController::transitionTo(Setup::getInstance());
    }
    #endif
}


void DeepSleep::setup()
{
    DeepSleep::notify(); // 通知

    DeepSleep::show(); // 画面更新

    M5.Lcd.setCursor(10,25);

    //while (M5.BtnC.isReleased())
    //{
    //    M5.update();
    //    INFO_LOG("");
    //    WARNING_LOG("");
    //    ERROR_LOG("");
    //    ERROR_LOG("");
    //}

}