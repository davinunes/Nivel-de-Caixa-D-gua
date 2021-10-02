#define WIFI_SSID "Eternia"
#define WIFI_PASS "celestia"

#define IO_USERNAME "ilunne"
#define IO_KEY ""

char  str[] = "A2hcJfpu07RxU7cBShWaNX98kjMh_oia";
char temp;

#include "AdafruitIO_WiFi.h"

#if !defined(SPIWIFI_SS) // if the wifi definition isnt in the board variant
  // Don't change the names of these #define's! they match the variant ones
  #define SPIWIFI SPI
  #define SPIWIFI_SS 10 // Chip select pin
  #define NINA_ACK 9    // a.k.a BUSY or READY pin
  #define NINA_RESETN 6 // Reset pin
  #define NINA_GPIO0 -1 // Not connected
#endif

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
