'''
Created on 5 juin 2018

@author: meo

Contient la classe Cle qui pend
'''
from Modules.Constantes import s_Id_Vis, s_Id_Anneau, s_Id_Cle, s_Succes,\
    s_Perdu
from Commun.ConstantesMain import s_Cle_Qui_Pend

class CleQuiPend:
    def __init__(self, p_ser, p_Buzzer):
        
        #Lien vers le module serial
        self.m_ser = p_ser
        
                # Module désarmocé ?
        self.m_module_desamorce = False
        
        # Module initialsé
        self.m_init = True
        
        #Lien vers le buzzer
        self.m_buzzer = p_Buzzer
        
        # Les vis
        self.m_Vis1 = 1
        self.m_Vis2 = 1
        
        # Compteur qui détecte le nombre de contacte sur l'anneau
        self.m_nb_anneau = 0
        
        # Nombre de contact apres lequel on a perdu
        self.m_contact = 10
        
        # L'interupteur a cle
        self.m_interupteur = 0
        
        
    def LireMessage(self, p_Buffer):
        """
        Lit le message de l'arduino
        """
        if not self.m_module_desamorce :
            for i, l_id in enumerate(p_Buffer):
                if l_id  == s_Id_Vis + "1":
                    if int(p_Buffer[i+1]) == 0 and self.m_Vis1 == 1:
                        self.m_buzzer.SonnerStress()
                        self.m_Vis1 = 0
                elif l_id  == s_Id_Vis + "2":
                    if int(p_Buffer[i+1]) == 0 and self.m_Vis2 == 1:
                        self.m_buzzer.SonnerStress()
                        self.m_Vis2 = 0
                elif l_id == s_Id_Anneau + "0":
                    if int(p_Buffer[i+1]) == 1 : 
                        self.m_nb_anneau += 1
                        print("Anneau", self.m_nb_anneau)
                elif l_id == s_Id_Cle + "0":
                    self.m_interupteur = int(p_Buffer[i+1])
                    
            # Gestion de gagne ou perdu
            if self.m_nb_anneau > self.m_contact:
                self.Perdu()
                self.m_module_desamorce = True
            elif self.m_interupteur == 1:
                self.Succes()
                self.m_module_desamorce = True
                
    def Succes(self):
        """
        Méthode chargée d'envoyer le message de succes
        """
        l_contenu = []
        l_contenu.append(s_Succes)
        
        self.m_ser.construireMessage(s_Cle_Qui_Pend,l_contenu)
        
    
    def Perdu(self):
        """
        Méthode chargée d'envoyer le message de perdu
        """
        l_contenu = []
        l_contenu.append(s_Perdu)
        
        self.m_ser.construireMessage(s_Cle_Qui_Pend,l_contenu)
        
        self.m_buzzer.SonnerErreur()
        pass
            
        
