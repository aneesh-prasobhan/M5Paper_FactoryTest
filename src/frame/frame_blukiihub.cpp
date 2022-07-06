#include "frame_blukiihub.h"
#include <WiFi.h>
#include "blukii_scan.h"

#define KEY_W 92
#define KEY_H 92
const uint16_t kBlukiisFoundCanvasY = 300;      // Position of blukiis found results.
static uint16_t devices_found;


void key_scan_blukiis_cb(epdgui_args_vector_t &args)
{

    devices_found = blukii_scan();      

    // Print number of blukiis here



    return;

}

void key_connect_wlan_cb(epdgui_args_vector_t &args)
{

    M5EPD_Canvas info(&M5.EPD);
    M5EPD_Canvas *title = (M5EPD_Canvas*)(args[0]);
    M5EPD_Canvas *tzone = (M5EPD_Canvas*)(args[1]);
    info.createCanvas(300, 100);
    info.fillCanvas(15);
    info.setTextSize(26);
    info.setTextColor(0);
    info.setTextDatum(CC_DATUM);

    char hardcoded_ssid[] = "HSW-WLAN";
    char hardcoded_pass[] = "wMqH0YAq9XAqVJq8GK";

    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Drawing Connecting"); 
        info.drawString("Connecting", 150, 55);
        Serial.println("Drew Connecting"); 
        info.pushCanvas(120, 430, UPDATE_MODE_GL16);
        Serial.println("Pushed canvas"); 
        M5.EPD.WriteFullGram4bpp(GetWallpaper());
        
        Serial.println("Wrote fullgram wallpaper");
        //title->pushCanvas(0, 8, UPDATE_MODE_NONE);
        //Serial.println("Pushed canvas title");
       // tzone->pushCanvas(4, kBlukiisFoundCanvasY, UPDATE_MODE_NONE);
        //Serial.println("Pushed canvas Time zone");
        EPDGUI_Draw(UPDATE_MODE_NONE);
        Serial.println("EPDGUI Draw mode set");
        while(!M5.TP.avaliable());
        Serial.println("checked if M5 TP is available");
        // M5.EPD.UpdateFull(UPDATE_MODE_GL16);        // This is where it refreshes the entire display.
        // Serial.println("Full update");      
        delay(5000);


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
                //title->pushCanvas(0, 8, UPDATE_MODE_NONE);
                //tzone->pushCanvas(4, kBlukiisFoundCanvasY, UPDATE_MODE_NONE);
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
        Serial.println("COnnected to WLAN"); 
        
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
        //title->pushCanvas(0, 8, UPDATE_MODE_NONE);
        //tzone->pushCanvas(4, kBlukiisFoundCanvasY, UPDATE_MODE_NONE);
        EPDGUI_Draw(UPDATE_MODE_NONE);
        while(!M5.TP.avaliable());
        delay(3000);
        M5.EPD.UpdateFull(UPDATE_MODE_GL16);
    }
}

Frame_BlukiiHub::Frame_BlukiiHub(void)
{
    _frame_name = "Frame_BlukiiHub";

    _blukiis_found_canvas = new M5EPD_Canvas(&M5.EPD);
    _blukiis_found_canvas->createCanvas(540, 90);
    _blukiis_found_canvas->fillCanvas(0);
    _blukiis_found_canvas->setTextSize(26);
    _blukiis_found_canvas->setTextColor(15);
    _blukiis_found_canvas->setTextDatum(CL_DATUM);
 
    _key_connect_wlan = new EPDGUI_Button(4, 100, 532, 61);
    _key_scan_blukiis = new EPDGUI_Button(4, 160, 532, 61);


    String device_couter_string = String(device_counter);


    _key_connect_wlan->setBMPButton("  Connect HSW-WLAN", "\u25B6", ImageResource_item_icon_wifi_3_32x32);
    _key_scan_blukiis->setBMPButton("  Scan blukiis & Push to Cloud", "\u25B6", ImageResource_item_icon_ntptime_32x32);
    _blukiis_found_canvas->drawString("blukiis FOUND = ", 15, 35);
    exitbtn("Home");
    _canvas_title->drawString("blukii HUB", 270, 34);


    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    _key_scan_blukiis->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_scan_blukiis->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cb);

    _key_connect_wlan->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_connect_wlan->Bind(EPDGUI_Button::EVENT_RELEASED, &key_connect_wlan_cb);

    _key_scan_blukiis->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, _canvas_title);
    _key_scan_blukiis->AddArgs(EPDGUI_Button::EVENT_RELEASED, 1, _blukiis_found_canvas);
    _key_scan_blukiis->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cb);

    device_counter = 25;
}




Frame_BlukiiHub::~Frame_BlukiiHub(void)
{
    delete _key_connect_wlan;
    delete _key_scan_blukiis;
}

int Frame_BlukiiHub::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    _blukiis_found_canvas->pushCanvas(0, kBlukiisFoundCanvasY, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_connect_wlan);
    EPDGUI_AddObject(_key_scan_blukiis);
    EPDGUI_AddObject(_key_exit);

    return 3;
}