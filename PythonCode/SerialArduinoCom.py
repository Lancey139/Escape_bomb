import serial
import sys
import string
import SerialComUtils as SU
from Commun.ConstantesMain import s_Fils_Complexes, s_Int_LED_Compteur,\
    s_Cle_Qui_Pend, s_Composants_Module_Main, s_Chinois_Boutons
from threading import Lock

class SerialArduinoCom:
    def __init__(self, pPort, pBaud):
        self.mPort = pPort
        self.mBaud = pBaud
        self.mSocket = serial.Serial(pPort, pBaud)
        self.mStart = 'S'
        self.mEnd = 'E'
        self.mBufferEnteteEnvoi = []
        self.mBufferEnvoi = []
        self.mBufferEnteteReception = []
        self.mBufferReception = ""
        self.mEntete = False
        self.mTailleMessageEnCours = 0
        
        # S'occupe du multi threading
        self.m_mutex = Lock()
        
        #Contient le pointeur vers le module fils complexes
        self.m_module_FCP = None
        
        # Contient le pointeur vers le module ILC
        self.m_module_ILC = None
        
        # Contient le pointeur vers le module CQP
        self.m_module_CQP = None
        
        # Contient le pointeur vers les composants main
        self.m_module_CMM = None
        
        # Contient le lien vers le module chinois
        self.m_module_MCB = None
    
    def lireMessage(self):
        lReturn = 0
        lByteLu = self.mSocket.read()
        lByteLu = lByteLu.decode()
        """
        Gestion de lecture de l'entete
        """
        if lByteLu == self.mStart:
            self.mEntete = True
        elif lByteLu == self.mEnd:
            self.mEntete = False
            self.lireEntete()
        elif self.mEntete:
            self.mBufferEnteteReception.append(lByteLu)
        elif self.mTailleMessageEnCours != 0:
            """
            Lecture du buffer
            """
            if self.mBufferReception.__len__() < self.mTailleMessageEnCours:
                #On rempli le buffer
                self.mBufferReception+=lByteLu
            if self.mBufferReception.__len__() == self.mTailleMessageEnCours:
                #Fin de la lecture du message
                self.mTailleMessageEnCours = 0
                self.lireBufferReception()
            
        return lReturn
            
    
    def lireEntete(self):
        if self.mBufferEnteteReception.__len__() == 3:
            lTaille = SU.conversionEntier(self.mBufferEnteteReception[0],
                                          self.mBufferEnteteReception[1],
                                          self.mBufferEnteteReception[2])
            self.mTailleMessageEnCours = lTaille
            self.mBufferEnteteReception = []
        else:
            print("Erreur lors de la lecture de l\'entete", self.mBufferEnteteReception)
            self.mTailleMessageEnCours =0
                
                
    def lireBufferReception(self):
        if self.mBufferReception.__len__() !=0:
            lBufferSplit =  self.mBufferReception.split(';')
            print("Reception du buffer", lBufferSplit)
            if lBufferSplit[0] == s_Fils_Complexes:
                self.m_module_FCP.LireMessage(lBufferSplit)
            if lBufferSplit[0] == s_Int_LED_Compteur:
                self.m_module_ILC.LireMessage(lBufferSplit)
            if lBufferSplit[0] == s_Cle_Qui_Pend:
                self.m_module_CQP.LireMessage(lBufferSplit)
            if lBufferSplit[0] == s_Composants_Module_Main:
                self.m_module_CMM.LireMessage(lBufferSplit)
            if lBufferSplit[0] == s_Chinois_Boutons:
                self.m_module_MCB.LireMessage(lBufferSplit)
                
                
                
        self.mBufferReception = ""
        
            
    def construireMessage(self, pIdModule, pContenu):
        """
        Construction du message
        """
        self.m_mutex.acquire()
        
        self.mBufferEnvoi = []
        for char in pIdModule:
            self.mBufferEnvoi.append(char)
            
        self.mBufferEnvoi.append(';')
        
        for element in pContenu:
            self.mBufferEnvoi.append(element)
            self.mBufferEnvoi.append(';') 
            
        self.mBufferEnvoi.append("\0")
        """
        Construction de l'entete
        """
        self.mBufferEnteteEnvoi = []
        self.mBufferEnteteEnvoi.append( self.mStart)
        lTaille = 0
        for elem in self.mBufferEnvoi:
            lTaille += len(elem)
        self.mBufferEnteteEnvoi += SU.conversionEntierToChar(lTaille)
        self.mBufferEnteteEnvoi.append(self.mEnd)
        self.sendMessage()
        
        self.m_mutex.release()
                   
    
    def sendMessage(self):
        for elem in self.mBufferEnteteEnvoi + self.mBufferEnvoi:
            self.mSocket.write(elem.encode())
