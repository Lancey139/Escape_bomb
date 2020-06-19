#include "09_Constantes.h"

char g_moduleSlotA[4];
char g_moduleSlotB[4];

char g_presenceMDA = '\0';
char g_presenceMGB = '\0';

bool g_checkMDA = false;
bool g_checkMGB = false;

bool g_doPres = false;
bool g_doInit = false;



void Ident_Module()
{
  int l_potA = analogRead(MA_POT);
  int l_potB = analogRead(MB_POT);

  /********************/
  /* Detection slot A */
  /********************/
  if ( ((Resistance_MDA - 10) <= l_potA) && (l_potA <= (Resistance_MDA + 10)) )
  {
    sprintf(g_moduleSlotA, "MDA");
    g_presenceMDA = 'A';
  }
  else if ( ((Resistance_MGB - 10) <= l_potA) && (l_potA <= (Resistance_MGB + 10)) )
  {
    sprintf(g_moduleSlotA, "MGB");
    g_presenceMGB = 'A';
  }

  /********************/
  /* Detection slot B */
  /********************/
  if ( ((Resistance_MDA - 10) <= l_potB) && (l_potB <= (Resistance_MDA + 10)) )
  {
    sprintf(g_moduleSlotB, "MDA");
    g_presenceMDA = 'B';
  }
  else if ( ((Resistance_MGB - 10) <= l_potB) && (l_potB <= (Resistance_MGB + 10)) )
  {
    sprintf(g_moduleSlotB, "MGB");
    g_presenceMGB = 'B';
  }
}

void setup() 
{
  setup_Serial_Comunication(9600);
  // Identification des modules branches sur les differents slots
  Ident_Module();
}

void loop() 
{
  // Si on a envoyé un message, on attends la réponse de la raspberry
  Serial_Read();

  if( g_doPres )
  {
    int  l_position = 8;
    char l_length[3];
    char l_sPresence[16];

    sprintf(l_sPresence, "PRR;CMM;");

    if( g_presenceMDA != '\0' )
    {
      sprintf(l_sPresence + l_position, "MDA;");
      l_position += 4;
    }
    if( g_presenceMGB != '\0' )
    {
      sprintf(l_sPresence + l_position, "MGB;");
      l_position += 4;
    }

    if( l_position < 10 )
    {
      sprintf( l_length, "00%d", l_position );
    }
    else
    {
      sprintf( l_length, "0%d", l_position );
    }

    Serial_Write(l_length, l_sPresence);
    
    g_doPres = false;
  }
  else if( g_doInit )
  {
    // Identification des modules branches sur les differents slots
    CMM_setup(); 

    if( g_presenceMDA != '\0' )
    {
      g_checkMDA = true;
      MDA_setup(g_presenceMDA);
    }

    if( g_presenceMGB != '\0' )
    {
      g_checkMGB = true;
      MGB_setup(g_presenceMGB);
    }
    
    g_doInit = false;
  }
  else
  {
    CMM_check_state();

    if( g_presenceMDA != '\0' && g_checkMDA )
    {
      // Lire les valeurs du module actif MDA
      MDA_check_state();
    }
  
    if( g_presenceMGB != '\0' && g_checkMGB )
    {
      // Lire les valeurs du module actif MGB
      MGB_check_state();
    }
  }
  
  delay(10);
}
