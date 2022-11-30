
// Sample Ping-Pong for nRF24L01 radio

// Some version tracking info
#define VER_NUM "v1.2"
#define VER_DATE "2022-11-29"
#define VERSION "nRF24Radio " VER_NUM " " VER_DATE

// Required includes
#include "eblocks_defs.h"
#include "mbed.h"
#include "nRF24L01P.h"

// Setup the radio's physical connections
nRF24L01P theRadio(PB_5, PB_4, PB_3, PB_10, PA_8,
                   PA_9); // mosi, miso, sck, csn, ce, irq

// These are connections for the EBlocks system at hud.ac.uk
// The radio shares IO with push buttons 6 & 7
// Best not to use those switches with the radio

// Settings for this application
const uint8_t TRANSFER_SIZE = 4; // Size of send/rec buffer

// Choose a specific frequency and 'mailing' address
const uint64_t ADDRESS = 0xA7B7C7D7E7; // Address, default is 0xE7E7E7E7E7
const uint8_t CHANNEL = 12;            // Channel for this 'network'

AnalogIn TMP36(RA2);
DigitalIn SendBtn(RD0, PullDown);

int main() {
  // Show firmware version & when it was created
  printf("\n\n########################################\n\n");
  printf(VERSION "\n\n");
  printf("Compiled " __DATE__ " " __TIME__);
  printf("\n\n########################################\n\n");

  // Setup code <-> radio data structures
  uint8_t txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];
  uint8_t rxDataCnt = 0;

  // Start up the radio & configure it
  theRadio.powerUp();
  theRadio.setRfFrequency(NRF24L01P_MIN_RF_FREQUENCY + CHANNEL);
  theRadio.setRfOutputPower(NRF24L01P_TX_PWR_MINUS_18_DB);
  theRadio.setAirDataRate(NRF24L01P_DATARATE_1_MBPS);
  theRadio.setTxAddress(ADDRESS);
  theRadio.setRxAddress(ADDRESS);
  theRadio.setTransferSize(TRANSFER_SIZE);

  // Display the setup of the nRF24L01+ chip
  printf("Frequency    : %dMHz\n", theRadio.getRfFrequency());
  printf("Output power : %ddBm\n", theRadio.getRfOutputPower());
  printf("Data Rate    : %dkbps\n", theRadio.getAirDataRate());
  printf("Tx Address   : 0x%010llX\n", theRadio.getTxAddress());
  printf("Rx Address   : 0x%010llX\n", theRadio.getRxAddress());
  printf("Transfer Size: %uBytes\n", TRANSFER_SIZE);

  // Each STM32 has a unique 96 bit number
  uint32_t *uid = (uint32_t *)0x1FFFF7E8;
  printf("Unique Id    : 0x%X 0x%X 0x%X\n", uid[0], uid[1], uid[2]);
  // This is a useful way of identifying different radio nodes

  uint8_t SendBtnWasPressed = 0;

  // Turn on receive mode & tell the radio to get started
  theRadio.setReceiveMode();
  theRadio.enable();

  // The super loop ...
  while (true) {

    // If button is now pressed and wasn't previously pressed
    if ((SendBtn == 1) & (SendBtnWasPressed == 0)) {
      SendBtnWasPressed = 1; // Set the flag to say the button was pressed

      printf("\nSending ...\n"); // Say we are doing it

      // Take sensor readings
      float TemperaturePercent = TMP36;
      printf("TemperaturePercent = %f%%\n", TemperaturePercent);

      float TemperatureV = TemperaturePercent * 3.3;
      printf("TemperatureV = %fV\n", TemperatureV);

      // 10mV per degC, 750mV at 25degC = offset of 50degC
      float Temperature = (TemperatureV / 0.01) - 50.0;
      printf("Temperature = %fdegC\n", Temperature);

      // Add 200 to allow for negative temps, 
      // then multiply by 100 for 2 decimal places
      // This is a 'descartes' standard
      uint16_t Temperature2DP = (uint16_t)((Temperature + 200.0) * 100.0);
      printf("Temperature2DP = %u\n", Temperature2DP);

      // Build payload
      txData[0] = (uint8_t)((Temperature2DP >> 8) & 0xFF);
      txData[1] = (uint8_t)(Temperature2DP & 0xFF);

      // Print the data we are sending in Hex
      printf("Sent: ");
      for (uint8_t c = 0; c < TRANSFER_SIZE; c++) {
        printf("0x%02X ", txData[c]);
      }
      printf("\n");

      // Send the txData buffer to the radio's memory
      theRadio.write(NRF24L01P_PIPE_P0, txData, TRANSFER_SIZE);

      // If button is not pressed and was previously pressed
    } else if ((SendBtn == 0) & (SendBtnWasPressed == 1)) {
      SendBtnWasPressed = 0; // Reset the flag

    } // SendBtn & SendBtnWasPressed??



    // Check to see if anything has been received
    if (theRadio.readable()) {

      // It was, so show some info.
      printf("\nReceived: ");

      // Move data from the radio's memory to our rxData buffer
      rxDataCnt = theRadio.read(NRF24L01P_PIPE_P0, rxData, TRANSFER_SIZE);

      // Print the data we received in Hex
      for (int i = 0; rxDataCnt > 0; rxDataCnt--, i++) {
        printf("0x%02X ", rxData[i]);
      }
      printf("\n");

      uint16_t RemoteTemperature2DP = (rxData[0] << 8) + rxData[1];

      float RemoteTemperature = (((float)RemoteTemperature2DP) / 100.0) - 200.0;
      printf("Remote temperature = %fdegC\n", RemoteTemperature);

    } // radio.readable

  } // while forever / true

} // main

/*

      // If button is pressed and was previously pressed
    } else if ((SendBtn == 1) & (SendBtnWasPressed == 1)) {
      // Don't send

      // If button is not pressed and was not previously pressed
    } else if ((SendBtn == 0) & (SendBtnWasPressed == 0)) {
      // Nothing to do

*/