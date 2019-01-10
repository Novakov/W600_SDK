/* upnp_control.c - UPnP RenderingControl routines
 *
 * Copyright (C) 2005-2007   Ivo Clarysse
 *
 * This file is part of GMediaRender.
 *
 * GMediaRender is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GMediaRender is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GMediaRender; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
 * MA 02110-1301, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "wm_config.h"

#if TLS_CONFIG_DLNA

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <upnp.h>
#include <ithread.h>
#include <upnptools.h>

#include "upnpdebug.h"
#include "upnp_webserver.h"
#include "upnp_if.h"
#include "upnp_control.h"
#include "upnp_device.h"
#include "logging.h"
#include "wm_dlna_dmr.h"

#define CONTROL_SERVICE "urn:upnp-org:serviceId:RenderingControl"
//#define CONTROL_SERVICE "urn:schemas-upnp-org:service:RenderingControl"
#define CONTROL_TYPE "urn:schemas-upnp-org:service:RenderingControl:1"
#define CONTROL_SCPD_URL "/upnp/rendercontrolSCPD.xml"
#define CONTROL_CONTROL_URL "/upnp/control/rendercontrol1"
#define CONTROL_EVENT_URL "/upnp/event/rendercontrol1"

#ifndef MIN
   #define MIN(x,y) ((x)<(y)?(x):(y))
#endif

typedef enum {
	//CONTROL_VAR_G_GAIN,
	//CONTROL_VAR_B_BLACK,
	//CONTROL_VAR_VER_KEYSTONE,
	//CONTROL_VAR_G_BLACK,
	CONTROL_VAR_VOLUME,
	CONTROL_VAR_LOUDNESS,
	CONTROL_VAR_AAT_INSTANCE_ID,
	//CONTROL_VAR_R_GAIN,
	//CONTROL_VAR_COLOR_TEMP,
	//CONTROL_VAR_SHARPNESS,
	//CONTROL_VAR_AAT_PRESET_NAME,
	//CONTROL_VAR_R_BLACK,
	//CONTROL_VAR_B_GAIN,
	CONTROL_VAR_MUTE,
	CONTROL_VAR_LAST_CHANGE,
	CONTROL_VAR_AAT_CHANNEL,
	//CONTROL_VAR_HOR_KEYSTONE,
	CONTROL_VAR_VOLUME_DB,
	//CONTROL_VAR_PRESET_NAME_LIST,
	//CONTROL_VAR_CONTRAST,
	//CONTROL_VAR_BRIGHTNESS,
	CONTROL_VAR_UNKNOWN,
	CONTROL_VAR_COUNT
} control_variable;

typedef enum {
	//CONTROL_CMD_GET_BLUE_BLACK,
	//CONTROL_CMD_GET_BLUE_GAIN,
	//CONTROL_CMD_GET_BRIGHTNESS,
	//CONTROL_CMD_GET_COLOR_TEMP,
	//CONTROL_CMD_GET_CONTRAST,
	//CONTROL_CMD_GET_GREEN_BLACK,
	//CONTROL_CMD_GET_GREEN_GAIN,
	//CONTROL_CMD_GET_HOR_KEYSTONE,
	CONTROL_CMD_GET_LOUDNESS,
	CONTROL_CMD_GET_MUTE,
	//CONTROL_CMD_GET_RED_BLACK,
	//CONTROL_CMD_GET_RED_GAIN,
	//CONTROL_CMD_GET_SHARPNESS,
	//CONTROL_CMD_GET_VERT_KEYSTONE,
	CONTROL_CMD_GET_VOL,
	CONTROL_CMD_GET_VOL_DB,
	CONTROL_CMD_GET_VOL_DBRANGE,
	//CONTROL_CMD_LIST_PRESETS,      
	//CONTROL_CMD_SELECT_PRESET,
	//CONTROL_CMD_SET_BLUE_BLACK,
	//CONTROL_CMD_SET_BLUE_GAIN,
	//CONTROL_CMD_SET_BRIGHTNESS,
	//CONTROL_CMD_SET_COLOR_TEMP,
	//CONTROL_CMD_SET_CONTRAST,
	//CONTROL_CMD_SET_GREEN_BLACK,
	//CONTROL_CMD_SET_GREEN_GAIN,
	//CONTROL_CMD_SET_HOR_KEYSTONE,
	CONTROL_CMD_SET_LOUDNESS,       
	CONTROL_CMD_SET_MUTE,
	//CONTROL_CMD_SET_RED_BLACK,
	//CONTROL_CMD_SET_RED_GAIN,
	//CONTROL_CMD_SET_SHARPNESS,
	//CONTROL_CMD_SET_VERT_KEYSTONE,
	CONTROL_CMD_SET_VOL,
	CONTROL_CMD_SET_VOL_DB,
	CONTROL_CMD_UNKNOWN,
	CONTROL_CMD_COUNT
} control_cmd;

static struct action control_actions[];

static const char *control_variables[] = {
	[CONTROL_VAR_LAST_CHANGE] = "LastChange",
	//[CONTROL_VAR_PRESET_NAME_LIST] = "PresetNameList",
	[CONTROL_VAR_AAT_CHANNEL] = "A_ARG_TYPE_Channel",
	[CONTROL_VAR_AAT_INSTANCE_ID] = "A_ARG_TYPE_InstanceID",
	//[CONTROL_VAR_AAT_PRESET_NAME] = "A_ARG_TYPE_PresetName",
	//[CONTROL_VAR_BRIGHTNESS] = "Brightness",
	//[CONTROL_VAR_CONTRAST] = "Contrast",
	//[CONTROL_VAR_SHARPNESS] = "Sharpness",
	//[CONTROL_VAR_R_GAIN] = "RedVideoGain",
	//[CONTROL_VAR_G_GAIN] = "GreenVideoGain",
	//[CONTROL_VAR_B_GAIN] = "BlueVideoGain",
	//[CONTROL_VAR_R_BLACK] = "RedVideoBlackLevel",
	//[CONTROL_VAR_G_BLACK] = "GreenVideoBlackLevel",
	//[CONTROL_VAR_B_BLACK] = "BlueVideoBlackLevel",
	//[CONTROL_VAR_COLOR_TEMP] = "ColorTemperature",
	//[CONTROL_VAR_HOR_KEYSTONE] = "HorizontalKeystone",
	//[CONTROL_VAR_VER_KEYSTONE] = "VerticalKeystone",
	[CONTROL_VAR_MUTE] = "Mute",
	[CONTROL_VAR_VOLUME] = "Volume",
	[CONTROL_VAR_VOLUME_DB] = "VolumeDB",
	[CONTROL_VAR_LOUDNESS] = "Loudness",
	[CONTROL_VAR_UNKNOWN] = NULL
};
/*
static const char *aat_presetnames[] =
{
	"FactoryDefaults",
	"InstallationDefaults",
	"Vendor defined",
	NULL
};*/

