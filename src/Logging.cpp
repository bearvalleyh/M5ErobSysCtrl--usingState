#include "Logging.h"

#ifdef M5_DEBUG
#include <M5Core2.h>
#define ERROR_LOG() M5.Lcd.printf(" [LOGGER ERROR] Line:%d\n", __LINE__)
#elif M5STICKCPLUS
#include <M5StickCPlus.h>
#define ERROR_LOG() ((void)0)
#else
#undef M5_DEBUG
#define ERROR_LOG() ((void)0)
#endif

extern bool updateLog;
extern String lastLoggedFileInfo;
extern String lastLoggedText;

/*--------------
class Logging
--------------*/

/* static member */
Logging* Logging::_singleton = NULL;
const char* Logging::_classname = "Logging";
const char* Logging::_filename = FILE_NAME;
const char* Logging::_encoding = ENCODE_TYPE;
const size_t Logging::_max_log_size = 256;
//displayInfo Logging::_currentDisplayInfo {7, 22, 306, 128, M5.Lcd.alphaBlend(128, DARKCYAN, BLACK)};
displayInfo Logging::_currentDisplayInfo {3, 3, 304, 234, M5.Lcd.alphaBlend(64, DARKGREY, BLACK)};
textInfo Logging::_currentTextInfo {WHITE, 1, 1, TL_DATUM};
pos Logging::_currentTextPos {3,3};

/* private method */
Logging::Logging(LogLevel level)
:   _level(level),
    _logText(NULL),
    _isLogging(false)
{
    // @ToDo: ファイルオープンとかの処理があればここに記述する
    _logText = new char();
}



/* static private method */
Logging* Logging::getInstance()
{
    if (_singleton == NULL) {
        ERROR_LOG();
        return NULL;
    }
    return _singleton;
}

/* public method */
Logging::~Logging()
{
    // @ToDo: 明示的にメモリ解放しなくてはならない処理があれば追加する
}

/* static public method */
bool Logging::configure(LogLevel level)
{
    if (_singleton == NULL) {
        _singleton = new Logging(level);
        if (_singleton == NULL) {
            ERROR_LOG();
            return false;
        }
    }
    return true;
}

void Logging::print(char* logText)
{
    // @ToDo: 以下の場合は処理を変更する
    // ・ファイルに記録する
    // ・UIを独自のクラスで出力する

    /* iPhoneにログを送信 */
    //lastLoggedFileInfo = logText;
    //updateLog = true;

    // TextSize(2) の時、text のhは?
    M5.Lcd.setTextSize(0);
    //M5.Lcd.setCursor(_currentTextPos.x,_currentTextPos.y);
    M5.Lcd.setTextDatum(TL_DATUM);    //Set text alignment to center-aligned
    if (_currentTextPos.y+2 < _currentDisplayInfo.y + _currentDisplayInfo.h - 20)
    {
        M5.Lcd.setCursor(_currentTextPos.x,_currentTextPos.y);
        M5.Lcd.printf(logText);
        //M5.Lcd.setCursor(_currentDisplayInfo.x+3,M5.Lcd.getCursorY()+2);
        _currentTextPos.x = _currentDisplayInfo.x+3;
        _currentTextPos.y = M5.Lcd.getCursorY()+2;
    }
    else
    {
        M5.Lcd.fillRect(_currentDisplayInfo.x, _currentDisplayInfo.y, _currentDisplayInfo.w, _currentDisplayInfo.h, M5.Lcd.alphaBlend(128, _currentDisplayInfo.color, BLACK)); 
        //M5.Lcd.setCursor(_currentDisplayInfo.x+3,M5.Lcd.getCursorY()+2);
        _currentTextPos.x = _currentDisplayInfo.x+3;
        _currentTextPos.y = _currentDisplayInfo.y+2;
        M5.Lcd.setCursor(_currentTextPos.x,_currentTextPos.y);
        M5.Lcd.printf(logText);
        //M5.Lcd.setCursor(_currentDisplayInfo.x+3,M5.Lcd.getCursorY()+2);
        _currentTextPos.x = _currentDisplayInfo.x+3;
        _currentTextPos.y = M5.Lcd.getCursorY()+2;
    }
}

