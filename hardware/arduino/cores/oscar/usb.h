/* The 'Arduino-y' USB library
 * Provides a simple wrapper of the USB interface to let the code by pretty.
 * Makes LOTS of assumptions. (See conf_usb.h and the assorted usb_* files in
 * core/oscar for all the _real_ usb work. (e.g. the usb task)).
 */

#ifndef USB_H
#define USB_H

#include <compiler.h>
#include <conf_usb.h>

#define EP_LEN 64

class USB
{
  public:
    USB();

    bool isEnumerated();
    bool hasData();
    bool canSend();

    int read(char* buf, int len);
    int write(char* buf, int len);
};

#endif
