#include "frame_main.h"
#include "frame_setting.h"
#include "frame_keyboard.h"
#include "frame_factorytest.h"
#include "frame_wifiscan.h"
#include "frame_lifegame.h"
#include "frame_fileindex.h"
#include "frame_compare.h"
#include "frame_home.h"
#include "frame_blukiihub.h"
#include "frame_zenreader.h"
#include "frame_fileindex.h"



enum
{
    kKeyFactoryTest = 0,
    kKeySetting,
    kKeyKeyboard,
    kKeyWifiScan,
    kKeySDFile,
    kKeyCompare,
    kKeyHome,
    kKeyLifeGame,
    kKeyBlukiiHub,
    kKeyZenReader
};

#define KEY_W 92
#define KEY_H 92

void key_setting_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Setting");
    if(frame == NULL)
    {
        frame = new Frame_Setting();
        EPDGUI_AddFrame("Frame_Setting", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_keyboard_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Keyboard");
    if(frame == NULL)
    {
        frame = new Frame_Keyboard(0);
        EPDGUI_AddFrame("Frame_Keyboard", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_factorytest_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_FactoryTest");
    if(frame == NULL)
    {
        frame = new Frame_FactoryTest();
        EPDGUI_AddFrame("Frame_FactoryTest", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_wifiscan_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_WifiScan");
    if(frame == NULL)
    {
        frame = new Frame_WifiScan();
        EPDGUI_AddFrame("Frame_WifiScan", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_lifegame_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Lifegame");
    if(frame == NULL)
    {
        frame = new Frame_Lifegame();
        EPDGUI_AddFrame("Frame_Lifegame", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_sdfile_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = new Frame_FileIndex("/");
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_compare_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Compare");
    if(frame == NULL)
    {
        frame = new Frame_Compare();
        EPDGUI_AddFrame("Frame_Compare", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void key_home_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Home");
    if(frame == NULL)
    {
        frame = new Frame_Home();
        EPDGUI_AddFrame("Frame_Home", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

//blukiiHub
void key_blukiihub_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_BlukiiHub");
    if(frame == NULL)
    {
        frame = new Frame_BlukiiHub();
        EPDGUI_AddFrame("Frame_BlukiiHub", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

//ZenReader
void key_zenreader_cb(epdgui_args_vector_t &args)
{
    Frame_Base *frame = EPDGUI_GetFrame("Frame_ZenReader");
    if(frame == NULL)
    {
        frame = new Frame_ZenReader("foo.txt");
        EPDGUI_AddFrame("Frame_ZenReader", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

Frame_Main::Frame_Main(void): Frame_Base(false)
{
    _frame_name = "Frame_Main";
    _frame_id = 1;

    _bar = new M5EPD_Canvas(&M5.EPD);
    _bar->createCanvas(540, 44);
    _bar->setTextSize(26);

    _names = new M5EPD_Canvas(&M5.EPD);
    _names->createCanvas(540, 32);
    _names->setTextDatum(CC_DATUM);

    //Creating custom canvas with 1 icon length to not over write the Engine wallpaper area
    _name_single = new M5EPD_Canvas(&M5.EPD);
    _name_single->createCanvas(135, 32);
    _name_single->setTextDatum(CC_DATUM);

    //Creating custom canvas with 1 icon length for zen reader
    _name_zenreader = new M5EPD_Canvas(&M5.EPD);
    _name_zenreader->createCanvas(135, 32);
    _name_zenreader->setTextDatum(CC_DATUM);
    
    for(int i = 0; i < 4; i++)
    {
        _key[i] = new EPDGUI_Button("测试", 20 + i * 136, 90, KEY_W, KEY_H);
    }

    for(int i = 0; i < 4; i++)
    {
        _key[i + 4] = new EPDGUI_Button("测试", 20 + i * 136, 240, KEY_W, KEY_H);
    }

    for(int i = 0; i < 2; i++)
    {
        _key[i + 8] = new EPDGUI_Button("测试", 20 + i * 136, 390, KEY_W, KEY_H);
    }

    _key[kKeySetting]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_setting_92x92);
    *(_key[kKeySetting]->CanvasPressed()) = *(_key[kKeySetting]->CanvasNormal());
    _key[kKeySetting]->CanvasPressed()->ReverseColor();
    _key[kKeySetting]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeySetting]->Bind(EPDGUI_Button::EVENT_RELEASED, key_setting_cb);

    _key[kKeyKeyboard]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_keyboard_92x92);
    *(_key[kKeyKeyboard]->CanvasPressed()) = *(_key[kKeyKeyboard]->CanvasNormal());
    _key[kKeyKeyboard]->CanvasPressed()->ReverseColor();
    _key[kKeyKeyboard]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyKeyboard]->Bind(EPDGUI_Button::EVENT_RELEASED, key_keyboard_cb);

    _key[kKeyFactoryTest]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_factorytest_92x92);
    *(_key[kKeyFactoryTest]->CanvasPressed()) = *(_key[kKeyFactoryTest]->CanvasNormal());
    _key[kKeyFactoryTest]->CanvasPressed()->ReverseColor();
    _key[kKeyFactoryTest]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyFactoryTest]->Bind(EPDGUI_Button::EVENT_RELEASED, key_factorytest_cb);

    _key[kKeyWifiScan]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_wifi_92x92);
    *(_key[kKeyWifiScan]->CanvasPressed()) = *(_key[kKeyWifiScan]->CanvasNormal());
    _key[kKeyWifiScan]->CanvasPressed()->ReverseColor();
    _key[kKeyWifiScan]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyWifiScan]->Bind(EPDGUI_Button::EVENT_RELEASED, key_wifiscan_cb);

    _key[kKeyLifeGame]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_lifegame_92x92);
    *(_key[kKeyLifeGame]->CanvasPressed()) = *(_key[kKeyLifeGame]->CanvasNormal());
    _key[kKeyLifeGame]->CanvasPressed()->ReverseColor();
    _key[kKeyLifeGame]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyLifeGame]->Bind(EPDGUI_Button::EVENT_RELEASED, key_lifegame_cb);

    _key[kKeySDFile]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_sdcard_92x92);
    *(_key[kKeySDFile]->CanvasPressed()) = *(_key[kKeySDFile]->CanvasNormal());
    _key[kKeySDFile]->CanvasPressed()->ReverseColor();
    _key[kKeySDFile]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeySDFile]->Bind(EPDGUI_Button::EVENT_RELEASED, key_sdfile_cb);

    _key[kKeyCompare]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_compare_92x92);
    *(_key[kKeyCompare]->CanvasPressed()) = *(_key[kKeyCompare]->CanvasNormal());
    _key[kKeyCompare]->CanvasPressed()->ReverseColor();
    _key[kKeyCompare]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyCompare]->Bind(EPDGUI_Button::EVENT_RELEASED, key_compare_cb);

    _key[kKeyHome]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_home_92x92);
    *(_key[kKeyHome]->CanvasPressed()) = *(_key[kKeyHome]->CanvasNormal());
    _key[kKeyHome]->CanvasPressed()->ReverseColor();
    _key[kKeyHome]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyHome]->Bind(EPDGUI_Button::EVENT_RELEASED, key_home_cb);

    // Custom APP Beginning
    _key[kKeyBlukiiHub]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_main_icon_blukiihub_92x92);
    *(_key[kKeyBlukiiHub]->CanvasPressed()) = *(_key[kKeyBlukiiHub]->CanvasNormal());
    _key[kKeyBlukiiHub]->CanvasPressed()->ReverseColor();
    _key[kKeyBlukiiHub]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyBlukiiHub]->Bind(EPDGUI_Button::EVENT_RELEASED, key_blukiihub_cb);

    //ZenReader
    _key[kKeyZenReader]->CanvasNormal()->pushImage(0, 0, 92, 92, ImageResource_zenreader_92x92);
    *(_key[kKeyZenReader]->CanvasPressed()) = *(_key[kKeyZenReader]->CanvasNormal());
    _key[kKeyZenReader]->CanvasPressed()->ReverseColor();
    _key[kKeyZenReader]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key[kKeyZenReader]->Bind(EPDGUI_Button::EVENT_RELEASED, key_zenreader_cb);



    _time = 0;
    _next_update_time = 0;
}


Frame_Main::~Frame_Main(void)
{
    for(int i = 0; i < 10; i++)
    {
        delete _key[i];
    }
}

void Frame_Main::AppName(m5epd_update_mode_t mode)
{
    if(!_names->isRenderExist(20))
    {
        _names->createRender(20, 26);
    }


    //Added new
    if(!_name_single->isRenderExist(20))
    {
        _name_single->createRender(20, 26);
    }

    if(!_name_zenreader->isRenderExist(20))
    {
        _name_zenreader->createRender(20, 26);
    }

    _names->setTextSize(20);
    _name_single->setTextSize(20);      // Added new
    _name_zenreader->setTextSize(20);      // Added new



    _names->fillCanvas(0);
    uint8_t language = GetLanguage();
    _names->drawString("WLAN", 20 + 46 + 3 * 136, 16);
    if(language == LANGUAGE_JA)
    {
        _names->drawString("工場テスト", 20 + 46, 16);
        _names->drawString("設定", 20 + 46 + 136, 16);
        _names->drawString("鍵盤", 20 + 46 + 2 * 136, 16);
    }
    else if(language == LANGUAGE_ZH)
    {
        _names->drawString("出厂测试", 20 + 46, 16);
        _names->drawString("设定", 20 + 46 + 136, 16);
        _names->drawString("键盘", 20 + 46 + 2 * 136, 16);
    }
    else
    {
        _names->drawString("Test", 20 + 46, 16);
        _names->drawString("Setting", 20 + 46 + 136, 16);
        _names->drawString("Keyboard", 20 + 46 + 2 * 136, 16);
    }
    _names->pushCanvas(0, 186, mode);
    
    _names->fillCanvas(0);
    if(language == LANGUAGE_JA)
    {
        _names->drawString("メモリー", 20 + 46, 16);
        _names->drawString("刷新比較", 20 + 46 + 136, 16);
        _names->drawString("家", 20 + 46 + 2 * 136, 16);
        _names->drawString("ライフゲーム", 20 + 46 + 3 * 136, 16);
    }
    else if(language == LANGUAGE_ZH)
    {
        _names->drawString("存储", 20 + 46, 16);
        _names->drawString("刷新比较", 20 + 46 + 136, 16);
        _names->drawString("家", 20 + 46 + 2 * 136, 16);
        _names->drawString("生命游戏", 20 + 46 + 3 * 136, 16);
    }
    else
    {
        _names->drawString("Storage", 20 + 46, 16);
        _names->drawString("Compare", 20 + 46 + 136, 16);
        _names->drawString("Home", 20 + 46 + 2 * 136, 16);
        _names->drawString("LifeGame", 20 + 46 + 3 * 136, 16);
    }
    _names->pushCanvas(0, 337, mode);
    

    //New line for writing app name on the 3rd row, but only for 1 icon with a custom canvas 
    _name_single->fillCanvas(0);            
    _name_single->drawString("blukiiHUB", 20 + 46, 16);
    _name_single->pushCanvas(0, 488, mode);

    _name_zenreader->fillCanvas(0);            
    _name_zenreader->drawString("ZenReader", 20 + 46 + 136, 16);
    _name_zenreader->pushCanvas(110, 488, mode);
    
}

void Frame_Main::StatusBar(m5epd_update_mode_t mode)
{
    if((millis() - _time) < _next_update_time)
    {
        return;
    }
    char buf[20];
    _bar->fillCanvas(0);
    _bar->drawFastHLine(0, 43, 540, 15);
    _bar->setTextDatum(CL_DATUM);
    _bar->drawString("M5Paper", 10, 27);

    // Battery
    _bar->setTextDatum(CR_DATUM);
    _bar->pushImage(498, 8, 32, 32, ImageResource_status_bar_battery_32x32);
    uint32_t vol = M5.getBatteryVoltage();

    if(vol < 3300)
    {
        vol = 3300;
    }
    else if(vol > 4350)
    {
        vol = 4350;
    }
    float battery = (float)(vol - 3300) / (float)(4350 - 3300);
    if(battery <= 0.01)
    {
        battery = 0.01;
    }
    if(battery > 1)
    {
        battery = 1;
    }
    uint8_t px = battery * 25;
    sprintf(buf, "%d%%", (int)(battery * 100));
    // _bar->drawString(buf, 498 - 10, 27);
    _bar->fillRect(498 + 3, 8 + 10, px, 13, 15);
    // _bar->pushImage(498, 8, 32, 32, 2, ImageResource_status_bar_battery_charging_32x32);

    // Time
    rtc_time_t time_struct;
    rtc_date_t date_struct;
    M5.RTC.getTime(&time_struct);
    M5.RTC.getDate(&date_struct);
    sprintf(buf, "%2d:%02d", time_struct.hour, time_struct.min);
    _bar->setTextDatum(CC_DATUM);
    _bar->drawString(buf, 270, 27);
    _bar->pushCanvas(0, 0, mode);

    _time = millis();
    _next_update_time = (60 - time_struct.sec) * 1000;
}


int Frame_Main::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    for(int i = 0; i < 10; i++)
    {
        EPDGUI_AddObject(_key[i]);
    }
    _time = 0;
    _next_update_time = 0;
    StatusBar(UPDATE_MODE_NONE);
    AppName(UPDATE_MODE_NONE);
    return 9;
}

int Frame_Main::run()
{
    Frame_Base::run();
    StatusBar(UPDATE_MODE_GL16);
    return 1;
}