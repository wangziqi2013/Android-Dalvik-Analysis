
#pragma once

#ifndef _RES_TBALE_H
#define _RES_TBALE_H

#include "common.h"
#include "res_base.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class ResourceTable - Represents binary resource table
 */
class ResourceTable : public ResourceBase {
 // type declarations
 private: 

  /*
   * class TableHeader - The resource table header which is at the beginning of 
   *                     the table
   */
  class TableHeader {
   public: 
    CommonHeader common_header;
    
    // Number of packages included in this resource table
    uint32_t package_count; 
  } BYTE_ALIGNED;
  
  /*
   * class PackageHeader - Package header that records metadata about package
   */
  class PackageHeader {
   public:
    CommonHeader common_header; 
     
    // Base package's ID (also in the res identifier)
    // If 0 then it is not a base package; it is always 0x7F for application
    // package type
    uint32_t id; 
    
    // UTF-16 encoded name of the package
    // The length is fixed 256 bytes encoded in UTF16 though UTF16 itself
    // is a variable length encoding scheme
    unsigned char name_utf16[128 * sizeof(char16_t)];
    
    // An offset from the beginning of this struct to the string pool for types
    uint32_t type_string_pool_offset;
    
    // Index into string pool to indicate the last type visible
    uint32_t last_public_type;
    
    // Same - an offset from this struct as the key string pool
    uint32_t key_string_pool_offset;
    
    // The last index of a key that is visible
    uint32_t last_public_key;
    
  } BYTE_ALIGNED;
  
  /*
   * class TypeSpecHeader - Type specification header
   *
   * This header is followed by an array of uint32_t integers denoting the 
   * configuration of resources available for a certain resource of this
   * type. There is excatly one entry for each resource instance
   */
  class TypeSpecHeader {
   public: 
    CommonHeader common_header;
    
    // Low 8 bits of a 32 bit integer represents the type id
    uint8_t id;
    uint8_t zero1;
    uint16_t zero2;
    
    // Number of entries after this struct
    uint32_t entry_count;
    
    // The pointer to the first entry (it does not consume struct space)
    uint32_t data[0];
  } BYTE_ALIGNED;
  
  /*
   * class TypeConfig - The configuration under which values should be used
   *
   * This structure is a complete set of possible configurations a Android
   * device could be configured
   */
  class TypeConfig {
   public:
    uint32_t size;
    
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
  
    bool localeScriptWasComputed;
  
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
  
  /*
   * class TypeHeader - Header of the type instance chunk
   */
  class TypeHeader {
   public: 
    CommonHeader common_header;
    
    // This is the ID of the type being described here
    uint8_t type_id;
    uint8_t zero1;
    uint16_t zero2;
    
    // Number of resource value instances inside this chunk
    uint32_t entry_count;
    
    // The offset to the starting address of the data part
    // Between this structure and the header is an array of uint32_t
    // that records the offset of each instance into the data part
    uint32_t data_offset;
    
    // This structure records the config of this group of values
    // i.e. they are used under common configurations
    TypeConfig config;
  } BYTE_ALIGNED;
  
  // We need the pointer type before it is declared
  class TypeSpec;
  
  /*
   * class ResourceId - Resource identifier in 32-bit field
   */
  union ResourceId {
   public:
    // The structural of resource ID is 0xpptteeee
    // so we start declaring the entry ID at low address and then
    // type ID and then package ID
    struct {
      uint16_t entry_id;
      uint8_t type_id;
      uint8_t package_id;
    } BYTE_ALIGNED;
    
    // 32 bit identifier used as a whole
    uint32_t data;
  } BYTE_ALIGNED;
  
  // Make sure the size of the union is always correct
  static_assert(sizeof(ResourceId) == sizeof(uint32_t), 
                "Invalid size of resource ID");
  
  /*
   * class ResourceEntryField - If a resource entry is a complicated one then
   *                            this represents fields inside the resource entry
   */
  class ResourceEntryField {
   public:
    
    /*
     * Res_MAKEINTERNAL() - Makes special name field values for attr types
     */
    #define Res_MAKEINTERNAL(entry) (0x01000000 | (entry&0xFFFF))
    
    // Special values for 'name' when defining attribute resources.
    enum {
      // This entry holds the attribute's type code.
      ATTR_TYPE = Res_MAKEINTERNAL(0),

      // For integral attributes, this is the minimum value it can hold.
      ATTR_MIN = Res_MAKEINTERNAL(1),

      // For integral attributes, this is the maximum value it can hold.
      ATTR_MAX = Res_MAKEINTERNAL(2),

      // Localization of this resource is can be encouraged or required with
      // an aapt flag if this is set
      ATTR_L10N = Res_MAKEINTERNAL(3),

