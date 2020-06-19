'''
Created on 10 juin 2018

@author: meo
'''
from Commun.ConstantesMain import s_pave_numerique, s_temps_sec0,\
    s_Composants_Module_Main, s_temps_min0, s_temps_min1, s_temps_sec1, s_calcul,\
    s_gagne
from threading import Timer
import time
import random
from Commun.Difficultes import s_sec_resoudre_cal

class ComposantsModuleMain:
    
    def __init__(self, p_ser, p_Buzzer, p_temps_total, p_nb_calcul):
        
        # Lien vers la com série
        self.m_ser = p_ser
        
        # Table des chiffres stockés
        self.m_chiffres = "0"
        
        # Indique si on est en mode admin
        self.m_is_admin = False
        
        # Contient le numéro à trouver
        self.m_calcul_sol = 10
        
        # Contient si un calcul est en cours
        self.m_is_calcul = 0
        
        # Temps du desarmocage
        self.m_temps_total = p_temps_total
        
        # Contient les heures auxquels un calcul doit etre lancé
        self.m_heures_calculs = self.initHeureCalcul(p_nb_calcul)
        self.m_heures_fin_calcul = self.initFinHeureCalcul()
        
        # Pointeur vers buzzer
        self.m_buzzer = p_Buzzer
        

        # Temps restant 
        self.m_temps_now = time.time()
        self.m_temps_ecoule = self.m_temps_total * 60
        self.m_temps_str_last = "00:00"
        
        # Timer
        self.timer = Timer(1.0 , self.Timing )
        self.timer.start()
        
        # GAGNE
        self.m_gagne = False
        
    def LireMessage(self, p_Buffer):
        for i, l_id in enumerate(p_Buffer):
                if l_id  == s_pave_numerique:
                    if self.m_is_admin:
                        pass
                    elif p_Buffer[i+1] == '#': 
                        self.Is_Solution_Calcul()
                        self.m_chiffres = "0"
                    elif self.m_is_calcul:
                        self.m_chiffres += (p_Buffer[i+1])
                elif l_id == s_calcul:
                    self.m_calcul_sol = int(p_Buffer[i+1])
                        
    def Is_Solution_Calcul(self):
        if int(self.m_chiffres) == self.m_calcul_sol:
            l_contenu = []
            l_contenu.append(s_calcul)
            l_contenu.append("1")
            self.m_ser.construireMessage(s_Composants_Module_Main,l_contenu)
            self.m_is_calcul = False
        else : 
            self.m_buzzer.SonnerErreur()
            
    def initHeureCalcul(self, p_nb):
        l_tab_heure = []
        l_tab_min = []
        for i in range(p_nb):
            rad1 = random.randint(0,self.m_temps_total)
            rad2 = random.randint(0, 60 )
            # On intrrdit 2 cas dans la meme minute
            if rad1 in l_tab_min:
                rad1 -= 1
            
            if rad1 < 10:
                rad1_str = "0" + str(rad1)
            else : 
                rad1_str = str(rad1)
            if rad2 < 10:
                rad2_str = "0" + str(rad2)
            else : 
                rad2_str = str(rad2)
                
            l_tab_min.append(rad1)    
            l_tab_heure.append(rad1_str + ":" + rad2_str)
        return l_tab_heure
    
    def initFinHeureCalcul(self):
        l_tab_heure_fin = []
        for elem in self.m_heures_calculs : 
            l_min = int(elem[0:2])
            l_sec = int(elem[3:5])
            l_sec -= s_sec_resoudre_cal
            if l_sec < 0:
                if l_min > 1:
                    l_min -= 1
                else:
                    l_min = 0
                l_sec += 60
            if l_min < 10:
                l_min_str = "0" + str(l_min)
            else : 
                l_min_str = str(l_min)
            if l_sec < 10:
                l_sec_str = "0" + str(l_sec)
            else : 
                l_sec_str = str(l_sec)
            l_tab_heure_fin.append(l_min_str+ ":" + l_sec_str)
        print(l_tab_heure_fin)
        return l_tab_heure_fin
    
    def Timing (self):
        del self.timer
        self.m_temps_ecoule -= time.time() - self.m_temps_now
        self.m_temps_now = time.time()
        
        l_temps_str = time.strftime("%M:%S", time.gmtime(self.m_temps_ecoule))
        l_contenu = []
        
        # Mise a jour du temps sur l'afficheur
        if l_temps_str[0] != self.m_temps_str_last[0]:
            l_contenu = []
            l_contenu.append(s_temps_min1)
            l_contenu.append(l_temps_str[0])
            self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        if l_temps_str[1] != self.m_temps_str_last[1]:
            l_contenu = []
            l_contenu.append(s_temps_min0)
            l_contenu.append(l_temps_str[1])
            self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        if l_temps_str[3] != self.m_temps_str_last[3]:
            l_contenu = []
            l_contenu.append(s_temps_sec1)
            l_contenu.append(l_temps_str[3])
            self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        if l_temps_str[4] != self.m_temps_str_last[4]:
            l_contenu = []
            l_contenu.append(s_temps_sec0)
            l_contenu.append(l_temps_str[4])
            self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
        
        # Verification des event perturbateurs
        if l_temps_str in self.m_heures_calculs : 
            l_contenu = []
            l_contenu.append(s_calcul)
            l_contenu.append("0")
            self.m_ser.construireMessage(s_Composants_Module_Main,l_contenu)
            self.m_is_calcul = True
        elif l_temps_str in self.m_heures_fin_calcul:
            if self.m_is_calcul == True:
                self.m_buzzer.SonnerErreur()
                l_contenu = []
                l_contenu.append(s_calcul)
                l_contenu.append("0")
                self.m_ser.construireMessage(s_Composants_Module_Main,l_contenu)
                self.m_is_calcul = False
        
        if not self.m_gagne:
            self.m_temps_str_last = l_temps_str
            self.timer = Timer(1.0 , self.Timing )
            self.timer.start()
    
    def GAGNE(self):
        self.m_gagne = True
        l_contenu = []
        l_contenu.append(s_gagne)
        self.m_ser.construireMessage(s_Composants_Module_Main, l_contenu)
                
