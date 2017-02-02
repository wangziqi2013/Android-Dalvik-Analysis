
#pragma once

#ifndef _TYPE_CONFIG_H
#define _TYPE_CONFIG_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class TypeConfig - The configuration under which values should be used
 *
 * This structure is a complete set of possible configurations a Android
 * device could be configured
 */
class TypeConfig {
 public:
  uint32_t size;
  
  /*
   * operator==() - Fast operator overloading for comparing two 
   *                config objects
   */
  bool operator==(const TypeConfig &other) const {
    return memcmp(this, &other, sizeof(TypeConfig)) == 0;
  }
  
  // The following are copied and pasted from Android runtime code
  
  // Define flags and constants for various subsystem configurations.
  enum {
    ACONFIGURATION_ORIENTATION_ANY  = 0x0000,
    ACONFIGURATION_ORIENTATION_PORT = 0x0001,
    ACONFIGURATION_ORIENTATION_LAND = 0x0002,
    ACONFIGURATION_ORIENTATION_SQUARE = 0x0003,
    ACONFIGURATION_TOUCHSCREEN_ANY  = 0x0000,
    ACONFIGURATION_TOUCHSCREEN_NOTOUCH  = 0x0001,
    ACONFIGURATION_TOUCHSCREEN_STYLUS  = 0x0002,
    ACONFIGURATION_TOUCHSCREEN_FINGER  = 0x0003,
    ACONFIGURATION_DENSITY_DEFAULT = 0,
    ACONFIGURATION_DENSITY_LOW = 120,
    ACONFIGURATION_DENSITY_MEDIUM = 160,
    ACONFIGURATION_DENSITY_TV = 213,
    ACONFIGURATION_DENSITY_HIGH = 240,
    ACONFIGURATION_DENSITY_XHIGH = 320,
    ACONFIGURATION_DENSITY_XXHIGH = 480,
    ACONFIGURATION_DENSITY_XXXHIGH = 640,
    ACONFIGURATION_DENSITY_ANY = 0xfffe,
    ACONFIGURATION_DENSITY_NONE = 0xffff,
    ACONFIGURATION_KEYBOARD_ANY  = 0x0000,
    ACONFIGURATION_KEYBOARD_NOKEYS  = 0x0001,
    ACONFIGURATION_KEYBOARD_QWERTY  = 0x0002,
    ACONFIGURATION_KEYBOARD_12KEY  = 0x0003,
    ACONFIGURATION_NAVIGATION_ANY  = 0x0000,
    ACONFIGURATION_NAVIGATION_NONAV  = 0x0001,
    ACONFIGURATION_NAVIGATION_DPAD  = 0x0002,
    ACONFIGURATION_NAVIGATION_TRACKBALL  = 0x0003,
    ACONFIGURATION_NAVIGATION_WHEEL  = 0x0004,
    ACONFIGURATION_KEYSHIDDEN_ANY = 0x0000,
    ACONFIGURATION_KEYSHIDDEN_NO = 0x0001,
    ACONFIGURATION_KEYSHIDDEN_YES = 0x0002,
    ACONFIGURATION_KEYSHIDDEN_SOFT = 0x0003,
    ACONFIGURATION_NAVHIDDEN_ANY = 0x0000,
    ACONFIGURATION_NAVHIDDEN_NO = 0x0001,
    ACONFIGURATION_NAVHIDDEN_YES = 0x0002,
    ACONFIGURATION_SCREENSIZE_ANY  = 0x00,
    ACONFIGURATION_SCREENSIZE_SMALL = 0x01,
    ACONFIGURATION_SCREENSIZE_NORMAL = 0x02,
    ACONFIGURATION_SCREENSIZE_LARGE = 0x03,
    ACONFIGURATION_SCREENSIZE_XLARGE = 0x04,
    ACONFIGURATION_SCREENLONG_ANY = 0x00,
    ACONFIGURATION_SCREENLONG_NO = 0x1,
    ACONFIGURATION_SCREENLONG_YES = 0x2,
    ACONFIGURATION_SCREENROUND_ANY = 0x00,
    ACONFIGURATION_SCREENROUND_NO = 0x1,
    ACONFIGURATION_SCREENROUND_YES = 0x2,
    ACONFIGURATION_UI_MODE_TYPE_ANY = 0x00,
    ACONFIGURATION_UI_MODE_TYPE_NORMAL = 0x01,
    ACONFIGURATION_UI_MODE_TYPE_DESK = 0x02,
    ACONFIGURATION_UI_MODE_TYPE_CAR = 0x03,
    ACONFIGURATION_UI_MODE_TYPE_TELEVISION = 0x04,
    ACONFIGURATION_UI_MODE_TYPE_APPLIANCE = 0x05,
    ACONFIGURATION_UI_MODE_TYPE_WATCH = 0x06,
    ACONFIGURATION_UI_MODE_NIGHT_ANY = 0x00,
    ACONFIGURATION_UI_MODE_NIGHT_NO = 0x1,
    ACONFIGURATION_UI_MODE_NIGHT_YES = 0x2,
    ACONFIGURATION_SCREEN_WIDTH_DP_ANY = 0x0000,
    ACONFIGURATION_SCREEN_HEIGHT_DP_ANY = 0x0000,
    ACONFIGURATION_SMALLEST_SCREEN_WIDTH_DP_ANY = 0x0000,
    ACONFIGURATION_LAYOUTDIR_ANY  = 0x00,
    ACONFIGURATION_LAYOUTDIR_LTR  = 0x01,
    ACONFIGURATION_LAYOUTDIR_RTL  = 0x02,
    ACONFIGURATION_MCC = 0x0001,
    ACONFIGURATION_MNC = 0x0002,
    ACONFIGURATION_LOCALE = 0x0004,
    ACONFIGURATION_TOUCHSCREEN = 0x0008,
    ACONFIGURATION_KEYBOARD = 0x0010,
    ACONFIGURATION_KEYBOARD_HIDDEN = 0x0020,
    ACONFIGURATION_NAVIGATION = 0x0040,
    ACONFIGURATION_ORIENTATION = 0x0080,
    ACONFIGURATION_DENSITY = 0x0100,
    ACONFIGURATION_SCREEN_SIZE = 0x0200,
    ACONFIGURATION_VERSION = 0x0400,
    ACONFIGURATION_SCREEN_LAYOUT = 0x0800,
    ACONFIGURATION_UI_MODE = 0x1000,
    ACONFIGURATION_SMALLEST_SCREEN_SIZE = 0x2000,
    ACONFIGURATION_LAYOUTDIR = 0x4000,
    ACONFIGURATION_SCREEN_ROUND = 0x8000,
    ACONFIGURATION_MNC_ZERO = 0xffff,
  };
  