      // for plural support, see android.content.res.PluralRules#attrForQuantity(int)
      ATTR_OTHER = Res_MAKEINTERNAL(4),
      ATTR_ZERO = Res_MAKEINTERNAL(5),
      ATTR_ONE = Res_MAKEINTERNAL(6),
      ATTR_TWO = Res_MAKEINTERNAL(7),
      ATTR_FEW = Res_MAKEINTERNAL(8),
      ATTR_MANY = Res_MAKEINTERNAL(9), 
    };

    #undef Res_MAKEINTERNAL

    // Bit mask of allowed types, for use with ATTR_TYPE.
    enum {
      // No type has been defined for this attribute, use generic
      // type handling.  The low 16 bits are for types that can be
      // handled generically; the upper 16 require additional information
      // in the bag so can not be handled generically for TYPE_ANY.
      TYPE_ANY = 0x0000FFFF,

      // Attribute holds a references to another resource.
      TYPE_REFERENCE = 1<<0,

      // Attribute holds a generic string.
      TYPE_STRING = 1<<1,

      // Attribute holds an integer value.  ATTR_MIN and ATTR_MIN can
      // optionally specify a constrained range of possible integer values.
      TYPE_INTEGER = 1<<2,

      // Attribute holds a boolean integer.
      TYPE_BOOLEAN = 1<<3,

      // Attribute holds a color value.
      TYPE_COLOR = 1<<4,

      // Attribute holds a floating point value.
      TYPE_FLOAT = 1<<5,

      // Attribute holds a dimension value, such as "20px".
      TYPE_DIMENSION = 1<<6,

      // Attribute holds a fraction value, such as "20%".
      TYPE_FRACTION = 1<<7,

      // Attribute holds an enumeration.  The enumeration values are
      // supplied as additional entries in the map.
      TYPE_ENUM = 1<<16,

      // Attribute holds a bitmaks of flags.  The flag bit values are
      // supplied as additional entries in the map.
      TYPE_FLAGS = 1<<17
    };

    // Enum of localization modes, for use with ATTR_L10N.
    enum {
        L10N_NOT_REQUIRED = 0,
        L10N_SUGGESTED    = 1
    }; 
     
    // This field has different interpretations
    ResourceId name;
    ResourceValue value; 
  } BYTE_ALIGNED;
  
  /*
   * class ResourceEntry - Represents resource entry in the body of type chunk
   */
  class ResourceEntry {
   public: 
    // The length of only this structure (i.e. not responsible for 
    // data after this structure)
    uint16_t entry_length;
  
    /*
     * enum Flags
     */
    enum Flags : uint16_t {
      // This flag decides how the following data is organized
      // For a simple entry the following data is just a ResourceValue instance
      // Otherwise it is followed by a mapping descriptor and several maps
      // to form a composite value
      COMPLEX = 0x0001,
      PUBLIC = 0x0002,
      // If set, this is a weak resource and may be overriden by strong
      // resources of the same name/type. This is only useful during
      // linking with other resource tables.
      FLAG_WEAK = 0x0004,
    };
    
    // As defined above
    Flags flags;

    // A string into key string table of the package denoting the name of the
    // resource entry
    uint32_t key;
    
    // Different interpretation of the following bytes depending on
    // whether the type is complex or not
    // Note that the following structure is not counted as the length in
    // the common header
    
    union {
      // This struct is used only if the resource is a complex one
      struct {
        // The resource ID of its parent which refers to another resource
        // 0x00000000 if there is no parent
        ResourceId parent_id;
      
        // The number of key-value pairs after the body
        uint32_t entry_count;
        
        // This is the starting address of entry fields, there are 
        // entry_count of them
        ResourceEntryField field_data[0];
      } BYTE_ALIGNED;
      
      // If the resource is not a complex one then use this one
      ResourceValue value;
    } BYTE_ALIGNED;
    
    /*
     * IsComplex() - Whether the resource entry is composite
     */
    inline bool IsComplex() const {
      return flags & Flags::COMPLEX;
    }
    
    /*
     * IsPublic() - Returns true if the entry is in the public name space
     */
    inline bool IsPublic() const {
      return flags & Flags::PUBLIC; 
    }
  } BYTE_ALIGNED;
  
  /*
   * class Type - Represents a certain type of resource and all its contents
   */
  class Type {
   private: 
    // This is the number of bytes we use to initialize the buffer
    static constexpr size_t INIT_BUFFER_LENGTH = 16UL;
    
    // If the offset table entry has value like this then the entry does not exist
    static constexpr ENTRY_NOT_PRESENT = 0xFFFFFFFF;
    
   public:
    // Original pointer to the header
    TypeHeader *header_p;
    
    // A pointer to the spec object (not the header!)
    TypeSpec *type_spec_p;
     
    // This stores readable names of the type
    // Note that we need to specify a length for buffer objects because by
    // default the buffer uses 64 KB internal storage on initialization
    // Note that this name does not have any type information, so when
    // use this to create directory we need to prepend the directory name
    Buffer readable_name;
    
    // This is the name of the base type, i.e. attr without any postfix
    Buffer base_type_name;
    
    // Number of entries in this type table
    size_t entry_count;
    
