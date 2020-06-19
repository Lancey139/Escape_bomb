/*
 * Ce fichier gère les entrées sorties du module fils complexes
 * Ce module DOIT être cablés sur le connecteur A qui comporte 6 pins analogiques
 * Les pins femelees sur ce module sont organisées comme suit : 
 * GND Vcc
 * AN1 Potentiomètre
 * AN2 Fil complexe 1
 * AN3 Fil complexe 2
 * AN4 Fil complexe 3
 * AN5 Fil complexe 4
 * AN6 Fil complexe 5
 * DG1 LED 1
 * DG2 LED 2
 * DG3 LED 3
 * DG4 LED 4
 * DG5 LED 5
 * SCL LCD
 * SDA LCD
 */
 
#include "09_Constantes.h"

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>


// Constantes
#define Resistance_Fils_Complexe  85
#define ID_Fils_Complexe  "FCP"
#define FCP_LCD_addr 0x27 
#define Val_Fil_Rouge 1003
#define Val_Fil_Bleu 153
#define Val_Fil_Rouge_Bleu 640

#define COL_FIL_1 5
#define COL_FIL_2 8
#define COL_FIL_3 11
#define COL_FIL_4 14
#define COL_FIL_5 18

// Variables Globales
int FCP_Pin_LED1 = 0;
bool FCP_LED1 = 0;
int FCP_Pin_LED2 = 0;
bool FCP_LED2 = 0;
int FCP_Pin_LED3 = 0;
bool FCP_LED3 = 0;
int FCP_Pin_LED4 = 0;
bool FCP_LED4 = 0;
int FCP_Pin_LED5 = 0;
int FCP_Pin_Fil1 = 0;
int FCP_Valeur_Fil1 = 0;
int FCP_Pin_Fil2 = 0;
int FCP_Valeur_Fil2 = 0;
int FCP_Pin_Fil3 = 0;
int FCP_Valeur_Fil3 = 0;
int FCP_Pin_Fil4 = 0;
int FCP_Valeur_Fil4 = 0;
int FCP_Pin_Fil5 = 0;
int FCP_Valeur_Fil5 = 0;

LiquidCrystal_PCF8574 lcd(FCP_LCD_addr);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void lcd_print(char* l_message)
{
  lcd.setCursor(0, 0);
  lcd.print(l_message);
}

void FCP_print_lcd(bool fil1, bool fil2, bool fil3, bool fil4, bool fil5)
{
  lcd.begin(20, 4);
  lcd.setBacklight(255);
  lcd.home(); 
  lcd.clear();
  
  for (int i = 0; i < 5; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      switch(i)
      {
        case 1:
          if(fil1)
          {
            lcd.setCursor(COL_FIL_1, j);
            lcd.print("*");
          }
        case 2:
          if(fil2)
          {
            lcd.setCursor(COL_FIL_2, j);
            lcd.print("*");
          }
        case 3:
          if(fil3)
          {
            lcd.setCursor(COL_FIL_3, j);
            lcd.print("*");
          }
        case 4:
          if(fil4)
          {
            lcd.setCursor(COL_FIL_4, j);
            lcd.print("*");
          }
        case 5:
          if(fil5)
          {
            lcd.setCursor(COL_FIL_5, j);
            lcd.print("*");
          }
      }
    }
  }
}

