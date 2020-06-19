

#include <LiquidCrystal_PCF8574.h>

// LES CARACTERES CHINOIS (ou pas)
char matrix[30][6] = {
  "XXXXX\0", "XXXXY\0", "XXXYX\0", "XXXYY\0", "XXYXX\0",
  "XXYXX\0", "XXYXY\0", "XXYYX\0", "XXYYY\0", "XYXXX\0", 
  "XYXXY\0", "XYXYX\0", "XYXYY\0", "XYYXX\0", "XYYXY\0", 
  "XYYYX\0", "XYYYY\0", "YXXXX\0", "YXXXZ\0", "YXXZX\0",
  "YXXZZ\0", "YXZXX\0", "YXZXZ\0", "YXZZX\0", "YXZZZ\0",
  "YZXXX\0", "YZXXZ\0", "YZXZX\0", "YZXZZ\0", "YZZZX\0"};

int MCB_Pin_LED_Actif = 0;
int MCB_LED_Actif = 0;

int MCB_Pin_LED1 = 0;
int MCB_LED1 = 0;
int MCB_Pin_LED2 = 0;
int MCB_LED2 = 0;
int MCB_Pin_LED3 = 0;
int MCB_LED3 = 0;
int MCB_Pin_LED4 = 0;
int MCB_LED4 = 0;

int MCB_Pin_Btn1 = 0;
int MCB_Btn1 = 0;
int MCB_Pin_Btn2 = 0;
int MCB_Btn2 = 0;
int MCB_Pin_Btn3 = 0;
int MCB_Btn3 = 0;
int MCB_Pin_Btn4 = 0;
int MCB_Btn4 = 0;

int COL_CHAR_1 = 0;
int COL_CHAR_2 = 10;
int COL_CHAR_3 = 0;
int COL_CHAR_4 = 10;

int LINE_CHAR_1 = 0;
int LINE_CHAR_2 = 0;
int LINE_CHAR_3 = 4;
int LINE_CHAR_4 = 4;


#define MCB_LCD_addr 0x25
LiquidCrystal_PCF8574 mcb_lcd(MCB_LCD_addr);


void MCB_print_lcd(char* char1, char* char2, char* char3, char* char4)
{
  mcb_lcd.begin(20, 4);
  mcb_lcd.setBacklight(255);
  mcb_lcd.home(); 
  mcb_lcd.clear();

  mcb_lcd.setCursor(0, 1);
  mcb_lcd.print("____________________");
  mcb_lcd.setCursor(9, 0);
  mcb_lcd.print("|");
  mcb_lcd.setCursor(9, 1);
  mcb_lcd.print("|");
  mcb_lcd.setCursor(9, 2);
  mcb_lcd.print("|");
  mcb_lcd.setCursor(9, 3);
  mcb_lcd.print("|");


  mcb_lcd.setCursor(COL_CHAR_1, LINE_CHAR_1);
  mcb_lcd.print(char1);

  mcb_lcd.setCursor(COL_CHAR_2, LINE_CHAR_2);
  mcb_lcd.print(char2);

  mcb_lcd.setCursor(COL_CHAR_3, LINE_CHAR_3);
  mcb_lcd.print(char3);

  mcb_lcd.setCursor(COL_CHAR_4, LINE_CHAR_4);
  mcb_lcd.print(char4);

}


void MCB_setup(char p_slot)
{
  // Initialiser les différentes variables globales du module
  if ( p_slot == 'C' )
  {
    MCB_Pin_LED_Actif = MC_A2;
    MCB_Pin_LED1 = MC_D5;
    MCB_Pin_LED2 = MC_D6;
    MCB_Pin_LED3 = MC_D8;
    MCB_Pin_LED4 = MC_D7;

    MCB_Pin_Btn1 = MC_D1;
    MCB_Pin_Btn2 = MC_D2;
    MCB_Pin_Btn3 = MC_D4;
    MCB_Pin_Btn4 = MC_D3;
  }
  else if( p_slot == 'D' )
  {
    MCB_Pin_LED_Actif = MD_A2;
    MCB_Pin_LED1 = MD_D5;
    MCB_Pin_LED2 = MD_D6;
    MCB_Pin_LED3 = MD_D8;
    MCB_Pin_LED4 = MD_D7;

    MCB_Pin_Btn1 = MD_D1;
    MCB_Pin_Btn2 = MD_D2;
    MCB_Pin_Btn3 = MD_D4;
    MCB_Pin_Btn4 = MD_D3;
  }
  else
  {
    Serial.println("!!!! Le module Chinois Bouton doit obligatoirement etre branche sur le port C ou D !!!!");
    return;
  }
  
  // Réglage des PINs mode
  pinMode(MCB_Pin_LED_Actif, OUTPUT);
  pinMode(MCB_Pin_LED1, OUTPUT);
  pinMode(MCB_Pin_LED2, OUTPUT);
  pinMode(MCB_Pin_LED3, OUTPUT);
  pinMode(MCB_Pin_LED4, OUTPUT);
  
  pinMode(MCB_Pin_Btn1, INPUT_PULLUP);
  pinMode(MCB_Pin_Btn2, INPUT_PULLUP);
  pinMode(MCB_Pin_Btn3, INPUT_PULLUP);
  pinMode(MCB_Pin_Btn4, INPUT_PULLUP);

  // Initialisation de la LED
  digitalWrite(MCB_Pin_LED_Actif, LOW);
  digitalWrite(MCB_Pin_LED1, LOW);
  digitalWrite(MCB_Pin_LED2, LOW);
  digitalWrite(MCB_Pin_LED3, LOW);
  digitalWrite(MCB_Pin_LED4, LOW);
}