    // This points to the offset table
    uint32_t *offset_table;
    
    // Points to the resource entry indexed by the offset table
    unsigned char *data_p;
    
    /*
     * Constructor
     */
    Type() :
      header_p{nullptr},
      type_spec_p{nullptr},
      readable_name{INIT_BUFFER_LENGTH},
      base_type_name{INIT_BUFFER_LENGTH},
      entry_count{0UL},
      offset_table{nullptr},
      data_p{nullptr}
    {}
    
    /*
     * IsEntryValid() - Checks whether an entry ID is valid
     *
     * We do two checkings here: 
     *   (1) Checks whether it is out of bound
     *   (2) Checks whether it refers to an invalid (0xFFFFFFFF) entry
     *
     * If either is unsatisfied return false. The caller should either response
     * or raise error
     */
    bool IsEntryIdValid(size_t entry_id) const {
      return (entry_id < entry_count) && \
             (offset_table[entry_id] != ENTRY_NOT_PRESENT);
    }
    
    /*
     * WriteXml() - Writes an XML file that represents the structure of the
     *              current type
     *
     * The XML file is written according to the type of the resource. We
     * hardcode the format for each type and its identifier, e.g. attr, string,
     * drawable, etc.
     */
    void WriteXml() {
      if(base_type_name == "attr") {
        WriteAttrXml("attrs.xml");
      } else {
#ifndef NDEBUG
        dbg_printf("Unknown attribute name: ");
        base_type_name.WriteToFile(stderr);
        fprintf(stderr, "\n");
#endif

        ReportError(UNKNOWN_TYPE_TO_WRITE_XML); 
      }
      
      return;
    }
    
    static constexpr const char *RES_PATH = "res";
    static constexpr const char *VALUE_PATH_PREFIX = "values";
    static constexpr const char *XML_SUFFIX = ".xml";
    
    /*
     * SwitchToValuesDir() - Switch to the values directory and opens the 
     *                       file for current base type
     *
     * This function takes care of possible postfix of "values"; The CWD
     * is not changed after returning. If file open or directory operations
     * returns error then exception is thrown
     */
    FILE *SwitchToValuesDir(const char *file_name) {
      // Save current directory first to get back after we have finished this 
      const char *cwd = FileUtility::GetCwd();
      
      // Enters 'res' first
      FileUtility::CreateOrEnterDir(RES_PATH);
      
      // So the total length we need is "values-" + length of the readable name
      // and if readable name is empty just omit the dash after "values"
      // so need 1 more bytes for '\0' and 1 byte for the possible '-'
      size_t value_path_length = strlen(VALUE_PATH_PREFIX) + \
                                 readable_name.GetLength() + \
                                 2;
      
      Buffer value_path{value_path_length};
      value_path.Append("values");
      // If there is a special name then append them also after the dash
      if(readable_name.GetLength() != 0UL) {
        value_path.Append('-');
        value_path.Append(readable_name); 
      }
      
      // Make it a valid C string
      value_path.Append('\0');
      
      // And then enters the dir or creates it if first time
      FileUtility::CreateOrEnterDir( \
        static_cast<const char *>(value_path.GetData()));

      FILE *fp = FileUtility::OpenFile(file_name, "w");
      
      // Frees current directory after switching back
      FileUtility::EnterDir(cwd);
      delete[] cwd;
      
      return fp;
    }
    
    /*
     * PrintAttrFormat() - Prints the format of attributes
     */
    void PrintAttrFormat(Buffer *buffer_p, uint32_t format) {
      if(format & ResourceEntryField::TYPE_REFERENCE) {
        buffer_p->Append("reference|");
      }
      
      if(format & ResourceEntryField::TYPE_STRING) {
        buffer_p->Append("string|");
      }
      
      if(format & ResourceEntryField::TYPE_INTEGER) {
        buffer_p->Append("integer|");
      }
      
      if(format & ResourceEntryField::TYPE_BOOLEAN) {
        buffer_p->Append("boolean|");
      }
      
      if(format & ResourceEntryField::TYPE_COLOR) {
        buffer_p->Append("color|");
      }
      
      if(format & ResourceEntryField::TYPE_FLOAT) {
        buffer_p->Append("float|");
      }
      
      if(format & ResourceEntryField::TYPE_DIMENSION) {
        buffer_p->Append("dimension|");
      }
      
      if(format & ResourceEntryField::TYPE_FRACTION) {
        buffer_p->Append("fraction|");
      }
      
      buffer_p->Rewind(1);
      
      return;
    }
    
    /*
     * PrintAttrEnumFlags() - Prints nested attr types, i.e. enum or flags
     */
    void PrintAttrEnumFlags(FILE *fp,
                            Buffer *buffer_p, 
                            uint32_t format,
                            ResourceEntryField *field_p,
                            uint32_t entry_count) {
      // Make sure it is enum or flag type
      assert((format & \
              (ResourceEntryField::TYPE_ENUM | \
               ResourceEntryField::TYPE_FLAGS)) != 0x00000000);
               
      if((format & ResourceEntryField::TYPE_ENUM) != 0x00000000) {
        // If it is a type enum then print it at the 2nd ident level
        FileUtility::WriteString(fp, "<enum name=\"", 2);
        
      } else {
        // If it is a type flag
      }
      
      return;
    }
    
