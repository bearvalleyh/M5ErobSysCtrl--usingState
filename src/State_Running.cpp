#include "SystemController.h"
#include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

/*--------------
class Running
--------------*/


/* static member */
Running* Running::_singleton = NULL;
const char* Running::_classname = "Running";
//RunScene* Running::_currentScene = NULL;
//RunScene* Running::_scene = new RunScene[4];
//SystemController* _systemController = NULL;


/* private method */
Running::Running()
{
}


/* static private method */
Running* Running::getInstance()
{
    if (_singleton == NULL) {
        ERROR_LOG("getInstance is failed.");
        return NULL;
    }
    return _singleton;
}


/* public method */
Running::~Running()
{
    // @ToDo: 明示的にメモリ解放しなくてはならない処理があれば追加する
    //delete[] _scene;
}


/* static public method */
bool Running::configure()
{
    if (_singleton == NULL) {
        _singleton = new Running();
        if (_singleton == NULL) {
            ERROR_LOG("configure failed.");
            return false;
        }
    }
    return true;
}


void Running::show()
{
    M5.Axp.SetLed(1);
    
    #ifdef M5_DEBUG
    M5.Lcd.clear();

    // ログ表示領域
    M5.Lcd.fillRect(7, 22, 306, 128, M5.Lcd.alphaBlend(128, OLIVE, BLACK)); 

    // ヘッダ
    ViewDesigner::headerView(_classname, OLIVE);

    // ボタンの説明
    ViewDesigner::buttonExplainView("transition to Waiting ", "transition to Gohome ");

    //ボタンの枠
    ViewDesigner::buttonView(); 
    #endif
}


void Running::write()
{
    // throw
}


void Running::notify()
{
    ViewDesigner::notifyView(_classname, OLIVE);

    ViewDesigner::notifyLED();
}


void Running::run()
{
    // 現在のシーンが終了したら、シーンを更新する
    //if (_currentScene.isFinished())
    //{
    //    if (_currentScene.ぶつかって終了())
    //    {
    //      // 一旦後退するシーンを追加
    //      goBackScene *goBack = new goBackScene();
    //      goBack.setNextScene(_currentScene.getNextScene());
    //      _currentScene = goBack;
    //    }
    //    else {_currentScene = _currentScene.getNextScene();}
    //    // このシナリオのシーンが全て終了した場合、次の状態に遷移
    //    if (_currentScene == NULL)
    //    {
    //        SystemController::transitionTo(Gohome::getInstance());
    //    }
    //}

    // シーンを実行
    //_currentScene.execute();
}


void Running::update()
{
    Running::run(); // runモードの実行

    M5.update();


    // ボタンを押された時の処理
    if (M5.BtnA.wasPressed())
    {
        ViewDesigner::buttonPressedView(0, OLIVE);
        delay(1000);
        SystemController::transitionTo(Waiting::getInstance());
        return;
    }

    if (M5.BtnB.wasPressed())
    {
        ViewDesigner::buttonPressedView(1, OLIVE);
        delay(1000);
        SystemController::transitionTo(Gohome::getInstance());
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
        ViewDesigner::buttonPressedView(2, OLIVE);
    }


}


void Running::setup()
{
    // 通知
    Running::notify();
    delay(100);

    // 画面更新
    Running::show();
    M5.Lcd.setCursor(10,25);

    // このRunモードで利用するSceneのインスタンスを数ぶん生成する必要がある
    // 各Sceneに対して以下を実施する
    // 次のSceneをセットする( setNextScene(nextScene) ) NULLの場合はモードの終了を意味する
    // 最初のシーンを初期化し、実行する

    // シーンのインスタンスを必要な分生成する
    //UturnScene* leftTurn =  new UturnScene(0); // 左に曲がる
    //UturnScene* rightTurn =  new UturnScene(1); // 左に曲がる
    //goStraightScene* goStraightAfterLeftTurn = new goStraightScene(); // 直進
    //goStraightScene* goStraightAfterRightTurn = new goStraightScene(); // 直進
    //
    //// 各シーンに次のシーンをセットする
    //goStraightAfterLeftTurn.setNextScene(rightTurn);
    //rightTurn.setNextScene(goStraightAfterRightTurn);
    //goStraightAfterRightTurn.setNextScene(goStraightAfterLeftTurn);
    //leftTurn.setNextScene(goStraightAfterLeftTurn);
//
    //// 右側へUターンする直進のシーンからスタート
    //_currentScene = goStraightAfterLeftTurn;
    //_currentScene.execute();
}

