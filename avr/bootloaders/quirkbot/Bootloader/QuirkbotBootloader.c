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
 *  Main source file for the Quirkbot bootloader. This file contains the complete bootloader logic.
 */

#define  INCLUDE_FROM_BOOTLOADERCDC_C
#include "QuirkbotBootloader.h"

/** LUFA MIDI Class driver interface configuration and state information. This structure is
 *  passed to all MIDI Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface =
	{
		.Config =
			{
				.StreamingInterfaceNumber = INTERFACE_ID_AudioStream,
				.DataINEndpoint           =
					{
						.Address          = MIDI_STREAM_IN_EPADDR,
						.Size             = MIDI_STREAM_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint          =
					{
						.Address          = MIDI_STREAM_OUT_EPADDR,
						.Size             = MIDI_STREAM_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Contains the current baud rate and other settings of the first virtual serial port. This must be retained as some
 *  operating systems will not open the port unless the settings can be set successfully.
 */
static CDC_LineEncoding_t LineEncoding = { .BaudRateBPS = 0,
										   .CharFormat  = CDC_LINEENCODING_OneStopBit,
										   .ParityType  = CDC_PARITY_None,
										   .DataBits	= 8							};

/** Current address counter. This stores the current address of the FLASH or EEPROM as set by the host,
 *  and is used when reading or writing to the AVRs memory (either FLASH or EEPROM depending on the issued
 *  command.)
 */
static uint16_t CurrAddress;

/** A buffer that will hold all bytes from a page.
 */
static uint8_t FirmwarePageIndex;
static uint8_t FirmwarePageBuffer[SPM_PAGESIZE];

/** Timer to indicate if the bootloader should be running, or should exit and allow the application code to run
 *  via a watchdog reset. If it reaches the timeout value the bootloader will exit, starting the watchdog and entering
 *  an infinite loop until the AVR restarts and the application runs.
 */
static uint8_t BootloaderTimer = BOOTLOADER_TIMEOUT;

static void ResetBootloaderTimer(void)
{
	BootloaderTimer = BOOTLOADER_TIMEOUT;
}
static void ExpireBootloaderTimer(void)
{
	BootloaderTimer = 0;
}


/** Magic lock for forced application start. If the HWBE fuse is programmed and BOOTRST is unprogrammed, the bootloader
 *  will start if the /HWB line of the AVR is held low and the system is reset. However, if the /HWB line is still held
 *  low when the application attempts to start via a watchdog reset, the bootloader will re-start. If set to the value
 *  \ref MAGIC_BOOT_KEY the special init function \ref Application_Jump_Check() will force the application to start.
 */
uint16_t MagicBootKey ATTR_NO_INIT;

/** Special startup routine to check if the bootloader was started via a watchdog reset, and if the magic application
 *  start key has been loaded into \ref MagicBootKey. If the bootloader started via the watchdog and the key is valid,
 *  this will force the user application to start via a software jump.
 */
void Application_Jump_Check(void)
{
	/* Check the reason for the reset so we can act accordingly */
	uint8_t  mcusr_state = MCUSR;		// store the initial state of the Status register
	MCUSR = 0;							// clear all reset flags

	/* If a request has been made to jump to the user application, honor it */
	if (
		// If it's not an external reset
		!(mcusr_state & (1<<EXTRF))
		// And after a power-on reset...
		&& ((mcusr_state & (1<<PORF))
		// ... or Boot Key is set
		|| MagicBootKey == MAGIC_BOOT_KEY)
		// And there is a firmware in memory
		&& (pgm_read_word_near(0) != 0xFFFF))
	{
		/* Turn off the watchdog */
		MCUSR &= ~(1 << WDRF);
		wdt_disable();

		/* Clear the boot key and jump to the user application */
		MagicBootKey = 0;

		// cppcheck-suppress constStatement
		((void (*)(void))0x0000)();
	}
}

/** Main program entry point. This routine configures the hardware required by the bootloader, then continuously
 *  runs the bootloader processing routine until instructed to soft-exit, or hard-reset via the watchdog to start
 *  the loaded application code.
 */
int main(void)
{
	/* Setup hardware required for the bootloader */
	SetupHardware();

	/* Turn on first LED on the board to indicate that the bootloader has started */
	LEDs_SetAllLEDs(LEDS_LED1);

	/* Enable global interrupts so that the USB stack can function */
	GlobalInterruptEnable();

	while (BootloaderTimer)
	{
		MIDI_Task();
		CDC_Task();
		MIDI_Device_USBTask(&Keyboard_MIDI_Interface);
		USB_USBTask();
	}

	/* Wait a short time to end all USB transactions and then disconnect */
	_delay_us(1000);

	/* Disconnect from the host - USB interface will be reset later along with the AVR */
	USB_Detach();

	/* Unlock the forced application start mode of the bootloader if it is restarted */
	MagicBootKey = MAGIC_BOOT_KEY;

	/* Enable the watchdog and force a timeout to reset the AVR */
	wdt_enable(WDTO_250MS);

	for (;;);
}

