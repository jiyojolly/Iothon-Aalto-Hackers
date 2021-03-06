/*
   IOTHonArduinoNB1500 sketch

   This sketch is rapid modification from SerialSARAPassthrough (easier to debug LTE-M).

   The sketch allows you to send basic AT commands from the USB CDC serial port
   of the MKR NB 1500 board to the onboard ublox SARA-R410 celluar module.

   If you want to add more commands the list of supported AT commands are found here:
   https://www.u-blox.com/sites/default/files/u-blox-CEL_ATCommands_%28UBX-13002752%29.pdf

   Circuit:
   - MKR NB 1500 board
   - Antenna
   - 1500 mAh or higher lipo battery connected recommended
   - Telia's Global IoT SIM card

   Make sure the Serial Monitor's line ending is set to "Both NL and CR"

   Telia's bands are 3 (city areas) + 20 (rural areas) for NB-IoT and LTE-M.

   APN=lpwa.telia.iot
   MCC/MNC=24491

   NB-IoT supports UDP protocol (IPv4)
   LTE-M supports UDP and TCP protocol (IPv4)

   LTE-M has been enabled on selected area @Otaniemi + five other test areas.

   Telia's NB-IoT coverage maps are found here ->
      Sweden: https://www.telia.se/foretag/support/tackningskartor
      Finland: https://www.telia.fi/asiakastuki/verkko/verkko/verkkokartta
      Norway: https://telia.no/dekning/
      Denmark: https://www.telia.dk/privat/hjalp/dakning-og-drift/iot-dakningskort/

  If you want to learn more about LTE-M nw capabilities GSMA's LTE-M Deployment Guide is found here ->
      https://www.gsma.com/iot/ltem-deployment-guide-v2/

  If you want to learn more about NB-IoT nw capabilities GSMA's NB-IoT Deployment Guide is found here ->
      https://www.gsma.com/iot/nbiot-deployment-guide-v2/


  Ublox has two different test services, both on UDP and TCP protocols:
      Echo – a simple service that returns whatever is being sent to it
      Daytime – a service that returns the current time at the server The services available follow the Internet standards for echo (RFC 862) and daytime (RFC 867).
      More information here https://www.u-blox.com/sites/default/files/products/documents/TestServerForSocketOperations_AppNote_%28UBX-14005690%29.pdf

  Echo service address (ublox)
      Server name: echo.u-blox.com
      Port: 7
      Protocols: TCP, UDP, IPv4


  Daytime service configuration (ublox)
      Server name: echo.u-blox.com
      Port: 13
      Protocols: TCP, UDP, IPv4


   create 30 April 2019
   Tomi Sarajisto
*/

// libraries
#include <MKRNB.h>

#define DEBUG_STREAM Serial
#define MODEM_STREAM SerialSARA


// baud rate used for both Serial ports
unsigned long baud = 115200;
char command;

void setup() {
  // enable the POW_ON pin
  pinMode(SARA_PWR_ON, OUTPUT);
  digitalWrite(SARA_PWR_ON, HIGH);

  // reset the ublox module
  pinMode(SARA_RESETN, OUTPUT);
  digitalWrite(SARA_RESETN, HIGH);
  delay(100);
  digitalWrite(SARA_RESETN, LOW);

  DEBUG_STREAM.begin(baud);
  MODEM_STREAM.begin(baud);
}

//// initialize the library instance
//NB nbAccess = true;     // include a 'true' parameter to enable debugging
//NBScanner scannerNetworks;
//NBModem modemTest;

