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
#include <hidapi.h>

#define HID_AUXD_DISPLAY_PARAMS      0x01 // Display parameters report
#define HID_AUXD_CURSOR_POS          0x02 // Cursor position report
#define HID_AUXD_CHAR_REPORT         0x03 // Character report
#define HID_AUXD_FONT_REPORT         0x04 // Font report


// Display params flags
#define HID_AUXD_ASCII_CHARSET      0x80 // Screen supports
#define HID_AUXD_DATAREAD_BACK      0x40 // Character Report can be read back when set
#define HID_AUXD_FONTREAD_BACK      0x20 // Font Report can be read back when set

//================================================================================

typedef struct
{
  unsigned char rows;     // 0x35
  unsigned char cols;     // 0x36
  unsigned char chrw;     // 0x3d char width 
  unsigned char chrh;     // 0x3e char height
  unsigned char flags;    // 0x21|0x22|0x29|5 unused
} HIDDisplayParams;

HIDDisplayParams* HID_API_EXPORT HID_API_CALL hidlcd_get_display_params(hid_device *dev);

int HID_API_EXPORT HID_API_CALL hidlcd_set_cursor(hid_device *dev, unsigned char row, unsigned char col);

int HID_API_EXPORT HID_API_CALL hidlcd_print(hid_device *dev, 
                                            HIDDisplayParams *display_params, 
                                            const unsigned char *string);

#ifdef __cplusplus
}
#endif

#endif /* HIDLCD_H */