  union {
    struct {
      // Mobile country code (from SIM).  0 means "any".
      uint16_t mcc;
      // Mobile network code (from SIM).  0 means "any".
      uint16_t mnc;
    };
    
    uint32_t imsi;
  };
  
  union {
    struct {
      char language[2];
      char country[2];
    };
    
    uint32_t locale;
  };
  
  enum {
    ORIENTATION_ANY  = ACONFIGURATION_ORIENTATION_ANY,
    ORIENTATION_PORT = ACONFIGURATION_ORIENTATION_PORT,
    ORIENTATION_LAND = ACONFIGURATION_ORIENTATION_LAND,
    ORIENTATION_SQUARE = ACONFIGURATION_ORIENTATION_SQUARE,
  };
  
  enum {
    TOUCHSCREEN_ANY  = ACONFIGURATION_TOUCHSCREEN_ANY,
    TOUCHSCREEN_NOTOUCH  = ACONFIGURATION_TOUCHSCREEN_NOTOUCH,
    TOUCHSCREEN_STYLUS  = ACONFIGURATION_TOUCHSCREEN_STYLUS,
    TOUCHSCREEN_FINGER  = ACONFIGURATION_TOUCHSCREEN_FINGER,
  };
  
  enum {
    DENSITY_DEFAULT = ACONFIGURATION_DENSITY_DEFAULT,
    DENSITY_LOW = ACONFIGURATION_DENSITY_LOW,
    DENSITY_MEDIUM = ACONFIGURATION_DENSITY_MEDIUM,
    DENSITY_TV = ACONFIGURATION_DENSITY_TV,
    DENSITY_HIGH = ACONFIGURATION_DENSITY_HIGH,
    DENSITY_XHIGH = ACONFIGURATION_DENSITY_XHIGH,
    DENSITY_XXHIGH = ACONFIGURATION_DENSITY_XXHIGH,
    DENSITY_XXXHIGH = ACONFIGURATION_DENSITY_XXXHIGH,
    DENSITY_ANY = ACONFIGURATION_DENSITY_ANY,
    DENSITY_NONE = ACONFIGURATION_DENSITY_NONE,
  };
  
