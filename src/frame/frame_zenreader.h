#ifndef _FRAME_ZENREADER_H_
#define _FRAME_ZENREADER_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

class Frame_ZenReader : public Frame_Base
{
public:
    Frame_ZenReader();
    ~Frame_ZenReader();
    int init(epdgui_args_vector_t &args);


private:

    int device_counter;
//    M5EPD_Canvas *_blukiis_found_canvas;

    EPDGUI_Button *_key_connect_wlanxx;
    EPDGUI_Button *_key_scan_blukiisxx;
    EPDGUI_Button *_key_scan_blukiisxx2;

};


#endif //_FRAME_ZENREADER_H_