/*
 * tab:4
 *
 * rtc.c - functions needed for rtc
 *
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:        Chuxuan Hu
 * Version:       1
 * Creation Date: Sat Mar 19 2022
 * Filename:      rtc.c
 * History:
 *    CH    1    Sat Mar 19 2022
 *        First written.
 */
#include "lib.h"
#include "rtc.h"
#include "i8259.h"

/* 
 * rtc_init
 *   DESCRIPTION: Initialize RTC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Set up initialization for RTC
 */

void rtc_init(){
    //Setting Registers
    // cli();
    // outb(0x8A, 0x70);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
    // outb(0x20, 0x71);	// write to CMOS/RTC RAM
    // sti();

    //Turning on IRQ8
    //cli();
    outb(0x8B, 0x70);		// select register B, and disable NMI
    unsigned char prev = inb(0x71);	// read the current value of register B
    outb(0x8B, 0x70);		// set the index again (a read will reset the index to register D)
    outb(prev | 0x40, 0x71);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
    //sti();


    enable_irq(0x08); //should be initialized in PIC


    //Changing Interrupt Rates
    char rate = 6;    //default to maximum frequency
    rate &= 0x0F;
    cli();
    outb(0x8A, 0x70);		// set index to register A, disable NMI
    char prev_rate = inb(0x71);	// get initial value of register A
    outb(0x8A, 0x70);		// reset index to A
    outb( (prev_rate & 0xF0) | rate , 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.
    sti();

}

/* 
 * rtc_handler
 *   DESCRIPTION: Handle RTC interrupts
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Handle interrupts
 */
void rtc_handler(){

    //call test_interrupts()
    test_interrupts();

    // cli();
    outb(0x8C, 0x70);	// select register C
    inb(0x71);		// just throw away contents
    // sti();

    send_eoi(8);
}

