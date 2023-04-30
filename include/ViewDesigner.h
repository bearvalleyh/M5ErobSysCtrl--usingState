/**
 * @file           ViewDesigner.h
 * @brief          M5のLCDの状態を設定する構造体と、各UIを生成するViewDesignerクラスの定義
 * @author         hmk
 * @date           2023/04/01
 * 
 */

#ifndef __VIEWDESIGNER_H__INCLUDED__
#define __VIEWDESIGNER_H__INCLUDED__
#include <Arduino.h>
#include <M5Core2.h>

/**
 * @brief LCDの情報を設定する構造体
 * @details 各StateクラスやLoggingクラスでそれぞれ保持されるスクリーンのデザインの定義
 * @param x 水平方向の表示開始位置
 * @param y 垂直方向の表示開始位置
 * @param w 内容領域の幅
 * @param h 内容領域の高さ
 * @param color 背景色
 */
typedef struct
{   
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
    uint16_t color;
} displayInfo;

/**
 * @brief LCDで表示されるテキストの情報を設定する構造体
 * @details 各StateクラスやLoggingクラスでそれぞれ保持されるテキストのデザインの定義
 * @param color 文字色
 * @param size 文字サイズ
 * @param font 文字のフォント 1-7で設定
 * @param datum テキストの表示位置 デフォルト：TL_DATUM, 中央：MC_DATUM, etc
 */
typedef struct
{   
    uint16_t color;
    uint8_t size;
    uint8_t font;
    uint8_t datum;
} textInfo;


class ViewDesigner
{    
    public:
        static int inputVal;
        static int updateVal;
        static bool keyPressed;
        static const char num[10];
        static const String boolean[2];

    /* デバッグモード用UI */
    public:
        /* 画面遷移通知 */
        static void notifyView(const char* classname, int backGroundColor = DARKCYAN, int textColor = WHITE);
        /* 画面遷移通知（Lチカ） */
        static void notifyLED();
        /* ボタンA,B,C表示  | BtnA | BtnB | BtnC | */
        static void buttonView(const char* btnA="BtnA", const char* btnB="BtnB", const char* btnC="BtnC");
        /* 各ボタンを押下した時のアクション表示 */
        static void buttonExplainView(String btnA = "     ------------     ", String btnB = "     ------------     ", String btnC = "     ------------     ");
        /* 各ボタンが押下されたことをUI上で通知 */
        static void buttonPressedView(short btn, int backGroundColor);
        /* 現在のモードとコンパイル時の日付を表示するヘッダー */
        static void headerView(const char* classname, int backGroundColor=DARKCYAN);
        /* テスト用入力キーボード表示 */
        static void initKeyboardView();
        /* テスト用入力キーボード表示 */
        static void updateKeyboardView();
        /* センサ出力値表示 */
        static void initSensorValView();
        /* センサ出力値表示の更新 */
        static void updateSensorValView();
        static void updateSensorValView(int32_t enc0, int32_t enc1, uint16_t tof0, uint16_t tof1, uint16_t tof2, uint16_t tof3, bool photo0, bool photo1);
        /* センサ値(4桁)を文字列に変換して出力 */
        static void printSensorVal(int32_t val);
        static void printSensorVal(uint16_t val);

    /* リリースモード用View */
    public:
        static void waitingView();
        static void runningView();
        static void gohomeView();
        static void errorView();
};

#endif