/** Configures all hardware required for the bootloader. */
static void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Relocate the interrupt vector table to the bootloader section */
	MCUCR = (1 << IVCE);
	MCUCR = (1 << IVSEL);

	/* Initialize the USB and other board hardware drivers */
	USB_Init();
	LEDs_Init();

	/* Bootloader active LED toggle timer initialization */
	TIMSK1 = (1 << TOIE1);
	TCCR1B = ((1 << CS11) | (1 << CS10));
}

/** Task to read in AVR109 commands from the CDC data OUT endpoint, process them, perform the required actions
 *  and send the appropriate response back to the host.
 */
static void CDC_Task(void)
{
	/* Select the OUT endpoint */
	Endpoint_SelectEndpoint(CDC_RX_EPADDR);

	/* Check if endpoint has a command in it sent from the host */
	if (!(Endpoint_IsOUTReceived()))
	  return;

	/* Reset the timer */
	ResetBootloaderTimer();

	/* LED feedback */
	LEDs_ToggleLEDs(LEDS_LED1 | LEDS_LED2);

	/* Read in the bootloader command (first byte sent from host) */
	uint8_t Command = CDC_FetchNextCommandByte();

	if (Command == AVR109_COMMAND_ExitBootloader)
	{
		/* Exipre the timer */
		ExpireBootloaderTimer();

		/* Send confirmation byte back to the host */
		CDC_WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_SelectDeviceType)
	{
		/* Just flush this byte... */
		CDC_FetchNextCommandByte();

		/* Send confirmation byte back to the host */
		CDC_WriteNextResponseByte('\r');
	}
	else if ((Command == AVR109_COMMAND_EnterProgrammingMode) || (Command == AVR109_COMMAND_LeaveProgrammingMode))
	{
		/* Send confirmation byte back to the host */
		CDC_WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_ReadPartCode)
	{
		/* Return ATMEGA128 part code - this is only to allow AVRProg to use the bootloader */
		CDC_WriteNextResponseByte(0x44);
		CDC_WriteNextResponseByte(0x00);
	}
	else if (Command == AVR109_COMMAND_ReadAutoAddressIncrement)
	{
		/* Indicate auto-address increment is supported */
		CDC_WriteNextResponseByte('Y');
	}
	else if (Command == AVR109_COMMAND_SetCurrentAddress)
	{
		/* Set the current address to that given by the host (translate 16-bit word address to byte address) */
		CurrAddress   = (CDC_FetchNextCommandByte() << 9);
		CurrAddress  |= (CDC_FetchNextCommandByte() << 1);

		/* Send confirmation byte back to the host */
		CDC_WriteNextResponseByte('\r');
	}
	else if (Command == AVR109_COMMAND_ReadBootloaderInterface)
	{
		/* Indicate serial programmer back to the host */
		CDC_WriteNextResponseByte('S');
	}
	else if (Command == AVR109_COMMAND_ReadBootloaderIdentifier)
	{
		/* Write the 7-byte software identifier to the endpoint */
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[0]);
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[1]);
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[2]);
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[3]);
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[4]);
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[5]);
		CDC_WriteNextResponseByte(SOFTWARE_IDENTIFIER[6]);

	}
	else if (Command == AVR109_COMMAND_ReadBootloaderSWVersion)
	{
		CDC_WriteNextResponseByte('0' + BOOTLOADER_VERSION_MAJOR);
		CDC_WriteNextResponseByte('0' + BOOTLOADER_VERSION_MINOR);
	}
	else if (Command == AVR109_COMMAND_ReadSignature)
	{
		CDC_WriteNextResponseByte(AVR_SIGNATURE_3);
		CDC_WriteNextResponseByte(AVR_SIGNATURE_2);
		CDC_WriteNextResponseByte(AVR_SIGNATURE_1);
	}
	else if (Command == AVR109_COMMAND_GetBlockWriteSupport)
	{
		CDC_WriteNextResponseByte('Y');

		/* Send block size to the host */
		CDC_WriteNextResponseByte(SPM_PAGESIZE >> 8);
		CDC_WriteNextResponseByte(SPM_PAGESIZE & 0xFF);
	}
	else if ((Command == AVR109_COMMAND_BlockWrite) || (Command == AVR109_COMMAND_BlockRead))
	{
		/* The first 2 bytes are are the size of the page, but as we already know
		 * that from SPM_PAGESIZE, just skip them.
		 */
		CDC_FetchNextCommandByte();
		CDC_FetchNextCommandByte();

		/* The next byte is the memory type (Flash vs EEPROM), but as we just support
		 * FLASH, we can ignore that byte too */
		CDC_FetchNextCommandByte();

		/* For clarity, handle the write/read commands in their dedicated methods */
		if(Command == AVR109_COMMAND_BlockWrite)
		{
			CDC_WriteMemoryBlock();
			/* Send response byte back to the host */
			CDC_WriteNextResponseByte('\r');
		}
		else
		{
			CDC_ReadMemoryBlock();
		}
	}
	else if (Command != AVR109_COMMAND_Sync)
	{
		/* Unknown (non-sync) command, return fail code */
		CDC_WriteNextResponseByte('?');
	}

	/* Terminate the task */
	CDC_Conclude();
}

