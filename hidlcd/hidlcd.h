/*
 * Copyright (C) 2021 abratchik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   hidlcd.h
 * Author: abratchik
 *
 * Created on November 12, 2021, 5:48 PM
 */

#ifndef HIDLCD_H
#define HIDLCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <hidapi/hidapi.h>

#define HID_AUXD_DISPLAY_PARAMS      0x01 // Display parameters report
#define HID_AUXD_CURSOR_POS          0x02 // Cursor position report
#define HID_AUXD_CHAR_REPORT         0x03 // Character report
#define HID_AUXD_FONT_REPORT         0x04 // Font report
#define HID_AUXD_CTRL_REPORT         0x05 // Control report
#define HID_AUXD_CURSOR_FLAGS        0x06 // Control report


// Display params flags
#define HID_AUXD_ASCII_CHARSET      0x80 // Screen supports
#define HID_AUXD_DATAREAD_BACK      0x40 // Character Report can be read back when set
#define HID_AUXD_FONTREAD_BACK      0x20 // Font Report can be read back when set

// Display control flags
#define HID_AUXD_CLEAR              0x80 // Clear Display command
#define HID_AUXD_ENABLE             0x40 // Display enable
#define HID_AUXD_SSENABLE           0x20 // Screen Saver enable
#define HID_AUXD_VSCROLL            0x10 // Vertical Scroll
#define HID_AUXD_HSCROLL            0x08 // Horizintal Scroll    
#define HID_AUXD_DISABLE            0x0  // Display disable
    
#define HID_ADCMD_MODE_DEFAULT      0x0  // command flags are added to the existing state of screen
#define HID_ADCMD_MODE_OFF          0x1  // clear the command/state
#define HID_ADCMD_MODE_OVERWRITE    0x2  // ovewrite the state of the screen  
    
// Cursor control flags
#define HID_ADCC_PIXELPOS           0x80 // Cursor Pixel Positioning
#define HID_ADCC_INCREMENT          0x40 // Cursor Mode = Increment
#define HID_ADCC_ENABLE             0x20 // Cursor Enable
#define HID_ADCC_BLINK              0x10 // Cursor Blink     
    
//================================================================================

typedef struct
{
  u_int8_t rows;     // 0x35
  u_int8_t cols;     // 0x36
  u_int8_t chrw;     // 0x3d char width 
  u_int8_t chrh;     // 0x3e char height
  u_int8_t flags;    // 0x21|0x22|0x29|5 unused
} HIDDisplayParams;

HIDDisplayParams* HID_API_EXPORT HID_API_CALL hidlcd_get_display_params(hid_device *dev);

HID_API_EXPORT hid_device * HID_API_CALL hidlcd_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number);
void HID_API_EXPORT HID_API_CALL hidlcd_close(hid_device *dev);
int HID_API_EXPORT HID_API_CALL hidlcd_exit(void);
int HID_API_EXPORT HID_API_CALL hidlcd_init(void);


int HID_API_EXPORT HID_API_CALL hidlcd_set_cursor(hid_device *dev, u_int8_t row, u_int8_t col);

int HID_API_EXPORT HID_API_CALL hidlcd_set_cursor_flags_ext(hid_device *dev, u_int8_t flags, u_int8_t mode);

int HID_API_EXPORT HID_API_CALL hidlcd_print(hid_device *dev, 
                                            HIDDisplayParams *display_params, 
                                            const unsigned char *string);

int HID_API_EXPORT HID_API_CALL hidlcd_send_command_ext(hid_device *dev, u_int8_t command, u_int8_t mode);

int HID_API_EXPORT HID_API_CALL hidlcd_send_command(hid_device *dev, u_int8_t command);



#ifdef __cplusplus
}
#endif

#endif /* HIDLCD_H */

