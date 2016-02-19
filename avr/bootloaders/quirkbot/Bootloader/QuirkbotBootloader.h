/*
             LUFA Library
     Copyright (C) Dean Camera, 2015.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2015  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/*
			Quirkbot Bootloader
	Copyright (C) Paulo Barcelos, 2016.

		paulo [at] quirkbot [dot] com
			   quirkbot.com
*/

/*
  Copyright 2016 Paulo Barcelos (paulo [at] quirkbot [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Header file for QuirkbotBootloader.c.
 */

#ifndef _CDC_H_
#define _CDC_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/boot.h>
		#include <avr/eeprom.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <stdbool.h>
		#include <string.h>
		#include <stdlib.h>

		#include "Descriptors.h"
		#include "BootloaderAPI.h"
		#include "Config/AppConfig.h"

		#include "Board/LEDs.h"

		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>

	/* Preprocessor Checks: */
		#if !defined(__OPTIMIZE_SIZE__)
			#error This bootloader requires that it be optimized for size, not speed, to fit into the target device. Change optimization settings and try again.
		#endif

	/* Macros: */
		/** Version major of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MAJOR     0x01

		/** Version minor of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MINOR     0x00

		/** Hardware version major of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MAJOR   0x01

		/** Hardware version minor of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MINOR   0x00

		/** Eight character bootloader firmware identifier reported to the host when requested. */
		#define SOFTWARE_IDENTIFIER          DEVICE_SID

		/** Magic bootloader key to unlock forced application start mode. */
		#define MAGIC_BOOT_KEY               BOOT_KEY

		/** Size of the UUID report */
		#define UUID_SIZE                    16

		/** UUID report delimiters */
		#define UUID_REPORT_START_DELIMITER  0xFA
		#define UUID_REPORT_ID_DELIMITER     0xFB
		#define UUID_REPORT_END_DELIMITER    0xFF

	/* Enums: */
		/** Possible memory types that can be addressed via the bootloader. */
		enum AVR109_Memories
		{
			MEMORY_TYPE_FLASH  = 'F',
			MEMORY_TYPE_EEPROM = 'E',
		};

		/** Possible commands that can be issued to the bootloader. */
		enum AVR109_Commands
		{
			AVR109_COMMAND_Sync                     = 27,
			AVR109_COMMAND_ReadEEPROM               = 'd',
			AVR109_COMMAND_WriteEEPROM              = 'D',
			AVR109_COMMAND_ReadFLASHWord            = 'R',
			AVR109_COMMAND_WriteFlashPage           = 'm',
			AVR109_COMMAND_FillFlashPageWordLow     = 'c',
			AVR109_COMMAND_FillFlashPageWordHigh    = 'C',
			AVR109_COMMAND_GetBlockWriteSupport     = 'b',
			AVR109_COMMAND_BlockWrite               = 'B',
			AVR109_COMMAND_BlockRead                = 'g',
			AVR109_COMMAND_ReadExtendedFuses        = 'Q',
			AVR109_COMMAND_ReadHighFuses            = 'N',
			AVR109_COMMAND_ReadLowFuses             = 'F',
			AVR109_COMMAND_ReadLockbits             = 'r',
			AVR109_COMMAND_WriteLockbits            = 'l',
			//AVR109_COMMAND_EraseFLASH               = 'e',
			AVR109_COMMAND_ReadSignature            = 's',
			AVR109_COMMAND_ReadBootloaderSWVersion  = 'V',
			AVR109_COMMAND_ReadBootloaderHWVersion  = 'v',
			AVR109_COMMAND_ReadBootloaderIdentifier = 'S',
			AVR109_COMMAND_ReadBootloaderInterface  = 'p',
			AVR109_COMMAND_SetCurrentAddress        = 'A',
			AVR109_COMMAND_ReadAutoAddressIncrement = 'a',
			AVR109_COMMAND_ReadPartCode             = 't',
			AVR109_COMMAND_EnterProgrammingMode     = 'P',
			AVR109_COMMAND_LeaveProgrammingMode     = 'L',
			AVR109_COMMAND_SelectDeviceType         = 'T',
			//AVR109_COMMAND_SetLED                   = 'x',
			//AVR109_COMMAND_ClearLED                 = 'y',
			AVR109_COMMAND_ExitBootloader           = 'E',
			AVR109_COMMAND_ReadUUID                 = 'U',
		};

		/** Possible MIDI commands that can be issued to the bootloader. */
		enum MIDI_Commands
		{
			MIDI_COMMAND_Sync                       = 0xa,
			MIDI_COMMAND_EnterBootloader            = 0xb,
			MIDI_COMMAND_StartFirmware              = 0xc,
			MIDI_COMMAND_Data                       = 0xd,
			MIDI_COMMAND_ExitBootloader             = 0xe,
			MIDI_COMMAND_ReadUUID                   = 0xf,
		};

	/* Type Defines: */
		/** Type define for a non-returning pointer to the start of the loaded application in flash memory. */
		typedef void (*AppPtr_t)(void) ATTR_NO_RETURN;

	/* Function Prototypes: */

		void Application_Jump_Check(void) ATTR_INIT_SECTION(3);

		static void SetupHardware(void);

		static void CDC_Task(void);
		static void CDC_WriteMemoryBlock(void);
		static void CDC_ReadMemoryBlock(void);
		static uint8_t CDC_FetchNextCommandByte(void);
		static void CDC_WriteNextResponseByte(const uint8_t Response);
		static bool CDC_WaitDelivery(void);
		static void CDC_Conclude(void);

		static void MIDI_Task(void);

		static void PushFirmwareByte(uint8_t FirmwareByte);

		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

#endif

