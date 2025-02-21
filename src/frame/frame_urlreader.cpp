#include <WiFi.h>
#include <HTTPClient.h>
#include <MD5Builder.h>

#include "frame_urlreader.h"
#include "../utils/urlencoder.h"

void urlreader_key_txtreader_exit_cb(epdgui_args_vector_t &args)
{
    EPDGUI_PopFrame(true);
    *((int*)(args[0])) = 0;
}

int8_t urlreader_key_operation = 0;
void urlreader_key_nextpage_released_cb(epdgui_args_vector_t &args)
{
    urlreader_key_operation = 1;
}

void urlreader_key_prevpage_released_cb(epdgui_args_vector_t &args)
{
    urlreader_key_operation = -1;
}

Frame_urlReader::Frame_urlReader(String url)
{
    _frame_name = "Frame_urlReader";
    _url = url;
    //_text_size = GetTextSize();
    _text_size = 26;

    _canvas_prev = new M5EPD_Canvas(&M5.EPD);
    _canvas_current = new M5EPD_Canvas(&M5.EPD);
    _canvas_next = new M5EPD_Canvas(&M5.EPD);
    _canvas_page = new M5EPD_Canvas(&M5.EPD);

    _key_next = new EPDGUI_Button("", 270, 72, 270, 888, EPDGUI_Button::STYLE_INVISABLE);
    _key_prev = new EPDGUI_Button("", 0, 72, 270, 888, EPDGUI_Button::STYLE_INVISABLE);
    // _key_next->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, &_key_operation);
    _key_next->Bind(EPDGUI_Button::EVENT_RELEASED, urlreader_key_nextpage_released_cb);
    // _key_prev->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, &_key_operation);
    _key_prev->Bind(EPDGUI_Button::EVENT_RELEASED, urlreader_key_prevpage_released_cb);

    _language = GetLanguage();
    if(_language == LANGUAGE_JA)
    {
        exitbtn("戻る");
    }
    else if(_language == LANGUAGE_ZH)
    {
        exitbtn("返回");
    }
    else
    {
        exitbtn("Back");
    }

    _canvas_title->drawString(url.substring(url.lastIndexOf("/") + 1, url.lastIndexOf(".")), 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &urlreader_key_txtreader_exit_cb);
}

Frame_urlReader::~Frame_urlReader(void)
{
    if(_text_size != 26)
    {
        _canvas_prev->destoryRender(_text_size);
    }
    delete _canvas_prev;
    delete _canvas_current;
    delete _canvas_next;
    delete _canvas_page;
    delete _key_next;
    delete _key_prev;
    _txt_file.close();
}

uint32_t Frame_urlReader::renderText(uint32_t cursor, uint32_t length, M5EPD_Canvas *canvas)
{
    canvas->fillCanvas(0);
    canvas->setTextArea(10, 10, 520, 868);
    canvas->setTextSize(_text_size);
    canvas->setTextColor(15);
    char buf[length + 1];
    _txt_file.seek(cursor, SeekSet);
    uint32_t readlen = _txt_file.readBytes(buf, length);
    buf[readlen] = '\0';
    log_d("trying to Print RSS big chunk Text");
    canvas->print(buf);
    log_d("After RSS print");
    //M5.EPD.UpdateFull(UPDATE_MODE_GL16);
    log_d("After Updating");
   

    return canvas->getExceedOffset();
}

