/*
 * tab:4
 *
 * kb.c - functions needed for keyboard
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
 * Filename:      kb.c
 * History:
 *    CH    1    Sat Mar 19 2022
 *        First written.
 */
#include "kb.h"
#include "lib.h"
#include "i8259.h"


/* 
 * kb_init
 *   DESCRIPTION: Initialize Keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Set up initialization for Keyboard
 */
void kb_init(){
    enable_irq(0x01);  //allow for interrupts
}

/* 
 * kb_handler
 *   DESCRIPTION: Handle Keyboard strokes and print onto console
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Set up initialization for Keyboard
 */
void kb_handler(){
    cli();
    char numbers[10] = {
        '0','1','2','3','4','5','6','7','8','9'
    };
    char letter_1[10] = {
        'q','w','e','r','t','y','u','i','o','p'
    };
    char letter_2[9] = {
        'a','s','d','f','g','h','j','k','l'
    };
    char letter_3[7] = {
        'z','x','c','v','b','n','m'
    };

    uint8_t note = inb(0x60);   //0x60 is the port for keyboard
    if(note>=0x02 && note<=0x0B){
        putc(numbers[(int)(note-0x01)%10]);
    }else if(note >= 0x10 && note <= 0x19){
        putc(letter_1[note-0x10]);
    }else if(note >= 0x2C && note <= 0x32){
        putc(letter_3[note-0x2C]);
    }else if(note >= 0x1E && note <= 0x26){
        putc(letter_2[note - 0x1E]);
    }

    send_eoi(1);
    sti();
}