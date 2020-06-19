#include "09_Constantes.h"
#include <Arduino.h>
#include <TM1637Display.h>


// Variables Globales
int ILC_Pin_LED1 = 0;
bool ILC_LED1 = 0;
int ILC_Pin_LED2 = 0;
bool ILC_LED2 = 0;
int ILC_Pin_LED3 = 0;
bool ILC_LED3 = 0;
int ILC_Pin_LED4 = 0;
bool ILC_LED4 = 0;
int ILC_Pin_LED5 = 0;

int ILC_Pin_Int1 = 0;
int ILC_Valeur_Int1 = 0;
int ILC_Pin_Int2 = 0;
int ILC_Valeur_Int2 = 0;
int ILC_Pin_Int3 = 0;
int ILC_Valeur_Int3 = 0;
int ILC_Pin_Int4 = 0;
int ILC_Valeur_Int4 = 0;
int ILC_Pin_Int5 = 0;
int ILC_Valeur_Int5 = 0;


TM1637Display display(CLK, DIO);
const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };


void ILC_send_config()
{
  char message[9];
  
  // Int 1
  if( ILC_Valeur_Int1 )
  {
    strcpy(message, "ILC;I1;1");
  }
  else
  {
    strcpy(message, "ILC;I1;0");
  }
  Serial_Write("008", message);

  // Int 2
  if( ILC_Valeur_Int2 )
  {
    strcpy(message, "ILC;I2;1");
  }
  else
  {
    strcpy(message, "ILC;I2;0");
  }
  Serial_Write("008", message);

  // Int 3
  if( ILC_Valeur_Int3 )
  {
    strcpy(message, "ILC;I3;1");
  }
  else
  {
    strcpy(message, "ILC;I3;0");
  }
  Serial_Write("008", message);

  // Int 4
  if( ILC_Valeur_Int4 )
  {
    strcpy(message, "ILC;I4;1");
  }
  else
  {
    strcpy(message, "ILC;I4;0");
  }
  Serial_Write("008", message);

  // Int 5
  if( ILC_Valeur_Int5 )
  {
    strcpy(message, "ILC;I5;1");
  }
  else
  {
    strcpy(message, "ILC;I5;0");
  }
  Serial_Write("008", message);
}


void ILC_setup(char p_slot)
{
  // Verifier que l'on est bien cablé sur le module de Ints complexe (Valeur potentiomètre Module ILC = Interupteur_Led_Compteur)
  if (p_slot != 'B')
  {
    Serial.println("!!!! Le module Interrupteur Led Compteur doit obligatoirement etre branche sur le port B !!!!");
    return;
  }
  
  // Initialiser les différentes variables globales du module
  // Dans ce cas particulier pour ce module on sait que l'on doit prendre les pins du module ILC
  ILC_Pin_LED1 = MB_A2;
  ILC_Pin_LED2 = MB_A3;
  ILC_Pin_LED3 = MB_D1;
  ILC_Pin_LED4 = MB_D2;
  ILC_Pin_LED5 = MB_D3;
  ILC_Pin_Int1 = MB_D8;
  ILC_Pin_Int2 = MB_D7;
  ILC_Pin_Int3 = MB_D6;
  ILC_Pin_Int4 = MB_D5;
  ILC_Pin_Int5 = MB_D4;

  // Réglage des PINs mode
  pinMode(ILC_Pin_LED1, OUTPUT);
  pinMode(ILC_Pin_LED2, OUTPUT);
  pinMode(ILC_Pin_LED3, OUTPUT);
  pinMode(ILC_Pin_LED4, OUTPUT);
  pinMode(ILC_Pin_LED5, OUTPUT);

  pinMode(ILC_Pin_Int1, INPUT_PULLUP);
  pinMode(ILC_Pin_Int2, INPUT_PULLUP);
  pinMode(ILC_Pin_Int3, INPUT_PULLUP);
  pinMode(ILC_Pin_Int4, INPUT_PULLUP);
  pinMode(ILC_Pin_Int5, INPUT_PULLUP);
 
  digitalWrite(ILC_Pin_LED1, HIGH);
  digitalWrite(ILC_Pin_LED2, HIGH);
  digitalWrite(ILC_Pin_LED3, HIGH);
  digitalWrite(ILC_Pin_LED4, HIGH);
  digitalWrite(ILC_Pin_LED5, HIGH);
  
  ILC_Valeur_Int1 = !digitalRead(ILC_Pin_Int1);
  ILC_Valeur_Int2 = !digitalRead(ILC_Pin_Int2);
  ILC_Valeur_Int3 = !digitalRead(ILC_Pin_Int3);
  ILC_Valeur_Int4 = !digitalRead(ILC_Pin_Int4);
  ILC_Valeur_Int5 = !digitalRead(ILC_Pin_Int5);

  display.setBrightness(0x0f);
  // All segments on
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(0);
  data[2] = display.encodeDigit(0);
  data[3] = display.encodeDigit(0);
  display.setSegments(data);
  
  
  //On transmet la configuration des Ints a la rasberry
  ILC_send_config();
}


