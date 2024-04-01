/* kernel.c - the C part of the kernel
 * vim:ts=4 noexpandtab
 */

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "debug.h"
#include "tests.h"

#include "idt_linkage.h"
#include "handler.h"
#include "rtc.h"
#include "kb.h"
#include "paging.h"


#define RUN_TESTS

/* Macros. */
/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags, bit)   ((flags) & (1 << (bit)))

void set_idt();

/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */
void entry(unsigned long magic, unsigned long addr) {

    multiboot_info_t *mbi;

    /* Clear the screen. */
    clear();

    /* Am I booted by a Multiboot-compliant boot loader? */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number: 0x%#x\n", (unsigned)magic);
        return;
    }

    /* Set MBI to the address of the Multiboot information structure. */
    mbi = (multiboot_info_t *) addr;

    /* Print out the flags. */
    printf("flags = 0x%#x\n", (unsigned)mbi->flags);

    /* Are mem_* valid? */
    if (CHECK_FLAG(mbi->flags, 0))
        printf("mem_lower = %uKB, mem_upper = %uKB\n", (unsigned)mbi->mem_lower, (unsigned)mbi->mem_upper);

    /* Is boot_device valid? */
    if (CHECK_FLAG(mbi->flags, 1))
        printf("boot_device = 0x%#x\n", (unsigned)mbi->boot_device);

    /* Is the command line passed? */
    if (CHECK_FLAG(mbi->flags, 2))
        printf("cmdline = %s\n", (char *)mbi->cmdline);

    if (CHECK_FLAG(mbi->flags, 3)) {
        int mod_count = 0;
        int i;
        module_t* mod = (module_t*)mbi->mods_addr;
        while (mod_count < mbi->mods_count) {
            printf("Module %d loaded at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_start);
            printf("Module %d ends at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_end);
            printf("First few bytes of module:\n");
            for (i = 0; i < 16; i++) {
                printf("0x%x ", *((char*)(mod->mod_start+i)));
            }
            printf("\n");
            mod_count++;
            mod++;
        }
    }
    /* Bits 4 and 5 are mutually exclusive! */
    if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
        printf("Both bits 4 and 5 are set.\n");
        return;
    }

    /* Is the section header table of ELF valid? */
    if (CHECK_FLAG(mbi->flags, 5)) {
        elf_section_header_table_t *elf_sec = &(mbi->elf_sec);
        printf("elf_sec: num = %u, size = 0x%#x, addr = 0x%#x, shndx = 0x%#x\n",
                (unsigned)elf_sec->num, (unsigned)elf_sec->size,
                (unsigned)elf_sec->addr, (unsigned)elf_sec->shndx);
    }

    /* Are mmap_* valid? */
    if (CHECK_FLAG(mbi->flags, 6)) {
        memory_map_t *mmap;
        printf("mmap_addr = 0x%#x, mmap_length = 0x%x\n",
                (unsigned)mbi->mmap_addr, (unsigned)mbi->mmap_length);
        for (mmap = (memory_map_t *)mbi->mmap_addr;
                (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (memory_map_t *)((unsigned long)mmap + mmap->size + sizeof (mmap->size)))
            printf("    size = 0x%x, base_addr = 0x%#x%#x\n    type = 0x%x,  length    = 0x%#x%#x\n",
                    (unsigned)mmap->size,
                    (unsigned)mmap->base_addr_high,
                    (unsigned)mmap->base_addr_low,
                    (unsigned)mmap->type,
                    (unsigned)mmap->length_high,
                    (unsigned)mmap->length_low);
    }

    /* Construct an LDT entry in the GDT */
    {
        seg_desc_t the_ldt_desc;
        the_ldt_desc.granularity = 0x0;
        the_ldt_desc.opsize      = 0x1;
        the_ldt_desc.reserved    = 0x0;
        the_ldt_desc.avail       = 0x0;
        the_ldt_desc.present     = 0x1;
        the_ldt_desc.dpl         = 0x0;
        the_ldt_desc.sys         = 0x0;
        the_ldt_desc.type        = 0x2;

        SET_LDT_PARAMS(the_ldt_desc, &ldt, ldt_size);
        ldt_desc_ptr = the_ldt_desc;
        lldt(KERNEL_LDT);
    }

    /* Construct a TSS entry in the GDT */
    {
        seg_desc_t the_tss_desc;
        the_tss_desc.granularity   = 0x0;
        the_tss_desc.opsize        = 0x0;
        the_tss_desc.reserved      = 0x0;
        the_tss_desc.avail         = 0x0;
        the_tss_desc.seg_lim_19_16 = TSS_SIZE & 0x000F0000;
        the_tss_desc.present       = 0x1;
        the_tss_desc.dpl           = 0x0;
        the_tss_desc.sys           = 0x0;
        the_tss_desc.type          = 0x9;
        the_tss_desc.seg_lim_15_00 = TSS_SIZE & 0x0000FFFF;

        SET_TSS_PARAMS(the_tss_desc, &tss, tss_size);

        tss_desc_ptr = the_tss_desc;

        tss.ldt_segment_selector = KERNEL_LDT;
        tss.ss0 = KERNEL_DS;
        tss.esp0 = 0x800000;
        ltr(KERNEL_TSS);
    }
    // setting idt entries 
    set_idt();

    /* Init the PIC */
    i8259_init();
    kb_init();
    // rtc_init();
    // paging_init();

    /* Initialize devices, memory, filesystem, enable device interrupts on the
     * PIC, any other initialization stuff... */

    /* Enable interrupts */
    /* Do not enable the following until after you have set up your
     * IDT correctly otherwise QEMU will triple fault and simple close
     * without showing you any output */
    // printf("Enabling Interrupts\n");
    // sti();

#ifdef RUN_TESTS
    /* Run tests */
    launch_tests();
#endif
    /* Execute the first program ("shell") ... */

    /* Spin (nicely, so we don't chew up cycles) */
    asm volatile (".1: hlt; jmp .1;");
}

void set_idt()
{
    // Initialize IDT handler pointers
    // 0x00-0x1f
    SET_IDT_ENTRY(idt[0], &exception_00);
    SET_IDT_ENTRY(idt[1], &exception_01);
    SET_IDT_ENTRY(idt[2], &exception_02);
    SET_IDT_ENTRY(idt[3], &exception_03);
    SET_IDT_ENTRY(idt[4], &exception_04);
    SET_IDT_ENTRY(idt[5], &exception_05);
    SET_IDT_ENTRY(idt[6], &exception_06);
    SET_IDT_ENTRY(idt[7], &exception_07);
    SET_IDT_ENTRY(idt[8], &exception_08);
    SET_IDT_ENTRY(idt[9], &exception_09);
    SET_IDT_ENTRY(idt[10], &exception_0a);
    SET_IDT_ENTRY(idt[11], &exception_0b);
    SET_IDT_ENTRY(idt[12], &exception_0c);
    SET_IDT_ENTRY(idt[13], &exception_0d);
    SET_IDT_ENTRY(idt[14], &exception_0e);
    SET_IDT_ENTRY(idt[15], &exception_0f);
    SET_IDT_ENTRY(idt[16], &exception_10);
    SET_IDT_ENTRY(idt[17], &exception_11);
    SET_IDT_ENTRY(idt[18], &exception_12);
    SET_IDT_ENTRY(idt[19], &exception_13);
    SET_IDT_ENTRY(idt[20], &exception_14);
    SET_IDT_ENTRY(idt[21], &exception_15);
    SET_IDT_ENTRY(idt[22], &exception_16);
    SET_IDT_ENTRY(idt[23], &exception_17);
    SET_IDT_ENTRY(idt[24], &exception_18);
    SET_IDT_ENTRY(idt[25], &exception_19);
    SET_IDT_ENTRY(idt[26], &exception_1a);
    SET_IDT_ENTRY(idt[27], &exception_1b);
    SET_IDT_ENTRY(idt[28], &exception_1c);
    SET_IDT_ENTRY(idt[29], &exception_1d);
    SET_IDT_ENTRY(idt[30], &exception_1e);
    SET_IDT_ENTRY(idt[31], &exception_1f);
    // 0x20-0x2f
    SET_IDT_ENTRY(idt[32], &pic_20);
    SET_IDT_ENTRY(idt[33], &pic_21);
    SET_IDT_ENTRY(idt[34], &pic_22);
    SET_IDT_ENTRY(idt[35], &pic_23);
    SET_IDT_ENTRY(idt[36], &pic_24);
    SET_IDT_ENTRY(idt[37], &pic_25);
    SET_IDT_ENTRY(idt[38], &pic_26);
    SET_IDT_ENTRY(idt[39], &pic_27);

    SET_IDT_ENTRY(idt[40], &pic_28);
    SET_IDT_ENTRY(idt[41], &pic_29);
    SET_IDT_ENTRY(idt[42], &pic_2a);
    SET_IDT_ENTRY(idt[43], &pic_2b);
    SET_IDT_ENTRY(idt[44], &pic_2c);
    SET_IDT_ENTRY(idt[45], &pic_2d);
    SET_IDT_ENTRY(idt[46], &pic_2e);
    SET_IDT_ENTRY(idt[47], &pic_2f);
    // 0x80
    SET_IDT_ENTRY(idt[128], &idt_80);
    // else
    int i;
    for(i=48;i<256;i++){
         if(i == 128){
              continue;
         }
         SET_IDT_ENTRY(idt[i], idt_else);
    }

    // set up P to 1 to enable, dpl = 0 except for syscall
    for(i=0;i<256;i++){
         idt[i].seg_selector = KERNEL_CS;    // seg = kernel code
         // reserved
         idt[i].reserved0 = 0;
         idt[i].reserved1 = 1;
         idt[i].reserved2 = 1;
         idt[i].reserved3 = 0;
         idt[i].reserved4 = 0;

         idt[i].size = 1;          // seg = kernel code
         //idt[i].present = 1;       // p

         // dpl
         if(i != 0x80){
              idt[i].dpl = 0;      // hard interrupt and exception kernel
         }
         else{
              idt[i].dpl = 3;      // syscall user
         }

    }

    for (i=0;i<32;i++){
        idt[i].present = 1;       // p
    }

    idt[0x20].present = 1;       // p
    idt[0x21].present = 1;       // p
    idt[0x22].present = 1;       // p
    idt[0x28].present = 1;       // p
    idt[0x80].present = 1;       // p
    return;
}
