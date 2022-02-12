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

/**
 * @file 
 * @author   Alex Bratchik 
 * @defgroup API HID API
 * @mainpage HIDLCD Driver library for HID-compliant LCD Display
 * 
 * This driver allows the host to communicate with HID-compliant Auxiliary Display based on Arduino. 
 * The main idea of this library is to provide a simple unified communication with the small LCD displays 
 * supported by Arduino. Such displays can be used for showing system information such as hardware temperature, 
 * fan speed, percentage of memory available etc., and used by host applications as necessary.
 * HIDLCD is build on top of the HIDAPI library and provides the next level of abstraction specific to 
 * the HID-compliant auxiliary LCD displays.
 * 
 * @ref API
 */

#ifndef HIDLCD_H
#define HIDLCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <hidapi/hidapi.h>

#define HID_AUXD_DISPLAY_PARAMS      0x01 ///< Display parameters report
#define HID_AUXD_CURSOR_POS          0x02 ///< Cursor position report
#define HID_AUXD_CHAR_REPORT         0x03 ///< Character report
#define HID_AUXD_FONT_REPORT         0x04 ///< Font report
#define HID_AUXD_CTRL_REPORT         0x05 ///< Control report
#define HID_AUXD_CURSOR_FLAGS        0x06 ///< Control report


// Display params flags
#define HID_AUXD_ASCII_CHARSET      0x80 ///< Screen supports
#define HID_AUXD_DATAREAD_BACK      0x40 ///< Character Report can be read back when set
#define HID_AUXD_FONTREAD_BACK      0x20 ///< Font Report can be read back when set

// Display control flags
#define HID_AUXD_CLEAR              0x80 ///< Clear Display command
#define HID_AUXD_ENABLE             0x40 ///< Display enable
#define HID_AUXD_SSENABLE           0x20 ///< Screen Saver enable
#define HID_AUXD_VSCROLL            0x10 ///< Vertical Scroll
#define HID_AUXD_HSCROLL            0x08 ///< Horizintal Scroll    
#define HID_AUXD_DISABLE            0x0  ///< Display disable
    
#define HID_ADCMD_MODE_DEFAULT      0x0  ///< Command flags are added to the existing state of screen
#define HID_ADCMD_MODE_OFF          0x1  ///< Clear the command/state
#define HID_ADCMD_MODE_OVERWRITE    0x2  ///< Ovewrite the state of the screen  
    
// Cursor control flags
#define HID_ADCC_PIXELPOS           0x80 ///< Cursor Pixel Positioning
#define HID_ADCC_INCREMENT          0x40 ///< Cursor Mode = Increment
#define HID_ADCC_ENABLE             0x20 ///< Cursor Enable
#define HID_ADCC_BLINK              0x10 ///< Cursor Blink     
    
/**
 * @brief The structure holding the physical parameters and other capabilities of
 *        the LCD display
 */
    
typedef struct
{
  u_int8_t rows;     ///< number of rows (HID usage 0x35)  
  u_int8_t cols;     ///< nuber of columns (HID usage 0x36)
  u_int8_t chrw;     ///< character width (HID usage 0x3d)
  u_int8_t chrh;     ///< character height (HID usage 0x3e)
  u_int8_t flags;    ///< display capabilities bitmap. 0x21|0x22|0x29|5 unused
} HIDDisplayParams;

/**
 * @brief Returns the physical parameters of the connected auxiliary LCD display.
 * 
 * @ingroup API
 * @param   dev A device handle returned from hid_open().
 * @return  the pointer to HIDDisplayParams structure with the parameters of the display. 
 */
HIDDisplayParams* HID_API_EXPORT HID_API_CALL hidlcd_get_display_params(hid_device *dev);

/**
 * @brief Returns the device handle
 * 
 * @ingroup API
 * @param vendor_id     Vendor ID.
 * @param product_id    Product ID. 
 * @param serial_number Serial number of the device if multiple LCD with the same VID/PID are connected
 * @return LCD device handle.
 */
HID_API_EXPORT hid_device * HID_API_CALL hidlcd_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number);

/**
 * @brief       Closes the device handle and releases associated resources
 * 
 * @ingroup API
 * @param dev   A device handle returned from hid_open().
 */
void HID_API_EXPORT HID_API_CALL hidlcd_close(hid_device *dev);

/**
 * @brief   Closes the HIDLCD library and releases associated resources
 * 
 * @ingroup API
 * @return 
 */
int HID_API_EXPORT HID_API_CALL hidlcd_exit(void);

/**
 * @brief   Initialize the HIDLCD library.
 * 
 * @ingroup API
 * @return  This function returns 0 on success and -1 on error.
 */
int HID_API_EXPORT HID_API_CALL hidlcd_init(void);

/**
 * @brief   This function sets the cursor of the LCD display in the specified position.
 * 
 * @ingroup API
 * @param dev   A device handle returned from hid_open().
 * @param row   Row number, starting from 0
 * @param col   Column number starting from 0
 * @return Returns positive value if success and -1 if error.
 */
int HID_API_EXPORT HID_API_CALL hidlcd_set_cursor(hid_device *dev, u_int8_t row, u_int8_t col);

/**
 * @brief This function sets the cursor extended parameters and display mode.
 * 
 * @ingroup API
 * @param dev   A device handle returned from hid_open().
 * @param flags A bitmap field specifying the cursor mode (enable/disable/blinking etc)
 * @param mode  LCD command mode 
 * @return Returns positive value if success and -1 if error.
 */
int HID_API_EXPORT HID_API_CALL hidlcd_set_cursor_flags_ext(hid_device *dev, u_int8_t flags, u_int8_t mode);

/**
 * @brief Prints the text on the LCD screen
 * 
 * @ingroup API
 * @param dev               A device handle returned from hid_open().
 * @param display_params    Parameters of the display returned by hidlcd_get_display_params().
 * @param string            The string to display
 * @return                  Returns positive value if success and -1 if error.
 */
int HID_API_EXPORT HID_API_CALL hidlcd_print(hid_device *dev, 
                                            HIDDisplayParams *display_params, 
                                            const char *string);

/**
 * @brief Sends the command to the LCD display with mode
 * 
 * @ingroup API
 * @param dev               A device handle returned from hid_open().
 * @param command           Command
 * @param mode              Mode of command
 * @return                  Returns positive value if success and -1 if error.
 */
int HID_API_EXPORT HID_API_CALL hidlcd_send_command_ext(hid_device *dev, u_int8_t command, u_int8_t mode);

/**
 * @brief Sends the command to the LCD display 
 * 
 * @ingroup API
 * @param dev               A device handle returned from hid_open().
 * @param command           Command
 * @return                  Returns positive value if success and -1 if error. 
 */
int HID_API_EXPORT HID_API_CALL hidlcd_send_command(hid_device *dev, u_int8_t command);



#ifdef __cplusplus
}
#endif

#endif /* HIDLCD_H */

