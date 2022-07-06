#ifndef _FRAME_BLUKIIHUB_H_
#define _FRAME_BLUKIIHUB_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_BlukiiHub : public Frame_Base
{
public:
    Frame_BlukiiHub();
    ~Frame_BlukiiHub();
    int init(epdgui_args_vector_t &args);

private:

    EPDGUI_Button *_key_connect_wlan;
    EPDGUI_Button *_key_scan_blukiis;


    int device_counter;
    M5EPD_Canvas *_blukiis_found_canvas;
};

#endif //_FRAME_BLUKIIHUB_H_