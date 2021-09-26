#include "common.h"
#include "mini_uart.h"
#include "printf.h"
#include "irq.h"
#include "timer.h"
#include "i2c.h"
#include "spi.h"
#include "led_display.h"
#include "mailbox.h"
#include "video.h"

void putc(void *p, char c) {
    if (c == '\n') {
        uart_send('\r');
    }

    uart_send(c);
}

u32 get_el();


void kernel_main() {
    uart_init();
    init_printf(0, putc);
    printf("\nRasperry PI Bare Metal OS Initializing...\n");

    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();
    timer_init();

#if RPI_VERSION == 3
    printf("\tBoard: Raspberry PI 3\n");
#endif

#if RPI_VERSION == 4
    printf("\tBoard: Raspberry PI 4\n");
#endif

    printf("\nException Level: %d\n", get_el());

    printf("Sleeping 200 ms...\n");
    timer_sleep(200);


    //Do video...
    video_init();

    printf("YES DMA...\n");
    video_set_dma(true);

#if 0
    printf("Resolution 1900x1200\n");
    video_set_resolution(1900, 1200, 32);

    printf("Resolution 1024x768\n");
    video_set_resolution(1024, 768, 32);

    printf("Resolution 800x600\n");
    video_set_resolution(800, 600, 32);

    printf("Resolution 1900x1200\n");
    video_set_resolution(1900, 1200, 8);

    printf("Resolution 1024x768\n");
    video_set_resolution(1024, 768, 8);

#endif

    printf("Resolution 800x600\n");
    video_set_resolution(800, 600, 8);
    setup_vid_buffer();

    while(1) {
        // XXX u32 cur_temp = 0;

        // XXX mailbox_generic_command(RPI_FIRMWARE_GET_TEMPERATURE, 0, &cur_temp);

        // XXX printf("Cur temp: %dC MAX: %dC\n", cur_temp / 1000, max_temp / 1000);

        video_update();
        timer_sleep(250);
    }
}
