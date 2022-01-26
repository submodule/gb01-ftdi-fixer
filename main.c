#include <stdio.h>
#include <libserialport.h>


int main() {
    struct sp_port **port_list;
    if (sp_list_ports(&port_list) != SP_OK) {
        printf("sp_list_ports() failed!\n");
        return NULL;
    }

    for (unsigned idx = 0; port_list[idx] != NULL; idx++) {
        struct sp_port *port = port_list[idx];
        char *port_name = sp_get_port_name(port);
        char *port_manuf = sp_get_port_usb_manufacturer(port);
        printf("%s (manufacturer: %s)\n", port_name, port_manuf);
    }

    sp_free_port_list(port_list);
}