    /*
     * WriteAttrXml() - Treat this as an attribute type resource and write XML
     *
     * We try to create and enter the directory res/values-???/ where ??? is the
     * type information, and then create an XML file named "attrs.xml"
     *
     * Note that this function takes the file name tobe written since the file
     * name is different from what is recorded in the type string pool
     */
    void WriteAttrXml(const char *file_name) {
      FILE *fp = SwitchToValuesDir(file_name);
      
      FileUtility::WriteString(fp, XML_HEADER_LINE);
      
      // We re-use this buffer for each entry and rewind it to 
      // the beginning everytime
      Buffer buffer;
      
      // Then loop through all attributes and print them to the file 
      for(size_t i = 0;i < entry_count;i++) {
        uint32_t offset = offset_table[i];
        ResourceEntry *entry_p = reinterpret_cast<ResourceEntry *>( \
         data_p + offset);
        
        // Attributes must be complex because we want to know its format
        if(entry_p->IsComplex() == false) {
          ReportError(INVALID_ATTR_ENTRY, "Attribute entry must be complex"); 
        } else if(entry_p->entry_count == 0) {
          ReportError(INVALID_ATTR_ENTRY, 
                      "Attribute entry must have at least 1 field"); 
        } else if(entry_p->parent_id.data != 0x00000000) {
          ReportError(INVALID_ATTR_ENTRY, 
                      "Attribute parent ID must be 0"); 
        }
        
        Package *package_p = type_spec_p->package_p;
        
        // First print name:
        buffer.Append("<attr name=\"");
        package_p->key_string_pool.AppendToBuffer(entry_p->key, 
                                                  &buffer);
        buffer.Append('\"');
        
        // This points to the only field
        ResourceEntryField *field_p = entry_p->field_data; 
        // This defines the format of the following data, so must appear here
        assert(field_p->name.data == 0x01000000);
        
        // This is a bit mask that specifies the format of data allowed
        uint32_t format_mask = field_p->value.data;
        
        // Common case: Just one attribute, print format
        if(entry_p->entry_count == 1) {
          buffer.Append(" format=\"");
          
          // Must have at least type specified here
          assert((format_mask & ResourceEntryField::TYPE_ANY) != 0x00000000);
          
          PrintAttrFormat(&buffer, format_mask);
          
          buffer.Append("\" />\n");
          
          // Terminate this string
          buffer.Append('\0');
          // Write with identation level = 1
          FileUtility::WriteString(fp, buffer.GetCharData(), 1);
          
          buffer.Reset();
        } else {
          // Close the current line and make it to the file
          buffer.Append(">\n");
          buffer.Append('\0');
          FileUtility::WriteString(fp, buffer.GetCharData(), 1);
          buffer.Reset();
          
          // It is an array, so need to pass the starting element and 
          // entry count
          PrintAttrEnumFlags(fp,
                             &buffer, 
                             format_mask, 
                             field_p + 1, 
                             entry_p->entry_count - 1);
                             
          FileUtility::WriteString(fp, "</attr>\n", 1);
          buffer.Reset();
        }
      } // loop through all entries
      
      FileUtility::WriteString(fp, RESOURCE_END_TAG);
      FileUtility::CloseFile(fp);
      
      return;
    }
  };
  
  class Package;
  
  /*
   * class TypeSpec - General type specification on configurations
   */
  class TypeSpec {
   public: 
    // This points to the type spec header
    TypeSpecHeader *header_p;
    
    // This points to the containing package instance (not the package header)
    Package *package_p;
    
    // Type ID - begins at 1, and 0 means invalid (so whenever we use this
    // to probe the string table we need to decrease it by 1)
    uint32_t type_id;
    
    // Number of entries in the table
    size_t entry_count;
    
    // Pointer to the configuration table about configurations of different
    // value instances (i.e. a bit field indicating which kind of resources 
    // are available)
    uint32_t *config_table;
    
    // Type values of different configurations
    std::vector<Type> type_list;
    
    /*
     * Constructor
     */
    TypeSpec() :
      header_p{nullptr},
      package_p{nullptr},
      entry_count{0UL},
      config_table{nullptr},
      type_list{}
    {}
  };
  
  /*
   * class Package - Represents internals of a package
   */
  class Package {
   public: 
    // Pointer to the package header
    PackageHeader *header_p;
    
    // These two are used to store headers
    StringPoolHeader *type_string_pool_header_p;
    StringPoolHeader *key_string_pool_header_p;
    
    // Two string pools indicated in the header
    StringPool type_string_pool;
    StringPool key_string_pool;
    
    // A list of type spec objects
    std::vector<TypeSpec> type_spec_list;
      
