/*This file has been prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the possible external configuration of the USB
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  ATmega32U4
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2007, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "usb_commun.h"
#include "usb_commun_hid.h"
#include "usb_commun_ms.h"
#include "usb_commun_cdc.h"

//! @defgroup usb_general_conf USB application configuration
//!
//! @{

      #define USB_DEVICE_FEATURE          ENABLED


   // _________________ USB REGULATOR CONFIGURATION _______________________
   //
   //! @defgroup USB_reg_mode USB regulator configuration
   //! @{

   //! @brief Enable the internal regulator for USB pads
   //!
   //! When the application voltage is lower than 3.5V, to optimize power consumption
   //! the internal USB pads regulatr can be disabled.
#ifndef USE_USB_PADS_REGULATOR
   #define USE_USB_PADS_REGULATOR   DISABLE      // Possible values ENABLE or DISABLE
#endif
   //! @}



// _________________ DEVICE MODE CONFIGURATION __________________________

   //! @defgroup USB_device_mode_cfg USB device operating mode configuration
   //!
   //! @{

#define USB_LOW_SPEED_DEVICE DISABLE

#define USB_DEVICE_SN_USE          ENABLE            // DISABLE
#define USE_DEVICE_SN_UNIQUE       DISABLE            // ignore if USB_DEVICE_SN_USE = DISABLE

#define NB_ENDPOINTS          4  //  number of endpoints in the application including control endpoint
#define EP_TEMP_IN            1
#define EP_TEMP_OUT           2

#define USB_REMOTE_WAKEUP_FEATURE     DISABLED   //! don't allow remote wake up

#define VBUS_SENSING_IO       DISABLED   //! device will connect directly on reset

#define USB_RESET_CPU         DISABLED   //! an USB reset does not reset the CPU

#define Usb_unicode(a)         ((U16)(a))

   //! @defgroup device_cst_actions USB device custom actions
   //!
   //! @{
   // write here the action to associate to each USB event
   // be carefull not to waste time in order not disturbing the functions
#define Usb_sof_action()         sof_action();
#define Usb_wake_up_action()
#define Usb_resume_action()
#define Usb_suspend_action()
#define Usb_reset_action()
#define Usb_vbus_on_action()
#define Usb_vbus_off_action()
#define Usb_set_configuration_action()
   //! @}

extern void sof_action(void);
   //! @}

//! @}

#endif // _CONF_USB_H_
