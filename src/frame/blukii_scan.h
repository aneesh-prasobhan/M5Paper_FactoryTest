#ifndef _FRAME_BLUKII_SCAN_H_
#define _FRAME_BLUKII_SCAN_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"

M5EPD_Canvas *_blukiis_found_results_canvas;
const uint16_t kBlukiisFoundResultCanvasY = 300;


int devs;
String dev_string;

uint16_t blukii_scan();


uint16_t blukii_scan()
{

    _blukiis_found_results_canvas = new M5EPD_Canvas(&M5.EPD);
    _blukiis_found_results_canvas->createCanvas(70, 90);
    _blukiis_found_results_canvas->fillCanvas(0);
    _blukiis_found_results_canvas->setTextSize(26);
    _blukiis_found_results_canvas->setTextColor(15);
    _blukiis_found_results_canvas->setTextDatum(CL_DATUM);


    devs = 10;
    dev_string =  String(devs);

    _blukiis_found_results_canvas->fillCanvas(0);
    _blukiis_found_results_canvas->drawString(dev_string, 15, 35);
    _blukiis_found_results_canvas->pushCanvas(271, kBlukiisFoundResultCanvasY, UPDATE_MODE_A2);

    delay(2000);
    devs = devs + 20;

    dev_string =  String(devs);

    _blukiis_found_results_canvas->fillCanvas(0);
    _blukiis_found_results_canvas->drawString(dev_string, 15, 35);
    _blukiis_found_results_canvas->pushCanvas(271, kBlukiisFoundResultCanvasY, UPDATE_MODE_A2);


    return(devs);
}

#endif //_FRAME_BLUKII_SCAN_H_