    /*
     * GetTypeCount() - Returns the number of types defined as resources
     *
     * Since the number of strings in the string pool defines all types
     * we could treat this as the number of types
     */
    size_t GetTypeCount() const {
      return type_string_pool.string_count;
    }
  };

 // Data members  
 private:
  TableHeader *table_header_p;
  
  // This is a mapping between package ID and the package pointer stored
  // also inside this class. We have two access methods for packages: 
  // either through this mapping using the package ID or through the 
  // array of package objects
  std::unordered_map<uint8_t, Package *> package_map;
  
  // A list of packages
  // We do reserve space for this vector such that only 1 malloc() is done
  std::vector<Package> package_list;
 public:
  
  /*
   * Constructor
   */
  ResourceTable(unsigned char *p_raw_data_p, 
                size_t p_length, 
                bool p_own_data=false) :
    ResourceBase{p_raw_data_p, p_length, p_own_data},
    table_header_p{nullptr} {
    
    // Check whether all header fields are valid; if not just exit
    CommonHeader *next_header_p = VerifyTableHeader();  
    if(IsValidTable() == false) {
      table_header_p = nullptr;
      
      return; 
    }
    
    while(next_header_p != nullptr) {
      next_header_p = ParseNext(next_header_p);
    }
    
    assert(string_pool_header_p != nullptr);
    
    return;    
  }
  
  /*
   * Destructor
   */
  ~ResourceTable() {
    return; 
  }
  
  /*
   * PrintResourceKey() - Prints the resource key to a buffer object
   *
   * Since each resource has a unique resource identifier we could locate them
   * using the identifier and print its name. Whether or not the resource
   * is a complex one does not matter since each resource has a name in
   * the key string pool
   *
   * In the case that there are multiple resource types for different 
   * configurations, we need a type config object to match all possible
   * configurations, and in the case none matches, just fall back to the
   * default
   *
   * Note that this function only prints in the current resource table. If
   * the package ID is not in the current resource table then we need to
   * check external packages
   */
  void PrintResourceKey(Buffer *buffer_p, 
                        ResourceId id, 
                        const TypeConfig &type_config) {
    uint8_t package_id = id.package_id;
    uint8_t type_id = id.type_id;
    uint16_t entry_id = id.entry_id;
    
    auto it = package_map.find(package_id);
    if(it == package_map.end()) {
      ReportError(PACKAGE_ID_NOT_FOUND, id.data); 
    }
    
    // This is the package pointer found
    Package *package_p = it->second;
    
    // Then verify whether the type ID is correct
    if(package_p->GetTypeCount() <= type_id) {
      ReportError(INVALID_TYPE_ID, static_cast<uint32_t>(type_id));
    }
    
    // This is a pointer to the type spec header
    TypeSpec *type_spec_p = package_p->type_spec_list[type_id];
    for(size_t i = 0;i < type_spec_p->type_list.size();i++) {
      Type *type_p = &type_spec_p->type_list[i];
      
      if(type_config == type_p->header_p->config) {
        if(entry_id >= type_p->entry_count) {
          ReportError(INVALID_ENTRY_ID, static_cast<uint32_t>(entry_id)); 
        }
        
        if(type_p->offset_table[entry_id] == 0xFFFFFFFF) 
      }
    }
  }
  
  /*
   * IsValidTable() - Whether the resource table is valid after verification
   */
  inline bool IsValidTable() const {
    return table_header_p != nullptr; 
  }
  
  /*
   * VerifyTableHeader() - Check fields in the table header
   *
   * If all check pass then return the byte next to table header. Otherwise 
   * return nullptr.
   *
   * This function always sets table_header_p to the first byte of the stream.
   * However if check fails the caller is responsible for setting the pointer
   * to nullptr to indicate that the resource table is not valid 
   *
   * This function has exactly the same structure as VerifyXmlHeader()
   */
  CommonHeader *VerifyTableHeader() {
    table_header_p = reinterpret_cast<TableHeader *>(raw_data_p);
    
    if(table_header_p->common_header.type != ChunkType::RESOURCE_TABLE) {
      dbg_printf("Resource table type 0x%X is wrong (expecting 0x%X)\n", 
                 static_cast<uint32_t>(table_header_p->common_header.type), 
                 static_cast<uint32_t>(ChunkType::RESOURCE_TABLE));
      
      return nullptr; 
    } else if(table_header_p->common_header.header_length != \
              sizeof(TableHeader)) {
      dbg_printf("Resource table length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->common_header.header_length, 
                 sizeof(TableHeader));
      
      return nullptr;      
    } else if(table_header_p->common_header.total_length != length) {
      dbg_printf("XML total length 0x%X is wrong (expecting 0x%lX)\n", 
                 table_header_p->common_header.total_length, 
                 length);
      
      // We require that the entire document is part of the XML
      // Otherwise we are unable to decode the rest of it
      return nullptr; 
    }
    
    dbg_printf("Verified resource table header; package count = %u\n",
               table_header_p->package_count);
               
    // This serves as an optimization such that we only allocate once
    // for the parsing process
    package_list.reserve(table_header_p->package_count);
    
    // Return the next byte and cast it as common header for later parsing
    return reinterpret_cast<CommonHeader *>(
             TypeUtility::Advance(table_header_p, sizeof(TableHeader))); 
  }
  
