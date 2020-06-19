'''
Created on 3 juin 2018

@author: meo
'''
from Commun.ConstantesMain import s_Composants_Module_Main, s_buzzer, s_erreur
from Commun.Difficultes import s_nb_err

class Buzzer:
    def __init__(self, p_SerialCom):
        self.m_ser = p_SerialCom
        
        self.m_erreurs = 0
        
    def SonnerErreur(self):
        print("BUZZER SONNE ERREUR")
        # Sonne le buzeer
        l_contenu = []
        l_contenu.append(s_buzzer)
        l_contenu.append("1")
        self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        # Compte le nommbre d'erreurs
        self.m_erreurs += 1
        l_contenu = []
        l_contenu.append(s_erreur)
        l_contenu.append(str(self.m_erreurs))
        self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        
        if self.m_erreurs == s_nb_err:
            self.SonnerExplosion()
        
    def SonnerExplosion(self):
        print("BUZZER SONNE EXPLOSION")

        l_contenu = []
        l_contenu.append(s_buzzer)
        l_contenu.append("0")
        
        self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        
    def SonnerStress(self):
        print("BUZZER SONNE STRESS")
        
        l_contenu = []
        l_contenu.append(s_buzzer)
        l_contenu.append("2")
        
        self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
    
