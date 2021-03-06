void
make_prog_command(char *command, char *old_serial)
{
    char serial[SERIAL_LENGTH + 1];
    make_serial(serial);

    strcpy(command, PROG_PATH " --manufacturer Submodule --product GB01 --new-serial-number SUB");
    strcat(command, serial);
    if (old_serial) {
        strcat(command, " --old-serial-number ");
        strcat(command, old_serial);
    }
}


void
select_and_program_device_posix(struct sp_port **port_list)
{
    if (!port_list) {
        printf("There was an issue searching for devices. Please double-check that your GB01 is connected.\n");
        printf("If you're sure the GB01 is connected, please contact us at support@submodule.co.\n");
        exit(1);
    }

    if (!port_list[0]) {
        printf("There don't seem to be any serial devices connected. Please double-check that your GB01 is connected.\n");
        printf("If you're sure the GB01 is connected, please contact us at support@submodule.co.\n");
        exit(1);
    }

    int n_ports = 0;
    for (int idx = 0; port_list[idx] != NULL; idx++) {
        n_ports++;
        struct sp_port *port = port_list[idx];
        char *port_name = sp_get_port_name(port);
        char *port_product = sp_get_port_usb_product(port);
        char *port_serial = sp_get_port_usb_serial(port);
        char *port_manuf = sp_get_port_usb_manufacturer(port);
        int vid = -1;
        int pid = -1;
        sp_get_port_usb_vid_pid(port, &vid, &pid);
        printf("%d: %s (manufacturer: %s) (product: %s) (serial: %s) (vid: %x) (pid: %x)\n",
            idx, port_name, port_manuf, port_product, port_serial, vid, pid);
    }

    printf(
        "\n"
        "Please type the number in front of your GB01 (0, 1, 2, ...) from the list above then press enter.\n"
    );
    #if !defined(PLATFORM_MACOS)
        printf(
            "The GB01 should have VID 403 and PID 6001.\n"
        );
    #endif
    printf(
        "If you're not sure which one the GB01 is, try unplugging other devices until you can narrow it down.\n"
        "If you don't see your GB01, make sure it's connected, exit this program (or type \"exit\"), and restart the program.\n"
        "If you still can't find it, exit this program (or type \"exit\") and contact us at support@submodule.co.\n"
        "\n"
    );

    printf("GB01 device > ");
    char line[1024];
    fgets(line, 1024, stdin);
    int selected_port = atoi(line);
    printf("\n");

    if ((selected_port == 0 && line[0] != '0') || selected_port < 0 || selected_port >= n_ports) {
        printf("No location chosen, exiting.\n");
        exit(1);
    }

    char *old_serial = sp_get_port_usb_serial(port_list[selected_port]);

    if (!old_serial) {
        printf(
            "WARNING: We couldn't get the device's old serial number. This tends to happen on macOS.\n"
            "Programming will probably still work, but just to be safe, please try to disconnect any other\n"
            "serial devices you recognise in the list above. When you've done so, press <Enter>.\n"
        );
        fgets(line, 1024, stdin);
    }

    char command[1024];
    make_prog_command(command, old_serial);
    printf("%s\n", command);
    int command_status = system(command);
    printf("\n");

    if (command_status != 0) {
        printf("There was an issue writing the new data to the device.\n");
        printf("If you get a message saying ???inappropriate permissions on device??? above,\n");
        printf("make sure you're running this program with sudo.\n");
        printf("Please contact us at support@submodule.co.\n");
        exit(1);
    }

    printf("The FTDI data has successfully been written to your device.\n");
    printf("We will now rewrite the device's firmware to make sure everything works correctly.\n");
    printf("\n");
    printf("Before we can do this, please disconnect and reconnect your GB01, then press <Enter>.\n");
    fgets(line, 1024, stdin);
}
