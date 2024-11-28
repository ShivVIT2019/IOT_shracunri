#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <dk_buttons_and_leds.h>

typedef enum {
    STATE_LOCKED,
    STATE_UNLOCKED,
    STATE_ALERT
} system_state_t;

static system_state_t current_state = STATE_LOCKED;
const struct device *uart_dev;

void uart_init(void) {
    uart_dev = device_get_binding(DT_LABEL(DT_NODELABEL(uart0)));
    if (!uart_dev) {
        printk("UART device not found!\n");
    } else {
        printk("UART initialized.\n");
    }
}

void send_state_to_lora(system_state_t state) {
    char *msg;
    switch (state) {
        case STATE_LOCKED:
            msg = "STATE:LOCKED\n";
            break;
        case STATE_UNLOCKED:
            msg = "STATE:UNLOCKED\n";
            break;
        case STATE_ALERT:
            msg = "STATE:ALERT\n";
            break;
    }

    for (size_t i = 0; msg[i] != '\0'; i++) {
        uart_poll_out(uart_dev, msg[i]);
    }
    printk("Sent state: %s", msg);
}

void handle_state() {
    switch (current_state) {
        case STATE_LOCKED:
            dk_set_led_on(DK_LED1);
            dk_set_led_off(DK_LED2);
            send_state_to_lora(STATE_LOCKED);
            break;
        case STATE_UNLOCKED:
            dk_set_led_on(DK_LED2);
            dk_set_led_off(DK_LED1);
            send_state_to_lora(STATE_UNLOCKED);
            break;
        case STATE_ALERT:
            dk_set_led_on(DK_LED1);
            dk_set_led_on(DK_LED2);
            send_state_to_lora(STATE_ALERT);
            k_sleep(K_MSEC(500));
            dk_set_led_off(DK_LED1);
            dk_set_led_off(DK_LED2);
            k_sleep(K_MSEC(500));
            break;
    }
}

void main(void) {
    printk("Starting Smart Bicycle Theft Prevention System\n");

    if (dk_leds_init() < 0) {
        printk("Failed to initialize LEDs\n");
        return;
    }

    uart_init();

    while (1) {
        handle_state();
        k_sleep(K_MSEC(100));
    }
}