/** Writes a block of memory (assumes it is a complete page)
 */
static void CDC_WriteMemoryBlock(void)
{
	/* Fill the page buffer */
	FirmwarePageIndex = 0;
	for (uint8_t i = 0; i < SPM_PAGESIZE; i++) {
		PushFirmwareByte(CDC_FetchNextCommandByte());
	}
}

/** Reads a block of memory (assumes it is a complete page)
 */
static void CDC_ReadMemoryBlock(void)
{
	boot_rww_enable();
	for (uint8_t i = 0; i < SPM_PAGESIZE; i++) {
		uint16_t address = CurrAddress + i;

		#if (FLASHEND > 0xFFFF)
			CDC_WriteNextResponseByte(pgm_read_byte_far(address));
			//CDC_WriteNextResponseByte(pgm_read_byte_far(address + 1));
		#else
			CDC_WriteNextResponseByte(pgm_read_byte(address));
			//CDC_WriteNextResponseByte(pgm_read_byte(address + 1));
		#endif
	}
}

/** Retrieves the next byte from the host in the CDC data OUT endpoint, and clears the endpoint bank if needed
 *  to allow reception of the next data packet from the host.
 *
 *  \return Next received byte from the host in the CDC data OUT endpoint
 */
static uint8_t CDC_FetchNextCommandByte(void)
{
	/* Select the OUT endpoint so that the next data byte can be read */
	Endpoint_SelectEndpoint(CDC_RX_EPADDR);

	/* If OUT endpoint empty, clear it and wait for the next packet from the host */
	while (!(Endpoint_IsReadWriteAllowed()))
	{
		Endpoint_ClearOUT();

		while (!(Endpoint_IsOUTReceived()))
		{
			if (USB_DeviceState == DEVICE_STATE_Unattached)
			  return 0;
		}
	}

	/* Fetch the next byte from the OUT endpoint */
	return Endpoint_Read_8();
}

/** Writes the next response byte to the CDC data IN endpoint, and sends the endpoint back if needed to free up the
 *  bank when full ready for the next byte in the packet to the host.
 *
 *  \param[in] Response  Next response byte to send to the host
 */
static void CDC_WriteNextResponseByte(const uint8_t Response)
{
	/* Select the IN endpoint so that the next data byte can be written */
	Endpoint_SelectEndpoint(CDC_TX_EPADDR);

	/* If IN endpoint full, clear it and wait until ready for the next packet to the host */
	if (!(Endpoint_IsReadWriteAllowed()))
	{
		Endpoint_ClearIN();

		if(CDC_WaitDelivery())
			return;
	}

	/* Write the next byte to the IN endpoint */
	Endpoint_Write_8(Response);
}

/* Wait until the CDC data has been sent to the host */
static bool CDC_WaitDelivery(void)
{
	while (!(Endpoint_IsINReady()))
	{
		if (USB_DeviceState == DEVICE_STATE_Unattached)
		  return true;
	}
	return false;
}

/** Terminates a CDC task (use it to send acknowledgments to host) */
static void CDC_Conclude(void)
{
	/* Select the IN endpoint */
	Endpoint_SelectEndpoint(CDC_TX_EPADDR);

	/* Send the endpoint data to the host */
	Endpoint_ClearIN();

	/* If a full endpoint's worth of data was sent, we need to send an empty packet afterwards to signal end of transfer */
	if (!(Endpoint_IsReadWriteAllowed()))
	{
		if(CDC_WaitDelivery())
			return;

		Endpoint_ClearIN();
	}

	/* Wait until the data has been sent to the host */
	if(CDC_WaitDelivery())
		return;

	/* Select the OUT endpoint */
	Endpoint_SelectEndpoint(CDC_RX_EPADDR);

	/* Acknowledge the command from the host */
	Endpoint_ClearOUT();
}