void FCP_send_config()
{
  char message[9];

  int delta = 100;

  // Fil 1
  if ( (FCP_Valeur_Fil1 - delta) < Val_Fil_Bleu && Val_Fil_Bleu < (FCP_Valeur_Fil1 + delta) )
  {
    strcpy(message, "FCP;F1;2");
    Serial_Write("008", message);
  }
  else if ( (FCP_Valeur_Fil1 - delta) < Val_Fil_Rouge_Bleu && Val_Fil_Rouge_Bleu < (FCP_Valeur_Fil1 + delta) )
  {
    strcpy(message, "FCP;F1;3");
    Serial_Write("008", message);
  }
  else if ((FCP_Valeur_Fil1 - delta) < Val_Fil_Rouge && Val_Fil_Rouge < (FCP_Valeur_Fil1 + delta))
  {
    strcpy(message, "FCP;F1;1");
    Serial_Write("008", message);
  }
  else
  {
    strcpy(message, "FCP;F1;0");
    Serial_Write("008", message);
  }

  // Fil 2
  if ( (FCP_Valeur_Fil2 - delta) < Val_Fil_Bleu && Val_Fil_Bleu < (FCP_Valeur_Fil2 + delta) )
  {
    strcpy(message, "FCP;F2;2");
    Serial_Write("008", message);
  }
  else if ( (FCP_Valeur_Fil2 - delta) < Val_Fil_Rouge_Bleu && Val_Fil_Rouge_Bleu < (FCP_Valeur_Fil2 + delta) )
  {
    strcpy(message, "FCP;F2;3");
    Serial_Write("008", message);
  }
  else if ((FCP_Valeur_Fil2 - delta) < Val_Fil_Rouge && Val_Fil_Rouge < (FCP_Valeur_Fil2 + delta))
  {
    strcpy(message, "FCP;F2;1");
    Serial_Write("008", message);
  }
  else
  {
    strcpy(message, "FCP;F2;0");
    Serial_Write("008", message);
  }

  // Fil 3
  if ( (FCP_Valeur_Fil3 - delta) < Val_Fil_Bleu && Val_Fil_Bleu < (FCP_Valeur_Fil3 + delta) )
  {
    strcpy(message, "FCP;F3;2");
    Serial_Write("008", message);
  }
  else if ( (FCP_Valeur_Fil3 - delta) < Val_Fil_Rouge_Bleu && Val_Fil_Rouge_Bleu < (FCP_Valeur_Fil3 + delta) )
  {
    strcpy(message, "FCP;F3;3");
    Serial_Write("008", message);
  }
  else if ((FCP_Valeur_Fil3 - delta) < Val_Fil_Rouge && Val_Fil_Rouge < (FCP_Valeur_Fil3 + delta))
  {
    strcpy(message, "FCP;F3;1");
    Serial_Write("008", message);
  }
  else
  {
    strcpy(message, "FCP;F3;0");
    Serial_Write("008", message);
  }

  // Fil 4
  if ( (FCP_Valeur_Fil4 - delta) < Val_Fil_Bleu && Val_Fil_Bleu < (FCP_Valeur_Fil4 + delta) )
  {
    strcpy(message, "FCP;F4;2");
    Serial_Write("008", message);
  }
  else if ( (FCP_Valeur_Fil4 - delta) < Val_Fil_Rouge_Bleu && Val_Fil_Rouge_Bleu < (FCP_Valeur_Fil4 + delta) )
  {
    strcpy(message, "FCP;F4;3");
    Serial_Write("008", message);
  }
  else if ((FCP_Valeur_Fil4 - delta) < Val_Fil_Rouge && Val_Fil_Rouge < (FCP_Valeur_Fil4 + delta))
  {
    strcpy(message, "FCP;F4;1");
    Serial_Write("008", message);
  }
  else
  {
    strcpy(message, "FCP;F4;0");
    Serial_Write("008", message);
  }

  // Fil 5
  if ( (FCP_Valeur_Fil5 - delta) < Val_Fil_Bleu && Val_Fil_Bleu < (FCP_Valeur_Fil5 + delta) )
  {
    strcpy(message, "FCP;F5;2");
    Serial_Write("008", message);
  }
  else if ( (FCP_Valeur_Fil5 - delta) < Val_Fil_Rouge_Bleu && Val_Fil_Rouge_Bleu < (FCP_Valeur_Fil5 + delta) )
  {
    strcpy(message, "FCP;F5;3");
    Serial_Write("008", message);
  }
  else if ((FCP_Valeur_Fil5 - delta) < Val_Fil_Rouge && Val_Fil_Rouge < (FCP_Valeur_Fil5 + delta))
  {
    strcpy(message, "FCP;F5;1");
    Serial_Write("008", message);
  }
  else
  {
    strcpy(message, "FCP;F5;0");
    Serial_Write("008", message);
  }
}


