
// Sample Ping-Pong for nRF24L01 radio

// Some version tracking info
#define VER_NUM "v1.1"
#define VER_DATE "2022-11-21"
#define VERSION "nRF24Radio " VER_NUM " " VER_DATE

// Required includes
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
const uint16_t INTERVAL = 2500;  // Interval in milliseconds between sends

// Choose a specific frequency and 'mailing' address
const uint64_t ADDRESS = 0xA7B7C7D7E7; // Address, default is 0xE7E7E7E7E7
const uint8_t CHANNEL = 12;            // Channel for this 'network'

int main() {
  // Show firmware version & when it was created
  printf("\n\n########################################\n\n");
  printf(VERSION "\n\n");
  printf("Compiled " __DATE__ " " __TIME__);
  printf("\n\n########################################\n\n");

  // Setup code <-> radio data structures
  uint8_t txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];
  uint8_t rxDataCnt = 0;

  // Put something in the send buffer
  txData[0] = 0x42;
  txData[1] = 66;
  txData[2] = 'B';
  txData[3] = 0b01000010;

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
  printf("Interval.    : %ums\n", INTERVAL);

  // Each STM32 has a unique 96 bit number
  uint32_t *uid = (uint32_t *)0x1FFFF7E8;
  printf("Unique Id    : 0x%X 0x%X 0x%X\n", uid[0], uid[1], uid[2]);
  // This is a useful way of identifying different radio nodes


  // Setup timer functions & variables
  Timer t;
  t.start();
  uint32_t tNow; // Technically this could be local to the while loop

  // tPrev stores last time the interval code was run
  // By initialising it as minus the interval, it means it will trigger an
  // immediate send. Not an issue when it's seconds, boring if it's minutes
  uint32_t tPrev = 0 - INTERVAL;

  // Turn on receive mode & tell the radio to get started
  theRadio.setReceiveMode();
  theRadio.enable();

  // The super loop ...
  while (true) {
    tNow = t.read_ms(); // Get current time

    // This is a non-blocking task without using a Thread
    if ((tNow - tPrev) > INTERVAL) {
      // If the difference between the last transmission & the 
      // time now is greater than the requested interval then it
      // is time to do our periodic task

      // Say we are doing it
      printf("Sending ...\n");

      // Send the txData buffer to the radio's memory
      theRadio.write(NRF24L01P_PIPE_P0, txData, TRANSFER_SIZE);

      // Set the last transmission time
      tPrev = tNow;

    } // interval - time to send


    // Check to see if anything has been received
    if (theRadio.readable()) {

      // It was, so show some info.
      printf("Heard at %ums: ", tNow);

      // Move data from the radio's memory to our rxData buffer
      rxDataCnt = theRadio.read(NRF24L01P_PIPE_P0, rxData, TRANSFER_SIZE);

      // Print the data we received in Hex
      for (int i = 0; rxDataCnt > 0; rxDataCnt--, i++) {
        printf("0x%02X ", rxData[i]);
      }
      printf("\n");

    } // radio.readable

  } // while forever / true

} // main