static const char *aat_channels[] =
{
	"Master",
	"LF",
	"RF",
	//"CF",
	//"LFE",
	//"LS",
	//"RS",
	//"LFC",
	//"RFC",
	//"SD",
	//"SL",
	//"SR",
	//"T",
	//"B",
	NULL
};

//static struct param_range brightness_range = { 0, 100, 1 };
//static struct param_range contrast_range = { 0, 100, 1 };
//static struct param_range sharpness_range = { 0, 100, 1 };
//static struct param_range vid_gain_range = { 0, 100, 1 };
//static struct param_range vid_black_range = { 0, 100, 1 };
//static struct param_range colortemp_range = { 0, 65535, 1 };
//static struct param_range keystone_range = { -32768, 32767, 1 };
static struct param_range volume_range = { 0, 100, 1 };
static struct param_range volume_db_range = { -32768, 32767, 0 };

static struct var_meta control_var_meta[] = {
	[CONTROL_VAR_LAST_CHANGE] =		{ SENDEVENT_YES, DATATYPE_STRING, NULL, NULL },
	//[CONTROL_VAR_PRESET_NAME_LIST] =	{ SENDEVENT_NO, DATATYPE_STRING, NULL, NULL },
	[CONTROL_VAR_AAT_CHANNEL] =		{ SENDEVENT_NO, DATATYPE_STRING, aat_channels, NULL },
	[CONTROL_VAR_AAT_INSTANCE_ID] =		{ SENDEVENT_NO, DATATYPE_UI4, NULL, NULL },
	//[CONTROL_VAR_AAT_PRESET_NAME] =		{ SENDEVENT_NO, DATATYPE_STRING, aat_presetnames, NULL },
	//[CONTROL_VAR_BRIGHTNESS] =		{ SENDEVENT_NO, DATATYPE_UI2, NULL, &brightness_range },
	//[CONTROL_VAR_CONTRAST] =		{ SENDEVENT_NO, DATATYPE_UI2, NULL, &contrast_range },
	//[CONTROL_VAR_SHARPNESS] =		{ SENDEVENT_NO, DATATYPE_UI2, NULL, &sharpness_range },
	//[CONTROL_VAR_R_GAIN] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &vid_gain_range },
	//[CONTROL_VAR_G_GAIN] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &vid_gain_range },
	//[CONTROL_VAR_B_GAIN] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &vid_gain_range },
	//[CONTROL_VAR_R_BLACK] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &vid_black_range },
	//[CONTROL_VAR_G_BLACK] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &vid_black_range },
	//[CONTROL_VAR_B_BLACK] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &vid_black_range },
	//[CONTROL_VAR_COLOR_TEMP] =		{ SENDEVENT_NO, DATATYPE_UI2, NULL, &colortemp_range },
	//[CONTROL_VAR_HOR_KEYSTONE] =		{ SENDEVENT_NO, DATATYPE_I2, NULL, &keystone_range },
	//[CONTROL_VAR_VER_KEYSTONE] =		{ SENDEVENT_NO, DATATYPE_I2, NULL, &keystone_range },
	[CONTROL_VAR_MUTE] =			{ SENDEVENT_NO, DATATYPE_BOOLEAN, NULL, NULL },
	[CONTROL_VAR_VOLUME] =			{ SENDEVENT_NO, DATATYPE_UI2, NULL, &volume_range },
	[CONTROL_VAR_VOLUME_DB] =		{ SENDEVENT_NO, DATATYPE_I2, NULL, &volume_db_range },
	[CONTROL_VAR_LOUDNESS] =		{ SENDEVENT_NO, DATATYPE_BOOLEAN, NULL, NULL },
	[CONTROL_VAR_UNKNOWN] =			{ SENDEVENT_NO, DATATYPE_UNKNOWN, NULL, NULL }
};

