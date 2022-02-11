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
 * File:   hidapi_test.c
 * Author: abratchik
 *
 * Created on November 4, 2021, 9:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <wchar.h> // wprintf

#include "hidlcd.h"

#define MAX_STR 255

#define USB_VID 0x2341
#define USB_PID 0x8036

#define TEST_DELAY 5000000

/*
 * Simple C Test Suite
 */


int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[256];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;
        
        struct hid_device_info *devs, *cur_dev;

	// Initialize the hidapi library
	if(hidlcd_init()) {
            wprintf(L"Could not init the hidraw library\n");
            return 1;
        }
        
        wprintf(L"HIDLCD Test. Compiled with hidapi version %s, runtime version %s.\n", HID_API_VERSION_STR, hid_version_str());
	if (hid_version()->major == HID_API_VERSION_MAJOR && hid_version()->minor == HID_API_VERSION_MINOR && hid_version()->patch == HID_API_VERSION_PATCH) {
		wprintf(L"Compile-time version matches runtime version of hidapi.\n\n");
	}
	else {
		wprintf(L"Compile-time version is different than runtime version of hidapi.\n]n");
	}        
        
	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hidlcd_open(USB_VID, USB_PID, NULL);
        
        if(handle == NULL) 
        {
            wprintf(L"Could not open device, error: %ls\n", hid_error(NULL));
            hid_exit();
            return 1;
        }
        

        wprintf(L"All good\n");

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %ls\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %ls\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%ld) %ls\n", wstr[0], wstr);


/*
        memset(buf,0,sizeof(buf)); // clear buffer
        buf[0] = HID_AUXD_DISPLAY_PARAMS;
        
        res = hid_get_feature_report(handle,buf,sizeof(buf));
        if(res < 0) {
            wprintf(L"Could not read feature %ld\n", HID_AUXD_DISPLAY_PARAMS);
            hid_exit();
            return 1;
        }
        else {
            wprintf(L"Display Parameters Report\n");
            for (i = 0; i < res; i++)
                wprintf(L"%ld ", buf[i]);
            wprintf(L"\n");
        }
*/
        
        HIDDisplayParams* dp = hidlcd_get_display_params(handle);
        
        if(dp != NULL) {
            wprintf(L"Display Parameters:\n");
            wprintf(L"%ld x %ld characters %ld x %ld \n", dp->cols, dp->rows, dp->chrw, dp->chrh);
        }
        
        res = hidlcd_set_cursor(handle, 0, 0);
        if(res > 0) {
            wprintf(L"Cursor set to (0,0)\n");
        }
        else {
            wprintf(L"set_cursor returned %ld\n", res);
            hid_close(handle);
            return -1;
        }                   
        res = hidlcd_print(handle, dp, " CPU temp = 47C ");
        if(res > 0) {
            wprintf(L"Successfully printed text (%ld bytes)\n", res);
        }
        else {
            wprintf(L"hidlcd_print failed: %ls\n", hid_error(handle));
            hid_close(handle);
            return -1;
        }       
        
        res = hidlcd_set_cursor(handle, 1, 0);
        if(res > 0) {
            wprintf(L"Cursor set to (1,0)\n");
        }
        else {
            wprintf(L"set_cursor returned %ld\n", res);
            hid_close(handle);
            return -1;
        }        

        res = hidlcd_print(handle, dp, " GPU temp = 98C ");
        if(res > 0) {
            wprintf(L"Successfully printed text (%ld bytes)\n", res);
        }
        else {
            wprintf(L"hidlcd_print failed: %ls\n", hid_error(handle));
            hid_close(handle);
            return -1;
        }    

        usleep(TEST_DELAY);
        
        res = hidlcd_send_command(handle, HID_AUXD_CLEAR);
        if(res > 0) {
            wprintf(L"Successfully cleared screen\n");
        }
        else {
            wprintf(L"hidlcd_send_command failed: %ls\n", hid_error(handle));
            hid_close(handle);
            return -1;
        }    
        
        res = hidlcd_print(handle, dp, "Hello,World!");
        
        usleep(TEST_DELAY);
        
        res = hidlcd_send_command_ext(handle, HID_AUXD_DISABLE, HID_ADCMD_MODE_OVERWRITE);
        if(res > 0) {
            wprintf(L"Successfully switched off the screen\n");
        }
        else {
            wprintf(L"hidlcd_send_command failed: %ls\n", hid_error(handle));
            hid_close(handle);
            return -1;
        }    
        
        usleep(TEST_DELAY);
        
        res = hidlcd_send_command(handle, HID_AUXD_ENABLE | HID_AUXD_HSCROLL);
        if(res > 0) {
            wprintf(L"Successfully switched on the screen\n");
        }
        else {
            wprintf(L"hidlcd_send_command failed: %ls\n", hid_error(handle));
            hid_close(handle);
            return -1;
        }   
        
        res = hidlcd_print(handle, dp, "Hello, LCD!");
        
        res = hidlcd_set_cursor_flags_ext(handle, HID_ADCC_ENABLE | HID_ADCC_BLINK,0);
                
        usleep(TEST_DELAY);
        res = hidlcd_set_cursor_flags_ext(handle, HID_ADCC_INCREMENT,HID_ADCMD_MODE_OVERWRITE);
        
        res = hidlcd_send_command_ext(handle, HID_AUXD_HSCROLL, HID_ADCMD_MODE_OFF);
        res = hidlcd_send_command(handle, HID_AUXD_CLEAR);
                
        res = hidlcd_print(handle, dp, "Hi, Arduino!");  
        
        res = hidlcd_set_cursor(handle, 1, 0);
        res = hidlcd_print(handle, dp, "Second line!"); 
        
        usleep(TEST_DELAY);
        
        res = hidlcd_send_command(handle, HID_AUXD_CLEAR);
        res = hidlcd_print(handle, dp, "SUCCESS!"); 
        

	// Close the device
	hidlcd_close(handle);

	// Finalize the hidapi library
	res = hidlcd_exit();

	return 0;
}
