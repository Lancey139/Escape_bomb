#include "09_Constantes.h"

// Variables Globales
int MDA_Pin_LED1_RED = 0;
int MDA_Pin_LED1_GREEN = 0;
int MDA_Pin_LED1_BLUE = 0;
bool MDA_LED1_RED = 0;
bool MDA_LED1_GREEN = 0;
bool MDA_LED1_BLUE = 0;

int MDA_Pin_LED2_RED = 0;
int MDA_Pin_LED2_GREEN = 0;
int MDA_Pin_LED2_BLUE = 0;
bool MDA_LED2_RED = 0;
bool MDA_LED2_GREEN = 0;
bool MDA_LED2_BLUE = 0;

int MDA_Pin_PORTE = 0;
bool MDA_PORTE = 0;

int MDA_Pin_BTN1 = 0;
bool MDA_BTN1 = 0;
int MDA_Pin_BTN2 = 0;
bool MDA_BTN2 = 0;


void MDA_setup(char p_slot)
{ 
  if( p_slot == 'A' )
  {
    MDA_Pin_BTN1 = MA_D1;
    MDA_Pin_LED1_RED = MA_D2;
    MDA_Pin_LED1_GREEN = MA_D3;
    MDA_Pin_LED1_BLUE = MA_D4;
  
    MDA_Pin_BTN2 = MA_D5;
    MDA_Pin_LED2_RED = MA_D6;
    MDA_Pin_LED2_GREEN = MA_D7;
    MDA_Pin_LED2_BLUE = MA_D8;
    
    MDA_Pin_PORTE = MA_A3;
  }
}


void MDA_check_state()
{
  
}


void MDA_Change_State(char* p_ids, int p_nbIds, char* p_valeurs, int p_nbVals)
{
  int rangeVal = 0;
  char id[4];

  for( int i = 0; i <= p_nbIds - 3; i += 3 )
  {
    id[0] = p_ids[i];
    id[1] = p_ids[i+1];
    id[2] = p_ids[i+2];
    id[3] = '\0';

    if( strcmp(id, "LD1") == 0 )
    {
      p_valeurs[rangeVal] - 48;
    }

    ++rangeVal;
  }
}

void MDA_Win()
{
  g_checkMDA = false;
}

void MDA_Lose()
{
  g_checkMDA = false;
}


void MDA_Read(char* p_message)
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

    if (l_char == 'W')
    {
      MDA_Win();
      return;
    }

    if (l_char == 'Z')
    {
      MDA_Lose();
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
      MDA_Change_State(l_ids, l_readCptId, l_valeurs, l_readCptVal);
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
