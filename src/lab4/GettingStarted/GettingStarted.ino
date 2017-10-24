/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios.
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting
 * with the serial monitor and sending a 'T'.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <VGAGraphics.h>

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


VGAGraphics lcd;
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000010LL, 0x0000000011LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();
  lcd.init();
   int lwidth = 3;
  int bwidth = (lcd.width()-5*lwidth)/5;
  int idx = bwidth/2;
  int llength = (bwidth+lwidth)*4;

  int y_off = (lcd.height()-llength)/2;

  lcd.clear();
  lcd.fillRect(0, y_off, lcd.width(), (lcd.height()-2*y_off), WHITE);
  for (int i=0; i < 5; i++) {
    lcd.fillRect(idx, y_off, lwidth, llength, BLACK);
    idx += lwidth+bwidth;
  }

  int idy = bwidth/2;
   for (int i=0; i < 4; i++) {
    lcd.fillRect(0, y_off+idy, lcd.width(), lwidth, BLACK);
    idy += lwidth+bwidth;
  }

  lcd.drawRect(0, y_off, lcd.width(),llength , RED);
  draw_update(0, 4); 
  while(1);

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

unsigned char maze[4][5] =
{
  0, 0, 0, 0, 
  0, 0, 0, 0, 
  0, 0, 0, 0, 
  0, 0, 0, 0,
  0, 0, 0, 0
};
unsigned char new_data;
//0-3 represents different states such as visited
//unsigned char x_coord = 4;
//unsigned char y_coord = 5;
//unsigned char pos_data = 1;


void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //

  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    //unsigned long time = millis();
    unsigned char data;

    unsigned char test_x = 2;
    unsigned char test_y = 2;
    unsigned char pos_state = 1;

    data = test_x << 5 | test_y << 2 | pos_state;
    //printf("Now sending %lu...",time);
    printf("Now sending position and state data.\n");
    //bool ok = radio.write( &time, sizeof(unsigned long) );
    bool ok = radio.write( &data, sizeof(data) );

    if (ok)
      printf("ok...sent payload %d, suck it trebek ",data);
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &got_time, sizeof(unsigned long) );

      // Spew it
      printf("Got response %lu, round-trip delay: %lu\n\r",got_time,millis()-got_time);
    }

    // Try again 1s later
    delay(1000);
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      unsigned char got_maze[5][5];
      bool done = false;
//      while (!done)
//      {
//        // Fetch the payload, and see if this was the last one.
//        //done = radio.read( &got_time, sizeof(unsigned long) );
//        done = radio.read( got_maze, sizeof(got_maze) );
//
//        // Print maze
//        for (int i=0; i < 5; i++) {
//          for (int j=0; j < 5; j++) {
//            printf("%d ", maze[i][j]);
//          }
//          printf("\n");
//        }
//
//        // Spew it
//        //printf("Got payload %lu...",got_time);
//
//        // Delay just a little bit to let the other unit
//        // make the transition to receiver
//        delay(20);
//
//      }

      unsigned char bot_data;
      //int upd;
      int pos_x;
      int pos_y;
      int stat;
      //bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &bot_data, sizeof(unsigned char) );
      
        // Print the received data as a decimal
        printf("Got payload %d... suck it trebek \n",bot_data);

        stat = bot_data & 0b11;
        pos_x = bot_data>>5 & 0b11;
        pos_y = bot_data>>2 & 0b111;

        maze[pos_x][pos_y] = stat; 

         draw_update(pos_x, pos_y); 
        for (int i=0; i < 4; i++) {
          for (int j=0; j < 5; j++) {
            printf("%d ", maze[i][j]);
          }
          printf("\n");
        }
        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);
      
      }
      
      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &got_time, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }

  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
    }
  }
}

void draw_update(uint8_t x, uint8_t y) {
    int lwidth = 3;
  int bwidth = (lcd.width()-5*lwidth)/5;
  int idx = bwidth/2;
  int llength = (bwidth+lwidth)*4;

  int y_off = (lcd.height()-llength)/2;

//y 0-5, x 0-4
  int ypos = y_off + bwidth/2 + (bwidth+lwidth)*x;
  int xpos = bwidth/2 +  (bwidth+lwidth)*y;
  lcd.fillCircle(xpos, ypos, 10, 0x6969);
}