static char *control_values[] = {
	[CONTROL_VAR_LAST_CHANGE] = "<Event xmlns = \"urn:schemas-upnp-org:metadata-1-0/AVT/\"><InstanceID val=\"0\"></InstanceID></Event>",
	//[CONTROL_VAR_PRESET_NAME_LIST] = "",
	[CONTROL_VAR_AAT_CHANNEL] = "",
	[CONTROL_VAR_AAT_INSTANCE_ID] = "0",
	//[CONTROL_VAR_AAT_PRESET_NAME] = "",
	//[CONTROL_VAR_BRIGHTNESS] = "0",
	//[CONTROL_VAR_CONTRAST] = "0",
	//[CONTROL_VAR_SHARPNESS] = "0",
	//[CONTROL_VAR_R_GAIN] = "0",
	//[CONTROL_VAR_G_GAIN] = "0",
	//[CONTROL_VAR_B_GAIN] = "0",
	//[CONTROL_VAR_R_BLACK] = "0",
	//[CONTROL_VAR_G_BLACK] = "0",
	//[CONTROL_VAR_B_BLACK] = "0",
	//[CONTROL_VAR_COLOR_TEMP] = "0",
	//[CONTROL_VAR_HOR_KEYSTONE] = "0",
	//[CONTROL_VAR_VER_KEYSTONE] = "0",
	[CONTROL_VAR_MUTE] = "0",
	[CONTROL_VAR_VOLUME] = "0",
	[CONTROL_VAR_VOLUME_DB] = "0",
	[CONTROL_VAR_LOUDNESS] = "0",
	[CONTROL_VAR_UNKNOWN] = NULL
};
static ithread_mutex_t control_mutex;


