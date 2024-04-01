// #include "paging.h"
// #include "lib.h"

// void paging_init(void){
//     int i;
//     for(i=0; i<PDSIZE;i++){
//         page_directory[i]=0;
//         page_table[i]=i*fourkb;
//     }
//     page_directory[0]=(uint32_t)&page_table[0];
//     page_directory[1]=KERNEL | PS_OFFSET;
//     loadPageDirectory(page_directory);
//     enable_4mb_paging();
//     enablePaging();
// }