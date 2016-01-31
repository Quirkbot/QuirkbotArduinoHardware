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

/** \file
 *  \brief Board hardware information driver.
 *
 *  This file is the master dispatch header file for the board-specific information driver, to give information
 *  on the hardware contained on a specific board.
 *
 *  User code should include this file, which will in turn include the correct board driver header file for the
 *  currently selected board.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/Board.h file in the user project
 *  directory.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 */

/** \ingroup Group_BoardDrivers
 *  \defgroup Group_BoardInfo Board Information Driver - LUFA/Drivers/Board/Board.h
 *  \brief Board hardware information driver.
 *
 *  \section Sec_BoardInfo_Dependencies Module Source Dependencies
 *  The following files must be built with any user project that uses this module:
 *    - None
 *
 *  @{
 */

#ifndef __BOARD_H__
#define __BOARD_H__

	/* Macros: */
		#define __INCLUDE_FROM_BOARD_H

	/* Includes: */
		#include "../../LUFA/Common/Common.h"

		#if (BOARD == BOARD_QUIRKBOT)
			#include "AVR8/QUIRKBOT/Board.h"
		#endif

#endif

/** @} */

