'''
Created on 3 juin 2018

@author: meo
'''
import random
from Modules.Constantes import s_Id_LED, s_Id_Compteur, s_Succes, s_Perdu
from Commun.ConstantesMain import s_Int_LED_Compteur

class InterupteurLEDCompteur:
    def __init__(self, p_ser, p_Buzzer):
        
        #Etat initial des LEDs
        self.m_LEDs = [0, 0, 0, 0, 0]
        
        #Etat initial des LEDs pour initialisation
        self.m_LEDs_ini = [0, 0, 0, 0, 0]
        
        #Lien vers le module serial
        self.m_ser = p_ser
        
        #Etat inital des interupteurs
        self.m_interupteurs = {'I1' : 5,'I2' : 5,'I3' : 5,'I4' : 5,'I5' : 5}
        
        # Tableau d'état des interrupteurs permettant d'obtenir l'intialisation
        self.m_interrupteurs_init = [0, 0, 0, 0, 0]
        
        # Nombre de coup 
        self.m_nb_coups = 3
        
        # Module désarmocé ?
        self.m_module_desamorce = False
        
        # Module initialsé
        self.m_init = True
        
        #Lien vers le buzzer
        self.m_buzzer = p_Buzzer
        
        
    def GenererScenario(self):
        """
        Cette méthode va générer la solution
        """
        l_resolution_echoue = True
        # initialisation des états des interrupteurs  et LEDs pour la séquence d'init
        while l_resolution_echoue : 
        
            for l_key, l_value in self.m_interupteurs.items(): 
                self.m_interrupteurs_init[int(l_key[1]) - 1] = l_value
                self.m_LEDs_ini[int(l_key[1]) - 1] = self.m_LEDs[int(l_key[1]) - 1] 
            
            for l_coup in range(self.m_nb_coups) :
                #Choix aléatoir d'un intérupteur à changer : 
                l_int_a_changer = random.randint(0,4)
                # Update de l'état des LEDs
                self.ChangementEtatInt(l_int_a_changer , self.m_interrupteurs_init[l_int_a_changer], True)
                # Verification que l'on a obtenu un etat valable (pas  0 0 0 0 0 )
                if 1 not in self.m_LEDs_ini : 
                    l_resolution_echoue = True
                
                
                self.m_interrupteurs_init[l_int_a_changer] = self.ChangerEtat(self.m_interrupteurs_init[l_int_a_changer])
                
                print("Solution ILC coup", l_coup, "Interrupteur déplacé  : ", l_int_a_changer + 1)
                
            l_resolution_echoue = False
        
        # Mise en place de la solution
        # Mise à jour de l'état des LEDs
        for i, l_val in enumerate(self.m_LEDs_ini):
            self.m_LEDs[i] = self.ChangerEtat(l_val)
        
        for l_key, l_value in self.m_interupteurs.items(): 
                self.m_interrupteurs_init[int(l_key[1]) - 1] = l_value
                
        self.EnvoyerMessageLEDs()
            
        
    def LireMessage(self, p_Buffer):
        """
        Lit le message de l'arduino
        """
        if not self.m_module_desamorce :
            
            for i, l_id in enumerate(p_Buffer):
                if l_id in self.m_interupteurs.keys():
                    if self.m_init:
                        self.m_interupteurs[l_id] = int(p_Buffer[i+1])
                    else :  
                        l_numero_int = int(l_id[1]) -1
                        l_etat_int = int(p_Buffer[i+1])
                        if self.m_interupteurs[l_id] != int(p_Buffer[i+1]):
                            # Mise à jour de l'état des LEDs
                            self.ChangementEtatInt(l_numero_int , self.m_interupteurs[l_id], False)
                            self.m_nb_coups -= 1
                            
                            # Msie a jour du dico
                            self.m_interupteurs[l_id] = int(p_Buffer[i+1])
                            
                            self.EnvoyerMessageLEDs()
                            
                            # Verification si le joueur a gagné ou perdu
                            if (self.m_nb_coups == 0 and 0 not in self.m_LEDs):
                                # On a gagngé
                                self.Succes()
                                self.m_module_desamorce = True
                            elif (self.m_nb_coups == 0 and 0 in self.m_LEDs):
                                # On a perdu
                                self.Perdu()
                                self.m_module_desamorce = True

            #Fin de la phase d'initialisation, génration de la solution        
            if 5 not in self.m_interupteurs.values() and self.m_init:
                self.GenererScenario()
                self.m_init = False
    
    def ChangerEtat(self, p_ancien_etat):
        if p_ancien_etat == 0:
            return 1
        else:
            return 0
    
    def ChangementEtatInt(self, p_num_int, p_changement, p_is_init):
        """
        p_num_int : numéro de l'int entre 0 et 4
        
        p_changement : 1 si interrupteur passe de 1 à 0
                       0 si interrupteur passe de 0 à 1
        """ 
        if p_is_init:
            l_LEDS = self.m_LEDs_ini
        else:
            l_LEDS = self.m_LEDs
        if p_changement == 1 : 
            # On passe de 1 à 0 => changement de la LED correspondat à cet interrupteur
            l_LEDS[p_num_int] = self.ChangerEtat(l_LEDS[p_num_int])
        elif p_changement == 0 : 
            if p_num_int ==  0:
                l_LEDS[p_num_int + 1] = self.ChangerEtat(l_LEDS[p_num_int + 1])
            elif p_num_int ==  4:
                l_LEDS[p_num_int - 1] = self.ChangerEtat(l_LEDS[p_num_int - 1])
            else:
                l_LEDS[p_num_int + 1] = self.ChangerEtat(l_LEDS[p_num_int + 1])
                l_LEDS[p_num_int - 1] = self.ChangerEtat(l_LEDS[p_num_int - 1])
        else:
            raise ValueError("ChangementEtatInt : p_changement doit valoir 0 ou 1")
    
    def EnvoyerMessageLEDs(self):
        """
        Envoi un message a l'arduino pour initialiser les LEDs
        """
        l_contenu = []
        for i, l_LED in enumerate(self.m_LEDs):
            l_contenu.append(s_Id_LED+str(i+1))
            l_contenu.append(str(l_LED))
        l_contenu.append(s_Id_Compteur + "0")
        l_contenu.append(str(self.m_nb_coups))
        self.m_ser.construireMessage(s_Int_LED_Compteur,l_contenu)
        
    def Succes(self):
        """
        Méthode chargée d'envoyer le message de succes
        """
        l_contenu = []
        l_contenu.append(s_Succes)
        
        self.m_ser.construireMessage(s_Int_LED_Compteur,l_contenu)
        
    
    def Perdu(self):
        """
        Méthode chargée d'envoyer le message de perdu
        """
        l_contenu = []
        l_contenu.append(s_Perdu)
        
        self.m_ser.construireMessage(s_Int_LED_Compteur,l_contenu)
        
        self.m_buzzer.SonnerErreur()
        pass
            
                
        
    
    