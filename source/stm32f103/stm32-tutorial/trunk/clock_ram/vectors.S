        .syntax unified
        .thumb

        .section        ".vectors","ax"
hal_vsr_table:

        .long           hal_startup_stack               //  0 Reset stack
        .long           reset_vector                    //  1 Reset entry

//==========================================================================
        .text
        .thumb
        
        .align  2
	.global reset_vector
        .thumb
        .thumb_func
        .type   reset_vector, %function
reset_vector:

        ldr     sp,=hal_startup_stack
        b       main

        .pool
