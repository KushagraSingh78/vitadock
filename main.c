#include <psp2/kernel/processmgr.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <vita2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FONT_PATH "app0:font.ttf"

int main() {
    vita2d_init();
    vita2d_set_clear_color(0xFF000000); 

    // Load custom font (Make sure font.ttf is in the VPK)
    vita2d_font *font = vita2d_load_font_file(FONT_PATH);

    // Power Saving
    scePowerSetArmClockFrequency(41); 
    scePowerSetBusClockFrequency(41);
    scePowerSetGpuClockFrequency(41);

    int shift_x = 0;
    int shift_y = 0;
    int last_minute = -1;
    srand(time(NULL));

    while (1) {
        SceDateTime time;
        sceRtcGetCurrentClockLocalTime(&time);
        
        int battery_percent = scePowerGetBatteryLifePercent();
        int is_charging = scePowerIsBatteryCharging();

        char time_str[10];
        char batt_str[20];
        sprintf(time_str, "%02d:%02d", time.hour, time.minute);
        
        // Add a "+" if charging, otherwise just the number
        if (is_charging)
            sprintf(batt_str, "+%d%%", battery_percent);
        else
            sprintf(batt_str, "%d%%", battery_percent);

        // Pixel Shift every minute for OLED protection
        if (time.minute != last_minute) {
            shift_x = (rand() % 30) - 15;
            shift_y = (rand() % 30) - 15;
            last_minute = time.minute;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        if (font) {
            // 1. DRAW MAIN CLOCK (Large)
            int tw, th;
            vita2d_font_text_dimensions(font, 180, time_str, &tw, &th);
            float cx = (960.0f / 2.0f) - (tw / 2.0f) + shift_x;
            float cy = (544.0f / 2.0f) + shift_y;
            vita2d_font_draw_text(font, cx, cy, 0xFFFFFFFF, 180, time_str);

            // 2. DRAW BATTERY (Smaller, underneath the clock)
            int bw, bh;
            vita2d_font_text_dimensions(font, 40, batt_str, &bw, &bh);
            float bx = (960.0f / 2.0f) - (bw / 2.0f) + shift_x;
            float by = cy + 80.0f; // Positioned safely below the clock
            
            // Subtle Cyan color if charging, Grey if not
            unsigned int batt_color = is_charging ? 0xFFFFFF00 : 0xFF888888;
            vita2d_font_draw_text(font, bx, by, batt_color, 40, batt_str);
        }

        vita2d_end_drawing();
        vita2d_wait_rendering_done();
        vita2d_swap_buffers();

        sceKernelDelayThread(1000 * 1000); 
    }

    vita2d_free_font(font);
    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}
