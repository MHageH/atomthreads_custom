/*
 * Copyright (c) 2015, Tido Klaassen. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>

#include "atomport.h"

/**
 * Set up USART2/USART3
 */
static void usart_setup(uint32_t baud){
    rcc_periph_clock_enable (RCC_GPIOA);
    //rcc_periph_clock_enable (RCC_GPIOD);
   // rcc_periph_clock_enable(RCC_AFIO);

    rcc_periph_clock_enable(RCC_USART2);
    
    /*
    rcc_periph_clock_enable (RCC_USART3);
    usart_disable(USART3);

   // nvic_enable_irq (NVIC_USART3_IRQ);  //uart interrupt WARNING : Conflicts with the thread functionnality, removed for now
    gpio_mode_setup (GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);  //GPD8 : Tx send from STM32 to ext
    gpio_mode_setup (GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);  //GPD9 : Rx recieve from ext to STM32
    gpio_set_output_options (GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO9);
    gpio_set_af (GPIOD, GPIO_AF7, GPIO8);
    gpio_set_af (GPIOD, GPIO_AF7, GPIO9);
    // Setup USART3 parameters. 
    usart_set_baudrate (USART3, baud);
    usart_set_databits (USART3, 8);
    usart_set_stopbits (USART3, USART_STOPBITS_1);
    usart_set_mode (USART3, USART_MODE_TX_RX);
    usart_set_parity (USART3, USART_PARITY_NONE);
    usart_set_flow_control (USART3, USART_FLOWCONTROL_NONE);
    // Enable USART3 Receive interrupt. 
   // usart_enable_rx_interrupt (USART3);
    // Finally enable the USART. 
    usart_enable (USART3);
    */
    

    // USART2 for Debugging
     usart_disable(USART2);
    // nvic_enable_irq (NVIC_USART2_IRQ); //uart interrupt
     gpio_mode_setup (GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);  //GPA2 : Tx send from STM32 to ext //GPA3 : Rx recieve from ext to STM32
     //gpio_set_output_options (GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);
     gpio_set_af (GPIOA, GPIO_AF7, GPIO2);
     //gpio_set_af (GPIOA, GPIO_AF7, GPIO3);
     // Setup USART3 parameters. 
     usart_set_baudrate (USART2, baud);
     usart_set_databits (USART2, 8);
     usart_set_stopbits (USART2, USART_STOPBITS_1);
     usart_set_mode (USART2, USART_MODE_TX_RX);
     usart_set_parity (USART2, USART_PARITY_NONE);
     usart_set_flow_control (USART2, USART_FLOWCONTROL_NONE);
     
     // Enable USART3 Receive interrupt. 
     //usart_enable_rx_interrupt (USART2); // Not necessary
     // Finally enable the USART. 
     usart_enable (USART2);
    }

/**
 * initialise and start SysTick counter. This will trigger the
 * sys_tick_handler() periodically once interrupts have been enabled
 * by archFirstThreadRestore()
 */
static void systick_setup(void){

    // MOD :: Orig
    //systick_set_frequency(SYSTEM_TICKS_PER_SEC, 16000000);

    // MOD :: Test
        systick_set_reload(16000000/SYSTEM_TICKS_PER_SEC);
        systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    //
        
    systick_interrupt_enable();
    systick_counter_enable();
    }

/**
 * Set up the core clock to something other than the internal 16MHz PIOSC.
 * Make sure that you use the same clock frequency in  systick_setup().
 */
static void clock_setup(void){
    // Using nucleo-f411re example :
    //Clock setting for stm32f4 is currently broken in libopencm3.
     //* Leave at internal 16MHz
    
    /* set core clock to 168Mhz, generated from external 8MHz crystal */
    //rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_84MHZ]);
    //rcc_clock_setup_in_hse_8mhz_out_72mhz();
    }

/**
 * Set up user LED and provide function for toggling it. This is for
 * use by the test suite programs
 */
static void test_led_setup(void){
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable (RCC_GPIOD);

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    /* LED is connected to GPIO6 on port A */
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);

    gpio_set(GPIOA, GPIO6);

    // Global check if everything works
      gpio_set(GPIOD, GPIO12);
    }

void test_led_toggle(void){
    // This is used to check if the main program (tests-main.c by default) works
    gpio_toggle(GPIOD, GPIO12);
    gpio_toggle(GPIOA, GPIO6);
}

/**
 * Callback from your main program to set up the board's hardware before
 * the kernel is started.
 */
int board_setup(void){
    /* Disable interrupts. This makes sure that the sys_tick_handler will
     * not be called before the first thread has been started.
     * Interrupts will be enabled by archFirstThreadRestore().
     */
    cm_mask_interrupts(true);

    /* configure system clock, user LED and UART */
    clock_setup();
    test_led_setup();
    usart_setup(57600); // MODED :: Orig : 115200 

    /* initialise SysTick counter */
    systick_setup();

    /* Set exception priority levels. Make PendSv the lowest priority and
     * SysTick the second to lowest
     */
    nvic_set_priority(NVIC_PENDSV_IRQ, 0xFF);
    nvic_set_priority(NVIC_SYSTICK_IRQ, 0xFE);

    return 0;
}

