#include "frame_blukiihub.h"
#include <WiFi.h>
#include "blukii_scan.h"
#include "./resources/wlan_credentials.h"

#define KEY_W 92
#define KEY_H 92
const uint16_t kScanStatusCanvasY = 330;      // Position of blukiis found results.

M5EPD_Canvas *_blukiis_found_text_canvas;
M5EPD_Canvas *_scan_status_canvas;

int scan_Interval_1 = 10;
int scan_Interval_2 = 60;

void key_scan_blukiis_cb(epdgui_args_vector_t &args)
{     

    M5EPD_Canvas *title = (M5EPD_Canvas*)(args[0]);

    if(WiFi.status() == WL_CONNECTED)
    {
        // Print scan text canvas and go to scan function
        _blukiis_found_text_canvas = new M5EPD_Canvas(&M5.EPD);
        _blukiis_found_text_canvas->createCanvas(270, 40);
        _blukiis_found_text_canvas->fillCanvas(0);
        _blukiis_found_text_canvas->setTextSize(26);
        _blukiis_found_text_canvas->setTextColor(15);
        _blukiis_found_text_canvas->setTextDatum(CL_DATUM);


        _scan_status_canvas = new M5EPD_Canvas(&M5.EPD);
        _scan_status_canvas->createCanvas(350, 40);
        _scan_status_canvas->fillCanvas(0);
        _scan_status_canvas->setTextSize(26);
        _scan_status_canvas->setTextColor(15);
        _scan_status_canvas->setTextDatum(CL_DATUM);

        _scan_status_canvas->drawString("Scan Started- Please Wait..", 15, 30);
        _scan_status_canvas->pushCanvas(0, kScanStatusCanvasY, UPDATE_MODE_A2);

        _blukiis_found_text_canvas->drawString("blukiis FOUND = ", 15, 30);
        _blukiis_found_text_canvas->pushCanvas(0, kBlukiisFoundResultCanvasY, UPDATE_MODE_A2);

        blukii_scan(*((int*)(args[1])));      // blukii scanning loop

        _scan_status_canvas->fillCanvas(0);
        _scan_status_canvas->drawString("Scan and POST Finished", 15, 30);
        _scan_status_canvas->pushCanvas(0, kScanStatusCanvasY, UPDATE_MODE_A2);
    }
    else
    {
        // Print scan text canvas and go to scan function
        _blukiis_found_text_canvas = new M5EPD_Canvas(&M5.EPD);
        _blukiis_found_text_canvas->createCanvas(270, 40);
        _blukiis_found_text_canvas->fillCanvas(0);
        _blukiis_found_text_canvas->setTextSize(26);
        _blukiis_found_text_canvas->setTextColor(15);
        _blukiis_found_text_canvas->setTextDatum(CL_DATUM);

        _blukiis_found_text_canvas->drawString("No WLAN Connection", 15, 30);
        _blukiis_found_text_canvas->pushCanvas(0, kBlukiisFoundResultCanvasY, UPDATE_MODE_A2);

    }
    title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    //*((int*)(args[0])) = 0;
    EPDGUI_Draw(UPDATE_MODE_NONE);
    while(!M5.TP.avaliable());
    M5.EPD.UpdateFull(UPDATE_MODE_GL16);
    return;

}

