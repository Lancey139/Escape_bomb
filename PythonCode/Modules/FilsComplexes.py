'''
Created on 13 mai 2018

@author: meo
'''
from Modules.Constantes import s_Pas_Fils, s_Fil_Rouge, s_Fil_Bleu,\
    s_Fil_Rouge_Bleu, s_Id_LED, s_Id_Etoile, s_Succes
from Commun.ConstantesMain import s_Connecteur, s_Fils_Complexes
import random

class FilsComplexes:
    def __init__(self,p_ser, p_Num_serie, p_Port, p_Nb_Pile, p_Buzzer):
        # Contient un tableau de boléen contenant l'état des 5 LEDs
        self.m_LEDs = []
        #Contient un dico contenant les ids des fils et la couleur des fils branchés
        self.m_fils = {'F1' : 0,'F2' : 0,'F3' : 0,'F4' : 0,'F5' : 0}
        #Contient un tableau contenant l'état de l'écran (des étoiles)
        self.m_etoiles = []
        #Numero de serie de la bombe
        self.m_serie = p_Num_serie
        #Port de la bombe
        self.m_Port = p_Port
        #Nombre de pile 
        self.m_Nb_Pile = p_Nb_Pile
        #Pointeur vers la communication série
        self.m_ser = p_ser
        #Booléen indiquant si le module a été initialisé
        self.m_init = False
        #Contient la matrice des fils qui doivent être débranché
        self.m_doit_etre_debranche = []
        #Indique si le midlue a ete desamorcé
        self.m_module_desamorce = False
        #Lien vers le buzzer
        self.m_buzzer = p_Buzzer
        
    def GenererScenario(self ):
        """
        Genere le scénario 
        """
        for i in range(5):
            self.m_LEDs.append(random.randint(0,1))
            self.m_etoiles.append( random.randint(0,1))
        
        """
        Envoi un message a l'arduino pour initialiser les LED/étoiles
        """
        l_contenu = []
        for i, l_LED in enumerate(self.m_LEDs):
            l_contenu.append(s_Id_LED+str(i+1))
            l_contenu.append(str(l_LED))
        for i, l_Etoile in enumerate(self.m_etoiles):
            l_contenu.append(s_Id_Etoile+str(i+1))
            l_contenu.append(str(l_Etoile))
        
        self.m_ser.construireMessage(s_Fils_Complexes,l_contenu)
            
        self.m_init = True
        
        for i in range(5):
            l_id = "F%d"%(i+1)
            self.m_doit_etre_debranche.append(self.DoitEtreDebranche(
                self.m_LEDs[i], 
                self.m_fils[l_id], 
                self.m_etoiles[i]))
            
        print("Solutions module FCP " , self.m_doit_etre_debranche)
            
    
    def LireMessage(self, p_Buffer):
        """
        Lit le message de l'arduino
        """
        if not self.m_module_desamorce :
            
            for i, l_id in enumerate(p_Buffer):
                if l_id in self.m_fils.keys():
                    if not self.m_init:
                        self.m_fils[l_id] = int(p_Buffer[i+1])
                    elif int(p_Buffer[i+1]) == 0 :  
                        l_numero_fil = int(l_id[1]) -1
                        l_Doit_Etre_Debranche = self.m_doit_etre_debranche[l_numero_fil]
                        self.m_doit_etre_debranche[l_numero_fil] = False
                        
                        if l_Doit_Etre_Debranche == False:
                            # ICI faire sonner le buzzer et compter une erreur
                            self.Faute()
                            pass
                        if True not in self.m_doit_etre_debranche : 
                            self.m_module_desamorce = True
                            self.Succes()
                    
            #Fin de la phase d'initialisation, génration de la solution        
            if 0 not in self.m_fils.values() and not self.m_init:
                self.GenererScenario()
        
    def Succes(self):
        """
        Méthode chargée d'envoyer le message de succes
        """
        l_contenu = []
        l_contenu.append(s_Succes)
        
        print(l_contenu)
        self.m_ser.construireMessage(s_Fils_Complexes,l_contenu)
        
    
    def Faute(self):
        self.m_buzzer.SonnerErreur()
        pass
    
    def DoitEtreDebranche(self, p_etat_LED, p_couleur_fil, p_etat_etoile):
        """
        Cette méthode renvoie un booléen true ou false si le fil devait etre debranché
        """
        
        l_retour = False
        l_lettre = ''
        if p_couleur_fil == s_Fil_Rouge:
            if p_etat_LED:
                if p_etat_etoile:
                    l_lettre = 'B'
                else:
                    l_lettre = 'B'
            else:
                if p_etat_etoile:
                    l_lettre = 'C'
                else:
                    l_lettre = 'S'
        elif p_couleur_fil == s_Fil_Bleu:
            if p_etat_LED:
                if p_etat_etoile:
                    l_lettre = 'P'
                else:
                    l_lettre = 'P'
            else:
                if p_etat_etoile:
                    l_lettre = 'D'
                else:
                    l_lettre = 'S'
        elif p_couleur_fil == s_Fil_Rouge_Bleu:
            if p_etat_LED:
                if p_etat_etoile:
                    l_lettre = 'D'
                else:
                    l_lettre = 'S'
            else:
                if p_etat_etoile:
                    l_lettre = 'P'
                else:
                    l_lettre = 'S'
                    
        if l_lettre == 'C':
            l_retour = True
        elif l_lettre == 'D':
            l_retour = False
        elif l_lettre == 'S':
            if self.m_serie%2 == 0:
                l_retour = True
            else:
                l_retour = False
        elif l_lettre == 'P':
            if self.m_Port == s_Connecteur["Con_Pa"]:
                l_retour = True
            else:
                l_retour = False
        elif l_lettre == 'B':
            if self.m_Nb_Pile >= 2: 
                l_retour = True
            else:
                l_retour = False
        
        return l_retour
    