void MCB_Win()
{
  g_checkMCB = false;
  
  digitalWrite(MCB_Pin_LED_Actif, LOW);
  digitalWrite(MCB_Pin_LED1, LOW);
  digitalWrite(MCB_Pin_LED2, LOW);
  digitalWrite(MCB_Pin_LED3, LOW);
  digitalWrite(MCB_Pin_LED4, LOW);

  mcb_lcd.clear();
  mcb_lcd.setCursor(5, 1);
  mcb_lcd.print("DESAMORCE");
}

void MCB_Lose()
{
  digitalWrite(MCB_Pin_LED_Actif, LOW);
  digitalWrite(MCB_Pin_LED1, LOW);
  digitalWrite(MCB_Pin_LED2, LOW);
  digitalWrite(MCB_Pin_LED3, LOW);
  digitalWrite(MCB_Pin_LED4, LOW);

  mcb_lcd.clear();
  mcb_lcd.setCursor(4, 0);
  mcb_lcd.print("ERREUR + 1");
  mcb_lcd.setCursor(1, 2);
  mcb_lcd.print("UN PAS DE PLUS ...");
  mcb_lcd.setCursor(2, 4);
  mcb_lcd.print("... VERS LA MORT");
}

void MCB_check_state()
{
  int curVal_Btn1 = !digitalRead(MCB_Pin_Btn1);
  int curVal_Btn2 = !digitalRead(MCB_Pin_Btn2);
  int curVal_Btn3 = !digitalRead(MCB_Pin_Btn3);
  int curVal_Btn4 = !digitalRead(MCB_Pin_Btn4);

  if (MCB_Btn1 != curVal_Btn1)
  {
    if( curVal_Btn1 )
    {
      digitalWrite(MCB_Pin_LED1, HIGH);
      Serial_Write("008", "MCB;B1;1");
    }
    else
    {
      Serial_Write("008", "MCB;B1;0");
    }
  }
  
  if (MCB_Btn2 != curVal_Btn2)
  {
    if( curVal_Btn2 )
    {
      digitalWrite(MCB_Pin_LED2, HIGH);
      Serial_Write("008", "MCB;B2;1");
    }
    else
    {
      Serial_Write("008", "MCB;B2;0");
    }
  }
  
  if (MCB_Btn3 != curVal_Btn3)
  {
    if( curVal_Btn3 )
    {
      digitalWrite(MCB_Pin_LED3, HIGH);
      Serial_Write("008", "MCB;B3;1");
    }
    else
    {
      Serial_Write("008", "MCB;B3;0");
    }
  }
 
  if (MCB_Btn4 != curVal_Btn4)
  {
    if( curVal_Btn4 )
    {
      digitalWrite(MCB_Pin_LED4, HIGH);
      Serial_Write("008", "MCB;B4;1");
    }
    else
    {
      Serial_Write("008", "MCB;B4;0");
    }
  }

  MCB_Btn1 = curVal_Btn1;
  MCB_Btn2 = curVal_Btn2;
  MCB_Btn3 = curVal_Btn3;
  MCB_Btn4 = curVal_Btn4;
}


void MCB_Change_State(char* p_ids, int p_nbIds, char* p_valeurs, int p_nbVals)
{
  int rangeVal = 0;
  char id[4];

  int l_valA = 0;
  int l_valB = 0;
  int l_valC = 0;
  int l_valD = 0;

  for( int i = 0; i <= p_nbIds - 3; i += 3 )
  {
    id[0] = p_ids[i];
    id[1] = p_ids[i+1];
    id[2] = p_ids[i+2];
    id[3] = '\0';

    if( strcmp(id, "CA0") == 0 )
    {
      l_valA += (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp( id, "CA1" ) == 0 )
    {
      l_valA += (p_valeurs[rangeVal] - 48) * 10;
    }
    else if( strcmp( id, "CB0" ) == 0 )
    {
      l_valB += (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp( id, "CB1" ) == 0 )
    {
      l_valB += (p_valeurs[rangeVal] - 48) * 10;
    }
    else if( strcmp( id, "CC0" ) == 0 )
    {
      l_valC += (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp( id, "CC1" ) == 0 )
    {
      l_valC += (p_valeurs[rangeVal] - 48) * 10;
    }
    else if( strcmp( id, "CD0" ) == 0 )
    {
      l_valD += (p_valeurs[rangeVal] - 48);
    }
    else if( strcmp( id, "CD1" ) == 0 )
    {
      l_valD += (p_valeurs[rangeVal] - 48) * 10;
    }

    ++rangeVal;
  }

  MCB_print_lcd(matrix[l_valA], matrix[l_valB], matrix[l_valC], matrix[l_valD]);
}


void MCB_Read(char* p_message)
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
      MCB_Win();
      return;
    }

    if (l_char == 'Z')
    {
      MCB_Lose();
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
      MCB_Change_State(l_ids, l_readCptId, l_valeurs, l_readCptVal);
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
