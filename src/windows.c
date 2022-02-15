void
make_xml(char *buf)
{
    char serial[SERIAL_LENGTH + 1];
    make_serial(serial);

    strcpy(buf, XML_PART_1);
    strcat(buf, serial);
    strcat(buf, XML_PART_2);
}


void
make_ft_xml()
{
    char xml[4096];
    make_xml(xml);
    FILE *f = fopen(FT_XML_PATH, "w");
    if (f == NULL) {
        printf("Could not open " FT_XML_PATH ", make sure you have sufficient permissions.");
        exit(1);
    }
    fputs(xml, f);
    fclose(f);
}


void
make_ft_scan_command(char *command)
{
    strcpy(command, FT_PATH " scan");
}


void
make_ft_write_command(char *command, int idx)
{
    char idx_as_str[32];
    sprintf(idx_as_str, "%d", idx);

    strcpy(command, FT_PATH " scan prog ");
    strcat(command, idx_as_str);
    strcat(command, " " FT_XML_PATH);
}


void
select_and_program_device_windows()
{
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
        printf("There was an issue searching for devices. Please double-check that your GB01 is connected.\n");
        printf("If you're sure the GB01 is connected, please contact us at support@submodule.co.\n");
        exit(1):
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
    char line[1024];
    fgets(line, 1024, stdin);
    int selected_port = atoi(line);
    printf("\n");

    if ((selected_port == 0 && line[0] != '0') || selected_port < 0 || selected_port >= n_ports) {
        printf("No location chosen, exiting.\n");
        exit(1);
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
        exit(1);
    }

    printf("The FTDI data has successfully been written to your device.\n");
    printf("We will now rewrite the device's firmware to make sure everything works correctly.\n");
    printf("\n");
}