void loop() {


  if (DEBUG_STREAM.available()) {
    command = DEBUG_STREAM.read();
    switch (command)
    {

      case '0':
        DEBUG_STREAM.println("0: AT command");
        MODEM_STREAM.println("AT");
        break;
      case '1':
        DEBUG_STREAM.println("1: Check IMEI");
        MODEM_STREAM.println("AT+GSN");
        break;
      case '2':
        DEBUG_STREAM.println("2: Check IMSI");
        MODEM_STREAM.println("AT+CIMI");
        break;
      case '3':
        DEBUG_STREAM.println("3: Check RAT type config");
        MODEM_STREAM.println("AT+URAT?");
        break;
      case '4':
        DEBUG_STREAM.println("4: Check band configuration");
        MODEM_STREAM.println("AT+UBANDMASK?");
        break;
      case '5':
        DEBUG_STREAM.println("5: Check signal");
        MODEM_STREAM.println("AT+CSQ");
        break;
      case '6':
        DEBUG_STREAM.println("6: Check APN configuration");
        MODEM_STREAM.println("AT+CGDCONT?");
        break;
      case '7':
        DEBUG_STREAM.println("7: Check registration status packet core");
        MODEM_STREAM.println("AT+CREG?");
        break;
      case '8':
        DEBUG_STREAM.println("8: Check Radio status");
        MODEM_STREAM.println("AT+CFUN?");
        break;
      case '9':
        DEBUG_STREAM.println("9: Display current configuration");
        MODEM_STREAM.println("AT&V");
        break;


      //Set basic settings
      case 'a':
        DEBUG_STREAM.println("a: Set verbose error resultcodes");
        MODEM_STREAM.println("AT+CMEE=2");
        break;

      case 'b':
        DEBUG_STREAM.println("b: Set Radio off");
        MODEM_STREAM.println("AT+CFUN=0");
        break;

      case 'c':
        DEBUG_STREAM.println("c: Set RAT = NB-IoT and LTE-M");
        MODEM_STREAM.println("AT+URAT=8,7");
        break;

      case 'd':
        DEBUG_STREAM.println("d: Set band configurations=3+20 ");
        MODEM_STREAM.println("AT+UBANDMASK=0,132");    //
        break;

      case 'e':
        DEBUG_STREAM.println("e: Set band configurations=3+20 ");
        MODEM_STREAM.println("AT+UBANDMASK=1,132");
        break;

      case 'f':
        DEBUG_STREAM.println("f: Select network");
        MODEM_STREAM.println("AT+COPS=1,2,\"24491\"\n");        //Telia Finland
        //MODEM_STREAM.println("");
        break;

      case 'g':
        DEBUG_STREAM.println("g: Set Radio on");
        MODEM_STREAM.println("AT+CFUN=1");
        break;

      case 'h':
        DEBUG_STREAM.println("h: Empty");
        MODEM_STREAM.println("AT");
        break;

      // Network

      case 'i':
        DEBUG_STREAM.println("i: Define PDP context #1");
        MODEM_STREAM.println("AT+CGDCONT=1,\"IP\",\"lpwa.telia.iot\"");
        break;
      case 'j':
        DEBUG_STREAM.println("j: Activate PDP context #1");
        MODEM_STREAM.println("AT+CGACT=1,1");
        break;
      case 'k':
        DEBUG_STREAM.println("k: Show address of the PDP contexts");
        MODEM_STREAM.println("AT+CGPADDR");
        break;
      case 'l':
        DEBUG_STREAM.println("l: Open UDP socket");
        MODEM_STREAM.println("AT+USOCR=17");
        break;
      case 'm':
        DEBUG_STREAM.println("m: Send UDP packet to echo server");
        MODEM_STREAM.println("AT+USOST=0,\"echo.u-blox.com\",7,16,\"16 bytes of data\"");
        break;
      case 'n':
        DEBUG_STREAM.println("n: Read UDP packets from echo server");
        MODEM_STREAM.println("AT+USORF=0,26");
        break;
      case 'o':
        DEBUG_STREAM.println("o: Send UDP packet to echo server");
        MODEM_STREAM.println("AT+USOST=0,\"echo.u-blox.com\",13,16,\"16 bytes of data\"");
        break;
      case 'p':
        DEBUG_STREAM.println("p: Empty");
        MODEM_STREAM.println("AT");
        break;

      // generic AT commands
      case 'q':
        DEBUG_STREAM.println("q: Set to factory defined configuration");
        MODEM_STREAM.println("AT&F");
        break;
      case 'r':
        DEBUG_STREAM.println("r: Reboot the module and save the settings to nvram ");
        MODEM_STREAM.println("AT+CFUN=15");
        break;
      case 's':
        DEBUG_STREAM.println("s: Set RAT = NB-IoT only");
        MODEM_STREAM.println("AT+URAT=8");
        break;
      case 't':
        DEBUG_STREAM.println("t: Set RAT = LTE-M only");
        MODEM_STREAM.println("AT+URAT=7");
        break;
      case 'u':
        DEBUG_STREAM.println("u:Register to the packet core");
        MODEM_STREAM.println("AT+CREG=1");
        break;
      case 'v':
        DEBUG_STREAM.println("v: Un-register from the packet core");
        MODEM_STREAM.println("AT+CREG=0");
        break;
      case 'w':
        DEBUG_STREAM.println("W: Save NVM parameters, without reset of the SIM card ");
        MODEM_STREAM.println("AT+CFUN=15");
        break;


      // Continue here after o (before PDP context is closed)

      case 'x':
        DEBUG_STREAM.println("x: Create TCP socket");
        MODEM_STREAM.println("AT+USOCR=6");
        break;
      case 'y':
        DEBUG_STREAM.println("y: Connect to server");
        MODEM_STREAM.println("AT+USOCO=1,\"195.34.89.241\",7");
        break;
      case 'z':
        DEBUG_STREAM.println("z: Receive greeting message");
        MODEM_STREAM.println("AT+USORD=1,32");
        break;
      case '!':
        DEBUG_STREAM.println("!: Write 4 characters");
        MODEM_STREAM.println("AT+USOWR=1,4,\"Venki is a good boy\"");
        break;
      case '?':
        DEBUG_STREAM.println("?: Read 4 echoed characters");
        MODEM_STREAM.println("AT+USORD=1,4");
        break;
    }
  }

  if (MODEM_STREAM.available()) {
    DEBUG_STREAM.write(MODEM_STREAM.read());
  }


}
