#include <stdio.h>
#include <stdlib.h>
#include <libserialport.h>


void print_serial_ports(struct sp_port **port_list) {
    for (unsigned idx = 0; port_list[idx] != NULL; idx++) {
        struct sp_port *port = port_list[idx];
        char *port_name = sp_get_port_name(port);
        char *port_manuf = sp_get_port_usb_manufacturer(port);
        printf("  * [%d] %s (manufacturer: %s)\n", idx + 1, port_name, port_manuf);
    }
}


int main() {
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
        exit(1);
    }
    print_serial_ports(port_list);

    printf(
        "\n"
        "Please type the number in front of your GB01 (1, 2, 3, ...) then press enter.\n"
        "If you don't see your GB01, make sure it's connected, close this window (or type \"exit\"), and restart the program.\n"
        "If you still can't find it, close this window (or type \"exit\") and contact us at support@submodule.co.\n"
        "\n"
    );

    printf("GB01 location > ");
    unsigned input_idx = 0;
    scanf("%u", &input_idx);

    if (input_idx == 0) {
        printf("\nNo location chosen, exiting.\n");
        exit(1);
    }

cleanup:
    sp_free_port_list(port_list);
}