void ILC_check_state()
{
  int curVal_Int1 = !digitalRead(ILC_Pin_Int1);
  int curVal_Int2 = !digitalRead(ILC_Pin_Int2);
  int curVal_Int3 = !digitalRead(ILC_Pin_Int3);
  int curVal_Int4 = !digitalRead(ILC_Pin_Int4);
  int curVal_Int5 = !digitalRead(ILC_Pin_Int5);

  
  if (ILC_Valeur_Int1 != curVal_Int1)
  {
    if( curVal_Int1 )
    {
      Serial_Write("008", "ILC;I1;1");
    }
    else
    {
      Serial_Write("008", "ILC;I1;0");
    }
  }
  if (ILC_Valeur_Int2 != curVal_Int2)
  {
    if( curVal_Int2 )
    {
      Serial_Write("008", "ILC;I2;1");
    }
    else
    {
      Serial_Write("008", "ILC;I2;0");
    }
  }
  if (ILC_Valeur_Int3 != curVal_Int3)
  {
    if( curVal_Int3 )
    {
      Serial_Write("008", "ILC;I3;1");
    }
    else
    {
      Serial_Write("008", "ILC;I3;0");
    }
  }
  if (ILC_Valeur_Int4 != curVal_Int4)
  {
    if( curVal_Int4 )
    {
      Serial_Write("008", "ILC;I4;1");
    }
    else
    {
      Serial_Write("008", "ILC;I4;0");
    }
  }
  if (ILC_Valeur_Int5 != curVal_Int5)
  {
    if( curVal_Int5 )
    {
      Serial_Write("008", "ILC;I5;1");
    }
    else
    {
      Serial_Write("008", "ILC;I5;0");
    }
  }

  ILC_Valeur_Int1 = !digitalRead(ILC_Pin_Int1);
  ILC_Valeur_Int2 = !digitalRead(ILC_Pin_Int2);
  ILC_Valeur_Int3 = !digitalRead(ILC_Pin_Int3);
  ILC_Valeur_Int4 = !digitalRead(ILC_Pin_Int4);
  ILC_Valeur_Int5 = !digitalRead(ILC_Pin_Int5);
}

void ILC_Print_Cpt(int value)
{
  data[3] = display.encodeDigit(value);
  display.setSegments(data);
}

void ILC_Change_State(char* p_ids, int p_nbIds, char* p_valeurs, int p_nbVals)
{
  int rangeVal = 0;
  char id[4];

  bool int1 = false, int2 = false, int3 = false, int4 = false, int5 = false;

  for( int i = 0; i <= p_nbIds - 3; i += 3 )
  {
    id[0] = p_ids[i];
    id[1] = p_ids[i+1];
    id[2] = p_ids[i+2];
    id[3] = '\0';

    if( strcmp(id, "LD1") == 0 )
    {
      digitalWrite( ILC_Pin_LED1, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD2" ) == 0 )
    {
      digitalWrite( ILC_Pin_LED2, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD3" ) == 0 )
    {
      digitalWrite( ILC_Pin_LED3, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD4" ) == 0 )
    {
      digitalWrite( ILC_Pin_LED4, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LD5" ) == 0 )
    {
      digitalWrite( ILC_Pin_LED5, (p_valeurs[rangeVal] == '0' ? LOW : HIGH) );
    }
    else if( strcmp( id, "LC0" ) == 0 )
    {
      // Ecrire nouvelle valeur sur le 7 segments
      ILC_Print_Cpt( p_valeurs[rangeVal] - 48 );
    }
    
    ++rangeVal;
  }
}


void ILC_Win()
{
  g_checkILC = false;
  
  digitalWrite(ILC_Pin_LED1, HIGH);
  digitalWrite(ILC_Pin_LED2, HIGH);
  digitalWrite(ILC_Pin_LED3, HIGH);
  digitalWrite(ILC_Pin_LED4, HIGH);
  digitalWrite(ILC_Pin_LED5, HIGH);

  display.setSegments(SEG_DONE);
}

void ILC_Lose()
{
  digitalWrite(ILC_Pin_LED1, LOW);
  digitalWrite(ILC_Pin_LED2, LOW);
  digitalWrite(ILC_Pin_LED3, LOW);
  digitalWrite(ILC_Pin_LED4, LOW);
  digitalWrite(ILC_Pin_LED5, LOW);

  data[0] = 0b00001000;
  data[1] = 0b00001000;
  data[2] = 0b00001000;
  data[3] = 0b00001000;
  display.setSegments(data);
}


void ILC_Read(char* p_message)
{ 
  bool l_reading = true;

  int l_readCptId = 0;
  int l_readCptVal = 0;
  char l_ids[100];
  char l_valeurs[100];

  // On doit savoir si on est en train de lire l'id (true) ou la valeur (false) 
  // On commence avec l'id
  bool l_isReadId = true;
  
  // Les 3 premiers characteres du message total representent l'ID + 1 pour le ";"
  int i = 4;
  while( l_reading )
  {
    char l_char = p_message[i];

    if (l_char == 'W')
    {
      ILC_Win();
      return;
    }

    if (l_char == 'Z')
    {
      ILC_Lose();
      return;
    }

    if (l_char == 'I')
    {
      ILC_send_config();
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
      ILC_Change_State(l_ids, l_readCptId, l_valeurs, l_readCptVal);
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