void FCP_setup(char p_slot)
{
  // Verifier que l'on est bien cablé sur le module de fils complexe (Valeur potentiomètre Module FCP = Resistance_Fils_Complexe)
  if (p_slot != 'A')
  {
    Serial.println("!!!! Le module Fil Complexe doit obligatoirement etre branche sur le port A !!!!");
    return;
  }
  
  // Initialiser les différentes variables globales du module
  // Dans ce cas particulier pour ce module on sait que l'on doit prendre les pins du module FCP
  FCP_Pin_LED1 = MA_D1;
  FCP_Pin_LED2 = MA_D2;
  FCP_Pin_LED3 = MA_D3;
  FCP_Pin_LED4 = MA_D4;
  FCP_Pin_LED5 = MA_D5;
  FCP_Pin_Fil1 = MA_A2;
  FCP_Pin_Fil2 = MA_A3;
  FCP_Pin_Fil3 = MA_A4;
  FCP_Pin_Fil4 = MA_A5;
  FCP_Pin_Fil5 = MA_A6;

  // Réglage des PINs mode
  pinMode(FCP_Pin_LED1, OUTPUT);
  pinMode(FCP_Pin_LED2, OUTPUT);
  pinMode(FCP_Pin_LED3, OUTPUT);
  pinMode(FCP_Pin_LED4, OUTPUT);
  pinMode(FCP_Pin_LED5, OUTPUT);

  digitalWrite(FCP_Pin_LED1, HIGH);
  digitalWrite(FCP_Pin_LED2, HIGH);
  digitalWrite(FCP_Pin_LED3, HIGH);
  digitalWrite(FCP_Pin_LED4, HIGH);
  digitalWrite(FCP_Pin_LED5, HIGH);
  
  FCP_Valeur_Fil1 = analogRead(FCP_Pin_Fil1);
  FCP_Valeur_Fil2 = analogRead(FCP_Pin_Fil2);
  FCP_Valeur_Fil3 = analogRead(FCP_Pin_Fil3);
  FCP_Valeur_Fil4 = analogRead(FCP_Pin_Fil4);
  FCP_Valeur_Fil5 = analogRead(FCP_Pin_Fil5);

  // Initialiser l'écran lcd
  FCP_print_lcd(true, true, true, true, true);

  //On transmet la configuration des fils a la rasberry
  FCP_send_config();
}

void FCP_check_state()
{
  int curVal_Fil1 = analogRead(FCP_Pin_Fil1);
  int curVal_Fil2 = analogRead(FCP_Pin_Fil2);
  int curVal_Fil3 = analogRead(FCP_Pin_Fil3);
  int curVal_Fil4 = analogRead(FCP_Pin_Fil4);
  int curVal_Fil5 = analogRead(FCP_Pin_Fil5);

  int comp = 100;
  
  if ( (FCP_Valeur_Fil1 != curVal_Fil1) 
        && (!((FCP_Valeur_Fil1 - comp) < curVal_Fil1 && curVal_Fil1 < (FCP_Valeur_Fil1 + comp) )) )
  {
    Serial_Write("008", "FCP;F1;0");
  }
  if ( (FCP_Valeur_Fil2 != curVal_Fil2) 
        && (!((FCP_Valeur_Fil2 - comp) < curVal_Fil2 && curVal_Fil2 < (FCP_Valeur_Fil2 + comp) )) )
  {
    Serial_Write("008", "FCP;F2;0");
  }
  if ( (FCP_Valeur_Fil3 != curVal_Fil3) 
        && (!((FCP_Valeur_Fil3 - comp) < curVal_Fil3 && curVal_Fil3 < (FCP_Valeur_Fil3 + comp) )) )
  {
    Serial_Write("008", "FCP;F3;0");
  }
  if ( (FCP_Valeur_Fil4 != curVal_Fil4) 
        && (!((FCP_Valeur_Fil4 - comp) < curVal_Fil4 && curVal_Fil4 < (FCP_Valeur_Fil4 + comp) )) )
  {
    Serial_Write("008", "FCP;F4;0");
  }
  if ( (FCP_Valeur_Fil5 != curVal_Fil5) 
        && (!((FCP_Valeur_Fil5 - comp) < curVal_Fil5 && curVal_Fil5 < (FCP_Valeur_Fil5 + comp) )) )
  {
    Serial_Write("008", "FCP;F5;0");
  }

  FCP_Valeur_Fil1 = curVal_Fil1;
  FCP_Valeur_Fil2 = curVal_Fil2;
  FCP_Valeur_Fil3 = curVal_Fil3;
  FCP_Valeur_Fil4 = curVal_Fil4;
  FCP_Valeur_Fil5 = curVal_Fil5;
}