  /*
   * InitPackage() - Initialize a package object
   *
   * This function initializes the type spec list with proper number
   * of slots to hold type spec, since we know the number of type spec objects
   * is exactly the size of the string pool
   */
  void InitPackage(Package *package_p, PackageHeader *package_header_p) {
    package_p->header_p = package_header_p;
    
    package_p->type_string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>( \
        TypeUtility::Advance(package_header_p, 
                             package_header_p->type_string_pool_offset));
    
    ConstructStringPool( \
      reinterpret_cast<CommonHeader *>(package_p->type_string_pool_header_p), 
      &package_p->type_string_pool);
                        
    package_p->key_string_pool_header_p = \
      reinterpret_cast<StringPoolHeader *>( \
        TypeUtility::Advance(package_header_p, 
                             package_header_p->key_string_pool_offset));
    
    ConstructStringPool( \
      reinterpret_cast<CommonHeader *>(package_p->key_string_pool_header_p), 
      &package_p->key_string_pool);
    
    // This is done to only allocate exactly GetTypeCount() slots for type
    // spec objects (i.e. string pool size)
    package_p->type_spec_list.resize(package_p->GetTypeCount());
    
    return;
  }
  
  /*
   * DebugPrintPackageTypeString() - Prints all type strings in a package
   *
   * This function is meant for debugging
   */
  void DebugPrintPackageTypeString(Package *package_p) {
    dbg_printf("    Resource types: ");
    
    if(package_p->GetTypeCount() > 0) {
      package_p->type_string_pool.DebugPrint(0, "%s");
      // Print out all types in debug output; if debug is turned off this will
      // be optimized out                   
      for(size_t i = 1;i < package_p->GetTypeCount();i++) {
        fprintf(stderr, " | ");
        package_p->type_string_pool.DebugPrint(i, "%s");
      }
      
      fprintf(stderr, "\n");
    } else {
      fprintf(stderr, "[None]\n"); 
    }
    
    return;
  }
  
  /*
   * ParsePackage() - Parses the package header and push a package
   *                  object to the package list
   *
   * This function locates all type specs inside the package and then parses
   * all type headers after the type spec header. Each typespec stores metadata
   * about a type and each type object stores the resource value under a certain
   * configuration (e.g. language, screen resolution, etc.)
   */
  void ParsePackage(CommonHeader *header_p) {
    PackageHeader *package_header_p = \
      reinterpret_cast<PackageHeader *>(header_p);
      
    dbg_printf("    Package ID = 0x%02X\n", package_header_p->id);  
    
    // Even in Android runtime this is not taken care, so ....
    assert(package_header_p->type_string_pool_offset != 0);
    assert(package_header_p->key_string_pool_offset != 0);
    
    // Construct a packge object at the back of the vector
    // This saves the cost of copying the object
    package_list.emplace_back();
    // This points to the package object we just inserted
    Package *package_p = &package_list.back();
    
    // Initialize members inside the package object
    InitPackage(package_p, package_header_p);
    
    // Also put the package object into the vector (we do not support
    // non-base packages)
    if(package_p->header_p->id == 0x00000000) {
      ReportError(ONLY_SUPPORT_BASE_PACKAGE);  
    }
    
    package_map[static_cast<uint8_t>(package_p->header_p->id)] = package_p;
    
#ifndef NDEBUG
    DebugPrintPackageTypeString(package_p);
#endif

    // The first type spec chunk must be after the key string pool
    // so use its total size to determine (hopefully string pool is aligned)
    CommonHeader *type_spec_header_p = \
      reinterpret_cast<CommonHeader *>( \
        TypeUtility::Advance(
          package_p->key_string_pool_header_p, 
          package_p->key_string_pool_header_p->common_header.total_length));
     
    // Each type will have a type spec chunk, so just use the number of 
    // elements in type string pool     
    for(size_t i = 0;i < package_p->GetTypeCount();i++) {
      uint32_t type_id = ParseTypeSpecHeader(type_spec_header_p, package_p);
      
      // Use its length field to find the following type spec chunk
      type_spec_header_p = \
        TypeUtility::Advance(type_spec_header_p, 
                             type_spec_header_p->total_length);

      // Then loop to parse types
      // Note that here everytime we speculate a type header we need to
      // check whether the pointer is still in the range of the data area
      while(IsValidPointer(type_spec_header_p) == true && \
            type_spec_header_p->type == ChunkType::TYPE) {
        ParseTypeHeader(type_spec_header_p, package_p, type_id);
        
        type_spec_header_p = \
          TypeUtility::Advance(type_spec_header_p, 
                               type_spec_header_p->total_length);
      }
    }
                        
    return;
  }
  
