/*
** Copyright (c) 2018, Bradley A. Minch
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**     1. Redistributions of source code must retain the above copyright
**        notice, this list of conditions and the following disclaimer.
**     2. Redistributions in binary form must reproduce the above copyright
**        notice, this list of conditions and the following disclaimer in the
**        documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
*/
#include "ws2812b.h"
#include "elecanisms.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define EASY                                0
#define MEDIUM                              1
#define HARD                                2
#define ON_SPEED                            3
#define MISSION_IMPOSSIBLE                  4
#define DIFFICULTY_SELECTION                5
#define DONE                                6
#define FLASH_ALL_LEDS_GREEN_4_TIMES        7
#define FLASH_ALL_LEDS_RED_4_TIMES          8
#define LED_SHOW                            9
#define READY_TO_PLAY                       10

#define NUM_LEDS          48
#define CENTER_LED        21
#define DATA_PIN          0                                 //DATA pin on the Braduino we'll working with
#define LED_TYPE          WS2812b
#define COLOR_ORDER       RGB

int Startbutton = 0;
int LEDaddress = 0;
int difficulty = 1;


uint16_t state, counter;
uint8_t red[NUM_LEDS], green[NUM_LEDS], blue[NUM_LEDS];


void write_leds(void) {
  uint16_t i;

  disable_interrupts();
  for(i = 0; i < NUM_LEDS; i++) {
    ws2812b_write(red[i], green[i], blue[i]);
  }
  enable_interrupts();
}


void flash_all_leds_green() {
  int16_t i;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 0;
    green[i] = 255;
    blue[i] = 0;
  }
  write_leds();
  while(IFS0bits.T2IF == 0) {}
  IFS0bits.T2IF = 0;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 0;
    green[i] = 0;
    blue[i] = 0;
  }
  write_leds();
  while(IFS0bits.T2IF == 0) {}
  IFS0bits.T2IF = 0;
}


void flash_all_leds_red() {
  int16_t i;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 255;
    green[i] = 0;
    blue[i] = 0;
  }
  write_leds();
  while(IFS0bits.T2IF == 0) {}
  IFS0bits.T2IF = 0;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 0;
    green[i] = 0;
    blue[i] = 0;
  }
  write_leds();
  while(IFS0bits.T2IF == 0) {}
  IFS0bits.T2IF = 0;
}


void led_show() {
  int16_t i;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 0;
    green[i] = 128;
    blue[i] = 0;
  }
  write_leds();
  while(IFS0bits.T1IF == 0) {}
  IFS0bits.T1IF = 0;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 0;
    green[i] = 0;
    blue[i] = 128;
  }
  write_leds();
  while(IFS0bits.T1IF == 0) {}
  IFS0bits.T1IF = 0;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 200;
    green[i] = 200;
    blue[i] = 0;
  }
  write_leds();
  while(IFS0bits.T1IF == 0) {}
  IFS0bits.T1IF = 0;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 128;
    green[i] = 0;
    blue[i] = 128;
  }
  write_leds();
  while(IFS0bits.T1IF == 0) {}
  IFS0bits.T1IF = 0;
}


void clear_all_leds() {
  int16_t i;

  for(i = 0; i < NUM_LEDS; i++) {
    red[i] = 0;
    green[i] = 0;
    blue[i] = 0;
  }
}


