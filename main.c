#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libserialport.h>


#define FT_PATH "tools\\windows\\ft232r_prog\\ft232r_prog.exe"
#define AVRDUDE_PATH "tools\\windows\\avrdude\\bin\\avrdude"
#define AVRDUDE_CONF_PATH "tools/windows/avrdude/etc/avrdude.conf"
#define FIRMWARE_PATH "data/gb01-firmware.hex"
#define SERIAL_LENGTH 9 + 3


void print_serial_ports(struct sp_port **port_list) {
    for (int idx = 0; port_list[idx] != NULL; idx++) {
        struct sp_port *port = port_list[idx];
        char *port_name = sp_get_port_name(port);
        char *port_manuf = sp_get_port_usb_manufacturer(port);
        printf("  * [%d] %s (manufacturer: %s)\n", idx + 1, port_name, port_manuf);
    }
}


void make_serial(char *buf) {
    buf[0] = 'S';
    buf[1] = 'U';
    buf[2] = 'B';
    for (int idx = 3; idx < SERIAL_LENGTH; idx++) {
        buf[idx] = (rand() % 10) + '0';
    }
    buf[SERIAL_LENGTH] = '\0';
}


void make_ft_command(char *ft_command, char *serial) {
    strcpy(ft_command, FT_PATH " --manufacturer Test --product Test --new-serial-number test1234");
    /* strcpy(ft_command, FT_PATH " --manufacturer Submodule --product GB01 --new-serial-number "); */
    /* strcat(ft_command, serial); */
}


void make_avrdude_command(char *avrdude_command, char *port_name) {
    strcpy(avrdude_command, AVRDUDE_PATH " -q -V -D -C " AVRDUDE_CONF_PATH " -c arduino -p atmega32 -P ");
    strcat(avrdude_command, port_name);
    strcat(avrdude_command, " -b 115200 -U flash:w:" FIRMWARE_PATH ":i");
}


int main() {
    srand((unsigned)time(NULL));

    char serial[SERIAL_LENGTH + 1];
    make_serial(serial);
    printf("%s\n", serial);

    printf(
        "#\n"
        "# Submodule GB01 FTDI Fixer\n"
        "#\n"
        "\n"
        "This tool allows you to update your Submodule's GB01's FTDI information (and firmware).\n"
        "First of all, let's find your GB01.\n"
        "\n"
        "Currently connected serial devices: \n"
        "\n"
    );

    struct sp_port **port_list;
    if (sp_list_ports(&port_list) != SP_OK) {
        printf("Could not list serial ports! Please contact us at support@submodule.co.\n");
        goto cleanup;
    }

    int n_ports = 0;
    do {
        if (port_list[n_ports] == NULL) {
            break;
        }
        n_ports++;
    } while (true);

    print_serial_ports(port_list);

    printf(
        "\n"
        "Please type the number in front of your GB01 (1, 2, 3, ...) then press enter.\n"
        "If you don't see your GB01, make sure it's connected, close this window (or type \"exit\"), and restart the program.\n"
        "If you still can't find it, close this window (or type \"exit\") and contact us at support@submodule.co.\n"
        "\n"
    );

    printf("GB01 location > ");
    int selected_port = 0;
    scanf("%u", &selected_port);

    if (selected_port == 0 || selected_port > n_ports) {
        printf("\nNo location chosen, exiting.\n");
        goto cleanup;
    }

    char *port_name = sp_get_port_name(port_list[selected_port - 1]);

    char ft_command[1024];
    make_ft_command(ft_command, serial);

    char avrdude_command[1024];
    make_avrdude_command(avrdude_command, port_name);

    printf("%s\n", ft_command);
    system(ft_command);

    /* printf("%s\n", avrdude_command); */

cleanup:
    sp_free_port_list(port_list);
}
