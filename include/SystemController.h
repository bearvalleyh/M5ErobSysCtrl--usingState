/**
 * @file           SystemController.h
 * @brief          Utilのシステム全体を制御するSystemControllerクラスの定義
 * @author         hmk
 * @date           2023/04/02
 */

#ifndef __SYSTEMCONTROLLER_H__INCLUDED__
#define __SYSTEMCONTROLLER_H__INCLUDED__

#ifndef M5CORE2
#define M5CORE2
#include <M5Core2.h>
#endif

#include <Arduino.h>
//#include <StatePattern.h>

class State; //前方宣言

/**
 * @brief Utilのシステム全体を制御するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドによってのみインスタンスが1つだけ生成される
 */
class SystemController
{
    private:
        /**
         * @brief SystemControllerクラスのコンストラクタ
         * @details Systemが遷移しうる全ての状態（State）のインスタンスを生成する。
         * @details インスタンスの生成が完了したら最初の状態をセットする
         * @details インスタンスの生成/セットに失敗したらシステムを即終了させる。
         */
        SystemController();
        
    public:
        /**
         * @brief SystemControllerクラスのデストラクタ
         * @details コンストラクタで生成した全ての状態（State）のインスタンスを破棄する。
         */
        ~SystemController();

    public:
        /**
         * @brief SystemController のインスタンスを取得する
         * @return SystemController のsingletonを指すポインタ
         */
        static SystemController* getInstance();

        /**
         * @brief SystemController のインスタンスを生成する
         * @return true: 生成成功 / false: 生成失敗
         */
        static bool configure();
    
    public:
        /**
         * @brief Systemの状態を変更する
         * @details 各Stateで発生するアクションをトリガーとして呼び出され、Systemの状態遷移を実行する。
         * @param state 遷移先の状態。基底となる抽象クラスStateの具象クラスのインスタンスが与えられる。
         */
        static void transitionTo(State* state);

        /**
         * @brief Systemの状態を変更する
         * @details 各Stateで発生するアクションをトリガーとして呼び出され、Systemの状態遷移を実行する。
         * @param state 遷移先の状態。基底となる抽象クラスStateの具象クラスのインスタンスが与えられる。
         */
        static void update();

        /**
         * @brief Systemの現在の状態を返す
         * @return Systemの現在の状態を指すポインタ
         */
        static State* getCurrentState();

        /**
         * @brief Systemのシャットダウンを行う
         * @details シャットダウン前にSystemで統一した処理を実行したい場合、この関数に処理を追加する。
         */
        static void shutdown();
        
    private:
        static const char* _classname;
        static SystemController* _singleton;
        static State* _currentState;  // 現在の状態
};

#endif
