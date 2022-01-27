#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <libserialport.h>


#define FT_PATH "tools\\windows\\ft_prog\\FT_Prog-CmdLine.exe"
#define FT_XML_PATH "gb01.xml"
#define AVRDUDE_PATH "tools\\windows\\avrdude\\bin\\avrdude"
#define AVRDUDE_CONF_PATH "tools/windows/avrdude/etc/avrdude.conf"
#define FIRMWARE_PATH "data/gb01-firmware.hex"
#define SERIAL_LENGTH 9
#define XML_PART_1 \
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
    "<FT_EEPROM>\n" \
      "<Chip_Details>\n" \
        "<Type>FT232R</Type>\n" \
      "</Chip_Details>\n" \
      "<USB_Device_Descriptor>\n" \
        "<VID_PID>0</VID_PID>\n" \
        "<idVendor>0403</idVendor>\n" \
        "<idProduct>6001</idProduct>\n" \
        "<bcdUSB>USB 2.0</bcdUSB>\n" \
      "</USB_Device_Descriptor>\n" \
      "<USB_Config_Descriptor>\n" \
        "<bmAttributes>\n" \
          "<RemoteWakeupEnabled>true</RemoteWakeupEnabled>\n" \
          "<SelfPowered>false</SelfPowered>\n" \
          "<BusPowered>true</BusPowered>\n" \
        "</bmAttributes>\n" \
        "<IOpullDown>false</IOpullDown>\n" \
        "<MaxPower>90</MaxPower>\n" \
      "</USB_Config_Descriptor>\n" \
      "<USB_String_Descriptors>\n" \
        "<Manufacturer>Submodule</Manufacturer>\n" \
        "<Product_Description>GB01</Product_Description>\n" \
        "<SerialNumber_Enabled>true</SerialNumber_Enabled>\n" \
        "<SerialNumber>SUB"
#define XML_PART_2 \
            "</SerialNumber>\n" \
        "<SerialNumberPrefix>SU</SerialNumberPrefix>\n" \
        "<SerialNumber_AutoGenerate>false</SerialNumber_AutoGenerate>\n" \
      "</USB_String_Descriptors>\n" \
      "<Hardware_Specific>\n" \
        "<HighIO>false</HighIO>\n" \
        "<D2XX>false</D2XX>\n" \
        "<ExternalOscillator>false</ExternalOscillator>\n" \
        "<Invert_RS232_Signals>\n" \
          "<TXD>false</TXD>\n" \
          "<RXD>false</RXD>\n" \
          "<RTS>false</RTS>\n" \
          "<CTS>false</CTS>\n" \
          "<DTR>false</DTR>\n" \
          "<DSR>false</DSR>\n" \
          "<DCD>false</DCD>\n" \
          "<RI>false</RI>\n" \
        "</Invert_RS232_Signals>\n" \
        "<IO_Controls>\n" \
          "<C0>TXLED#</C0>\n" \
          "<C1>RXLED#</C1>\n" \
          "<C2>TXDEN</C2>\n" \
          "<C3>PWREN#</C3>\n" \
          "<C4>SLEEP#</C4>\n" \
        "</IO_Controls>\n" \
      "</Hardware_Specific>\n" \
    "</FT_EEPROM>\n"


void make_serial(char *buf) {
    for (int idx = 0; idx < SERIAL_LENGTH; idx++) {
        buf[idx] = (rand() % 10) + '0';
    }
    buf[SERIAL_LENGTH] = '\0';
}


void make_xml(char *buf) {
    char serial[SERIAL_LENGTH + 1];
    make_serial(serial);

    strcpy(buf, XML_PART_1);
    strcat(buf, serial);
    strcat(buf, XML_PART_2);
}


void make_ft_scan_command(char *command) {
    strcpy(command, FT_PATH " scan");
}


void make_ft_write_command(char *command, int idx) {
    char idx_as_str[32];
    sprintf(idx_as_str, "%d", idx);

    strcpy(command, FT_PATH " scan prog ");
    strcat(command, idx_as_str);
    strcat(command, " " FT_XML_PATH);
}


void make_avrdude_command(char *command, char *port_name) {
    strcpy(command, AVRDUDE_PATH " -q -V -D -C " AVRDUDE_CONF_PATH " -c arduino -p atmega32 -P ");
    strcat(command, port_name);
    strcat(command, " -b 115200 -U flash:w:" FIRMWARE_PATH ":i");
}


