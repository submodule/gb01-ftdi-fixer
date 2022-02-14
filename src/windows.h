#include <conio.h>

#define AVRDUDE_PATH "tools\\windows\\avrdude\\bin\\avrdude"
#define AVRDUDE_CONF_PATH "tools/windows/avrdude/etc/avrdude.conf"
#define FT_PATH "tools\\windows\\ft_prog\\FT_Prog-CmdLine.exe"
#define FT_XML_PATH "gb01.xml"
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
