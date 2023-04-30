
#ifndef __STATEPATTERN_H__INCLUDED__
#define __STATEPATTERN_H__INCLUDED__

#ifndef M5CORE2
#define M5CORE2
#endif

#include <Arduino.h>
#include <M5Core2.h>
// #include <SystemController.h>
#include <StateBase.h>
#include <Logging.h>
#include <ViewDesigner.h>
//#include <RunScene.h>

//class SystemController; //前方宣言


/******************************************************/

/********************* DeepSleep **********************/

/******************************************************/


class DeepSleep : public State
{
    private:
        DeepSleep();
    public:
        ~DeepSleep();
        static bool configure();
        static DeepSleep* getInstance();

    public:
        void show()     override;
        void write()    override;
        void notify()   override;
        void update()   override;
        void setup()    override;
        
    private:
        static DeepSleep* _singleton;
        static const char* _classname;
        static displayInfo _currentDisplayInfo;
        static textInfo _currentTextInfo;
};


/******************************************************/

/******************** LightSleep **********************/

/******************************************************/


class LightSleep : public State
{
    private:
        LightSleep();
    public:
        ~LightSleep();
        static bool configure();
        static LightSleep* getInstance();

    public:
        void show()     override;
        void write()    override;
        void notify()   override;
        void update()   override;
        void setup()    override;
    
    private:
        LightSleep* getNextState(UtilState statename);
    
    private:
        static LightSleep* _singleton;
        static const char* _classname;
        static unsigned long _prevTime;
        static unsigned long _currentTime;
        static bool _isPressed;
};


/******************************************************/

/*********************** Setup ************************/

/******************************************************/


class Setup : public State
{
    private:
        Setup();
    public:
        ~Setup();
        static bool configure();
        static Setup* getInstance();

    public:
        void show()     override;
        void write()    override;
        void notify()   override;
        void update()   override;
        void setup()    override;
        void progressBar(uint8_t percent);
    
    private:
        Setup* getNextState(UtilState statename);
    
    private:
        static Setup* _singleton;
        static const char* _classname;
        static displayInfo _currentDisplayInfo;
        static textInfo _currentTextInfo;
};


/******************************************************/

/********************** Waiting ***********************/

/******************************************************/

class Waiting : public State
{
    private:
        Waiting();
    public:
        ~Waiting();
        static bool configure();
        static Waiting* getInstance();

    public:
        void show()     override;
        void write()    override;
        void notify()   override;
        void update()   override;
        void setup()    override;
    
    private:
        Waiting* getNextState(UtilState statename);
    
    private:
        static Waiting* _singleton;
        static const char* _classname;
        static displayInfo _currentDisplayInfo;
        static textInfo _currentTextInfo;
        static RTC_TimeTypeDef TimeStruct;
};

/******************************************************/

/********************** Running ***********************/

/******************************************************/


class Running : public State
{
    private:
        Running();
    public:
        ~Running();
        static bool configure();
        static Running* getInstance();

    public:
        void show()     override;
        void write()    override;
        void notify()   override;
        void run();
        void update()   override;
        void setup()    override;
    
    private:
        Running* getNextState(UtilState statename);
    
    private:
        static Running* _singleton;
        static const char* _classname;
        static displayInfo _currentDisplayInfo;
        //static textInfo _currentTextInfo;
        //static RunScene* _currentScene;
};

/******************************************************/

/********************** Gohome ************************/

/******************************************************/



class Gohome : public State
{
    private:
        Gohome();
    public:
        ~Gohome();
        static bool configure();
        static Gohome* getInstance();

    public:
        void show()     override;
        void write()    override;
        void notify()   override;
        void run();
        void update()   override;
        void setup()    override;
    
    private:
        Gohome* getNextState(UtilState statename);
    
    private:
        static Gohome* _singleton;
        static const char* _classname;
        static displayInfo _currentDisplayInfo;
        //static textInfo _currentTextInfo;
        //static RunScene* _currentScene;
        //static RunScene* _scene;
};

#endif
