#define Start 'S'
#define End 'E'
#define SIZEOF_MESSAGESIZE 3

bool mReadingHeader;

char  m_message[100] ;
int m_sizeofMessage = 0;

char m_buffer[SIZEOF_MESSAGESIZE];
int m_cpt = 0;

void setup_Serial_Comunication(int pBaud)
{
  Serial1.begin(pBaud);
  Serial.begin(pBaud);
  mReadingHeader = false;
}

void Serial_Write(char * p_length, char * p_message)
{
  int l_length = atoi(p_length);
  
  // Envoi de l'entete
  Serial1.write(Start);
  Serial1.write(p_length[0]);
  Serial1.write(p_length[1]);
  Serial1.write(p_length[2]);
  Serial1.write(End);

  // Envoi du message
  for( int i = 0; i < l_length; ++i)
  {
    Serial1.write(p_message[i]);
  }

  Serial1.flush();
  
} //Serial_Write()



int Serial_Read()
{
  byte l_incomingByte;

  while(Serial1.available() > 0) 
  {
      // read the incoming byte:
      l_incomingByte = Serial1.read();
      switch (l_incomingByte)
      {
        case Start:
          mReadingHeader = true;
          break;
        case End:
          mReadingHeader = false;
          m_sizeofMessage = atoi(m_buffer);
          //m_message = new char[m_sizeofMessage];
          for(int i = 0; i < 100; ++i)
          {
            m_message[i]=';';
          }
          
          m_cpt=0;
          break;
        default:
         if(mReadingHeader && m_cpt < SIZEOF_MESSAGESIZE)
          {
            m_buffer[m_cpt] = l_incomingByte;
            ++m_cpt;
          }
          Read_Message(l_incomingByte);
          break;
      }   
  }
}

void Read_Message(byte p_incomingByte)
{
  if( ! mReadingHeader)
  {
    if (m_cpt < m_sizeofMessage)
    {
        // read the incoming byte:
        m_message[m_cpt] = p_incomingByte;
        ++m_cpt;
    }
    
    if (m_cpt == m_sizeofMessage)
    {
      m_cpt =0;
      // Analyse la trame
      //Extraction de l'id
      char id[4] = ";;;\0";
      for ( int i = 0; i < 3 ; ++i)
      {
        id[i] = m_message[i];
      }

      if( strcmp(id, MOD_PRES) == 0)
      {
        g_doPres = true;
      }
      else if( strcmp(id, MOD_INIT) == 0)
      {
        g_doInit = true;
      }
      else if(strcmp(id, ComposantModuleMain) == 0)
      {
        CMM_Read(m_message);
      }
      else if (strcmp(id, DehorsA) == 0)
      {
        MDA_Read(m_message);
      }
      else if (strcmp(id, GrosBouton) == 0)
      {
        MGB_Read(m_message);
      }
    }
  }
}

int Convertion(int Centaine, int Dizaine, int Unite)
{
  return Centaine*100 + Dizaine *10 + Unite;
}