  /*
   * ParseTypeSpecHeader() - Parses type specification header and
   *                         returns the type ID
   */
  uint32_t ParseTypeSpecHeader(CommonHeader *header_p, Package *package_p) {
    dbg_printf("Parsing TypeSpec chunk @ offset 0x%lX\n", 
               TypeUtility::GetPtrDiff(raw_data_p, header_p));
    assert(header_p->type == ChunkType::TYPE_SPEC);
    
    TypeSpecHeader *type_spec_header_p = \
      reinterpret_cast<TypeSpecHeader *>(header_p);
    
    // Get the type ID which also represents its position in the vector
    // NOTE: The real ID is always 1 less then the recodrd ID
    uint32_t type_id = static_cast<uint32_t>(type_spec_header_p->id);
    
    // It could not be 0 and also could not exceed the maximum
    if(type_id == 0 || type_id > package_p->GetTypeCount()) {
      ReportError(INVALID_TYPE_ID, type_id);
    }
    
    // This is the type spec object already allocated on the type spec list
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_id - 1];
    
    // Assert the type has never be seen
    assert(type_spec_p->header_p == nullptr);
    assert(type_spec_p->type_list.size() == 0);
    
    // Assign data members
    type_spec_p->header_p = type_spec_header_p;
    type_spec_p->package_p = package_p; 
    type_spec_p->type_id = type_id;
    type_spec_p->entry_count = type_spec_header_p->entry_count;
    type_spec_p->config_table = type_spec_header_p->data;
    
#ifndef NDEBUG
    dbg_printf("    Type id = %u; entry_count = %u (type name = ", 
               static_cast<uint32_t>(type_spec_header_p->id),
               static_cast<uint32_t>(type_spec_header_p->entry_count));

    Buffer buffer{128};
    package_p->type_string_pool.AppendToBuffer(type_id - 1, &buffer);
    buffer.Append(")\n");
    buffer.WriteToFile(stderr);
#endif

    return type_id;
  }

  /*
   * DebugPrintAllTypeEntryNames() - Prints on stderr names of all entries in the
   *                                 type chunk body
   */
  void DebugPrintAllTypeEntryNames(Package *package_p, Type *type_p) {
	  for(size_t i = 0;i < type_p->entry_count;i++) {
	    uint32_t offset = type_p->offset_table[i];

      // Resource entry does not exist for current configuration
      if(offset == Type::ENTRY_NOT_PRESENT) {
        //dbg_printf("        [INVALID ENTRY]\n");
        continue;
      }

      // This is a pointer to the resource entry
      ResourceEntry *resource_entry_p = reinterpret_cast<ResourceEntry *>( \
        type_p->data_p + offset);

      dbg_printf("        Name %lu = ", i);
      Buffer buffer{128};

      package_p->key_string_pool.AppendToBuffer(resource_entry_p->key, &buffer);

      if(resource_entry_p->IsComplex() == true || \
         resource_entry_p->IsPublic() == true) {
        buffer.Append(" (");
      }

      // For complex types it has two more fields - parent resource ID and
      // count of the key value pair that follows
      if(resource_entry_p->IsComplex() == true) {
        assert(resource_entry_p->entry_length == 16UL);
        buffer.Append("COMPLEX ");
      } else {
        assert(resource_entry_p->entry_length == 8UL);
      }

      if(resource_entry_p->IsPublic() == true) {
        buffer.Append("PUBLIC ");
      }

      if(resource_entry_p->IsComplex() == true || \
         resource_entry_p->IsPublic() == true) {
        // Eat back the last space character
        buffer.Rewind(1);
        buffer.Append(')');
      }

      buffer.Append('\n');
      buffer.WriteToFile(stderr);

      // In the next line print out the extra complex field
      if(resource_entry_p->IsComplex() == true) {
        dbg_printf("            * Parent ID = 0x%X; entry count = %u\n",
                   resource_entry_p->parent_id.data,
                   resource_entry_p->entry_count);

        // This is the starting address of the array of ResourceEntryField
        ResourceEntryField *entry_field_p = resource_entry_p->field_data;

        // Loop through each entry to see its internal data
        for(uint32_t j = 0;j < resource_entry_p->entry_count;j++) {
          // This is the current entry filed being processed
          ResourceEntryField *field_p = entry_field_p + j;
          
#ifndef NDEBUG          
          // Print out the 32 bit integer resource ID
          dbg_printf("            "
                     "entry name = 0x%.8X; type = 0x%.4X, data = 0x%.8X\n",
                     field_p->name.data,
                     static_cast<uint32_t>(field_p->value.type),
                     field_p->value.data);
          
          buffer.Reset();
          AppendResourceValueToBuffer(&field_p->value, &buffer);
          dbg_printf("                Printed value \"");
          buffer.Append("\"\n");
          buffer.WriteToFile(stderr);
#endif          
          
          // If the type ID is not attr then the field name must have
          // a ATTR type ID
          //if(type_p->type_spec_p->type_id != 0x01) {
          //  assert(field_p->name.type_id == 0x01);
          //}
        } // Loop through entry fields
      } else {
        dbg_printf("            "
                   "entry type = 0x%.4X, data = 0x%.8X\n",
                   static_cast<uint32_t>(resource_entry_p->value.type),
                   resource_entry_p->value.data);
        
        buffer.Reset();
        AppendResourceValueToBuffer(&resource_entry_p->value, &buffer);
        dbg_printf("            Printed value \"");
        buffer.Append("\"\n");
        buffer.WriteToFile(stderr);
      } // If is complex type then ... else ...
	  } // for resource entry for the current type

    return;
  }
  
