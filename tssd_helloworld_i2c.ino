/**
 * Copyright (c) 2018 Tara Keeling
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <Arduino.h>
#include <Wire.h>
#include "src/tssd1306/ssd1306.h"

void WireWrapperBeginTransmission( tssd1306* DeviceHandle, bool IsCommand );
size_t WireWrapperWrite( tssd1306* DeviceHandle, const uint8_t* Buffer, size_t Length );
void WireWrapperWriteByte( tssd1306* DeviceHandle, const uint8_t Data );
void WireWrapperEndTransmission( tssd1306* DeviceHandle );
void ResetDisplay( void );

// If you want to manually reset the display, set this to the GPIO you plan to use
const int MasterResetPin = -1;

void WireWrapperBeginTransmission( tssd1306* DeviceHandle, bool IsCommand ) {
  Wire.beginTransmission( DeviceHandle->Address );
  Wire.write( ( IsCommand == true ) ? SSD1306_I2C_Command : SSD1306_I2C_Data );
}

size_t WireWrapperWrite( tssd1306* DeviceHandle, const uint8_t* Buffer, size_t Length ) {
  return Wire.write( Buffer, Length );
}

void WireWrapperWriteByte( tssd1306* DeviceHandle, const uint8_t Data ) {
  Wire.write( Data );
}

void WireWrapperEndTransmission( tssd1306* DeviceHandle ) {
  Wire.endTransmission( );
}

static DisplayInterfaceProcs WireWrapper = {
  WireWrapperBeginTransmission,
  WireWrapperWrite,
  WireWrapperWriteByte,
  WireWrapperEndTransmission,
  ( size_t ) ( BUFFER_LENGTH - 1 ) // 1 byte always used for data/command byte
};

tssd1306 I2CDisplay = {
  &WireWrapper, // i2c interface callback
  128,  // 128px wide
  64, // 64px tall
  0x3C, // i2c address
  -1, // chip select not used for i2c
  0,  // text print x coordinate
  0,  // text print y coordinate
  &Font_Droid_Sans_Fallback_17x18 // font
};

void ResetDisplay( void ) {
  if ( MasterResetPin > -1 ) {
    pinMode( MasterResetPin, OUTPUT );

    digitalWrite( MasterResetPin, LOW );
    delay( 10 );

    digitalWrite( MasterResetPin, HIGH );
    delay( 15 );
  } else {
    // In case we're using an external reset circuit or one is
    // built into the display, just make sure it's finished.
    delay( 25 );
  }
}

void setup( void ) {
  ResetDisplay( );

  Wire.begin( );
  
  SSD1306_Init( &I2CDisplay );

  // If your display is upside-down uncomment these below:
  // Note that horizontal flip only takes effect with new data
  //SSD1306_SendSingleByteCommand( &I2CDisplay, SSD1306_Op_Horizontal_Flip_On );
  //SSD1306_SendSingleByteCommand( &I2CDisplay, SSD1306_Op_Vertical_Flip_On );
  
  SSD1306_PrintString( &I2CDisplay, "Hello, World!" );
}

void loop( void ) {
}