void key_connect_wlan_cb(epdgui_args_vector_t &args)
{

    M5EPD_Canvas info(&M5.EPD);
    M5EPD_Canvas *title = (M5EPD_Canvas*)(args[0]);
    info.createCanvas(300, 100);
    info.fillCanvas(15);
    info.setTextSize(26);
    info.setTextColor(0);
    info.setTextDatum(CC_DATUM);


    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Drawing Connecting"); 
        info.drawString("Connecting", 150, 55);
        Serial.println("Drew Connecting"); 
        info.pushCanvas(120, 430, UPDATE_MODE_GL16);
        Serial.println("Pushed canvas"); 
        M5.EPD.WriteFullGram4bpp(GetWallpaper());
        
        Serial.println("Wrote fullgram wallpaper");

        title->pushCanvas(0, 8, UPDATE_MODE_NONE);
        EPDGUI_Draw(UPDATE_MODE_NONE);
        Serial.println("EPDGUI Draw mode set");
        while(!M5.TP.avaliable());
        Serial.println("checked if M5 TP is available");
        // M5.EPD.UpdateFull(UPDATE_MODE_GL16);        // This is where it refreshes the entire display.
        // Serial.println("Full update");      
        //delay(5000);

        WiFi.disconnect();
        Serial.println("Disconnected existing wlan");
        WiFi.begin(hardcoded_ssid,hardcoded_pass);
        Serial.println("trying to connect to HSW WLAN");
        uint32_t start_time = millis();
       Serial.println("Given WLAN begin command");  
        while (WiFi.status() != WL_CONNECTED)
        {
            if (millis() - start_time > 10000)
            { 
                Serial.println("Time Limit Reached");  
                info.fillCanvas(255);
                info.drawString("Connection Failed", 150, 55);
                info.pushCanvas(120, 430, UPDATE_MODE_GL16);
                delay(3000);
                M5.EPD.WriteFullGram4bpp(GetWallpaper());

                title->pushCanvas(0, 8, UPDATE_MODE_NONE);
                EPDGUI_Draw(UPDATE_MODE_NONE);
                while(!M5.TP.avaliable());
                M5.EPD.UpdateFull(UPDATE_MODE_GL16);
                WiFi.disconnect();      // Need to use disconnect here otherwise WiFi search Frame won't work
                break;
            }

        }
        Serial.println("Broke out from while loop");  
    }
    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WLAN"); 
        
        info.fillCanvas(255);
        info.drawString("WLAN Connected", 150, 55);
        info.pushCanvas(120, 430, UPDATE_MODE_GL16);


        bool ret = SyncNTPTime();
        
        if(ret == 0)
        {
            info.fillCanvas(255);
            info.drawString("Network Error!", 150, 55);
        
            info.pushCanvas(120, 430, UPDATE_MODE_GL16);
            
        }
        else
        {
            Serial.println("Before Printing Success"); 
            info.fillCanvas(255);
            info.drawString("Internet Working!", 150, 55);
        
            info.pushCanvas(120, 430, UPDATE_MODE_GL16);
            Serial.println("After Printing Success"); 
        }

        Serial.println("Trying to fullgram write"); 
        M5.EPD.WriteFullGram4bpp(GetWallpaper());
        Serial.println("After Printing Fullgram"); 

        title->pushCanvas(0, 8, UPDATE_MODE_NONE);
        EPDGUI_Draw(UPDATE_MODE_NONE);
        while(!M5.TP.avaliable());
        delay(500);
        M5.EPD.UpdateFull(UPDATE_MODE_GL16);
    }
    //*((int*)(args[0])) = 0;
}

Frame_BlukiiHub::Frame_BlukiiHub(void)
{
    _frame_name = "Frame_BlukiiHub";


    _key_connect_wlan = new EPDGUI_Button(4, 100, 532, 61);
    _key_scan_blukiis = new EPDGUI_Button(4, 160, 532, 61);
    _key_scan_blukiis2 = new EPDGUI_Button(4, 220, 532, 61);



    String device_couter_string = String(device_counter);


    _key_connect_wlan->setBMPButton("  Connect to HSW-WLAN", "\u25B6", ImageResource_item_icon_wifi_3_32x32);
    _key_scan_blukiis->setBMPButton("  Scan blukiis (10 sec) & Push to Cloud", "\u25B6", ImageResource_item_icon_ntptime_32x32);
    _key_scan_blukiis2->setBMPButton("  Scan blukiis (60 sec) & Push to Cloud", "\u25B6", ImageResource_item_icon_ntptime_32x32);

    exitbtn("Home");

    _canvas_title->drawString("blukii HUB", 270, 34);
    //_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);


    //_key_connect_wlan->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_connect_wlan->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_connect_wlan->Bind(EPDGUI_Button::EVENT_RELEASED, &key_connect_wlan_cb);


    _key_scan_blukiis->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_scan_blukiis->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&scan_Interval_1));
    _key_scan_blukiis->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cb);

    _key_scan_blukiis2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_scan_blukiis2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&scan_Interval_2));
    _key_scan_blukiis2->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cb);

}




Frame_BlukiiHub::~Frame_BlukiiHub(void)
{
    delete _key_connect_wlan;
    delete _key_scan_blukiis;
    delete _key_scan_blukiis2;
}

int Frame_BlukiiHub::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    
    EPDGUI_AddObject(_key_connect_wlan);
    EPDGUI_AddObject(_key_scan_blukiis);
    EPDGUI_AddObject(_key_scan_blukiis2);
    EPDGUI_AddObject(_key_exit);

    return 3;
}