/*
static struct argument *arguments_list_presets[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentPresetNameList", PARAM_DIR_OUT, CONTROL_VAR_PRESET_NAME_LIST },
	NULL
};
static struct argument *arguments_select_preset[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "PresetName", PARAM_DIR_IN, CONTROL_VAR_AAT_PRESET_NAME },
	NULL
};
static struct argument *arguments_get_brightness[] = {        
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentBrightness", PARAM_DIR_OUT, CONTROL_VAR_BRIGHTNESS },
	NULL
};
static struct argument *arguments_set_brightness[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredBrightness", PARAM_DIR_IN, CONTROL_VAR_BRIGHTNESS },
	NULL
};
static struct argument *arguments_get_contrast[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentContrast", PARAM_DIR_OUT, CONTROL_VAR_CONTRAST },
	NULL
};
static struct argument *arguments_set_contrast[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredContrast", PARAM_DIR_IN, CONTROL_VAR_CONTRAST },
	NULL
};
static struct argument *arguments_get_sharpness[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentSharpness", PARAM_DIR_OUT, CONTROL_VAR_SHARPNESS },
	NULL
};
static struct argument *arguments_set_sharpness[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredSharpness", PARAM_DIR_IN, CONTROL_VAR_SHARPNESS },
	NULL
};
static struct argument *arguments_get_red_gain[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentRedVideoGain", PARAM_DIR_OUT, CONTROL_VAR_R_GAIN },
	NULL
};
static struct argument *arguments_set_red_gain[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredRedVideoGain", PARAM_DIR_IN, CONTROL_VAR_R_GAIN },
	NULL
};
static struct argument *arguments_get_green_gain[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentGreenVideoGain", PARAM_DIR_OUT, CONTROL_VAR_G_GAIN },
	NULL
};
static struct argument *arguments_set_green_gain[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredGreenVideoGain", PARAM_DIR_IN, CONTROL_VAR_G_GAIN },
	NULL
};
static struct argument *arguments_get_blue_gain[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentBlueVideoGain", PARAM_DIR_OUT, CONTROL_VAR_B_GAIN },
	NULL
};
static struct argument *arguments_set_blue_gain[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredBlueVideoGain", PARAM_DIR_IN, CONTROL_VAR_B_GAIN },
	NULL
};
static struct argument *arguments_get_red_black[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentRedVideoBlackLevel", PARAM_DIR_OUT, CONTROL_VAR_R_BLACK },
	NULL
};
static struct argument *arguments_set_red_black[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredRedVideoBlackLevel", PARAM_DIR_IN, CONTROL_VAR_R_BLACK },
	NULL
};
static struct argument *arguments_get_green_black[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentGreenVideoBlackLevel", PARAM_DIR_OUT, CONTROL_VAR_G_BLACK },
	NULL
};
static struct argument *arguments_set_green_black[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredGreenVideoBlackLevel", PARAM_DIR_IN, CONTROL_VAR_G_BLACK },
	NULL
};
static struct argument *arguments_get_blue_black[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentBlueVideoBlackLevel", PARAM_DIR_OUT, CONTROL_VAR_B_BLACK },
	NULL
};
static struct argument *arguments_set_blue_black[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredBlueVideoBlackLevel", PARAM_DIR_IN, CONTROL_VAR_B_BLACK },
	NULL
};
static struct argument *arguments_get_color_temp[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentColorTemperature", PARAM_DIR_OUT, CONTROL_VAR_COLOR_TEMP },
	NULL
};
static struct argument *arguments_set_color_temp[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredColorTemperature", PARAM_DIR_IN, CONTROL_VAR_COLOR_TEMP },
	NULL
};
static struct argument *arguments_get_hor_keystone[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentHorizontalKeystone", PARAM_DIR_OUT, CONTROL_VAR_HOR_KEYSTONE },
	NULL
};
static struct argument *arguments_set_hor_keystone[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredHorizontalKeystone", PARAM_DIR_IN, CONTROL_VAR_HOR_KEYSTONE },
	NULL
};
static struct argument *arguments_get_vert_keystone[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "CurrentVerticalKeystone", PARAM_DIR_OUT, CONTROL_VAR_VER_KEYSTONE },
	NULL
};
static struct argument *arguments_set_vert_keystone[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "DesiredVerticalKeystone", PARAM_DIR_IN, CONTROL_VAR_VER_KEYSTONE },
	NULL
};*/
static struct argument *arguments_get_mute[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "CurrentMute", PARAM_DIR_OUT, CONTROL_VAR_MUTE },
	NULL
};
static struct argument *arguments_set_mute[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "DesiredMute", PARAM_DIR_IN, CONTROL_VAR_MUTE },
	NULL
};
static struct argument *arguments_get_vol[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "CurrentVolume", PARAM_DIR_OUT, CONTROL_VAR_VOLUME },
	NULL
};
static struct argument *arguments_set_vol[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "DesiredVolume", PARAM_DIR_IN, CONTROL_VAR_VOLUME },
	NULL
};
static struct argument *arguments_get_vol_db[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "CurrentVolume", PARAM_DIR_OUT, CONTROL_VAR_VOLUME_DB },
	NULL
};
static struct argument *arguments_set_vol_db[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "DesiredVolume", PARAM_DIR_IN, CONTROL_VAR_VOLUME_DB },
	NULL
};
static struct argument *arguments_get_vol_dbrange[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "MinValue", PARAM_DIR_OUT, CONTROL_VAR_VOLUME_DB },
	& (struct argument) { "MaxValue", PARAM_DIR_OUT, CONTROL_VAR_VOLUME_DB },
	NULL
};
static struct argument *arguments_get_loudness[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "CurrentLoudness", PARAM_DIR_OUT, CONTROL_VAR_LOUDNESS },
	NULL
};
static struct argument *arguments_set_loudness[] = {
	& (struct argument) { "InstanceID", PARAM_DIR_IN, CONTROL_VAR_AAT_INSTANCE_ID },
	& (struct argument) { "Channel", PARAM_DIR_IN, CONTROL_VAR_AAT_CHANNEL },
	& (struct argument) { "DesiredLoudness", PARAM_DIR_IN, CONTROL_VAR_LOUDNESS },
	NULL
};


