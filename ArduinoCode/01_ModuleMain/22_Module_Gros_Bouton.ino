#include "09_Constantes.h"

// Variables Globales
int MGB_Pin_LED_RED = 0;
int MGB_Pin_LED_GREEN = 0;
int MGB_Pin_LED_BLUE = 0;

int MGB_Pin_BTN = 0;
bool MGB_BTN = 0;

#define CMM_LCD_addr 0x26
LiquidCrystal_PCF8574 mgb_lcd (CMM_LCD_addr);


void MGB_Couleur_LED(int p_couleur)
{
  // Couleurs
  /*
  # Variables du Gros Boutons
  color_blue   = 1
  color_white  = 2
  color_yellow = 3
  color_red    = 4
  color_green  = 5
  */
  switch (p_couleur)
  {
    case 1:
      digitalWrite(MGB_Pin_LED_RED, LOW);
      digitalWrite(MGB_Pin_LED_GREEN, LOW);
      digitalWrite(MGB_Pin_LED_BLUE, HIGH);
      break;
    case 2:
      digitalWrite(MGB_Pin_LED_RED, LOW);
      digitalWrite(MGB_Pin_LED_GREEN, LOW);
      digitalWrite(MGB_Pin_LED_BLUE, LOW);
      break;
    case 3:
      digitalWrite(MGB_Pin_LED_RED, HIGH);
      digitalWrite(MGB_Pin_LED_GREEN, LOW);
      digitalWrite(MGB_Pin_LED_BLUE, LOW);
      break;
    case 4:
      digitalWrite(MGB_Pin_LED_RED, LOW);
      digitalWrite(MGB_Pin_LED_GREEN, HIGH);
      digitalWrite(MGB_Pin_LED_BLUE, LOW);
      break;
    default:
      Serial.println("default");
      break;
  }
}

void MGB_Print_LCD( char * p_string, int p_length, int p_line )
{
  if( p_length == 0 )
  {
    // Effacement de la ligne
    mgb_lcd.setCursor(0, p_line);
    mgb_lcd.print("                ");
  }
  else
  {
    // Effacement de la ligne
    mgb_lcd.setCursor(0, p_line);
    mgb_lcd.print("                ");

    // Ecriture
    int l_posX = 8 - p_length / 2;
    if( l_posX < 0 )
    {
      l_posX = 0;
    }
    mgb_lcd.setCursor(l_posX, p_line);
    mgb_lcd.print(p_string);
  }
}

void MGB_Val_LCD( int p_val )
{
  // Valeurs
  // 1 > Abort
  // 2 > Detonate
  // 3 > Hold
  // 4 > CAR
  // 5 > FRK
  mgb_lcd.setBacklight(1);
  switch (p_val)
  {
    case 1:
      MGB_Print_LCD("Abort", 5, 0);
    break;
    case 2:
      MGB_Print_LCD("Detonate", 8, 0);
    break;
    case 3:
      MGB_Print_LCD("Hold", 4, 0);
    break;
    case 4:
      MGB_Print_LCD("CAR", 3, 0);
    break;
    case 5:
      MGB_Print_LCD("FRK", 3, 0);
    break;
    default:
      MGB_Print_LCD("", 0, 0);
    break;
  }
}


void MGB_setup(char p_slot)
{ 
  if( p_slot == 'A' )
  {
    MGB_Pin_BTN = MA_D1;
    MGB_Pin_LED_RED = MA_D2;
    MGB_Pin_LED_GREEN = MA_D3;
    MGB_Pin_LED_BLUE = MA_D4;
  }
  else if( p_slot == 'B' )
  {
    MGB_Pin_BTN = MB_D1;
    MGB_Pin_LED_RED = MB_D2;
    MGB_Pin_LED_GREEN = MB_D3;
    MGB_Pin_LED_BLUE = MB_D4;
  }

  // Le Réglage des PINs mode se fait dans la pin MGB_Couleur_LED
  pinMode(MGB_Pin_LED_RED, OUTPUT);
  pinMode(MGB_Pin_LED_GREEN, OUTPUT);
  pinMode(MGB_Pin_LED_BLUE, OUTPUT);
  
  pinMode(MGB_Pin_BTN, INPUT_PULLUP);
  MGB_BTN = !digitalRead(MGB_Pin_BTN);

  mgb_lcd.begin(16, 2);
  mgb_lcd.setBacklight(1);
  mgb_lcd.home(); 
  mgb_lcd.clear();
}


void MGB_check_state()
{
  int curVal_BTN = !digitalRead(MGB_Pin_BTN);
  
  if (MGB_BTN != curVal_BTN)
  {
    if( curVal_BTN )
    {
      Serial_Write("008", "MGB;B1;1");
    }
    else
    {
      Serial_Write("008", "MGB;B1;0");
    }
  }

  MGB_BTN = curVal_BTN;
}


void MGB_Change_State(char* p_ids, int p_nbIds, char* p_valeurs, int p_nbVals)
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
      MGB_Couleur_LED( p_valeurs[rangeVal] - 48 );
    }
    else if( strcmp(id, "CHN") == 0 )
    {
      MGB_Val_LCD( p_valeurs[rangeVal] - 48 );
    }
    
    ++rangeVal;
  }
}

void MGB_Win()
{
  g_checkMGB = false;
  mgb_lcd.setBacklight(0);
  digitalWrite(MGB_Pin_LED_RED, LOW);
  digitalWrite(MGB_Pin_LED_GREEN, LOW);
  digitalWrite(MGB_Pin_LED_BLUE, LOW);
}

void MGB_Lose()
{
  Serial.println("Loose");
   mgb_lcd.setBacklight(0);
  digitalWrite(MGB_Pin_LED_RED, LOW);
  digitalWrite(MGB_Pin_LED_GREEN, LOW);
  digitalWrite(MGB_Pin_LED_BLUE, LOW);
}

void MGB_Read(char* p_message)
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
      MGB_Win();
      return;
    }

    if (l_char == 'Z')
    {
      MGB_Lose();
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
      MGB_Change_State(l_ids, l_readCptId, l_valeurs, l_readCptVal);
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
