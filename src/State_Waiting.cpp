#include "SystemController.h"
#include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class Waiting
--------------*/


/* static member */
Waiting* Waiting::_singleton = NULL;
const char* Waiting::_classname = "Waiting";
//SystemController* _systemController = NULL;
RTC_TimeTypeDef Waiting::TimeStruct;


/* private method */
Waiting::Waiting()
{
    // タイマーの設定
    TimeStruct.Hours   = 0;
    TimeStruct.Minutes = 0;
    TimeStruct.Seconds = 0;
    M5.Rtc.SetTime(&TimeStruct);
}


/* static private method */
Waiting* Waiting::getInstance()
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
Waiting::~Waiting()
{
    // @ToDo: 明示的にメモリ解放しなくてはならない処理があれば追加する
}


/* static public method */
bool Waiting::configure()
{
    if (_singleton == NULL) {
        _singleton = new Waiting();
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


void Waiting::show()
{
    M5.Axp.SetLed(1);
    
    #ifdef M5_DEBUG
    M5.Lcd.clear();
    // ログ表示領域
    M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, DARKGREEN, BLACK)); 

    // ヘッダ
    ViewDesigner::headerView(_classname, DARKGREEN);

    // ボタンの説明
    ViewDesigner::buttonExplainView("transition to setup  ", "transition to running"); 

    //ボタンの枠
    ViewDesigner::buttonView();
    #endif
}


void Waiting::write()
{

}


void Waiting::notify()
{
    ViewDesigner::notifyView(_classname, DARKGREEN);

    ViewDesigner::notifyLED();
}


void Waiting::update()
{
    // １分経ったら LightSleep になる
    //M5.Rtc.GetTime(&TimeStruct);
    //if (TimeStruct.Minutes>=1 || TimeStruct.Hours>=1)
    //{
    //    delay(1000);
    //    SystemController::transitionTo(LightSleep::getInstance());
    //    return;
    //}

    M5.update();
    if (M5.BtnA.wasPressed())
    {
        ViewDesigner::buttonPressedView(0, DARKGREEN);
        delay(1000);
        SystemController::transitionTo(Setup::getInstance());
        return;
    }

    if (M5.BtnB.wasPressed())
    {
        M5.Lcd.print("a");
        ViewDesigner::buttonPressedView(1, DARKGREEN);
        delay(1000);
        SystemController::transitionTo(Running::getInstance());
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
        ViewDesigner::buttonPressedView(2, DARKGREEN);
    }


}


void Waiting::setup()
{
    // 通知
    Waiting::notify();

    delay(100);

    // 画面更新
    Waiting::show();

    M5.Lcd.setCursor(10,25);

}


