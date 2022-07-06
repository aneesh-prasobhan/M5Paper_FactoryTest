#include "frame_blukiihub.h"

const uint16_t kBlukiisFoundCanvasY = 260;      // Position of blukiis found results.


void key_scan_blukiis_cb(epdgui_args_vector_t &args)
{
    Shutdown();
}

void key_connect_wlan_cb(epdgui_args_vector_t &args)
{
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
    SaveSetting();
    esp_restart();
}

Frame_BlukiiHub::Frame_BlukiiHub(void)
{
    _frame_name = "Frame_BlukiiHub";

    _blukiis_found_canvas = new M5EPD_Canvas(&M5.EPD);
    _blukiis_found_canvas->createCanvas(540, 100);
    _blukiis_found_canvas->fillCanvas(0);
    _blukiis_found_canvas->setTextSize(90);
    _blukiis_found_canvas->setTextColor(15);
    _blukiis_found_canvas->setTextDatum(CL_DATUM);
 
    _key_connect_wlan = new EPDGUI_Button(4, 100, 532, 61);
    _key_scan_blukiis = new EPDGUI_Button(4, 160, 532, 61);


    String device_couter_string = String(device_counter);


    _key_connect_wlan->setBMPButton("  Connect HSW-WLAN", "\u25B6", ImageResource_item_icon_wifi_3_32x32);
    _key_scan_blukiis->setBMPButton("  Scan & Push to Cloud", "\u25B6", ImageResource_item_icon_ntptime_32x32);
    _blukiis_found_canvas->drawString("blukiis FOUND = ", 15, 35);
    exitbtn("Home");
    _canvas_title->drawString("blukii HUB", 270, 34);


    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    _key_scan_blukiis->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_scan_blukiis->Bind(EPDGUI_Button::EVENT_RELEASED, &key_scan_blukiis_cb);

    _key_connect_wlan->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_connect_wlan->Bind(EPDGUI_Button::EVENT_RELEASED, &key_connect_wlan_cb);


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