  union {
    struct {
      uint8_t orientation;
      uint8_t touchscreen;
      uint16_t density;
    };
    
    uint32_t screenType;
  };
  
  enum {
    KEYBOARD_ANY  = ACONFIGURATION_KEYBOARD_ANY,
    KEYBOARD_NOKEYS  = ACONFIGURATION_KEYBOARD_NOKEYS,
    KEYBOARD_QWERTY  = ACONFIGURATION_KEYBOARD_QWERTY,
    KEYBOARD_12KEY  = ACONFIGURATION_KEYBOARD_12KEY,
  };
  
  enum {
    NAVIGATION_ANY  = ACONFIGURATION_NAVIGATION_ANY,
    NAVIGATION_NONAV  = ACONFIGURATION_NAVIGATION_NONAV,
    NAVIGATION_DPAD  = ACONFIGURATION_NAVIGATION_DPAD,
    NAVIGATION_TRACKBALL  = ACONFIGURATION_NAVIGATION_TRACKBALL,
    NAVIGATION_WHEEL  = ACONFIGURATION_NAVIGATION_WHEEL,
  };
  
  enum {
    MASK_KEYSHIDDEN = 0x0003,
    KEYSHIDDEN_ANY = ACONFIGURATION_KEYSHIDDEN_ANY,
    KEYSHIDDEN_NO = ACONFIGURATION_KEYSHIDDEN_NO,
    KEYSHIDDEN_YES = ACONFIGURATION_KEYSHIDDEN_YES,
    KEYSHIDDEN_SOFT = ACONFIGURATION_KEYSHIDDEN_SOFT,
  };
  
  enum {
    MASK_NAVHIDDEN = 0x000c,
    SHIFT_NAVHIDDEN = 2,
    NAVHIDDEN_ANY = ACONFIGURATION_NAVHIDDEN_ANY << SHIFT_NAVHIDDEN,
    NAVHIDDEN_NO = ACONFIGURATION_NAVHIDDEN_NO << SHIFT_NAVHIDDEN,
    NAVHIDDEN_YES = ACONFIGURATION_NAVHIDDEN_YES << SHIFT_NAVHIDDEN,
  };
  
  union {
    struct {
      uint8_t keyboard;
      uint8_t navigation;
      uint8_t inputFlags;
      uint8_t inputPad0;
    };
    
    uint32_t input;
  };
  
  enum {
    SCREENWIDTH_ANY = 0,
  };
  
  enum {
    SCREENHEIGHT_ANY = 0,
  };
  
  union {
    struct {
      uint16_t screenWidth;
      uint16_t screenHeight;
    };
    
    uint32_t screenSize;
  };
  
  enum {
    SDKVERSION_ANY = 0,
  };
  
  enum {
    MINORVERSION_ANY = 0,
  };
    
  union {
    struct {
      uint16_t sdkVersion;
      // For now minorVersion must always be 0!!!  Its meaning
      // is currently undefined.
      uint16_t minorVersion;
    };
    
    uint32_t version;
  };
    
  enum {
    // screenLayout bits for screen size class.
    MASK_SCREENSIZE = 0x0f,
    SCREENSIZE_ANY = ACONFIGURATION_SCREENSIZE_ANY,
    SCREENSIZE_SMALL = ACONFIGURATION_SCREENSIZE_SMALL,
    SCREENSIZE_NORMAL = ACONFIGURATION_SCREENSIZE_NORMAL,
    SCREENSIZE_LARGE = ACONFIGURATION_SCREENSIZE_LARGE,
    SCREENSIZE_XLARGE = ACONFIGURATION_SCREENSIZE_XLARGE,
    
    // screenLayout bits for wide/long screen variation.
    MASK_SCREENLONG = 0x30,
    SHIFT_SCREENLONG = 4,
    SCREENLONG_ANY = ACONFIGURATION_SCREENLONG_ANY << SHIFT_SCREENLONG,
    SCREENLONG_NO = ACONFIGURATION_SCREENLONG_NO << SHIFT_SCREENLONG,
    SCREENLONG_YES = ACONFIGURATION_SCREENLONG_YES << SHIFT_SCREENLONG,

