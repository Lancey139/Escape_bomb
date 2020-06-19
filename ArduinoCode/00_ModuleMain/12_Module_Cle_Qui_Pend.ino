#include "09_Constantes.h"

// Variables Globales
int CQP_Pin_LED_Eclairage = 0;
int CQP_Pin_LED_Desact = 0;

int CQP_Pin_Vis1 = 0;
int CQP_Vis1 = 0;
int CQP_Pin_Vis2 = 0;
int CQP_Vis2 = 0;
int CQP_Pin_Int_Cle = 0;
int CQP_Int_Cle = 0;
int CQP_Pin_Anneau = 0;
int CQP_Anneau = 0;



void CQP_setup(char p_slot)
{
  // Initialiser les différentes variables globales du module
  if ( p_slot == 'C' )
  {
    CQP_Pin_Int_Cle = MC_D2;
    CQP_Pin_Vis1 = MC_D3;
    CQP_Pin_Vis2 = MC_D4;
    CQP_Pin_Anneau = MC_D5;
    CQP_Pin_LED_Eclairage = MC_D7;
    CQP_Pin_LED_Desact = MC_D1;
    
  }
  else if( p_slot == 'D' )
  {
    CQP_Pin_Int_Cle = MD_D2;
    CQP_Pin_Vis1 = MD_D3;
    CQP_Pin_Vis2 = MD_D4;
    CQP_Pin_Anneau = MD_D5;
    CQP_Pin_LED_Eclairage = MD_D7;
    CQP_Pin_LED_Desact = MD_D1;
  }
  else
  {
    Serial.println("!!!! Le module Cle qui pend doit obligatoirement etre branche sur le port C ou D !!!!");
    return;
  }

  
  // Réglage des PINs mode
  pinMode(CQP_Pin_LED_Eclairage, OUTPUT);
  pinMode(CQP_Pin_LED_Desact, OUTPUT);
  
  pinMode(CQP_Pin_Vis1, INPUT_PULLUP);
  pinMode(CQP_Pin_Vis2, INPUT_PULLUP);
  pinMode(CQP_Pin_Anneau, INPUT_PULLUP);
  pinMode(CQP_Pin_Int_Cle, INPUT_PULLUP);

  // Initialisation de la LED
  digitalWrite(CQP_Pin_LED_Eclairage, HIGH);
  digitalWrite(CQP_Pin_LED_Desact, LOW);

  // On fait une première lecture
  CQP_Vis1 = !digitalRead(CQP_Pin_Vis1);
  CQP_Vis2 = !digitalRead(CQP_Pin_Vis2);
  CQP_Anneau = !digitalRead(CQP_Pin_Anneau);
  CQP_Int_Cle = !digitalRead(CQP_Pin_Int_Cle);
 
}

void CQP_check_state()
{
  int curVal_Vis1 = !digitalRead(CQP_Pin_Vis1);
  int curVal_Vis2 = !digitalRead(CQP_Pin_Vis2);
  int curVal_An = !digitalRead(CQP_Pin_Anneau);
  int curVal_Cle = !digitalRead(CQP_Pin_Int_Cle);

  if (CQP_Vis1 != curVal_Vis1)
  {
    if( curVal_Vis1 )
    {
      Serial_Write("008", "CQP;V1;1");
    }
    else
    {
      Serial_Write("008", "CQP;V1;0");
    }
  }
  else if (CQP_Vis2 != curVal_Vis2)
  {
    if( curVal_Vis2 )
    {
      Serial_Write("008", "CQP;V2;1");
    }
    else
    {
      Serial_Write("008", "CQP;V2;0");
    }
  }
  else if (CQP_Anneau != curVal_An)
  {
    if( curVal_An )
    {
      Serial_Write("008", "CQP;A0;1");
    }
    else
    {
      Serial_Write("008", "CQP;A0;0");
    }
  }
  else if (CQP_Int_Cle != curVal_Cle)
  {
    if( curVal_Cle )
    {
      Serial_Write("008", "CQP;C0;1");
    }
    else
    {
      Serial_Write("008", "CQP;C0;0");
    }
  }

  CQP_Vis1 = curVal_Vis1;
  CQP_Vis2 = curVal_Vis2;
  CQP_Anneau = curVal_An;
  CQP_Int_Cle = curVal_Cle;
}

void CQP_Win()
{
  g_checkCQP = false;
  digitalWrite(CQP_Pin_LED_Eclairage, LOW);
  digitalWrite(CQP_Pin_LED_Desact, HIGH);
}

void CQP_Lose()
{
  g_checkCQP = false;
  digitalWrite(CQP_Pin_LED_Eclairage, LOW);
  digitalWrite(CQP_Pin_LED_Desact, LOW);
}

void CQP_Read(char* p_message)
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
      CQP_Win();
      return;
    }

    if (l_char == 'Z')
    {
      CQP_Lose();
      return;
    }
    
    ++i;
  } 
}
  

