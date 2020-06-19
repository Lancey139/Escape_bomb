#include "09_Constantes.h"

char g_moduleSlotA[4];
char g_moduleSlotB[4];
char g_moduleSlotC[4];
char g_moduleSlotD[4];

char g_presenceFCP = '\0';
char g_presenceILC = '\0';
char g_presenceCQP = '\0';
char g_presenceMCB = '\0';

bool g_checkFCP = false;
bool g_checkILC = false;
bool g_checkCQP = false;
bool g_checkMCB = false;

bool g_doPres = false;
bool g_doInit = false;

void Ident_Module()
{
  int l_potA = analogRead(MA_POT);
  int l_potB = analogRead(MB_POT);
  int l_potC = analogRead(MC_POT);
  int l_potD = analogRead(MD_POT);

  /********************/
  /* Detection slot A */
  /********************/
  if ( ((Resistance_FCP - 10) <= l_potA) && (l_potA <= (Resistance_FCP + 10)) )
  {
    sprintf(g_moduleSlotA, "FCP");
    g_presenceFCP = 'A';
  }
  else if ( ((Resistance_ILC - 10) <= l_potA) && (l_potA <= (Resistance_ILC + 10)) )
  {
    sprintf(g_moduleSlotA, "ILC");
    g_presenceILC = 'A';
  }
  else if ( (Resistance_CQP - 10) <= l_potA && l_potA <= (Resistance_CQP + 10) )
  {
    sprintf(g_moduleSlotA, "CQP");
    g_presenceCQP = 'A';
  }
  else if ( (Resistance_MCB - 10) <= l_potA && l_potA <= (Resistance_MCB + 10) )
  {
    sprintf(g_moduleSlotA, "MCB");
    g_presenceMCB = 'A';
  }

  /********************/
  /* Detection slot B */
  /********************/
  if ( ((Resistance_FCP - 10) <= l_potB) && (l_potB <= (Resistance_FCP + 10)) )
  {
    sprintf(g_moduleSlotB, "FCP");
    g_presenceFCP = 'B';
  }
  else if ( ((Resistance_ILC - 10) <= l_potB) && (l_potB <= (Resistance_ILC + 10)) )
  {
    sprintf(g_moduleSlotB, "ILC");
    g_presenceILC = 'B';
  }
  else if ( (Resistance_CQP - 10) <= l_potB && l_potB <= (Resistance_CQP + 10) )
  {
    sprintf(g_moduleSlotB, "CQP");
    g_presenceCQP = 'B';
  }
  else if ( (Resistance_MCB - 10) <= l_potB && l_potB <= (Resistance_MCB + 10) )
  {
    sprintf(g_moduleSlotB, "MCB");
    g_presenceMCB = 'B';
  }

  /********************/
  /* Detection slot C */
  /********************/
  if ( (Resistance_FCP - 10) <= l_potC && l_potC <= (Resistance_FCP + 10) )
  {
    sprintf(g_moduleSlotC, "FCP");
    g_presenceFCP = 'C';
  }
  else if ( (Resistance_ILC - 10) <= l_potC && l_potC <= (Resistance_ILC + 10) )
  {
    sprintf(g_moduleSlotC, "ILC");
    g_presenceILC = 'C';
  }
  else if ( (Resistance_CQP - 10) <= l_potC && l_potC <= (Resistance_CQP + 10) )
  {
    sprintf(g_moduleSlotC, "CQP");
    g_presenceCQP = 'C';
  }
  else if ( (Resistance_MCB - 10) <= l_potC && l_potC <= (Resistance_MCB + 10) )
  {
    sprintf(g_moduleSlotC, "MCB");
    g_presenceMCB = 'C';
  }

  /********************/
  /* Detection slot D */
  /********************/
  if ( (Resistance_FCP - 10) <= l_potD && l_potD <= (Resistance_FCP + 10) )
  {
    sprintf(g_moduleSlotD, "FCP");
    g_presenceFCP = 'D';
  }
  else if ( (Resistance_ILC - 10) <= l_potD && l_potD <= (Resistance_ILC + 10) )
  {
    sprintf(g_moduleSlotD, "ILC");
    g_presenceILC = 'D';
  }
  else if ( (Resistance_CQP - 10) <= l_potD && l_potD <= (Resistance_CQP + 10) )
  {
    sprintf(g_moduleSlotD, "CQP");
    g_presenceCQP = 'D';
  }
  else if ( (Resistance_MCB - 10) <= l_potD && l_potD <= (Resistance_MCB + 10) )
  {
    sprintf(g_moduleSlotD, "MCB");
    g_presenceMCB = 'D';
  }
}

void setup() 
{
  // put your setup code here, to run once:
  setup_Serial_Comunication(9600);

  // Identification des modules branches sur les differents slots
  Ident_Module();

}

void loop() 
{
  // Si on a envoyé un message, on attends la reponse de la raspberry
  Serial_Read();

  // Demande la liste des modules presents
  if( g_doPres )
  {
    int  l_position = 4;
    char l_length[3];
    char l_sPresence[20];

    sprintf(l_sPresence, "PRR;");
    
    if( g_presenceFCP != '\0' )
    {
      sprintf(l_sPresence + l_position, "FCP;");
      l_position += 4;
    }
    if( g_presenceILC != '\0' )
    {
      sprintf(l_sPresence + l_position, "ILC;");
      l_position += 4;
    }
    if( g_presenceCQP != '\0' )
    {
      sprintf(l_sPresence + l_position, "CQP;");
      l_position += 4;
    }
    if( g_presenceMCB != '\0' )
    {
      sprintf(l_sPresence + l_position, "MCB;");
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
  // Demande d'initialisation des modules
  else if( g_doInit )
  {
    if( g_presenceFCP != '\0' )
    {
      g_checkFCP = true;
      FCP_setup(g_presenceFCP);
    }
  
    if( g_presenceILC != '\0' )
    {
      g_checkILC = true;
      ILC_setup(g_presenceILC);
    }
  
    if( g_presenceCQP != '\0' )
    {
      g_checkCQP = true;
      CQP_setup(g_presenceCQP);
    }
  
    if( g_presenceMCB != '\0' )
    {
      g_checkMCB = true;
      MCB_setup(g_presenceMCB);
    }

    // Initialisation terminee
    g_doInit = false;
  }
  // Les modules sont initialises et envoient leurs etats
  else
  {
    if( g_presenceFCP != '\0' && g_checkFCP )
    {
      // Lire les valeurs du module actif FCP
      FCP_check_state();
    }
  
    if( g_presenceILC != '\0' && g_checkILC )
    {
      // Lire les valeurs du module actif ILC
      ILC_check_state();
    }
    
    if( g_presenceCQP != '\0' && g_checkCQP )
    {
      // Lire les valeurs du module actif CQP
      CQP_check_state();
    }
  
    if( g_presenceMCB != '\0' && g_checkMCB )
    {
      // Lire les valeurs du module actif MCB
      MCB_check_state();
    }
  }

  // Petite pause quand meme...
  delay(10);
}
