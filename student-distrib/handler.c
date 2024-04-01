/* handler.c - contains hanlers for interrupts */

#include "lib.h"
#include "i8259.h"
#include "rtc.h"
#include "kb.h"


/* exception handlers
 *
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage:
 */
void ex00(){
     printf("Divide by 0\n");
     while(1){}
}

void ex01(){
     printf("EXCEPTION");
     while(1){}
}

void ex02(){
     printf("NMI\n");
     while(1){}
}

void ex03(){
     printf("Breakpoint\n");
     while(1){}
}

void ex04(){
     printf("Overflow\n");
     while(1){}
}

void ex05(){
     printf("EXCEPTION");
     while(1){}
}

void ex06(){
     printf("EXCEPTION");
     while(1){}
}

void ex07(){
     printf("EXCEPTION");
     while(1){}
}

void ex08(){
     printf("EXCEPTION");
     while(1){}
}

void ex09(){
     printf("EXCEPTION");
     while(1){}
}

void ex0a(){
     printf("EXCEPTION");
     while(1){}
}

void ex0b(){
     printf("Segment not present\n");
     while(1){}
}

void ex0c(){
     printf("Stack segment fault\n");
     while(1){}
}

void ex0d(){
     printf("General protection fault\n");
     // (void) halt(0);

}

void ex0e(){
     printf("Page fault\n");
     // (void) halt(0);
}

void ex0f(){
     printf("EXCEPTION");
     while(1){}
}

void ex10(){
     printf("EXCEPTION");
     while(1){}
}

void ex11(){
     printf("EXCEPTION");
     while(1){}
}

void ex12(){
     printf("EXCEPTION");
     while(1){}
}

void ex13(){
     printf("EXCEPTION");
     while(1){}
}

void ex14(){
     printf("EXCEPTION");
     while(1){}
}

void ex15(){
     printf("15");
     while(1){}
}

void ex16(){
     printf("EXCEPTION");
     while(1){}
}

void ex17(){
     printf("EXCEPTION");
     while(1){}
}

void ex18(){
     printf("EXCEPTION");
     while(1){}
}

void ex19(){
     printf("EXCEPTION");
     while(1){}
}

void ex1a(){
     printf("EXCEPTION");
     while(1){}
}

void ex1b(){
     printf("EXCEPTION");
     while(1){}
}

void ex1c(){
     printf("EXCEPTION");
     while(1){}
}

void ex1d(){
     printf("EXCEPTION");
     while(1){}
}

void ex1e(){
     printf("EXCEPTION");
     while(1){}
}

void ex1f(){
     printf("EXCEPTION");
     while(1){}
}

/* Interrupt handlers
 *
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage:
 */
void pic20(){
}
// keyboard
void pic21(){
     kb_handler();  
}

void pic22(){
}

void pic23(){
}

void pic24(){
}

void pic25(){
}

void pic26(){
}

void pic27(){
}
// RTC
void pic28(){
     rtc_handler();
}

void pic29(){
}

void pic2a(){
}

void pic2b(){
}

void pic2c(){
}

void pic2d(){
}

void pic2e(){
}

void pic2f(){
}

/* System call handler
 *
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage:
 */
void idt80(){
     printf("System call\n");
}

/* handler placeholder
 *
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage:
 */
void idtelse(){
     printf("User defined interrupts\n");
}