    // screenLayout bits for layout direction.
    MASK_LAYOUTDIR = 0xC0,
    SHIFT_LAYOUTDIR = 6,
    LAYOUTDIR_ANY = ACONFIGURATION_LAYOUTDIR_ANY << SHIFT_LAYOUTDIR,
    LAYOUTDIR_LTR = ACONFIGURATION_LAYOUTDIR_LTR << SHIFT_LAYOUTDIR,
    LAYOUTDIR_RTL = ACONFIGURATION_LAYOUTDIR_RTL << SHIFT_LAYOUTDIR,
  };
    
  enum {
    // uiMode bits for the mode type.
    MASK_UI_MODE_TYPE = 0x0f,
    UI_MODE_TYPE_ANY = ACONFIGURATION_UI_MODE_TYPE_ANY,
    UI_MODE_TYPE_NORMAL = ACONFIGURATION_UI_MODE_TYPE_NORMAL,
    UI_MODE_TYPE_DESK = ACONFIGURATION_UI_MODE_TYPE_DESK,
    UI_MODE_TYPE_CAR = ACONFIGURATION_UI_MODE_TYPE_CAR,
    UI_MODE_TYPE_TELEVISION = ACONFIGURATION_UI_MODE_TYPE_TELEVISION,
    UI_MODE_TYPE_APPLIANCE = ACONFIGURATION_UI_MODE_TYPE_APPLIANCE,
    UI_MODE_TYPE_WATCH = ACONFIGURATION_UI_MODE_TYPE_WATCH,

    // uiMode bits for the night switch.
    MASK_UI_MODE_NIGHT = 0x30,
    SHIFT_UI_MODE_NIGHT = 4,
    UI_MODE_NIGHT_ANY = ACONFIGURATION_UI_MODE_NIGHT_ANY << SHIFT_UI_MODE_NIGHT,
    UI_MODE_NIGHT_NO = ACONFIGURATION_UI_MODE_NIGHT_NO << SHIFT_UI_MODE_NIGHT,
    UI_MODE_NIGHT_YES = ACONFIGURATION_UI_MODE_NIGHT_YES << SHIFT_UI_MODE_NIGHT,
  };

  union {
    struct {
      uint8_t screenLayout;
      uint8_t uiMode;
      uint16_t smallestScreenWidthDp;
    };
      
    uint32_t screenConfig;
  };
    
  union {
    struct {
      uint16_t screenWidthDp;
      uint16_t screenHeightDp;
    };
    
    uint32_t screenSizeDp;
  };

  // The ISO-15924 short name for the script corresponding to this
  // configuration. (eg. Hant, Latn, etc.). Interpreted in conjunction with
  // the locale field.
  char localeScript[4];

  // A single BCP-47 variant subtag. Will vary in length between 4 and 8
  // chars. Interpreted in conjunction with the locale field.
  char localeVariant[8];

  enum {
    // screenLayout2 bits for round/notround.
    MASK_SCREENROUND = 0x03,
    SCREENROUND_ANY = ACONFIGURATION_SCREENROUND_ANY,
    SCREENROUND_NO = ACONFIGURATION_SCREENROUND_NO,
    SCREENROUND_YES = ACONFIGURATION_SCREENROUND_YES,
  };

  // An extension of screenConfig.
  union {
    struct {
      uint8_t screenLayout2;      // Contains round/notround qualifier.
      uint8_t screenConfigPad1;   // Reserved padding.
      uint16_t screenConfigPad2;  // Reserved padding.
    };
    
    uint32_t screenConfig2;
  };

  // Not really sure whether this is counted as an actual data field
  bool localeScriptWasComputed;
  
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////