int16_t main(void) {
    init_elecanisms();
    D0_OD = 1;
    init_ws2812b();

    int16_t i;

    T1CON = 0x0030;         // set Timer1 period to 0.5s
    PR1 = 0x7A11/5;        //dividing by 10 to get 50 microseconds
    TMR1 = 0;               // set Timer1 count to 0
    IFS0bits.T1IF = 0;      // lower Timer1 interrupt flag
    T1CONbits.TON = 1;      // turn on Timer1


    T2CON = 0x0030;         // set Timer2 period to 0.5s
    PR2 = 0x7A11/10;
    TMR2 = 0;               // set Timer1 count to 0
    IFS0bits.T2IF = 0;      // lower Timer1 interrupt flag
    T2CONbits.TON = 1;      // turn on Timer1


    T3CON = 0x0030;         // set Timer2 period to 0.5s
    PR3 = 0x7A11/15;
    TMR3 = 0;               // set Timer1 count to 0
    IFS0bits.T3IF = 0;      // lower Timer1 interrupt flag
    T3CONbits.TON = 1;      // turn on Timer1


    T4CON = 0x0030;         // set Timer2 period to 0.5s
    PR4 = 0x7A11/20;
    TMR4 = 0;               // set Timer1 count to 0
    IFS1bits.T4IF = 0;      // lower Timer1 interrupt flag
    T4CONbits.TON = 1;      // turn on Timer1


    T5CON = 0x0030;         // set Timer2 period to 0.5s
    PR5 = 0x7A11/25;
    TMR5 = 0;               // set Timer1 count to 0
    IFS1bits.T5IF = 0;      // lower Timer1 interrupt flag
    T5CONbits.TON = 1;      // turn on Timer1

    state = READY_TO_PLAY;

    while (1) {
        switch (state) {
            case READY_TO_PLAY:
                if(D1 == 1)  {
                  state = DIFFICULTY_SELECTION;
                }
                break;

            case DIFFICULTY_SELECTION:
                if(difficulty == 1){
                  state = EASY;
                }
                if(difficulty == 2){
                  state = MEDIUM;
                }
                if(difficulty == 3){
                  state = HARD;
                }
                if(difficulty == 4){
                  state = ON_SPEED;
                }
                if(difficulty == 5){
                  state = MISSION_IMPOSSIBLE;
                }
                break;

            case EASY:
                clear_all_leds();
                green[LEDaddress] = 128;
                red[CENTER_LED] = 128;
                write_leds();
                LEDaddress++;

                if (LEDaddress == NUM_LEDS) {
                     LEDaddress = 0;
                }

                while(IFS0bits.T1IF == 0) {}
                IFS0bits.T1IF = 0;

                if(D2 == 1) {   //pressing the playbutton
                    state = DONE;
                }
                break;

            case MEDIUM:
                clear_all_leds();
                green[LEDaddress] = 128;
                red[CENTER_LED] = 128;
                write_leds();
                LEDaddress++;

                if (LEDaddress == NUM_LEDS) {
                      LEDaddress = 0;
                }

                while(IFS0bits.T2IF == 0) {}
                IFS0bits.T2IF = 0;

                if(D2 == 1) {   //pressing the playbutton
                    state = DONE;
                }
                break;

            case HARD:
                clear_all_leds();
                green[LEDaddress] = 128;
                red[CENTER_LED] = 128;
                write_leds();
                LEDaddress++;

                if (LEDaddress == NUM_LEDS) {
                      LEDaddress = 0;
                }

                while(IFS0bits.T3IF == 0) {}
                IFS0bits.T3IF = 0;

                if(D2 == 1) {   //pressing the playbutton
                    state = DONE;
                }
                break;
            case ON_SPEED:
                clear_all_leds();
                green[LEDaddress] = 128;
                red[CENTER_LED] = 128;
                write_leds();
                LEDaddress++;

                if (LEDaddress == NUM_LEDS) {
                      LEDaddress = 0;
                }

                while(IFS1bits.T4IF == 0) {}
                IFS1bits.T4IF = 0;

                if(D2 == 1) {   //pressing the playbutton
                    state = DONE;
                }
                break;
            case MISSION_IMPOSSIBLE:
                clear_all_leds();
                green[LEDaddress] = 128;
                red[CENTER_LED] = 128;
                write_leds();
                LEDaddress++;

                if (LEDaddress == NUM_LEDS) {
                      LEDaddress = 0;
                }

                while(IFS1bits.T5IF == 0) {}
                IFS1bits.T5IF = 0;

                if(D2 == 1) {   //pressing the playbutton
                    state = DONE;
                }
                break;

            case DONE:
                clear_all_leds();
                red[CENTER_LED] = 128;
                green[LEDaddress] = 128;
                write_leds();

                int diff = abs(CENTER_LED - LEDaddress);
                if(diff == 0) {
                  if(difficulty < 5)  {
                    state = FLASH_ALL_LEDS_GREEN_4_TIMES;
                  }
                  if(difficulty == 5)  {
                    difficulty = 1;
                    state = LED_SHOW;
                  }
                  difficulty++;
                }
                else  {
                  state = FLASH_ALL_LEDS_RED_4_TIMES;
                }
                break;

            case  FLASH_ALL_LEDS_GREEN_4_TIMES:
                clear_all_leds();
                while(IFS0bits.T1IF == 0) {}
                IFS0bits.T1IF = 0;
                for(i = 0; i < 4; i++)  {
                  flash_all_leds_green();
                }
                state = DIFFICULTY_SELECTION;
                break;


            case FLASH_ALL_LEDS_RED_4_TIMES:
                clear_all_leds();
                while(IFS0bits.T1IF == 0) {}
                IFS0bits.T1IF = 0;
                for(i = 0; i < 4; i++)  {
                  flash_all_leds_red();
                }
                difficulty = 1;
                LEDaddress = 0;
                state = READY_TO_PLAY;
                break;

            case LED_SHOW:
                clear_all_leds();
                while(IFS0bits.T1IF == 0) {}
                IFS0bits.T1IF = 0;
                for(i = 0; i < 10; i++)  {
                  led_show();
                }
                state = READY_TO_PLAY;
                break;
        }
    }
}
