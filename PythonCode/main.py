import serial
import sys
import string
import time
import SerialArduinoCom
from Modules.FilsComplexes import FilsComplexes
from Commun.Bombe import Bombe
from Commun.ConstantesMain import s_Connecteur
from Commun.Buzzer import Buzzer
from Modules.InterupteurLEDCompteurs import InterupteurLEDCompteur
from Modules.CleQuiPend import CleQuiPend
from Commun.ComposantsModuleMain import ComposantsModuleMain
from Commun.Difficultes import s_nb_calcul, s_temps_total
from Modules.ChinoisBoutons import ChinoisBoutons


def main():        
    
    ser = SerialArduinoCom.SerialArduinoCom('/dev/ttyUSB0', 9600)
    ser2 = None
    #Creation de la bombe
    l_bombe = Bombe(12345673, s_Connecteur["Con_Pa"], 1)
    
    #Creation du buzzer
    l_buzzer = Buzzer(ser)
    
    #Création provisoire des modules
    l_module_FCP = FilsComplexes(ser, l_bombe.m_serie, l_bombe.m_Port, l_bombe.m_Nb_Pile, l_buzzer)
    
    l_module_ILC = InterupteurLEDCompteur(ser, l_buzzer)
    
    l_module_CQP = CleQuiPend(ser, l_buzzer)
    
    l_module_CMM = ComposantsModuleMain(ser, l_buzzer,s_temps_total, s_nb_calcul)
    
    l_module_MCB = ChinoisBoutons(ser, l_buzzer)
    
    ser.m_module_FCP = l_module_FCP
    
    ser.m_module_ILC = l_module_ILC
    
    ser.m_module_CQP = l_module_CQP
    
    ser.m_module_CMM = l_module_CMM
    
    ser.m_module_MCB = l_module_MCB
    
    l_module_MCB.InitialisationModule()
    
    while True:
        ser.lireMessage()
        time.sleep(0.01)


if __name__ == "__main__":
    main()



