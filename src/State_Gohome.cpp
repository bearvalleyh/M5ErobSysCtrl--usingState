#include "SystemController.h"
#include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class Gohome
--------------*/


/* static member */
Gohome* Gohome::_singleton = NULL;
const char* Gohome::_classname = "Gohome";
//SystemController* _systemController = NULL;


/* private method */
Gohome::Gohome()
{

}


/* static private method */
Gohome* Gohome::getInstance()
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
Gohome::~Gohome()
{
    // @ToDo: 明示的にメモリ解放しなくてはならない処理があれば追加する
}


/* static public method */
bool Gohome::configure()
{
    if (_singleton == NULL) {
        _singleton = new Gohome();
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


void Gohome::show()
{
    M5.Axp.SetLed(1);
    
    #ifdef M5_DEBUG
    M5.Lcd.clear();
    // ログ表示領域
    M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, MAROON, BLACK)); 

    // ヘッダ
    ViewDesigner::headerView(_classname, MAROON);

    // ボタンの説明
    ViewDesigner::buttonExplainView("transition to Running ", "transition to Waiting ");

    //ボタンの枠
    ViewDesigner::buttonView();
    #endif
}


void Gohome::write()
{
    // throw
}


void Gohome::notify()
{
    ViewDesigner::notifyView(_classname, MAROON);

    ViewDesigner::notifyLED();
}


void Gohome::run()
{
    // throw
}


void Gohome::update()
{
    // 壊れていた場合は SystemController クラスの shutdown() を呼ぶ
    M5.update();

    if (M5.BtnA.wasPressed())
    {
        ViewDesigner::buttonPressedView(0, MAROON);
        delay(1000);
        SystemController::transitionTo(Running::getInstance());
        return;

    }

    if (M5.BtnB.wasPressed())
    {
        ViewDesigner::buttonPressedView(1, MAROON);
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
        ViewDesigner::buttonPressedView(2, MAROON);
    }

}


void Gohome::setup()
{
    // 通知
    Gohome::notify();

    delay(100);

    // 画面更新
    Gohome::show();

    M5.Lcd.setCursor(10,25);

}

