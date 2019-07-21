// IMPORTANT!
// DOWNLOAD the MD_Parola library
// https://github.com/MajicDesigns/MD_Parola


#include <stdio.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
//#include "Parola_Fonts_data.h"
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 8
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);



long randNumber;
int step_counter = 0;
int button_values[] = {913, 429, 267, 179, 110};
int btn_tol = 20;
int analogValue = 0;
int pin_p1 = A0;
int pin_p2 = A6;
int leds_cnt = 5;
int p1_leds[5] = {2,3,4,5,6};
int p2_leds[5] = {A2,A3,A4,A5,A1};
int p1_score = 0;
int p2_score = 0;
int action_speed = 2000;
int action_speed_min = 250;

void setup()
{
  Serial.begin(9600);

  randomSeed(analogRead(A7));
  
  pinMode(pin_p1, INPUT);
  pinMode(pin_p2, INPUT);

  for (int i = 0; leds_cnt > i; i++) {
    pinMode(p1_leds[i], OUTPUT);
  }
  for (int i = 0; leds_cnt > i; i++) {
    pinMode(p2_leds[i], OUTPUT);
  }  

  P.begin(2);
  P.setZone(0,0,3);
  P.setZone(1,4,7);
  P.displayZoneText(0, "abc", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.setZoneEffect(0, true, PA_FLIP_UD);
  P.displayZoneText(1, "abcd", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.setZoneEffect(1, true, PA_FLIP_UD);
  P.displayAnimate();

}

void loop()
{
  if(p1_score <= 100 and p2_score <= 100) {
    
    step_counter++;
    bool step_action = false;
    if (step_counter > action_speed) {
      step_counter = 0;
      step_action = true;  
      action_speed = action_speed - round(action_speed/50);
      if (action_speed < action_speed_min) {
        action_speed = action_speed_min;
      }
      Serial.println(action_speed);
    }
  
    if (step_action) {
      int pin_light = random(0,5);
      digitalWrite(p1_leds[pin_light], HIGH);
      digitalWrite(p2_leds[pin_light], HIGH);
      
    }
  
    analogValue = analogRead(pin_p1);
    for (int i = 0; leds_cnt > i; i++) {
      if ( analogValue < button_values[i] + btn_tol and analogValue > button_values[i] - btn_tol ){
        if(digitalRead(p1_leds[i]) == HIGH){
          digitalWrite(p1_leds[i], LOW);
          p1_score++;
        }
      }
    }
  
    analogValue = analogRead(pin_p2);
    for (int i = 0; leds_cnt > i; i++) {
      if ( analogValue < button_values[i] + btn_tol and analogValue > button_values[i] - btn_tol ){
        if(digitalRead(p2_leds[i]) == HIGH){
          digitalWrite(p2_leds[i], LOW);
          p2_score++;
        }
      }
    }
  
    if ( step_counter % 100 == 0){
      char Score1[80];
      sprintf(Score1, "%d", p1_score);
      char *chrdisp[] = {Score1};
  
      char Score2[80];
      sprintf(Score2, "%d", p2_score);
      char *chrdisp2[] = {Score2};
  
      P.displayZoneText(0, chrdisp[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayZoneText(1, chrdisp2[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayAnimate();
    }
  } else {
    if (p1_score > p2_score) {
      P.displayZoneText(0, "Winner", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayZoneText(1, "Looser", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    } else {
      P.displayZoneText(0, "Looser", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayZoneText(1, "Winner", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    }
    P.displayAnimate();  
    delay(500);
      char Score1[80];
      sprintf(Score1, "%d", p1_score);
      char *chrdisp[] = {Score1};
  
      char Score2[80];
      sprintf(Score2, "%d", p2_score);
      char *chrdisp2[] = {Score2};
    P.displayZoneText(0, chrdisp[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    P.displayZoneText(1, chrdisp2[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);    
    P.displayAnimate();
    delay(500);        
  }
}