int Frame_urlReader::run()
{
    if(_is_first)
    {
        LoadingAnime_32x32_Start(254, 500);
        _is_first = false;
        uint32_t cursor;
        _page_cursor.insert(std::pair<uint32_t, uint32_t>(0, 0));
        cursor = renderText(0, _render_len, _canvas_current);
        if(cursor == 0)
        {
            _page_end = 0;
            _end_accessed = true;
        }
        else
        {
            _page_end = _page + 1;
            _page_cursor.insert(std::pair<uint32_t, uint32_t>(1, cursor));
            uint32_t offset = renderText(_page_cursor[1], _render_len, _canvas_next);
            if(offset == 0)
            {
                _page_end = 1;
                _end_accessed = true;
            }
            else
            {
                _page_cursor.insert(std::pair<uint32_t, uint32_t>(2, cursor + offset));
            }
        }
        LoadingAnime_32x32_Stop();
        _canvas_current->pushCanvas(0, 72, UPDATE_MODE_GL16);
    }
    else
    {
        M5.update();
        if(M5.BtnR.wasReleased() || (urlreader_key_operation == 1))
        {
            urlreader_key_operation = 0;
            if(_page != _page_end)
            {
                _page++;
                _canvas_next->pushCanvas(0, 72, UPDATE_MODE_GL16);
                memcpy(_canvas_prev->frameBuffer(), _canvas_current->frameBuffer(), _canvas_current->getBufferSize());
                memcpy(_canvas_current->frameBuffer(), _canvas_next->frameBuffer(), _canvas_next->getBufferSize());

                if((_end_accessed == false) || (_page != _page_end))
                {
                    uint32_t offset = renderText(_page_cursor[_page + 1], _render_len, _canvas_next);

                    if(offset != 0)
                    {
                        if(_page_cursor.count(_page + 2) == 0)
                        {
                            _page_cursor.insert(std::pair<uint32_t, uint32_t>(_page + 2, _page_cursor[_page + 1] + offset));
                        }
                    }
                    else if(_end_accessed == false)
                    {
                        _page_end = _page + 1;
                        _end_accessed = true;
                    }
                    if(!_end_accessed)
                    {
                        _page_end = _page + 1;
                    }
                }
            }
        }
        else if(M5.BtnL.wasReleased() || (urlreader_key_operation == -1))
        {
            urlreader_key_operation = 0;
            if(_page > 0)
            {
                _page--;
                _canvas_prev->pushCanvas(0, 72, UPDATE_MODE_GL16);
                memcpy(_canvas_next->frameBuffer(), _canvas_current->frameBuffer(), _canvas_current->getBufferSize());
                memcpy(_canvas_current->frameBuffer(), _canvas_prev->frameBuffer(), _canvas_prev->getBufferSize());
                if(_page != 0)
                {
                    renderText(_page_cursor[_page - 1], _render_len, _canvas_prev);
                }
            }
        }
    }
    if(_last_page != _page)
    {
        _last_page = _page;
        _canvas_page->setTextSize(26);
        _canvas_page->fillCanvas(0);
        _canvas_page->setTextColor(15);
        char buf[5];
        sprintf(buf, "%d", _page + 1);
        _canvas_page->drawString(buf, 100, 42);
        _canvas_page->pushCanvas(440 - 15, 0, UPDATE_MODE_GL16);
    }
    return 1;
}

int Frame_urlReader::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    _canvas_prev->createCanvas(540, 888);
    _canvas_current->createCanvas(540, 888);
    _canvas_next->createCanvas(540, 888);
    _canvas_page->createCanvas(100, 60);
    _canvas_page->setTextDatum(CR_DATUM);
    if(!_canvas_prev->isRenderExist(_text_size))
    {
        _canvas_prev->createRender(_text_size, 128);
    }
    EPDGUI_AddObject(_key_exit);
    EPDGUI_AddObject(_key_next);
    EPDGUI_AddObject(_key_prev);

    MD5Builder md5b;
    md5b.begin();
    md5b.add(_url);
    md5b.calculate();

    String _url_hash = md5b.toString();
    String _cache_filename = "/artcache/" + _url_hash + ".txt";

    if (! SD.exists(_cache_filename)) {
        if (! cacheFileSD(_url, _cache_filename)) {
            log_e("URL %s cannot be cached.", _url.c_str());
        } else {
            log_d("URL %s cached as %s.", _url.c_str(), _cache_filename.c_str());
        }
    }
    
    _txt_file = SD.open(_cache_filename);

    _last_page = 0;
    return 3;
}

bool Frame_urlReader::cacheFileSD(String url, String filename) {
    if (WiFi.status() != WL_CONNECTED) {
        log_e("URL %s is not cached and you're not connected to the internet, gave up.", url.c_str());
        return false;
    }

    HTTPClient http;
    http.begin("http://article-proxy.tnhh.net/article/?url=" + URLEncoder::urlencode(url));
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        File f = SD.open(filename, FILE_WRITE);
        http.writeToStream(&f);
        f.close();
        log_d("URL %s cached successfully.", url.c_str());
    } else {
        log_d("URL %s caching failed.", url.c_str());
        return false;
    }

    return true;
}