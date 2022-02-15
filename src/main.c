#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || \
    defined(__NT__) || defined(__CYGWIN__) || defined(__MINGW32__)
  #define PLATFORM_WINDOWS
#elif defined(__APPLE__)
  #define PLATFORM_MACOS
  #define PLATFORM_POSIX
#elif defined(__linux__) || defined(__unix__)
  #define PLATFORM_UNIX
  #define PLATFORM_POSIX
#elif defined(__FreeBSD)
  #define PLATFORM_FREEBSD
  #define PLATFORM_POSIX
#else
  #error "Unknown platform"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <libserialport.h>

#if defined(PLATFORM_WINDOWS)
    #include "windows.h"
#endif

#if defined(PLATFORM_UNIX)
    #include "unix.h"
#endif

#if defined(PLATFORM_MACOS)
    #include "macos.h"
#endif


#define FIRMWARE_PATH "data/gb01-firmware.hex"
#define SERIAL_LENGTH 9


void
make_serial(char *buf)
{
    for (int idx = 0; idx < SERIAL_LENGTH; idx++) {
        buf[idx] = (rand() % 10) + '0';
    }
    buf[SERIAL_LENGTH] = '\0';
}


void
make_avrdude_command(char *command, char *port_name)
{
    strcpy(command, AVRDUDE_PATH " -q -V -D -C " AVRDUDE_CONF_PATH " -c arduino -p atmega32 -P ");
    strcat(command, port_name);
    strcat(command, " -b 115200 -U flash:w:" FIRMWARE_PATH ":i");
}


struct sp_port**
find_all_ports()
{
    struct sp_port **port_list;
    if (sp_list_ports(&port_list) != SP_OK) {
        printf("sp_list_ports() failed!\n");
        return NULL;
    }

    // Just clean up on exit
    /* sp_free_port_list(port_list); */

    return port_list;
}


void
find_gb01_port(char *gb01_port_name)
{
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


#if defined(PLATFORM_WINDOWS)
    #include "windows.c"
#else
    #include "posix.c"
#endif


int
main()
{
    #if !defined(PLATFORM_WINDOWS) && !defined(PLATFORM_UNIX) && !defined(PLATFORM_MACOS)
        printf("This software unfortunately doesn't support your platform. Try Windows, Linux or macOS.\n");
        printf("If you _are_ running on one of the above platforms, please let us know at support@submodule.co.\n");
        exit(1);
    #endif

    char command[1024];
    int command_status = -1;
    char gb01_port_name[256] = {'\0'};

    srand((unsigned)time(NULL));

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

    #if defined(PLATFORM_UNIX)
        printf("IMPORTANT NOTE: You will most likely need to run this program with sudo, so that it has the necessary\n");
        printf("permissions to access serial ports.\n");
        printf("\n");
    #endif

    #if defined(PLATFORM_WINDOWS)
        make_ft_xml();
        select_and_program_device_windows();
    #else
        select_and_program_device_posix(find_all_ports());
    #endif

    find_gb01_port(gb01_port_name);

    if (strlen(gb01_port_name) == 0) {
        printf("There was an issue flashing the firmware, because the GB01's serial port could not be found.\n");
        printf("This might not be a problem. It might also be solved by installing/uninstalling FTDI drivers.\n");
        printf("Test your GB01, and if you're still having trouble, contact us at support@submodule.co.\n");
        exit(1);
    }

    make_avrdude_command(command, gb01_port_name);
    printf("%s\n", command);
    command_status = system(command);

    if (command_status != 0) {
        printf("There was an issue flashing the firmware. This might not be a problem.\n");
        printf("Test your GB01, and if you're still having trouble, contact us at support@submodule.co.\n");
        #if defined(PLATFORM_UNIX)
            printf("Please note that this program currently only supports the amd64 architecture.\n");
            printf("If you need support for a different architecture, let us know.\n");
        #endif
    }

    printf("All data on your GB01 has been successfully updated!\n");
    printf("Your GB01 should work correctly now. If it doesn't, contact us at support@submodule.co.\n");
    printf("Please disconnect and reconnect your GB01, and restart the GB01 app.\n");
    printf("Thank you for using the GB01!\n");

    printf("(press <Enter> to exit)\n");
    #if defined(PLATFORM_WINDOWS)
        _getch();
    #else
        char line[1024];
        fgets(line, 1024, stdin);
    #endif
}
