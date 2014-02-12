#include <usb.h>
#include <usb_drv.h>
#include <usb_descriptors.h>
#include <usb_standard_request.h>

USB::USB()
{
}

bool USB::isEnumerated()
{
  return Is_device_enumerated();
}

bool USB::hasData()
{
  Usb_select_endpoint(EP_TEMP_OUT);
  return Is_usb_receive_out();
}

bool USB::canSend()
{
  Usb_select_endpoint(EP_TEMP_IN);
  return Is_usb_in_ready();
}

int USB::read(char* buf, int len)
{
  if(!isEnumerated())
    return 0;

  Usb_select_endpoint(EP_TEMP_OUT);

  if(!hasData())
    return 0;

  U16 num = Usb_byte_counter();
  if(len < num)
    num = len;

  char* ptr = buf;
  for(U16 i = num; i; i--)
    *ptr++ = Usb_read_byte();
  Usb_ack_receive_out();

  return num;
}

int USB::write(char* buf, int len)
{
  if(!isEnumerated())
    return 0;

  Usb_select_endpoint(EP_TEMP_IN);

  //if(!canSend())
  //  return 0;

  char* ptr = buf;
  for(int i = 0; i<len; i++)
    Usb_write_byte(*ptr++);
  for(int i; i<EP_IN_LENGTH_TEMP1; i++)
    Usb_write_byte(0);
  Usb_ack_in_ready();

  return len;
}
