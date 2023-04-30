/**
 * @file           Logging.h
 * @brief          ログを管理するLoggingクラスの定義
 * @author         hmk
 * @date           2023/03/19
 * 
 */

#ifndef __LOGGING_H__INCLUDED__
#define __LOGGING_H__INCLUDED__

#ifdef M5_DEBUG

#ifndef FILE_NAME 
#define FILE_NAME NULL
#endif

#ifndef ENCODE_TYPE
#define ENCODE_TYPE "utf-8"
#endif

#include <Arduino.h>
#include <ViewDesigner.h>

/**
 * @enum LogLevel
 * @brief ログ出力レベルを定義する構造体
 */
typedef enum
{
    NONE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    ASSERT
} LogLevel;

/**
 * @brief Logの記録位置の座標を保持する構造体
 * @param x 水平方向の表示開始位置
 * @param y 垂直方向の表示開始位置
 */
typedef struct
{   
    uint8_t x;
    uint8_t y;
} pos;

/**
 * @brief Logを管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class Logging
{
private:
    /**
     * @brief Loggingクラスのコンストラクタ
     * @param level ログ出力レベル
     * @param filename ログを記録するファイルの名前
     * @param encoding エンコードタイプ
     */
    Logging(LogLevel level);

private:
    /**
     * @brief Logging のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return Loggingのsingletonを指すポインタ
     */
    static Logging* getInstance();

public:
    /**
     * @brief Loggingクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_ToFUnitクラスのインスタンスをすべて解放する
     */
    ~Logging();

public:
    /**
     * @brief Loggingのインスタンスを生成する
     * @param level ログ出力レベル
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(LogLevel level=WARNING);

    /**
     * @brief ログの記録・出力を管理する関数
     * @param logText ログとして記録する文字列
     * @param level ログ出力レベル
     */
    static void print(char* logText);

    /**
     * @brief 出力先のディスプレイの情報を取得する関数
     * @details 開始位置の座標、出力範囲の幅・高さ、出力先の背景色を受け取る
     * @param displayInfo 上記の情報を持つ構造体
     */
    static void setDisplayInfo(displayInfo displayInfo);

    /**
     * @brief 出力先のディスプレイでのテキスト表示形式の情報を取得する関数
     * @details 文字色、文字サイズ、文字のフォント、テキストの表示位置を受け取る
     * @param textInfo 上記の情報を持つ構造体
     */
    static void setTextInfo(textInfo textInfo);

    /**
     * @brief デバッグ開始時の処理。開始時点の情報を出力する
     */
    static void debugStart();

    /**
     * @brief デバッグ終了時の処理。デバッグの内容をファイル出力等する場合、この関数に実装する。
     */
    static void debugEnd();


    /**
     * @brief ログ出力レベルに合わせた文字列を生成する。
     * @param level ログ出力レベル
     * @param filename 呼び出し元のファイル名
     * @param line 呼び出し元の行番号
     * @param text 付加情報を記録する文字列（任意）
     */
    static void record(const LogLevel level, const char* filename, const uint16_t line, const char* text);

private:
    static const char* _classname;
    static Logging *_singleton;
    static const char* _filename; /* ログ記録用ファイル（利用は任意） */
    static const char* _encoding; /* 文字コード */
    static const size_t _max_log_size;
    const char* _logText;
    LogLevel _level; /* ログ出力レベル */
    bool _isLogging;
    static displayInfo _currentDisplayInfo;
    static textInfo _currentTextInfo;
    static pos _currentTextPos;
};


// ログファイルを開く
#define DEBUG_START()  Logging::debugStart()

// 値の確認
#define ASSERT_LOG(b)      if(!b) Logging::record(ASSERT,   __FILENAME__, __LINE__, "")

// ログファイルを閉じる
#define DEBUG_END()    Logging::debugEnd()

#define DEBUG_LOG(memo)     Logging::record(DEBUG,      __FILENAME__, __LINE__, memo)
#define INFO_LOG(memo)      Logging::record(INFO,       __FILENAME__, __LINE__, memo)
#define WARNING_LOG(memo)   Logging::record(WARNING,    __FILENAME__, __LINE__, memo)
#define ERROR_LOG(memo)     Logging::record(ERROR,      __FILENAME__, __LINE__, memo)
#define CRITICAL_LOG(memo)  Logging::record(CRITICAL,   __FILENAME__, __LINE__, memo)



#else  // #ifdef M5_DEBUG
// リリース時

#define DEBUG_START()
#define ASSERT_LOG(b)
#define DEBUG_END()

#define DEBUG_LOG(memo)  
#define INFO_LOG(memo)  
#define WARNING_LOG(memo)  
#define ERROR_LOG(memo)  
#define CRITICAL_LOG(memo)  

#endif  // #ifdef M5_DEBUG

#endif  // # ifndef __LOGGING_H__INCLUDED__