  // Flags indicating a set of config values.  These flag constants must
  // match the corresponding ones in android.content.pm.ActivityInfo and
  // attrs_manifest.xml.
  enum {
    CONFIG_MCC = ACONFIGURATION_MCC,
    CONFIG_MNC = ACONFIGURATION_MNC,
    CONFIG_LOCALE = ACONFIGURATION_LOCALE,
    CONFIG_TOUCHSCREEN = ACONFIGURATION_TOUCHSCREEN,
    CONFIG_KEYBOARD = ACONFIGURATION_KEYBOARD,
    CONFIG_KEYBOARD_HIDDEN = ACONFIGURATION_KEYBOARD_HIDDEN,
    CONFIG_NAVIGATION = ACONFIGURATION_NAVIGATION,
    CONFIG_ORIENTATION = ACONFIGURATION_ORIENTATION,
    CONFIG_DENSITY = ACONFIGURATION_DENSITY,
    CONFIG_SCREEN_SIZE = ACONFIGURATION_SCREEN_SIZE,
    CONFIG_SMALLEST_SCREEN_SIZE = ACONFIGURATION_SMALLEST_SCREEN_SIZE,
    CONFIG_VERSION = ACONFIGURATION_VERSION,
    CONFIG_SCREEN_LAYOUT = ACONFIGURATION_SCREEN_LAYOUT,
    CONFIG_UI_MODE = ACONFIGURATION_UI_MODE,
    CONFIG_LAYOUTDIR = ACONFIGURATION_LAYOUTDIR,
    CONFIG_SCREEN_ROUND = ACONFIGURATION_SCREEN_ROUND,
  };
  
  /*
   * UnpackLanguageOrRegion() - As name suggests
   *
   * This function is copied form Android runtime code:
   *    https://github.com/android/platform_frameworks_base/blob/master/libs/androidfw/ResourceTypes.cpp
   * Apache 2.0 License
   */
  static size_t UnpackLanguageOrRegion(const char in[2], 
                                       const char base,
                                       char out[4]) {
    if (in[0] & 0x80) {
      const uint8_t first = in[1] & 0x1f;
      const uint8_t second = ((in[1] & 0xe0) >> 5) + ((in[0] & 0x03) << 3);
      const uint8_t third = (in[0] & 0x7c) >> 2;

      out[0] = first + base;
      out[1] = second + base;
      out[2] = third + base;
      out[3] = 0;

      return 3;
    }
  
    if (in[0]) {
      memcpy(out, in, 2);
      memset(out + 2, 0, 2);
      return 2;
    }
  
    memset(out, 0, 4);
    
    return 0;
  }
  
  /*
   * UnpackLanguage() - As name suggests
   */
  size_t UnpackLanguage(char output[4]) const {
    return UnpackLanguageOrRegion(language, 'a', output);
  }
  
  /*
   * UnpackRegion() - As name suggests
   */
  size_t UnpackRegion(char output[4]) const {
    return UnpackLanguageOrRegion(country, '0', output);
  }
  
  /*
   * AppendDirLocale() - Prints the dir and locale into a buffer
   *
   * This function is copied form Android runtime code:
   *    https://github.com/android/platform_frameworks_base/blob/master/libs/androidfw/ResourceTypes.cpp
   * Apache 2.0 License
   */
  void AppendDirLocale(Buffer *buffer_p) const {
    if(!language[0]) {
      return;
    }
    
    const bool scriptWasProvided = \
      localeScript[0] != '\0' && !localeScriptWasComputed;
      
    if(!scriptWasProvided && !localeVariant[0]) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }

      char buf[4];
      size_t len = UnpackLanguage(buf);
      buffer_p->Append(buf, len);

      if(country[0]){
        buffer_p->Printf("-r");
        len = UnpackRegion(buf);
        buffer_p->Append(buf, len);
      }
      
