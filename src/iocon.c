/*
* @brief
* This file is used to config IOCON module.
*
* @note
* Copyright(C) NXP Semiconductors, 2013
* All rights reserved.
*
* @par
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
*
* @par
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors' and its
* licensor's relevant copyrights in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
*/

#include "chip.h"    /* LPCOpen  Peripheral Registers for LPC812M101FD20 */


void IOCON_Init() {
	/* Enable IOCON clock */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);

	/* Pin I/O Configuration */
	/*LPC_IOCON->PIO0[IOCON_PIO0]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO1]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO2]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO3]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO4]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO5]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO6]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO7]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO8]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO9]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO10]= 0x80; */
	/*LPC_IOCON->PIO0[IOCON_PIO11]= 0x80; */
	/*LPC_IOCON->PIO0[IOCON_PIO12]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO13]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO14]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO15]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO16]= 0x90; */
	/*LPC_IOCON->PIO0[IOCON_PIO17]= 0x90; */
}
 /**********************************************************************
 **                            End Of File
 **********************************************************************/