static struct argument **argument_list[] = {
	//[CONTROL_CMD_LIST_PRESETS] =        	arguments_list_presets,
	//[CONTROL_CMD_SELECT_PRESET] =       	arguments_select_preset, 
	//[CONTROL_CMD_GET_BRIGHTNESS] =      	arguments_get_brightness,        
	//[CONTROL_CMD_SET_BRIGHTNESS] =      	arguments_set_brightness,
	//[CONTROL_CMD_GET_CONTRAST] =        	arguments_get_contrast,
	//[CONTROL_CMD_SET_CONTRAST] =        	arguments_set_contrast,
	//[CONTROL_CMD_GET_SHARPNESS] =       	arguments_get_sharpness,
	//[CONTROL_CMD_SET_SHARPNESS] =       	arguments_set_sharpness,
	//[CONTROL_CMD_GET_RED_GAIN] =        	arguments_get_red_gain,
	//[CONTROL_CMD_SET_RED_GAIN] =        	arguments_set_red_gain,
	//[CONTROL_CMD_GET_GREEN_GAIN] =      	arguments_get_green_gain,
	//[CONTROL_CMD_SET_GREEN_GAIN] =      	arguments_set_green_gain,
	//[CONTROL_CMD_GET_BLUE_GAIN] =       	arguments_get_blue_gain,
	//[CONTROL_CMD_SET_BLUE_GAIN] =       	arguments_set_blue_gain,
	//[CONTROL_CMD_GET_RED_BLACK] =       	arguments_get_red_black,
	//[CONTROL_CMD_SET_RED_BLACK] =       	arguments_set_red_black,
	//[CONTROL_CMD_GET_GREEN_BLACK] =     	arguments_get_green_black,
	//[CONTROL_CMD_SET_GREEN_BLACK] =     	arguments_set_green_black,
	//[CONTROL_CMD_GET_BLUE_BLACK] =      	arguments_get_blue_black,
	//[CONTROL_CMD_SET_BLUE_BLACK] =      	arguments_set_blue_black,
	//[CONTROL_CMD_GET_COLOR_TEMP] =      	arguments_get_color_temp,
	//[CONTROL_CMD_SET_COLOR_TEMP] =      	arguments_set_color_temp,
	//[CONTROL_CMD_GET_HOR_KEYSTONE] =    	arguments_get_hor_keystone,
	//[CONTROL_CMD_SET_HOR_KEYSTONE] =    	arguments_set_hor_keystone,
	//[CONTROL_CMD_GET_VERT_KEYSTONE] =   	arguments_get_vert_keystone,
	//[CONTROL_CMD_SET_VERT_KEYSTONE] =   	arguments_set_vert_keystone,
	[CONTROL_CMD_GET_MUTE] =            	arguments_get_mute,
	[CONTROL_CMD_SET_MUTE] =            	arguments_set_mute,
	[CONTROL_CMD_GET_VOL] =             	arguments_get_vol,
	[CONTROL_CMD_SET_VOL] =             	arguments_set_vol,
	[CONTROL_CMD_GET_VOL_DB] =          	arguments_get_vol_db,
	[CONTROL_CMD_SET_VOL_DB] =          	arguments_set_vol_db,
	[CONTROL_CMD_GET_VOL_DBRANGE] =     	arguments_get_vol_dbrange,
	[CONTROL_CMD_GET_LOUDNESS] =        	arguments_get_loudness,
	[CONTROL_CMD_SET_LOUDNESS] =        	arguments_set_loudness,           
	[CONTROL_CMD_UNKNOWN] =			NULL
};