void FCP_Init_Scenario(char* p_ids, int p_nbIds, char* p_valeurs, int p_nbVals)
{
  int rangeVal = 0;
  char id[4];

  bool fil1 = false, fil2 = false, fil3 = false, fil4 = false, fil5 = false;

  for( int i = 0; i <= p_nbIds - 3; i += 3 )
  {
    id[0] = p_ids[i];
    id[1] = p_ids[i+1];
    id[2] = p_ids[i+2];
    id[3] = '\0';

    if( strcmp(id, "LD1") == 0 )
    {
      digitalWrite( FCP_Pin_LED1, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD2" ) == 0 )
    {
      digitalWrite( FCP_Pin_LED2, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD3" ) == 0 )
    {
      digitalWrite( FCP_Pin_LED3, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD4" ) == 0 )
    {
      digitalWrite( FCP_Pin_LED4, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD5" ) == 0 )
    {
      digitalWrite( FCP_Pin_LED5, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LT1" ) == 0 )
    {
      p_valeurs[rangeVal] == '0' ? fil1 = false : fil1 = true;
    }
    else if( strcmp( id, "LT2" ) == 0 )
    {
      p_valeurs[rangeVal] == '0' ? fil2 = false : fil2 = true;
    }
    else if( strcmp( id, "LT3" ) == 0 )
    {
      p_valeurs[rangeVal] == '0' ? fil3 = false : fil3 = true;
    }
    else if( strcmp( id, "LT4" ) == 0 )
    {
      p_valeurs[rangeVal] == '0' ? fil4 = false : fil4 = true;
    }
    else if( strcmp( id, "LT5" ) == 0 )
    {
      p_valeurs[rangeVal] == '0' ? fil5 = false : fil5 = true;
    }
    
    ++rangeVal;
  }

  FCP_print_lcd(fil1, fil2, fil3, fil4, fil5);
}

void FCP_Win()
{
  g_checkFCP = false;
  
  digitalWrite(FCP_Pin_LED1, LOW);
  digitalWrite(FCP_Pin_LED2, LOW);
  digitalWrite(FCP_Pin_LED3, LOW);
  digitalWrite(FCP_Pin_LED4, LOW);
  digitalWrite(FCP_Pin_LED5, LOW);
  
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("DESAMORCE");
}

void FCP_Lose()
{
  g_checkFCP = false;
  
  digitalWrite(FCP_Pin_LED1, LOW);
  digitalWrite(FCP_Pin_LED2, LOW);
  digitalWrite(FCP_Pin_LED3, LOW);
  digitalWrite(FCP_Pin_LED4, LOW);
  digitalWrite(FCP_Pin_LED5, LOW);
  
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("DESAMORCE");
}

void FCP_Read(char* p_message)
{ 
  bool l_reading = true;

  int l_readCptId = 0;
  int l_readCptVal = 0;
  char l_ids[100];
  char l_valeurs[100];

  // On doit savoir si on est en train de lire l'id (true) ou la valeur (false) 
  // On commence avec l'id
  bool l_isReadId = true;
  
  // Les 3 premiers characteres du message total representent l'ID plus 1 pour le ";"
  int i = 4;
  while( l_reading )
  {
    char l_char = p_message[i];

    if (l_char == 'W')
    {
      FCP_Win();
      return;
    }
    
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
      FCP_Init_Scenario(l_ids, l_readCptId, l_valeurs, l_readCptVal);
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