void Logging::debugStart()
{
    // ログ記録モードにセット
    Logging *logger = Logging::getInstance();
    if (logger == NULL) {
        ERROR_LOG();
        return;
    }
    logger -> _isLogging = true;
    //M5.Lcd.setCursor(_currentDisplayInfo.x+3,M5.Lcd.getCursorY()+2);
    //_currentTextPos.x = _currentDisplayInfo.x+3;
    //_currentTextPos.y = M5.Lcd.getCursorY()+3;

    // @ToDo: ログファイルに記録する場合、ファイルに関する処理をここに記述する
    
    // ビルド時点の日時・デバッグ開始位置を出力
    char timeBuf[_max_log_size];
    snprintf(timeBuf, sizeof(timeBuf),"TIMESTAMP:  %s \n", __TIMESTAMP__);
    Logging::print(timeBuf);

    // デバッグ開始を通知
    char buf[_max_log_size];
    snprintf(buf, sizeof(buf), "Debug start>>\n");

    Logging::print(buf);
}


void Logging::debugEnd()
{
    // ログ記録モードを終了する
    Logging *logger = Logging::getInstance();
    if (logger == NULL) {
        ERROR_LOG();
        return;
    }

    if (logger -> _isLogging == false) return;

    // デバッグの終了を記録
    char buf[_max_log_size];
    snprintf(buf, sizeof(buf), "<<Debug end.\n");

    Logging::print(buf);

    // @ToDo: ログファイルに記録していた場合、ファイルに関する処理をここに記述する

    logger -> _isLogging = false;
}


void Logging::setDisplayInfo(displayInfo displayInfo)
{
    _currentDisplayInfo.x = displayInfo.x;
    _currentDisplayInfo.y = displayInfo.y;
    _currentDisplayInfo.w = displayInfo.w;
    _currentDisplayInfo.h = displayInfo.h;
    _currentDisplayInfo.color = displayInfo.color;

    _currentTextPos.x = _currentDisplayInfo.x+3;
    _currentTextPos.y = _currentDisplayInfo.y+2;
}

void Logging::setTextInfo(textInfo textInfo)
{
    _currentTextInfo.color = textInfo.color;
    _currentTextInfo.size = textInfo.size;
    _currentTextInfo.font = textInfo.font;
    _currentTextInfo.datum = textInfo.datum;
}


