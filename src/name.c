// To give your project a unique name, this code must be
// placed into a .c file (its own tab).  It can not be in
// a .cpp file or your main sketch (the .ino file).

#include "usb_names.h"

// USB_BOARD_NAME_LEN match the number of characters in USB_BOARD_NAME

#define USB_BOARD_NAME   {'m','i','k','r','o',' ','M', 'I', 'D', 'I'}
#define USB_BOARD_NAME_LEN  10

// do not touch anything below this point

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + USB_BOARD_NAME_LEN * 2,
        3,
        USB_BOARD_NAME
};