static u8 control_values_need_free[CONTROL_VAR_COUNT] = {0};

static dmr_control_callback mute_callback = NULL;
static dmr_control_callback volume_callback = NULL;
static dmr_control_callback volumedb_callback = NULL;
static dmr_control_callback loudness_callback = NULL;

void tls_dmr_set_mute_callback(dmr_control_callback callback)
{
	mute_callback = callback;
}
void tls_dmr_set_volume_callback(dmr_control_callback callback)
{
	volume_callback = callback;
}
void tls_dmr_set_volumedb_callback(dmr_control_callback callback)
{
	volumedb_callback = callback;
}
void tls_dmr_set_loudness_callback(dmr_control_callback callback)
{
	loudness_callback = callback;
}

static void change_var(struct action_event *event, int varnum,
		       char *new_value)
{
	ENTER();
	
	if ((varnum < 0) || (varnum >= CONTROL_VAR_UNKNOWN)) {
		LEAVE();
		return;
	}
	if (new_value == NULL) {
		LEAVE();
		return;
	}

	if (control_values_need_free[varnum] && control_values[varnum]) {
	      tls_mem_free(control_values[varnum]);
	}
	control_values[varnum] = strdup(new_value);
	if(control_values[varnum] == NULL)
	{
		LEAVE();
		return;
	}
	control_values_need_free[varnum] = 1;

	LEAVE();
}

static int cmd_obtain_variable(struct action_event *event, int varnum,
			       char *paramname)
{
	char *value;

	value = upnp_get_string(event, "InstanceID");
	if (value == NULL) {
		return -1;
	}
	UpnpPrintf( UPNP_INFO, API, __FILE__, __LINE__,
		"%s: InstanceID='%s'\n", __FUNCTION__, value);
	tls_mem_free(value);

	return upnp_append_variable(event, varnum, paramname);
}

/*static int list_presets(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_PRESET_NAME_LIST,
				   "CurrentPresetNameList");
}

static int get_brightness(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_BRIGHTNESS,
				   "CurrentBrightness");
}

static int get_contrast(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_CONTRAST,
				   "CurrentContrast");
}

static int get_sharpness(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_SHARPNESS,
				   "CurrentSharpness");
}

static int get_red_videogain(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_R_GAIN,
				   "CurrentRedVideoGain");
}

static int get_green_videogain(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_G_GAIN,
				   "CurrentGreenVideoGain");
}

static int get_blue_videogain(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_B_GAIN,
				   "CurrentBlueVideoGain");
}

static int get_red_videoblacklevel(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_R_BLACK,
				   "CurrentRedVideoBlackLevel");
}

static int get_green_videoblacklevel(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_G_BLACK,
				   "CurrentGreenVideoBlackLevel");
}

static int get_blue_videoblacklevel(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_B_BLACK,
				   "CurrentBlueVideoBlackLevel");
}

static int get_colortemperature(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_COLOR_TEMP,
				   "CurrentColorTemperature");
}

static int get_horizontal_keystone(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_HOR_KEYSTONE,
				   "CurrentHorizontalKeystone");
}

static int get_vertical_keystone(struct action_event *event)
{
	return cmd_obtain_variable(event, CONTROL_VAR_VER_KEYSTONE,
				   "CurrentVerticalKeystone");
}*/

