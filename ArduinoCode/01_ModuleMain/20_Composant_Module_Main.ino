/*
 * Ce module gère tous les composants du main
 */
#include "09_Constantes.h"

#include <Keypad.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <SPI.h>
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#include <LiquidCrystal_PCF8574.h>
#include "Adafruit_LEDBackpack.h"

#define CMM_7SEG_addr 0x74
#define OLED_RESET 11
#define CMM_OLED_addr 0x3c

Adafruit_7segment matrix = Adafruit_7segment();

MicroOLED oled(OLED_RESET, CMM_OLED_addr);

const byte ROWS = 4; 
const byte COLS = 3; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {PAV_7, PAV_6, PAV_5, PAV_4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {PAV_3, PAV_2, PAV_1}; //connect to the column pinouts of the keypad

int valeur_AMB = 255;
// False décroissant true croissant
bool sens_AMB = false;
int pas_AMB = 1;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

// Variables du temps affiche
int g_sec0 = 0;
int g_sec1 = 0;
int g_min0 = 0;
int g_min1 = 0;

// Variable du module calcul
int g_opGauche = 0;
int g_opDroite = 0;

// Valeur OVB
int g_ovb = 0;

void CMM_showTime(int p_sec0, int p_sec1, int p_min0, int p_min1) 
{
  matrix.clear();
  
  matrix.writeDigitNum(4, p_sec0, true);
  matrix.writeDigitNum(3, p_sec1, true);
  matrix.drawColon(true);
  matrix.writeDigitNum(1, p_min0, true);
  matrix.writeDigitNum(0, p_min1, true);
  
  matrix.writeDisplay();
}

void CMM_showErrors(int p_nbErrors) 
{
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.setFontType(0); // set font type 0, please see declaration in SFE_MicroOLED.cpp
  oled.setCursor(10, 0);
  oled.print("ERREURS");
  
  oled.setFontType(1);
  oled.setCursor(23, 20);
  oled.print(p_nbErrors);
  oled.display(); // display the memory buffer drawn
}

void CMM_showWin() 
{
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer

  oled.setFontType(1);
  oled.setCursor(10, 0);
  oled.print("GAGNE");
  oled.display(); // display the memory buffer drawn
}

 
void CMM_setup()
{
  // Buzzer
  pinMode(BUZ, OUTPUT);

  // LED Ambiance
  pinMode(AMB_1, OUTPUT);
  pinMode(AMB_2, OUTPUT);
  digitalWrite(AMB_1, HIGH);
  digitalWrite(AMB_2, HIGH);
  //analogWrite(AMB_1,254);
  //analogWrite(AMB_2,254);
  // Capot
  pinMode(OVB, INPUT_PULLUP);
  g_ovb = !digitalRead(OVB);

  // Afficheur Temps AdaFruit
  matrix.begin(CMM_7SEG_addr);
  matrix.setBrightness(15);
  matrix.clear();
  matrix.drawColon(true);
  matrix.writeDisplay();
  
  // Afficheur oled
  oled.begin();     // Initialize the OLED
  CMM_showErrors(0);
}


int i = 0;
void CMM_check_state()
{
  /*
  // Gestion des LEDs d'environnement
  if(sens_AMB == false)
  {
    // On est dans le sens décroissant
    valeur_AMB -= pas_AMB;
    if (valeur_AMB == 0)
    {
      sens_AMB = true;
    }
  }
  else
  {
    // Cas croissant
    valeur_AMB += pas_AMB;
    if (valeur_AMB == 255)
    {
      sens_AMB = false;
    }    
  }
  analogWrite(AMB_1,valeur_AMB);
  analogWrite(AMB_2,valeur_AMB);
  Serial.println(valeur_AMB);
  */
  
  // Cas du capteur capot
  int curVal_Ovb = !digitalRead(OVB);
  
  if (g_ovb != curVal_Ovb)
  {
    if( curVal_Ovb )
    {
      Serial_Write("009", "CMM;OVB;1");
    }
    else
    {
      Serial_Write("009", "CMM;OVB;0");
    }
  }
  g_ovb = curVal_Ovb;
  
  // Cas du pave num
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    char l_message[9];
    sprintf(l_message, "CMM;PAV;%c", customKey);
    Serial_Write("009", l_message);
  }
}


void CMM_Change_State(char* p_ids, int p_nbIds, char* p_valeurs, int p_nbVals)
{
  int rangeVal = 0;
  char id[4];
  
  for( int i = 0; i <= p_nbIds - 3; i += 3 )
  {
    id[0] = p_ids[i];
    id[1] = p_ids[i+1];
    id[2] = p_ids[i+2];
    id[3] = '\0';

    if( strcmp(id, "BUZ") == 0 )
    {
      digitalWrite(BUZ, LOW);
      if( p_valeurs[rangeVal] == '0' )
      {
        int duration = 60000;  // 1 minute
        tone(BUZ, NOTE_C5, duration);
      }
      else if( p_valeurs[rangeVal] == '1' )
      {
        int duration = 1000;  // miliseconds
        tone(BUZ, NOTE_B0, duration);
      }
      else
      {
        int duration = 250;  // miliseconds
        tone(BUZ,NOTE_B0, duration);
        delay(250);
        tone(BUZ,NOTE_B2, duration);
      }
    }
    else if( strcmp(id, "TK0") == 0 )
    {
      g_sec0 = (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp(id, "TK1") == 0 )
    {
      g_sec1 = (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp(id, "TM0") == 0 )
    {
      g_min0 = (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp(id, "TM1") == 0 )
    {
      g_min1 = (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp(id, "RRR") == 0 )
    {
      CMM_showErrors(p_valeurs[rangeVal] - 48);
    }
    else if( strcmp(id, "WWW") == 0 )
    {
      CMM_showWin();
    }

    CMM_showTime(g_sec0, g_sec1, g_min0, g_min1);
    
    ++rangeVal;
  }
}

void CMM_Read(char* p_message)
{
  bool l_reading = true;

  int l_readCptId = 0;
  int l_readCptVal = 0;
  char l_ids[100];
  char l_valeurs[100];

  // On doit savoir si on est en train de lire l'id (true) ou la valeur (false) 
  // On commence avec l'id
  bool l_isReadId = true;
  
  // Les 3 premiers caracteres du message total representent l'ID + 1 pour le ";"
  int i = 4;
  while( l_reading )
  {
    char l_char = p_message[i];    
    
    if (l_char == ';')
    {
      // Changement id/valeur

      // Si au moment ou on arrive ici (on est sur un ;) et qu'on lisait l'id alors on lit la valeur
      if ( l_isReadId )
      {
        l_isReadId = false;
      }
      // Si au moment ou on arrive ici (on est sur un ;) et qu'on lisait la valeur, on retourne lire le prochain id
      else
      {
        l_isReadId = true;
      }
    }
    else if ( l_char == '\0' )
    {
      // Fin du message, on traite le tout...
      CMM_Change_State(l_ids, l_readCptId, l_valeurs, l_readCptVal);
      l_reading = false;
    }
    else
    {
      // Lecture du message...
      if ( l_isReadId )
      {
        l_ids[l_readCptId] = l_char;
        ++l_readCptId;
      }
      else
      {
        l_valeurs[l_readCptVal] = l_char;
        ++l_readCptVal;
      }
    }
    
    ++i;
  } 
}

