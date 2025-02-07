//-*-c++-*-
#ifndef INCLUDED_CreateCommands_h_
#define INCLUDED_CreateCommands_h_

// Create Commands
const char CREATE_START               = 128;
const char CREATE_BAUD                = 129;
const char CREATE_SAFE_OLD            = 130;
const char CREATE_SAFE                = 131;
const char CREATE_FULL                = 132;
const char CREATE_COVER               = 135;
const char CREATE_DEMO                = 136;
const char CREATE_SPOT                = 134;
const char CREATE_DRIVE               = 137;
const char CREATE_LSD                 = 138;
const char CREATE_LEDS                = 139;
const char CREATE_SONG                = 140;
const char CREATE_PLAY_SONG           = 141;
const char CREATE_SENSORS             = 142;
const char CREATE_COVER_AND_DOCK      = 143;
const char CREATE_PWM_LSD             = 144;
const char CREATE_DRIVE_DIRECT        = 145;
const char CREATE_DIGITAL_OUT         = 147;
const char CREATE_STREAM              = 148;
const char CREATE_QUERY_LIST          = 149;
const char CREATE_PAUSE_RESUME_STREAM = 150;
const char CREATE_SCRIPT              = 152;
const char CREATE_SEND_IR             = 151;
const char CREATE_PLAY_SCRIPT         = 153;
const char CREATE_SHOW_SCRIPT         = 154;
const char CREATE_WAIT_TIME           = 155;
const char CREATE_WAIT_DISTANCE       = 156;
const char CREATE_WAIT_ANGLE          = 157;
const char CREATE_WAIT_EVENT          = 158;

// Create sensor packet code
const char CREATE_SENSOR_GROUP_0            = 0;
const char CREATE_SENSOR_GROUP_1            = 1;
const char CREATE_SENSOR_GROUP_2            = 2;
const char CREATE_SENSOR_GROUP_3            = 3;
const char CREATE_SENSOR_GROUP_4            = 4;
const char CREATE_SENSOR_GROUP_5            = 5;
const char CREATE_SENSOR_GROUP_6            = 6;

const char CREATE_SENSOR_DROP               = 7;
const char CREATE_SENSOR_WALL               = 8;
const char CREATE_SENSOR_CLIFF_LEFT         = 9;
const char CREATE_SENSOR_CLIFF_FRONT_LEFT   = 10;
const char CREATE_SENSOR_CLIFF_FRONT_RIGHT  = 11;
const char CREATE_SENSOR_CLIFF_RIGHT        = 12;
const char CREATE_SENSOR_VIRTUAL_WALL       = 13;
const char CREATE_SENSOR_OVERCURRENT        = 14;
const char CREATE_SENSOR_UNUSED_1           = 15;
const char CREATE_SENSOR_UNUSED_2           = 16;
const char CREATE_SENSOR_IR                 = 17;
const char CREATE_SENSOR_BUTTONS            = 18;
const char CREATE_SENSOR_DISTANCE           = 19;
const char CREATE_SENSOR_ANGLE              = 20;
const char CREATE_SENSOR_CHANGING_STATE     = 21;
const char CREATE_SENSOR_VOLTAGE            = 22;
const char CREATE_SENSOR_CURRENT            = 23;
const char CREATE_SENSOR_BATTERY_TEMP       = 24;
const char CREATE_SENSOR_BATTERY_CHARGE     = 25;
const char CREATE_SENSOR_BATTERY_CAPACITY   = 26;
const char CREATE_SENSOR_WALL_SIGNAL        = 27;
const char CREATE_SENSOR_CLIFF_L_SIGNAL     = 28;
const char CREATE_SENSOR_CLIFF_FL_SIGNAL    = 29;
const char CREATE_SENSOR_CLIFF_FR_SIGNAL    = 30;
const char CREATE_SENSOR_CLIFF_R_SIGNAL     = 31;
const char CREATE_SENSOR_DIGITAL_IN         = 32;
const char CREATE_SENSOR_ANALOG             = 33;
const char CREATE_SENSOR_CAN_CHARGE         = 34;
const char CREATE_SENSOR_OI_MODE            = 35;
const char CREATE_SENSOR_SONG_NUMBER        = 36;
const char CREATE_SENSOR_SONG_PLAYING       = 37;
const char CREATE_SENSOR_STREAM_SIZE        = 38;
const char CREATE_SENSOR_REQ_VELOCITY       = 39;
const char CREATE_SENSOR_REQ_RADIUS         = 40;
const char CREATE_SENSOR_REQ_RIGHT_VELOCITY = 41;
const char CREATE_SENSOR_REQ_LEFT_VELOCITY  = 42;

#endif
