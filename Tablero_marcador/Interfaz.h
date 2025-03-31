/*
   -- Prueba --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <BLEDevice.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 49 bytes
  { 255,2,0,0,0,42,0,19,0,0,0,0,186,1,106,200,1,1,3,0,
  1,10,105,21,14,1,134,31,43,0,1,34,105,21,14,1,36,31,45,0,
  129,24,64,18,32,64,79,48,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t home_plus; // =1 if button pressed, else =0
  uint8_t home_minus; // =1 if button pressed, else =0

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


void setupRemoteXY(){
    // RemoteXY begin
    RemoteXY_Init (); 
    // RemoteXY end
    
    // TODO you setup code
}
void loopRemoteXY(){
    // RemoteXY handler
    RemoteXY_Handler ();
    
    // TODO you loop code
    // use the RemoteXY structure for data transfer
    // do not call delay(), use instead RemoteXY_delay() 
}