void find_gb01_port(char *gb01_port_name) {
    struct sp_port **port_list;
    if (sp_list_ports(&port_list) != SP_OK) {
        printf("sp_list_ports() failed!\n");
        return;
    }

    for (int idx = 0; port_list[idx] != NULL; idx++) {
        struct sp_port *port = port_list[idx];
        char *port_name = sp_get_port_name(port);
        char *port_manuf = sp_get_port_usb_manufacturer(port);
        if (port_manuf && strcmp(port_manuf, "Submodule") == 0) {
            printf("Found %s (manufacturer: %s), using this port\n", port_name, port_manuf);
            strcpy(gb01_port_name, port_name);
            return;
        } else {
            printf("Found %s (manufacturer: %s), ignoring\n", port_name, port_manuf);
        }
    }

    // Just clean up on exit
    /* sp_free_port_list(port_list); */
}


int main() {
    srand((unsigned)time(NULL));

    char xml[4096];
    make_xml(xml);
    FILE *f = fopen(FT_XML_PATH, "w");
    if (f == NULL) {
        printf("Could not open " FT_XML_PATH ", make sure you have sufficient permissions.");
        goto cleanup;
    }
    fputs(xml, f);
    fclose(f);

    printf(
        "#\n"
        "# Submodule GB01 FTDI Fixer\n"
        "#\n"
        "\n"
        "This tool allows you to update your Submodule's GB01's FTDI information (and firmware).\n"
        "First of all, let's find your GB01. Make sure it's connected to your computer, but that\n"
        "no game cartridge is connected to the GB01.\n"
        "\n"
    );

    char command[1024];
    int command_status = -1;
    FILE *command_pipe;
    char result_line[1024];
    make_ft_scan_command(command);
    command_pipe = _popen(command, "rt");

    if (command_pipe != NULL) {
        while (fgets(result_line, sizeof(result_line), command_pipe) != NULL) {
            // Only print lines that list devices. Why? ft_prog sometimes prints errors when actually everything works
            // fine, and only on certain systems. :)
            if (strncmp(result_line, "Device ", 7) == 0) {
                printf("%s", result_line);
            }
        }

        command_status = _pclose(command_pipe);
    }

    if (command_pipe == NULL || command_status != 0) {
        printf("There was an issue with searching for devices. Please double-check that your GB01 is connected.\n");
        printf("If you're sure the GB01 is connected, please contact us at support@submodule.co.\n");
        goto cleanup;
    }

    printf(
        "\n"
        "Please type the number in front of your GB01 (0, 1, 2, ...) from the list above then press enter.\n"
        "The GB01 will say \"FT232R\". If there's only one such device, it's probably the GB01.\n"
        "If you don't see your GB01, make sure it's connected, close this window (or type \"exit\"), and restart the program.\n"
        "If you still can't find it, close this window (or type \"exit\") and contact us at support@submodule.co.\n"
        "\n"
    );

    printf("GB01 device > ");
    int selected_port = -1;
    scanf("%d", &selected_port);
    printf("\n");

    if (selected_port == -1) {
        printf("No location chosen, exiting.\n");
        goto cleanup;
    }

    make_ft_write_command(command, selected_port);
    command_pipe = _popen(command, "rt");

    if (command_pipe != NULL) {
        while (fgets(result_line, sizeof(result_line), command_pipe) != NULL) {
            // Only print lines that list devices. Why? ft_prog sometimes prints errors when actually everything works
            // fine, and only on certain systems. :)
            if (strncmp(result_line, "Device ", 7) == 0) {
                printf("%s", result_line);
            }
        }

        command_status = _pclose(command_pipe);
    }

    printf("\n");

    if (command_pipe == NULL || command_status != 0) {
        printf("There was an issue writing the new data to the device.\n");
        printf("Please contact us at support@submodule.co.\n");
        goto cleanup;
    }


    printf("The FTDI data has successfully been written to your device.\n");
    printf("We will now rewrite the device's firmware to make sure everything works correctly.\n");
    printf("\n");

    char gb01_port_name[256] = {'\0'};
    find_gb01_port(gb01_port_name);

    if (strlen(gb01_port_name) == 0) {
        printf("There was an issue flashing the firmware, because the GB01's serial port could not be found.\n");
        printf("This might not be a problem. It might also be solved by installing/uninstalling FTDI drivers.\n");
        printf("Test your GB01, and if you're still having trouble, contact us at support@submodule.co.\n");
        goto cleanup;
    }

    make_avrdude_command(command, gb01_port_name);
    printf("%s\n", command);
    command_status = system(command);

    if (command_status != 0) {
        printf("There was an issue flashing the firmware. This might not be a problem.\n");
        printf("Test your GB01, and if you're still having trouble, contact us at support@submodule.co.\n");
    }

    printf("All data on your GB01 has been successfully updated!\n");
    printf("Your GB01 should work correctly now. If it doesn't, contact us at support@submodule.co.\n");
    printf("Please disconnect and reconnect your GB01, and restart the GB01 app.\n");
    printf("Thank you for using the GB01!\n");

cleanup:
    printf("(press any key to exit)\n");
    _getch();
}
