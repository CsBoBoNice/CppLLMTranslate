
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

#include <thread>

#include "UDP_Client.h"
#include "MessageManager.h"
#include "ubantu_copy.h"

#include <cstring>

static lv_display_t *hal_init(int32_t w, int32_t h);

lv_obj_t *ta_input = NULL;
lv_obj_t *ta_output = NULL;

std::string copy_text_last;

void Clipboard_Singleton_timer(lv_timer_t *timer)
{
    std::string copy_text = ClipboardSingleton::getInstance().getCopyString();
    if (copy_text != copy_text_last) {
        lv_textarea_set_text(ta_input, copy_text.c_str());
        copy_text_last = copy_text;
    }
}

void my_timer(lv_timer_t *timer)
{
    std::string show_text;
    MessageManager &manager = MessageManager::getInstance();
    if (manager.popFromInputQueueNoWait(show_text)) {
        lv_textarea_add_text(ta_output, show_text.c_str());
    }
}

static void translate_button_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_CLICKED) {
        lv_textarea_set_text(ta_output, "");

        std::string src_text = lv_textarea_get_text(ta_input);

        MessageManager &manager = MessageManager::getInstance();
        manager.pushToOutputQueue(src_text);
    }
}

void ui_main()
{

    /*Create a font*/
    lv_font_t *font = lv_freetype_font_create("/home/csbobo/code/lv_port_pc_vscode/SmileySans-Oblique.ttf",
                                              LV_FREETYPE_FONT_RENDER_MODE_BITMAP, 16, LV_FREETYPE_FONT_STYLE_NORMAL);

    if (!font) {
        LV_LOG_ERROR("freetype font create failed.");
        return;
    }

    /*Create style with the new font*/
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, font);
    // lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    // /*Create a label with the new style*/
    // lv_obj_t *label = lv_label_create(lv_screen_active());
    // lv_obj_add_style(label, &style, 0);
    // lv_label_set_text(label, "Hello world\nI'm a font created with FreeType");
    // lv_obj_center(label);

    ta_input = lv_textarea_create(lv_screen_active());
    lv_obj_set_size(ta_input, 300, 500);
    lv_obj_set_pos(ta_input, 100, 0);
    lv_obj_add_style(ta_input, &style, 0);

    lv_textarea_set_placeholder_text(ta_input, "输入待翻译内容");
    // lv_textarea_add_text(ta_input, "100ask.net \nlvgl.100ask.net");//批量插入字符
    // 这会清空之前的所有文本再覆盖新的文本
    // lv_textarea_set_text(ta_input, "1234567890 100ask.net\nlvgl.100ask.net");

    ta_output = lv_textarea_create(lv_screen_active());
    lv_obj_set_size(ta_output, 300, 500);
    lv_obj_set_pos(ta_output, 400, 0);
    lv_obj_add_style(ta_output, &style, 0);

    lv_obj_t *translate_but = lv_btn_create(lv_screen_active());
    lv_obj_set_size(translate_but, 100, 50);
    lv_obj_set_pos(translate_but, 300, 500);

    lv_obj_t *ui_label_button = lv_label_create(translate_but);
    lv_label_set_text(ui_label_button, "翻译");
    lv_obj_add_style(ui_label_button, &style, 0);

    lv_obj_add_event_cb(translate_but, translate_button_cb, LV_EVENT_CLICKED, NULL);

    lv_timer_create(my_timer, 100, NULL);

    lv_timer_create(Clipboard_Singleton_timer, 100, NULL);
}

UDP_Client *client_p = nullptr;

void UDP_Client_Recv_thread()
{
    client_p->Recv_thread();
}

void UDP_Client_Send_thread()
{
    client_p->Send_thread();
}

void Clipboard_Singleton_thread()
{
    // 获取单例实例并启动剪贴板监控
    ClipboardSingleton::getInstance().run();
}

int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init(700, 600);

    // lv_demo_widgets();

    client_p = new UDP_Client("192.168.31.189", 59218);
    client_p->Initialize();
    std::thread t_UDP_Client_Recv_thread(UDP_Client_Recv_thread);
    std::thread t_UDP_Client_Send_thread(UDP_Client_Send_thread);

    std::thread t_Clipboard_Singleton_thread(Clipboard_Singleton_thread);

    t_UDP_Client_Recv_thread.detach();
    t_UDP_Client_Send_thread.detach();
    t_Clipboard_Singleton_thread.detach();

    ui_main();

    while (1) {
        /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t *hal_init(int32_t w, int32_t h)
{

    lv_group_set_default(lv_group_create());

    lv_display_t *disp = lv_sdl_window_create(w, h);

    lv_indev_t *mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    // LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
    // lv_obj_t *cursor_obj;
    // cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
    // lv_image_set_src(cursor_obj, &mouse_cursor_icon); /*Set the image source*/
    // lv_indev_set_cursor(mouse, cursor_obj);           /*Connect the image  object to the driver*/

    lv_indev_t *mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t *kb = lv_sdl_keyboard_create();
    lv_indev_set_display(kb, disp);
    lv_indev_set_group(kb, lv_group_get_default());

    return disp;
}