static void control_callback(struct action_event *event, dmr_control_callback callback, 
	enum dmr_control_type type, int varnum, const char* name)
{
	if(event->request->ActionRequest && callback)
	{
		do{
			IXML_NodeList * nodelist = NULL;
			IXML_Node * node = NULL;
			char * channel = NULL;
			char valbuf[12] = {0};
			int val = 0;

			if ((nodelist = ixmlDocument_getElementsByTagName(event->request->ActionRequest, "Channel")) == NULL) 
				break;
			node = nodelist->nodeItem;
			ixmlNodeList_free(nodelist);
			if(node == NULL)
			{
				break;
			}
			channel = node->firstChild->nodeValue;
			if(type == CONTROL_SET)
			{
				if ((nodelist = ixmlDocument_getElementsByTagName(event->request->ActionRequest, name)) == NULL) 
					break;
				node = nodelist->nodeItem;
				ixmlNodeList_free(nodelist);
				if(node == NULL)
					break;
				memcpy(valbuf, node->firstChild->nodeValue, MIN(12, strlen(node->firstChild->nodeValue)));
				val = atoi(valbuf);
			}
			callback(type, channel, &val);
			memset(valbuf, 0, 12);
			sprintf(valbuf, "%d", val);
			change_var(event, varnum, valbuf);
		}while(0);
	}
}

static int get_mute(struct action_event *event)
{
	/* FIXME - Channel */
	control_callback(event, mute_callback, CONTROL_GET, CONTROL_VAR_MUTE, "CurrentMute");
	return cmd_obtain_variable(event, CONTROL_VAR_MUTE, "CurrentMute");
}
static int set_mute(struct action_event *event)
{
	ENTER();
	control_callback(event, mute_callback, CONTROL_SET, CONTROL_VAR_MUTE, "DesiredMute");
	LEAVE();
	return 0;
}

static int get_volume(struct action_event *event)
{
	/* FIXME - Channel */
	control_callback(event, volume_callback, CONTROL_GET, CONTROL_VAR_VOLUME, "CurrentVolume");
	return cmd_obtain_variable(event, CONTROL_VAR_VOLUME,
				   "CurrentVolume");
}
static int set_volume(struct action_event *event)
{
	ENTER();
	control_callback(event, volume_callback, CONTROL_SET, CONTROL_VAR_VOLUME, "DesiredVolume");
	LEAVE();
	return 0;
}

static int get_volume_db(struct action_event *event)
{
	/* FIXME - Channel */
	control_callback(event, volumedb_callback, CONTROL_GET, CONTROL_VAR_VOLUME_DB, "CurrentVolumeDB");
	return cmd_obtain_variable(event, CONTROL_VAR_VOLUME_DB,
				   "CurrentVolumeDB");
}
static int set_volume_db(struct action_event *event)
{
	ENTER();
	control_callback(event, volumedb_callback, CONTROL_SET, CONTROL_VAR_VOLUME_DB, "DesiredVolumeDB");
	LEAVE();
	return 0;
}

static int get_loudness(struct action_event *event)
{
	/* FIXME - Channel */
	control_callback(event, loudness_callback, CONTROL_GET, CONTROL_VAR_LOUDNESS, "CurrentLoudness");
	return cmd_obtain_variable(event, CONTROL_VAR_LOUDNESS,
				   "CurrentLoudness");
}
static int set_loudness(struct action_event *event)
{
	ENTER();
	control_callback(event, loudness_callback, CONTROL_SET, CONTROL_VAR_LOUDNESS, "DesiredLoudness");
	LEAVE();
	return 0;
}