  /*
   * DebugWriteTypeXml() - Writes a XML file for a given type
   */
  void DebugWriteTypeXml(Type *type_p) {
    type_p->WriteXml();
    
    return;
  }

  /*
   * ParseTypeHeader() - Parses type header
   *
   * Note that the ID of the type recorded in the type spec header is
   * passed in to verify that the type header has the same ID
   */
  void ParseTypeHeader(CommonHeader *header_p, 
                       Package *package_p, 
                       uint32_t type_id) {
    TypeHeader *type_header_p = \
      reinterpret_cast<TypeHeader *>(header_p);
    
    // This points to the type spec object
    TypeSpec *type_spec_p = &package_p->type_spec_list[type_id - 1];
    
    // Construct a new type object in-place and grab a pointer to it
    type_spec_p->type_list.emplace_back();
    Type *type_p = &type_spec_p->type_list.back();
    
    type_p->header_p = type_header_p;
    type_p->type_spec_p = type_spec_p;
    type_header_p->config.GetName(&type_p->readable_name);
    // Also write the base type name into the buffer
    package_p->type_string_pool.AppendToBuffer(type_spec_p->type_id - 1, 
                                               &type_p->base_type_name);
    type_p->entry_count = type_header_p->entry_count;
    
    // The offset table is just located after the header
    // Note that since we are not certain about the length of the config
    // structure, currently just use header length to determine the end
    // of the header
    type_p->offset_table = reinterpret_cast<uint32_t *>( \
      TypeUtility::Advance(type_header_p, 
                           type_header_p->common_header.header_length));
    
    // This is the base address of offset_table and its offset itself is
    // specified by a header field
    type_p->data_p = reinterpret_cast<unsigned char *>( \
      TypeUtility::Advance(type_header_p, 
                           type_header_p->data_offset));
    
    // Under debug mode we print it out
#ifndef NDEBUG
    dbg_printf("    Parsing type header @ 0x%lX: ", 
               TypeUtility::GetPtrDiff(raw_data_p, header_p));
    
    type_p->readable_name.WriteToFile(stderr);
    fputc('\n', stderr);
    
    dbg_printf("        Entry count = %u\n", type_header_p->entry_count);
    
    DebugPrintAllTypeEntryNames(package_p, type_p);
#endif

#ifndef NDEBUG    
    DebugWriteTypeXml(type_p);
#endif
    
    // The ID must match
    assert(static_cast<uint32_t>(type_header_p->type_id) == type_id);
    
    return;
  }
  
  /*
   * IsValidPointer() - Checks whether a pointer is still in the range of the 
   *                    data area
   */
  inline bool IsValidPointer(void *p) const {
    return TypeUtility::GetPtrDiff(raw_data_p, p) < length;
  }
  
  /*
   * ParseNext() - Parse the contents of the resource table
   *
   * This function has the same structure as the one in class BinaryXml
   *
   * If we have reached the end then just return nullptr and that's it
   */ 
  CommonHeader *ParseNext(CommonHeader *next_header_p) {
    assert(next_header_p != nullptr);
    
    if(IsValidPointer(next_header_p) == false) {
      return nullptr;
    }
    
    CommonHeader *ret_header_p = \
      TypeUtility::Advance(next_header_p, next_header_p->total_length);
    
    dbg_printf("Parsing header of type %u @ offset 0x%lX\n",
               (uint32_t)next_header_p->type,
               TypeUtility::GetPtrDiff(raw_data_p, next_header_p));
    
    switch(next_header_p->type) {
      case ChunkType::RESOURCE_TABLE: {
        ReportError(UNEXPECTED_START_OF_RESOURCE_TABLE);
        break;
      } 
      case ChunkType::STRING_POOL: {
        ParseStringPool(next_header_p);
        break;
      } 
      case ChunkType::PACKAGE: {
        // This function also needs to recognize the end of data
        ParsePackage(next_header_p);
        break; 
      }
      default: {
        ReportError(UNKNOWN_CHUNK_TYPE, 
                    static_cast<uint32_t>(next_header_p->type),
                    (size_t)next_header_p - (size_t)raw_data_p); 
        break;
      }
    } // switch type
    
    return ret_header_p;
  }
};

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
