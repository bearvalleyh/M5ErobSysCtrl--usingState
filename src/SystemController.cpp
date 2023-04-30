#include <Arduino.h>
#include <M5Core2.h>
#include <math.h>
#include "SystemController.h"
#include "StatePattern.h"
#ifdef M5_DEBUG
#define ERRORLOG() M5.Lcd.printf("[ERROR] Class:%s, Line:%d\n", _classname, __LINE__)
#else
#define ERRORLOG() ((void)0)
#endif

const char* SystemController::_classname = "SystemController";
SystemController* SystemController::_singleton = NULL;
State* SystemController::_currentState = NULL;

SystemController::SystemController()
{
    // 各Stateのインスタンス生成
    bool b = true;
    b &= DeepSleep::configure();
    b &= Setup::configure();
    b &= Waiting::configure();
    b &= Running::configure();
    b &= Gohome::configure();
    // 1つでも生成に失敗したら終了
    if (!b)
    {
        SystemController::shutdown();
    }

    // 充電によって起動した場合はDeepSleep
    // それ以外はSetupを初期状態とする
    //_currentState = Setup::getInstance();
    _currentState = DeepSleep::getInstance();
    // セットに失敗したら終了
    if (!_currentState)
    {
        SystemController::shutdown();
    }

    _currentState->setup();
}

SystemController::~SystemController()
{
    // Stateのインスタンスが確保したメモリを明示的に解放する
}


SystemController* SystemController::getInstance()
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


bool SystemController::configure()
{
    if (_singleton == NULL) {
        _singleton = new SystemController();
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

void SystemController::transitionTo(State* state)
{
    if (state == NULL) SystemController::shutdown();
    if (_currentState == state) SystemController::shutdown();

    _currentState = state; // stateの更新

    _currentState->setup(); // 状態変更時の初期化処理

    delay(100);
}

void SystemController::update()
{
    _currentState->update();
}

State* SystemController::getCurrentState()
{
    return _currentState;
}

void SystemController::shutdown()
{
    // @ToDo: 終了処理があればここに追加する
    DEBUG_END();

    M5.shutdown();
}