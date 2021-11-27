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

#include "hidlcd.h"

HIDDisplayParams *hidlcd_get_display_params(hid_device *dev) {
    
    
    HIDDisplayParams *dp = NULL;
    
    unsigned char buf[sizeof(HIDDisplayParams)+1];
    
    buf[0] = HID_AUXD_DISPLAY_PARAMS;
    
    int res = hid_get_feature_report(dev,buf,sizeof(buf));
    if(res > 0)
    {   
        dp = (HIDDisplayParams*) malloc(sizeof(HIDDisplayParams));
        dp->rows = buf[1];
        dp->cols = buf[2];
        dp->chrw = buf[3];
        dp->chrh = buf[4];
        dp->flags = buf[5];
        
    }
    
    return dp;
}

int hidlcd_set_cursor(hid_device *dev, u_int8_t row, u_int8_t col) {
    
    if(!dev) return -1;
    
    u_int8_t buf[3] = {HID_AUXD_CURSOR_POS, row, col};
    
    int res = hid_send_feature_report(dev, buf, sizeof(buf));
    
    return res;
}

int hidlcd_set_cursor_flags_ext(hid_device *dev, u_int8_t flags, u_int8_t mode) {
    
    if(!dev) return -1;
    
    u_int8_t buf[2] = {HID_AUXD_CURSOR_FLAGS, 0};
    
    int res = hid_get_feature_report(dev, buf, sizeof(buf));
    if(res > 0) {
        
        switch(mode) {
            case HID_ADCMD_MODE_OFF:
                buf[1] &= ~flags;
                break;
            case HID_ADCMD_MODE_OVERWRITE:
                buf[1] = flags;
                break;
            default:
                buf[1] |= flags;
                break;
        }
        
        res = hid_send_feature_report(dev, buf, sizeof(buf));
    }

    return res;
}

int hidlcd_print(hid_device *dev, HIDDisplayParams *display_params, const unsigned char *str) {
    
    if(!dev || !display_params || !display_params->cols) return -1;
    
    int len = strlen(str);
    
    len = (len > display_params->cols?display_params->cols : len);
    
    size_t buffsize = display_params->cols+2;
    
    unsigned char* buf = (unsigned char*) malloc(buffsize); 
    
    *buf = HID_AUXD_CHAR_REPORT; // assign report ID to the first byte
    
    memcpy(buf+1, str, len+1);
    
    int res = hid_send_feature_report(dev, buf, buffsize);
    
    if(res<0) return res;
    
    free(buf);
    
    return len;
}

int hidlcd_send_command(hid_device *dev, u_int8_t command) {
    return hidlcd_send_command_ext(dev, command, HID_ADCMD_MODE_DEFAULT);
}

int hidlcd_send_command_ext(hid_device *dev, u_int8_t command, u_int8_t mode) {
    
    if(!dev) return -1;
    
    size_t buffsize = 2;
    
    u_int8_t buf[buffsize];
    
    buf[0] = HID_AUXD_CTRL_REPORT;
    
    int res = hid_get_feature_report(dev, buf, sizeof(buf));
    
    if(res > 0) {
        
        switch(mode) {
            case HID_ADCMD_MODE_OFF:
                buf[1] &= ~command;
                break;
            case HID_ADCMD_MODE_OVERWRITE:
                buf[1] = command;
                break;
            default:
                buf[1] |= command;
                break;
        }
        
        res = hid_send_feature_report(dev, buf, sizeof(buf));
    }
    
    return res;
    
}