      return;
    }
    
    // We are writing the modified BCP 47 tag.
    // It starts with 'b+' and uses '+' as a separator.

    if(buffer_p->GetLength() > 0) {
      buffer_p->AppendByte('-');
    }
    
    buffer_p->Printf("b+");

    char buf[4];
    size_t len = UnpackLanguage(buf);
    buffer_p->Append(buf, len);

    if(scriptWasProvided) {
      buffer_p->Printf("+");
      buffer_p->Append(localeScript, sizeof(localeScript));
    }

    if(country[0]) {
      buffer_p->Printf("+");
      len = UnpackRegion(buf);
      buffer_p->Append(buf, len);
    }

    if(localeVariant[0]) {
      buffer_p->Printf("+");
      buffer_p->Append(localeVariant, 
                       strnlen(localeVariant, sizeof(localeVariant)));
    }
    
    return;
  }
  
  /*
   * GetName() - Prints the name of a config into a buffer
   *
   * This function is copied form Android runtime code:
   *    https://github.com/android/platform_frameworks_base/blob/master/libs/androidfw/ResourceTypes.cpp
   * Apache 2.0 License
   */
  void GetName(Buffer *buffer_p) const {
    if(mcc != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      buffer_p->Printf("mcc%d", mcc);
    }
    
    if(mnc != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
        
      buffer_p->Printf("mnc%d", mnc);
    }
    
    // Adding country code and locale
    AppendDirLocale(buffer_p);

    if((screenLayout & MASK_LAYOUTDIR) != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
        
      switch(screenLayout & MASK_LAYOUTDIR) {
        case LAYOUTDIR_LTR:
          buffer_p->Printf("ldltr");
          break;
        case LAYOUTDIR_RTL:
          buffer_p->Printf("ldrtl");
          break;
        default:
          buffer_p->Printf("layoutDir=%d", screenLayout & MASK_LAYOUTDIR);
          break;
      }
    }
    
    if(smallestScreenWidthDp != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      buffer_p->Printf("sw%ddp", smallestScreenWidthDp);
    }
    
    if(screenWidthDp != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      buffer_p->Printf("w%ddp", screenWidthDp);
    }
    
    if(screenHeightDp != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      buffer_p->Printf("h%ddp", screenHeightDp);
    }
    
    if((screenLayout & MASK_SCREENSIZE) != SCREENSIZE_ANY) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(screenLayout & MASK_SCREENSIZE) {
        case SCREENSIZE_SMALL:
          buffer_p->Printf("small");
          break;
        case SCREENSIZE_NORMAL:
          buffer_p->Printf("normal");
          break;
        case SCREENSIZE_LARGE:
          buffer_p->Printf("large");
          break;
        case SCREENSIZE_XLARGE:
          buffer_p->Printf("xlarge");
          break;
        default:
          buffer_p->Printf("screenLayoutSize=%d",
                           screenLayout & MASK_SCREENSIZE);
          break;
      }
    }
    
    if((screenLayout & MASK_SCREENLONG) != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(screenLayout & MASK_SCREENLONG) {
        case SCREENLONG_NO:
          buffer_p->Printf("notlong");
          break;
        case SCREENLONG_YES:
          buffer_p->Printf("long");
          break;
        default:
          buffer_p->Printf("screenLayoutLong=%d",
                           screenLayout & MASK_SCREENLONG);
          break;
      }
    }
    
    if((screenLayout2 & MASK_SCREENROUND) != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(screenLayout2 & MASK_SCREENROUND) {
        case SCREENROUND_NO:
          buffer_p->Printf("notround");
          break;
        case SCREENROUND_YES:
          buffer_p->Printf("round");
          break;
        default:
          buffer_p->Printf("screenRound=%d", screenLayout2&MASK_SCREENROUND);
          break;
      }
    }
    
    if(orientation != ORIENTATION_ANY) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(orientation) {
        case ORIENTATION_PORT:
          buffer_p->Printf("port");
          break;
        case ORIENTATION_LAND:
          buffer_p->Printf("land");
          break;
        case ORIENTATION_SQUARE:
          buffer_p->Printf("square");
          break;
        default:
          buffer_p->Printf("orientation=%d", orientation);
          break;
      }
    }
    
    if((uiMode & MASK_UI_MODE_TYPE) != UI_MODE_TYPE_ANY) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(uiMode&MASK_UI_MODE_TYPE) {
        case UI_MODE_TYPE_DESK:
          buffer_p->Printf("desk");
          break;
        case UI_MODE_TYPE_CAR:
          buffer_p->Printf("car");
          break;
        case UI_MODE_TYPE_TELEVISION:
          buffer_p->Printf("television");
          break;
        case UI_MODE_TYPE_APPLIANCE:
          buffer_p->Printf("appliance");
          break;
        case UI_MODE_TYPE_WATCH:
          buffer_p->Printf("watch");
          break;
        default:
          buffer_p->Printf("uiModeType=%d",
                           screenLayout&MASK_UI_MODE_TYPE);
          break;
      }
    }
    
    if((uiMode & MASK_UI_MODE_NIGHT) != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(uiMode&MASK_UI_MODE_NIGHT) {
        case UI_MODE_NIGHT_NO:
          buffer_p->Printf("notnight");
          break;
        case UI_MODE_NIGHT_YES:
          buffer_p->Printf("night");
          break;
        default:
          buffer_p->Printf("uiModeNight=%d",
                           uiMode&MASK_UI_MODE_NIGHT);
          break;
        }
    }
    
    if(density != DENSITY_DEFAULT) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(density) {
        case DENSITY_LOW:
          buffer_p->Printf("ldpi");
          break;
        case DENSITY_MEDIUM:
          buffer_p->Printf("mdpi");
          break;
        case DENSITY_TV:
          buffer_p->Printf("tvdpi");
          break;
        case DENSITY_HIGH:
          buffer_p->Printf("hdpi");
          break;
        case DENSITY_XHIGH:
          buffer_p->Printf("xhdpi");
          break;
        case DENSITY_XXHIGH:
          buffer_p->Printf("xxhdpi");
          break;
        case DENSITY_XXXHIGH:
          buffer_p->Printf("xxxhdpi");
          break;
        case DENSITY_NONE:
          buffer_p->Printf("nodpi");
          break;
        case DENSITY_ANY:
          buffer_p->Printf("anydpi");
          break;
        default:
          buffer_p->Printf("%ddpi", density);
          break;
      }
    }
    
    if(touchscreen != TOUCHSCREEN_ANY) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(touchscreen) {
        case TOUCHSCREEN_NOTOUCH:
          buffer_p->Printf("notouch");
          break;
        case TOUCHSCREEN_FINGER:
          buffer_p->Printf("finger");
          break;
        case TOUCHSCREEN_STYLUS:
          buffer_p->Printf("stylus");
          break;
        default:
          buffer_p->Printf("touchscreen=%d", touchscreen);
          break;
      }
    }
    
    if((inputFlags & MASK_KEYSHIDDEN) != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(inputFlags&MASK_KEYSHIDDEN) {
        case KEYSHIDDEN_NO:
          buffer_p->Printf("keysexposed");
          break;
        case KEYSHIDDEN_YES:
          buffer_p->Printf("keyshidden");
          break;
        case KEYSHIDDEN_SOFT:
          buffer_p->Printf("keyssoft");
          break;
      }
    }
    
    if(keyboard != KEYBOARD_ANY) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(keyboard) {
        case KEYBOARD_NOKEYS:
          buffer_p->Printf("nokeys");
          break;
        case KEYBOARD_QWERTY:
          buffer_p->Printf("qwerty");
          break;
        case KEYBOARD_12KEY:
          buffer_p->Printf("12key");
          break;
        default:
          buffer_p->Printf("keyboard=%d", keyboard);
          break;
      }
    }
    
    if((inputFlags&MASK_NAVHIDDEN) != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch(inputFlags&MASK_NAVHIDDEN) {
        case NAVHIDDEN_NO:
          buffer_p->Printf("navexposed");
          break;
        case NAVHIDDEN_YES:
          buffer_p->Printf("navhidden");
          break;
        default:
          buffer_p->Printf("inputFlagsNavHidden=%d",
                           inputFlags & MASK_NAVHIDDEN);
          break;
      }
    }
    
    if(navigation != NAVIGATION_ANY) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      switch (navigation) {
        case NAVIGATION_NONAV:
          buffer_p->Printf("nonav");
          break;
        case NAVIGATION_DPAD:
          buffer_p->Printf("dpad");
          break;
        case NAVIGATION_TRACKBALL:
          buffer_p->Printf("trackball");
          break;
        case NAVIGATION_WHEEL:
          buffer_p->Printf("wheel");
          break;
        default:
          buffer_p->Printf("navigation=%d", navigation);
          break;
      }
    }
    
    if(screenSize != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      buffer_p->Printf("%dx%d", screenWidth, screenHeight);
    }
    
    if(version != 0) {
      if(buffer_p->GetLength() > 0) {
        buffer_p->AppendByte('-');
      }
      
      buffer_p->Printf("v%d", sdkVersion);
      if (minorVersion != 0) {
        buffer_p->Printf(".%d", minorVersion);
      }
    }

    return;
  }
} BYTE_ALIGNED;

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
