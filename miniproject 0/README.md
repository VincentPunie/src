# elecanisms miniproject 0
# Fien Boone + Vincent Punie
# DATE: 1/30/2020

1. COINSELECTOR SETUP

By following the procedure listed on the datasheet, we managed to initialize the coin selector.


2. BOARD SETUP

- connect the 12V voltage supply to the barrel jack
- connect the red line (DC 12V) to the Vin port of the board.
- connect the black line (GND) to the GND port of the board.
- connect the white line (COIN) to the breadboard and from the breadboard to the digital pin D0 port.
- connect a pull-up resistor (10kOhm) from the breadboard to VDD on the board.


3. CODE + CODE EXPLANATION

# include "elecanisms.h"          //import the elecansism.h libary
  int16_t main(void) {            //main method
      init_elecanisms();          //load the elecanisms libary

      while(1){                   //start the while-loop (while-loop keeps on running due to the true-statement)
        if(D0 == 1){              //checks if the coin selector detects a coin (D0 is initialized as 1),
          LED1 = ON;              //if the coin selector does NOT detects a coin (D0 == 1), LED1 is ON and LED2 is OFF
          LED2 = OFF;
        }
        else{                     //if the coin selector detects a coin (D0 == 0), LED1 turns OFF and LED2 turns ON
          LED1 = OFF;
          LED2 = ON;
        }
      }
    }


4. UPLOADING CODE TO THE BOARD

- Go to src>bootloader>software and run the bootloadergui.py
- Import the file 'cointest.hex'
- Click on 'write' and wait until the procedure is done.
- Click on 'Disconnect/Run'
- Good to go!