/** Task to read in MIDI commands from MIDI_Device_ReceiveEventPacket, process them, perform the required actions
 *  and send the appropriate response back to the host.
 */
static void MIDI_Task(void)
{
	/* Check if there is any events sent from the host */
	MIDI_EventPacket_t MIDIEvent;
	if (!MIDI_Device_ReceiveEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent))
		return;

	/* Reset the timer */
	ResetBootloaderTimer();

	/* Toggle LEDs for feedback */
	LEDs_ToggleLEDs(LEDS_LED1 | LEDS_LED2);

	/* Parse the message */
	uint8_t Command = (MIDIEvent.Data1 - 0x80) >> 2;
	uint8_t  Byte1 = ((MIDIEvent.Data1 & 0x3) << 6) | (MIDIEvent.Data2 >> 1);
	uint8_t  Byte2 = ((MIDIEvent.Data2 & 0x1) << 7) |  MIDIEvent.Data3;

	/* Detect the start of the Sysex message */
	if(Command == MIDI_COMMAND_StartFirmware)
	{
		FirmwarePageIndex = 0;
		CurrAddress = 0;
	}
	else if(Command == MIDI_COMMAND_WriteFirmwareWord)
	{
		PushFirmwareByte(Byte1);
		PushFirmwareByte(Byte2);
	}
	else if(Command == MIDI_COMMAND_ExitBootloader)
	{
		/* Exipre the timer */
		ExpireBootloaderTimer();
	}
	else if(Command == MIDI_COMMAND_Sync)
	{
		/** Send the acknowledgment to the host. **/
		MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);
		MIDI_Device_Flush(&Keyboard_MIDI_Interface);
	}
}

/** Pushes a flash byte into a buffer. When the buffer is full, the page will be
 * written to the memory.
 */
static void PushFirmwareByte(uint8_t FirmwareByte)
{
		/* Add the byte to the page buffer and increase the buffer index */
		FirmwarePageBuffer[FirmwarePageIndex] = FirmwareByte;
		FirmwarePageIndex++;

		/* Once the buffer is full, write the page */
		if(FirmwarePageIndex == SPM_PAGESIZE)
		{
			/* Erase whatever is currently on the page  */
			boot_page_erase(CurrAddress);
			/* Wait until removal operation has completed */
			boot_spm_busy_wait();

			/* Loop through the page buffer and fill the new page words */
			for (uint8_t i = 0; i < SPM_PAGESIZE; i+= 2) {
				/* The words are Little Endian, so watch out for the order of the bytes */
				uint16_t word = (FirmwarePageBuffer[i+1] << 8) | FirmwarePageBuffer[i];
				boot_page_fill(CurrAddress + i, word);
			}

			/* Commit the flash page to memory */
			boot_page_write(CurrAddress);
			/* Wait until write operation has completed */
			boot_spm_busy_wait();

			/* Reenable RWW-section again. We need this if we want to jump back to the application after bootloading.*/
			boot_rww_enable ();

			/* Advance the CurrAddress a whole page */
			CurrAddress += SPM_PAGESIZE;

			/* Reset the buffer index */
			FirmwarePageIndex = 0;
		}
}

/** Event handler for the USB_ConfigurationChanged event. This configures the device's endpoints ready
 *  to relay data to and from the attached USB host.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	/* Setup CDC Notification, Rx and Tx Endpoints */
	Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT,
							   CDC_NOTIFICATION_EPSIZE, 1);

	Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);

	Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);

	MIDI_Device_ConfigureEndpoints(&Keyboard_MIDI_Interface);
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{

	/* Ignore any requests that aren't directed to the CDC interface */
	if ((USB_ControlRequest.bmRequestType & (CONTROL_REQTYPE_TYPE | CONTROL_REQTYPE_RECIPIENT)) !=
		(REQTYPE_CLASS | REQREC_INTERFACE))
	{
		return;
	}

	/* Process the control requests */
	switch (USB_ControlRequest.bRequest)
	{
		/* Process CDC specific control requests */
		case CDC_REQ_GetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearOUT();
			}

			break;
		case CDC_REQ_SetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearIN();
			}

			break;
		case CDC_REQ_SetControlLineState:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
			}

			break;
		/* Default to a MIDI control requests */
		default:
			MIDI_Device_ProcessControlRequest(&Keyboard_MIDI_Interface);
	}
}

/** ISR to periodically toggle the LEDs on the board to indicate that the bootloader is active,
 * and to increase and check if the bootloader timeout has expired */
ISR(TIMER1_OVF_vect, ISR_BLOCK)
{
	LEDs_ToggleLEDs(LEDS_LED1 | LEDS_LED2);


	if(pgm_read_word_near(0) != 0xFFFF)
	{
		BootloaderTimer--;
	}
}