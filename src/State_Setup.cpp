#include "SystemController.h"
#include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class Setup
--------------*/


/* static member */
Setup* Setup::_singleton = NULL;
const char* Setup::_classname = "Setup";
//SystemController* _systemController = NULL;


/* private method */
Setup::Setup()
{

}


/* static private method */
Setup* Setup::getInstance()
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
Setup::~Setup()
{
    // @ToDo: 明示的にメモリ解放しなくてはならない処理があれば追加する
}


/* static public method */
bool Setup::configure()
{
    if (_singleton == NULL) {
        _singleton = new Setup();
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


void Setup::show()
{
    M5.Lcd.fillScreen(LIGHTGREY);

    #ifdef M5_DEBUG
    M5.Lcd.clear();
    // ログ表示領域
    M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, LIGHTGREY, BLACK)); 

    // ヘッダ
    ViewDesigner::headerView(_classname, LIGHTGREY);

    // ボタンの説明
    ViewDesigner::buttonExplainView("transition to DeepSleep", "transition to Waiting"); 

    //ボタンの枠
    ViewDesigner::buttonView();

    #else
    M5.Lcd.fillScreen(LIGHTGREY);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    M5.Lcd.drawString("Loading...", 160, 100, 2); 
    #endif
}


void Setup::write()
{
    // throw
}


void Setup::notify()
{
    ViewDesigner::notifyView(_classname, LIGHTGREY);

    ViewDesigner::notifyLED();
}


void Setup::update()
{
    // throw
}

void Setup::setup()
{
    Setup::notify();

    // 起動準備中は画面にプログレスバーを出す

    // 画面初期化
    Setup::show();

    delay(1000);

    M5.Lcd.setCursor(10,25);


    // @TODO: 初期化処理を追加する
    Setup::progressBar(0);
    Setup::progressBar(20);
    Setup::progressBar(40);
    Setup::progressBar(60);
    Setup::progressBar(80);
    Setup::progressBar(100);

    #ifdef M5_DEBUG
    while (true)
    {
        M5.update();
        if (M5.BtnA.wasPressed())
        {
            ViewDesigner::buttonPressedView(0, CYAN);
            delay(1000);
            SystemController::transitionTo(DeepSleep::getInstance());
            return;
        }

        if (M5.BtnB.wasPressed())
        {
            ViewDesigner::buttonPressedView(1, CYAN);
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
            ViewDesigner::buttonPressedView(2, CYAN);
        }


    }

    #else
    SystemController::transitionTo(Waiting::getInstance());
    #endif
}

void Setup::progressBar(uint8_t percent)
{
    #ifndef M5_DEBUG
    M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    M5.Lcd.progressBar(40, 120, 240, 20, percent);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
    delay(100);
    #endif
}


