/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"
#define MASK_ALL 0xFF 

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
// void i8259_init(void) 
// {
//     outb(MASK_ALL, MASTER_8259_PORT+1);
//     outb(MASK_ALL, SLAVE_8259_PORT+1); 

//     outb(ICW1, MASTER_8259_PORT);
//     outb(ICW2_MASTER, MASTER_8259_PORT+1);
//     outb(ICW3_MASTER, MASTER_8259_PORT+1);
//     outb(ICW4,MASTER_8259_PORT+1);

//     outb(ICW1, SLAVE_8259_PORT);
//     outb(ICW2_SLAVE, SLAVE_8259_PORT+1);
//     outb(ICW3_SLAVE, SLAVE_8259_PORT+1);
//     outb(ICW4, SLAVE_8259_PORT+1);

//     enable_irq(0x02);

//     return;
// }


void i8259_init(void) {
     master_mask = inb(MASTER_8259_PORT+1);
     slave_mask = inb(SLAVE_8259_PORT+1);
     // mask PICs
     outb(MASK_ALL, MASTER_8259_PORT+1);
     outb(MASK_ALL, SLAVE_8259_PORT+1);
     // initialize PICs
     outb(ICW1, MASTER_8259_PORT);
     outb(ICW2_MASTER, MASTER_8259_PORT+1);
     outb(ICW3_MASTER, MASTER_8259_PORT+1);

     outb(ICW1, SLAVE_8259_PORT);
     outb(ICW2_SLAVE, SLAVE_8259_PORT+1);
     outb(ICW3_SLAVE, SLAVE_8259_PORT+1);
     outb(ICW4, SLAVE_8259_PORT+1);
     // restore
     outb(MASK_ALL, MASTER_8259_PORT+1);
     outb(MASK_ALL, SLAVE_8259_PORT+1);

}

/* Enable (unmask) the specified IRQ */
// void enable_irq(uint32_t irq_num) 
// {
//     uint8_t temp_mask = 0xFE; 
//     if((irq_num<0x00) || (irq_num > 0x0f))
//     {
//         return;
//     }

//     if(irq_num < 0x08)
//     {
//         master_mask = inb(MASTER_8259_PORT+1);
//         master_mask = (temp_mask<<irq_num) & master_mask;
//         outb( master_mask ,MASTER_8259_PORT+1);
//     }
//     else 
//     {
//         slave_mask = inb(SLAVE_8259_PORT+1);
//         slave_mask = (temp_mask<< (irq_num-0x08)) & master_mask;
//         outb(slave_mask, SLAVE_8259_PORT+1 );
//     }

// }


void enable_irq(uint32_t irq_num) {
     uint8_t value_8;
     uint8_t mask;
     // check whether interrupt request line numner is valid
     if((irq_num < 0) || (irq_num > 0x0f)) return;
     // check master or slave
     // if master
     if(irq_num < 0x08){
          mask = inb(MASTER_8259_PORT+1);
          value_8 = (~(1 << irq_num)) & mask;
          outb(value_8, MASTER_8259_PORT+1);
     }
     // i slave
     else{
          mask = inb(SLAVE_8259_PORT+1);
          value_8 = (~(1 << (irq_num - 0x08))) & mask;
          outb(value_8, SLAVE_8259_PORT+1);
     }
}

/* Disable (mask) the specified IRQ */
// void disable_irq(uint32_t irq_num) 
// {
//     uint8_t temp_mask = 0x01;
//     if((irq_num<0x00) || (irq_num > 0x0f))
//     {
//         return;
//     }

//     if(irq_num < 0x08)
//     {
//         master_mask = inb(MASTER_8259_PORT+1);
//         master_mask = master_mask | (temp_mask<<irq_num);
//         outb(master_mask, MASTER_8259_PORT +1 );
//     }
//     else
//     {
//         master_mask = inb(SLAVE_8259_PORT+1);
//         master_mask = master_mask | (temp_mask<<(irq_num - 0x08));
//         outb(master_mask, SLAVE_8259_PORT +1);
//     }
// }

void disable_irq(uint32_t irq_num) {
     uint32_t value;
     uint8_t value_8;
     // check whether interrupt request line numner is valid
     if((irq_num < 0) || (irq_num > 0x0f)) return;
     // check master or slave
     // if master
     if(irq_num < 0x08){
          value = inb(MASTER_8259_PORT+1);
          value_8 = value | (1 << irq_num);
          outb(value_8, MASTER_8259_PORT+1);
     }
     //if slave
     else{
          value = inb(SLAVE_8259_PORT +1);
          value_8 = value | (1 << (irq_num - 0x08));
          outb(value_8, SLAVE_8259_PORT +1);
     }
}



/* Send end-of-interrupt signal for the specified IRQ */
// void send_eoi(uint32_t irq_num) 
// {
//     if((irq_num<0x00) || (irq_num > 0x0f))
//     {
//         return;
//     }

//     if(irq_num < 0x08)
//     {
//         master_mask = irq_num | EOI;
//         outb(master_mask, MASTER_8259_PORT);
//     }
//     else
//     {
//         slave_mask = (irq_num - 0x08) | EOI;
//         outb(slave_mask, SLAVE_8259_PORT);
//         outb(EOI | 0x02, MASTER_8259_PORT);
//     }

// }


void send_eoi(uint32_t irq_num) {
     uint8_t value;
     // check master or slave
     if(irq_num < 0x08){
          value = irq_num | EOI;
          outb(value, MASTER_8259_PORT);
     }
     // if slave, send eoi to both master and slave
     else{
          value = (irq_num - 0x08) | EOI;
          outb(value, SLAVE_8259_PORT);
          outb(EOI|0x02,MASTER_8259_PORT);
     }
}

