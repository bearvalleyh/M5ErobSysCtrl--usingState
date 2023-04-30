/**
 * @file           StateBase.h
 * @brief          Utilが取りうるStateの列挙体と、各Stateの基底となる抽象クラスの定義
 * @author         hmk
 * @date           2023/04/02
 */

#ifndef __STATE_H__INCLUDED__
#define __STATE_H__INCLUDED__

#ifndef M5CORE2
#define M5CORE2
#include <M5Core2.h>
#endif

#include <Arduino.h>


enum UtilState
{
    DEEPSLEEP,
    LIGHTSLEEP,
    SETUP,
    WAITING,
    RUNNING,
    GOHOME
};

/**
 * @brief 各Stateの基底となる抽象クラス
 * @details 本クラスの関数は全て純粋仮想関数で定義され、インスタンスが直接生成されることはない
 */
class State
{
    public:
        /**
        * @brief UI（LCD/LED）を更新する
        * @details 状態の初期化時や画面の更新時に呼び出される
        */
        virtual void show() = 0;

        /**
        * @brief UI（LCD/LED）に書き込みを行う
        * @details 正式な用途は未定。テスト時の値設定時に使用？
        */
        virtual void write() = 0;

        /**
        * @brief 状態遷移を通知する
        * @details 状態遷移後の初期化時に呼び出される
        */
        virtual void notify() = 0;

        /**
        * @brief 各状態の処理を実行する
        * @details main.cppのloop()で常に実行される
        * @details 状態遷移のトリガーとなるアクションが発生したら、SystemController::transitionTo()を呼び出す
        */
        virtual void update() = 0;

        /**
        * @brief システムの状態を更新する
        * @details システムの稼働開始時とSystemController::transitionTo()内でそれぞれ一回ずつ呼び出される
        */
        virtual void setup() = 0;
};


#endif
