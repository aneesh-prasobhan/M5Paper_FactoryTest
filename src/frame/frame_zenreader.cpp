#include "frame_zenreader.h"
#include <WiFi.h>
//#include "blukii_scan.h"
//#include "./resources/wlan_credentials.h"

#define KEY_W 92
#define KEY_H 92
const uint16_t kScanStatusCanvasY = 330;      // Position of blukiis found results.
const uint16_t kBlukiisFoundResultCanvasY = 300; //for testing

M5EPD_Canvas *_blukiis_found_text_canvasxx;
M5EPD_Canvas *_scan_status_canvasxx;

int scan_Interval_1xx = 10;
int scan_Interval_2xx = 60;

void key_scan_blukiis_cbxx(epdgui_args_vector_t &args)
{     

    M5EPD_Canvas *title = (M5EPD_Canvas*)(args[0]);

    if(WiFi.status() == WL_CONNECTED)
    {
        // Print scan text canvas and go to scan function
        _blukiis_found_text_canvasxx = new M5EPD_Canvas(&M5.EPD);
        _blukiis_found_text_canvasxx->createCanvas(270, 40);
        _blukiis_found_text_canvasxx->fillCanvas(0);
        _blukiis_found_text_canvasxx->setTextSize(26);
        _blukiis_found_text_canvasxx->setTextColor(15);
        _blukiis_found_text_canvasxx->setTextDatum(CL_DATUM);


        _scan_status_canvasxx = new M5EPD_Canvas(&M5.EPD);
        _scan_status_canvasxx->createCanvas(350, 40);
        _scan_status_canvasxx->fillCanvas(0);
        _scan_status_canvasxx->setTextSize(26);
        _scan_status_canvasxx->setTextColor(15);
        _scan_status_canvasxx->setTextDatum(CL_DATUM);

        _scan_status_canvasxx->drawString("Scan Started- Please Wait..", 15, 30);
        _scan_status_canvasxx->pushCanvas(0, kScanStatusCanvasY, UPDATE_MODE_A2);

        _blukiis_found_text_canvasxx->drawString("blukiis FOUND = ", 15, 30);
        _blukiis_found_text_canvasxx->pushCanvas(0, kBlukiisFoundResultCanvasY, UPDATE_MODE_A2);

        return;      // blukii scanning loop

        _scan_status_canvasxx->fillCanvas(0);
        _scan_status_canvasxx->drawString("Scan and POST Finished", 15, 30);
        _scan_status_canvasxx->pushCanvas(0, kScanStatusCanvasY, UPDATE_MODE_A2);
    }
    else
    {
        // Print scan text canvas and go to scan function
        _blukiis_found_text_canvasxx = new M5EPD_Canvas(&M5.EPD);
        _blukiis_found_text_canvasxx->createCanvas(270, 40);
        _blukiis_found_text_canvasxx->fillCanvas(0);
        _blukiis_found_text_canvasxx->setTextSize(26);
        _blukiis_found_text_canvasxx->setTextColor(15);
        _blukiis_found_text_canvasxx->setTextDatum(CL_DATUM);

        _blukiis_found_text_canvasxx->drawString("No WLAN Connection", 15, 30);
        _blukiis_found_text_canvasxx->pushCanvas(0, kBlukiisFoundResultCanvasY, UPDATE_MODE_A2);

    }
    title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    //*((int*)(args[0])) = 0;
    EPDGUI_Draw(UPDATE_MODE_NONE);
    while(!M5.TP.avaliable());
    M5.EPD.UpdateFull(UPDATE_MODE_GL16);
    return;

}

void key_connect_wlan_cbxx(epdgui_args_vector_t &args)
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
        WiFi.begin("WN-1-3","unDWWhY8!n4TvTr%KQJw");
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

Frame_ZenReader::Frame_ZenReader(void)
{
    _frame_name = "Frame_ZenReader";


    _key_connect_wlanxx = new EPDGUI_Button(4, 100, 532, 61);
    _key_scan_blukiisxx = new EPDGUI_Button(4, 160, 532, 61);
    _key_scan_blukiisxx2 = new EPDGUI_Button(4, 220, 532, 61);



    String device_couter_string = String(device_counter);


    _key_connect_wlanxx->setBMPButton("  Connect to HSW-WLAN", "\u25B6", ImageResource_item_icon_wifi_3_32x32);
    _key_scan_blukiisxx->setBMPButton("  Scan blukiis (10 sec) & Push to Cloud", "\u25B6", ImageResource_item_icon_ntptime_32x32);
    _key_scan_blukiisxx2->setBMPButton("  Scan blukiis (60 sec) & Push to Cloud", "\u25B6", ImageResource_item_icon_ntptime_32x32);

    exitbtn("Home");

    _canvas_title->drawString("blukii HUB", 270, 34);
    //_canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);


    //_key_connect_wlanxx->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_connect_wlanxx->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_connect_wlanxx->Bind(EPDGUI_Button::EVENT_RELEASED, &key_connect_wlan_cbxx);


    _key_scan_blukiisxx->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_scan_blukiisxx->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&scan_Interval_1xx));
    _key_scan_blukiisxx->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cbxx);

    _key_scan_blukiisxx2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_scan_blukiisxx2->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, (void*)(&scan_Interval_2xx));
    _key_scan_blukiisxx2->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cbxx);

}




Frame_ZenReader::~Frame_ZenReader(void)
{
    delete _key_connect_wlanxx;
    delete _key_scan_blukiisxx;
    delete _key_scan_blukiisxx2;
}

int Frame_ZenReader::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    
    EPDGUI_AddObject(_key_connect_wlanxx);
    EPDGUI_AddObject(_key_scan_blukiisxx);
    EPDGUI_AddObject(_key_scan_blukiisxx2);
    EPDGUI_AddObject(_key_exit);

    return 3;
}