void Logging::record(const LogLevel level, const char* filename, const uint16_t line, const char* text)
{
    Logging *logger = Logging::getInstance();
    if (logger == NULL) {
        ERROR_LOG();
        return;
    }
    if ( !(logger -> _isLogging) || (logger -> _level > level && !(level == NONE))) return;
    
    char buf[_max_log_size];
    char fileInfo[_max_log_size];
    switch (level)
    {
        case NONE:
            snprintf(buf, sizeof(buf), " %s\n", text);
            _currentTextInfo = {WHITE, 1, 1, TL_DATUM};
            break;
        case DEBUG:
            //snprintf(buf, sizeof(buf), " [DEBUG] File: %s, Line: %d \n    memo: %s\n", filename, line, text);
            snprintf(fileInfo, sizeof(fileInfo), " [DEBUG] File: %s, Line: %d", filename, line);
            snprintf(buf, sizeof(buf), "%s \n    memo: %s\n", fileInfo, text);
            _currentTextInfo = {WHITE, 1, 1, TL_DATUM};
            break;
        case INFO:
            //snprintf(buf, sizeof(buf), " [INFO] File: %s, Line: %d \n    memo: %s\n", filename, line, text);
            snprintf(fileInfo, sizeof(fileInfo), " [INFO] File: %s, Line: %d", filename, line);
            snprintf(buf, sizeof(buf), "%s \n    memo: %s\n", fileInfo, text);
            _currentTextInfo = {LIGHTGREY, 1, 1, TL_DATUM};
            break;
        case WARNING:
            //snprintf(buf, sizeof(buf), " [WARNING] File: %s, Line: %d \n    memo: %s\n", filename, line, text);
            snprintf(fileInfo, sizeof(fileInfo), " [WARNING] File: %s, Line: %d", filename, line);
            snprintf(buf, sizeof(buf), "%s \n    memo: %s\n", fileInfo, text);
            _currentTextInfo = {YELLOW, 1, 1, TL_DATUM};
            break;
        case ERROR:
            //snprintf(buf, sizeof(buf), " [ERROR] File: %s, Line: %d \n    memo: %s\n", filename, line, text);
            snprintf(fileInfo, sizeof(fileInfo), " [ERROR] File: %s, Line: %d", filename, line);
            snprintf(buf, sizeof(buf), "%s \n    memo: %s\n", fileInfo, text);
            _currentTextInfo = {RED, 1, 1, TL_DATUM};
            break;
        case CRITICAL:
            //snprintf(buf, sizeof(buf), " [CRITICAL] File: %s, Line: %d \n    memo: %s\n", filename, line, text);
            snprintf(fileInfo, sizeof(fileInfo), " [CRITICAL] File: %s, Line: %d", filename, line);
            snprintf(buf, sizeof(buf), "%s \n    memo: %s\n", fileInfo, text);
            _currentTextInfo = {RED, 1, 1, TL_DATUM};
            break;
        case ASSERT:
            //snprintf(buf, sizeof(buf), " [ASSERTION ERROR] File: %s, Line: %d \n", filename, line);
            snprintf(buf, sizeof(buf), " [ASSERTION ERROR] File: %s, Line: %d \n", filename, line);
            _currentTextInfo = {RED, 1, 1, TL_DATUM};
            break;
        default:
            break;
    }

    /* iPhoneにログを送信 */
    if (!updateLog) 
    {
        lastLoggedFileInfo = fileInfo;
        lastLoggedText = text;
        updateLog = true;
    }


    Logging::print(buf);

    // ERROR 以上の場合出力をストップ
    if (level >= ERROR) {
        Logging::print("continue?\n");
        // ボタンの説明
        M5.Lcd.fillRect(0, 150, 320, 60, BLACK); 
        M5.Lcd.setTextSize(1);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
        M5.Lcd.drawString("BtnA...  Yes  ", 160, 160, 2); 
        M5.Lcd.drawString("BtnB... Reboot", 160, 180, 2); 
        M5.Lcd.drawString("BtnC...   No  ", 160, 200, 2); 

        //ボタン
        M5.Lcd.setTextSize(1);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center-aligned
        M5.Lcd.fillRect(7, 216, 100, 18, DARKGREY); 
        M5.Lcd.drawRect(7, 216, 100, 18, WHITE); 
        M5.Lcd.drawString("BtnA", 57, 225, 2); 
        M5.Lcd.fillRect(110, 216, 100, 18, DARKGREY); 
        M5.Lcd.drawRect(110, 216, 100, 18, WHITE); 
        M5.Lcd.drawString("BtnB", 160, 225, 2); 
        M5.Lcd.fillRect(213, 216, 100, 18, DARKGREY); 
        M5.Lcd.drawRect(213, 216, 100, 18, WHITE); 
        M5.Lcd.drawString("BtnC", 263, 225, 2); 

        while (true) 
        {
            M5.update();
            if (M5.BtnA.isPressed()) break;
            if (M5.BtnB.isPressed()) M5.shutdown(1000); // Reboot
            if (M5.BtnC.isPressed()) M5.shutdown();
        }
    }
}