static struct action control_actions[] = {
	//[CONTROL_CMD_LIST_PRESETS] =        	{"ListPresets", list_presets},
	//[CONTROL_CMD_SELECT_PRESET] =       	{"SelectPreset", NULL},
	//[CONTROL_CMD_GET_BRIGHTNESS] =      	{"GetBrightness", get_brightness}, /* optional */
	//[CONTROL_CMD_SET_BRIGHTNESS] =      	{"SetBrightness", NULL}, /* optional */
	//[CONTROL_CMD_GET_CONTRAST] =        	{"GetContrast", get_contrast}, /* optional */
	//[CONTROL_CMD_SET_CONTRAST] =        	{"SetContrast", NULL}, /* optional */
	//[CONTROL_CMD_GET_SHARPNESS] =       	{"GetSharpness", get_sharpness}, /* optional */
	//[CONTROL_CMD_SET_SHARPNESS] =       	{"SetSharpness", NULL}, /* optional */
	//[CONTROL_CMD_GET_RED_GAIN] =        	{"GetRedVideoGain", get_red_videogain}, /* optional */
	//[CONTROL_CMD_SET_RED_GAIN] =        	{"SetRedVideoGain", NULL}, /* optional */
	//[CONTROL_CMD_GET_GREEN_GAIN] =      	{"GetGreenVideoGain", get_green_videogain}, /* optional */
	//[CONTROL_CMD_SET_GREEN_GAIN] =      	{"SetGreenVideoGain", NULL}, /* optional */
	//[CONTROL_CMD_GET_BLUE_GAIN] =       	{"GetBlueVideoGain", get_blue_videogain}, /* optional */
	//[CONTROL_CMD_SET_BLUE_GAIN] =       	{"SetBlueVideoGain", NULL}, /* optional */
	//[CONTROL_CMD_GET_RED_BLACK] =       	{"GetRedVideoBlackLevel", get_red_videoblacklevel}, /* optional */
	//[CONTROL_CMD_SET_RED_BLACK] =       	{"SetRedVideoBlackLevel", NULL}, /* optional */
	//[CONTROL_CMD_GET_GREEN_BLACK] =     	{"GetGreenVideoBlackLevel", get_green_videoblacklevel}, /* optional */
	//[CONTROL_CMD_SET_GREEN_BLACK] =     	{"SetGreenVideoBlackLevel", NULL}, /* optional */
	//[CONTROL_CMD_GET_BLUE_BLACK] =      	{"GetBlueVideoBlackLevel", get_blue_videoblacklevel}, /* optional */
	//[CONTROL_CMD_SET_BLUE_BLACK] =      	{"SetBlueVideoBlackLevel", NULL}, /* optional */
	//[CONTROL_CMD_GET_COLOR_TEMP] =      	{"GetColorTemperature", get_colortemperature}, /* optional */
	//[CONTROL_CMD_SET_COLOR_TEMP] =      	{"SetColorTemperature", NULL}, /* optional */
	//[CONTROL_CMD_GET_HOR_KEYSTONE] =    	{"GetHorizontalKeystone", get_horizontal_keystone}, /* optional */
	//[CONTROL_CMD_SET_HOR_KEYSTONE] =    	{"SetHorizontalKeystone", NULL}, /* optional */
	//[CONTROL_CMD_GET_VERT_KEYSTONE] =   	{"GetVerticalKeystone", get_vertical_keystone}, /* optional */
	//[CONTROL_CMD_SET_VERT_KEYSTONE] =   	{"SetVerticalKeystone", NULL}, /* optional */
	[CONTROL_CMD_GET_MUTE] =            	{"GetMute", get_mute}, /* optional */
	[CONTROL_CMD_SET_MUTE] =            	{"SetMute", set_mute}, /* optional */
	[CONTROL_CMD_GET_VOL] =             	{"GetVolume", get_volume}, /* optional */
	[CONTROL_CMD_SET_VOL] =             	{"SetVolume", set_volume}, /* optional */
	[CONTROL_CMD_GET_VOL_DB] =          	{"GetVolumeDB", get_volume_db}, /* optional */
	[CONTROL_CMD_SET_VOL_DB] =          	{"SetVolumeDB", set_volume_db}, /* optional */
	[CONTROL_CMD_GET_VOL_DBRANGE] =     	{"GetVolumeDBRange", NULL}, /* optional */
	[CONTROL_CMD_GET_LOUDNESS] =        	{"GetLoudness", get_loudness}, /* optional */
	[CONTROL_CMD_SET_LOUDNESS] =        	{"SetLoudness", set_loudness}, /* optional */
	[CONTROL_CMD_UNKNOWN] =			{NULL, NULL}
};


struct service control_service = {
	.service_name =	CONTROL_SERVICE,
	.type =	CONTROL_TYPE,
        .scpd_url = CONTROL_SCPD_URL,
        .control_url = CONTROL_CONTROL_URL,
        .event_url = CONTROL_EVENT_URL,
	.actions =	control_actions,
	.action_arguments =	argument_list,
	.variable_names =	control_variables,
	.variable_values =	control_values,
	.variable_meta =	control_var_meta,
	.variable_count =	CONTROL_VAR_UNKNOWN,
	.command_count =	CONTROL_CMD_UNKNOWN,
	.service_mutex =	&control_mutex,
  .scpd_location =     0x000effec
};

void control_init(void)
{
}
#endif

