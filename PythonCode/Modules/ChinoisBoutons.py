'''
Created on 10 juin 2018

@author: meo
'''
# Contient les séquences des 5 colonnes de symboles
from Modules.Constantes import s_Id_Chi_A, s_Id_Chi_B, s_Id_Chi_C, s_Id_Chi_D,\
    s_Succes
import random
import operator
from Commun.ConstantesMain import s_Chinois_Boutons
Matrice_Symbole = [
    [21 ,2 ,13 ,16 ,2 ,23 ,13],
    [15 ,24 ,10 ,0 ,26 ,11 ,17],
    [3 ,23 ,15 ,29 ,2 ,4 ,24],
    [17 ,12 ,13 ,6 ,3 ,5 ,27 ],
    [25 ,17 ,3 ,8 ,13 ,20 ,2 ],
    [14 ,10 ,23 ,29 ,20 ,19 ,10]
]

class ChinoisBoutons:
    def __init__(self, p_ser, p_Buzzer):
        
        # Valeur du port série
        self.m_ser = p_ser
        
        # Pointeur vers lebuzzer
        self.m_Buzzer = p_Buzzer
        
        self.m_Solution = { '1' : 0, '2' : 0, '3' : 0, '4' : 0}
        self.m_solution_tab = []
        
        # Module est désarmoce 
        self.m_module_desamorce = False
        
        self.m_sequence = 0
        
    def InitialisationModule(self):
        # Valeur lors de l'init
        # On envoie un chiffre entre 0 et 29 pour chaque bouton
        # Celui ci correspondra au symbole choisi
        
        # Selection de la colonne a uitliser
        l_colonne = random.randint(0,5)
        l_colonne_chiffre = Matrice_Symbole[l_colonne]
        
        print(l_colonne_chiffre)
        l_contenu = []
        
        
        l_charactere_str, self.m_Solution['1'] = self.ChoisirChar(l_colonne_chiffre)    
        l_contenu.append(s_Id_Chi_A + "0")
        l_contenu.append(l_charactere_str[1])
        l_contenu.append(s_Id_Chi_A + "1")
        l_contenu.append(l_charactere_str[0])
        l_charactere_str, self.m_Solution['2'] = self.ChoisirChar(l_colonne_chiffre) 
        l_contenu.append(s_Id_Chi_B + "0")
        l_contenu.append(l_charactere_str[1])
        l_contenu.append(s_Id_Chi_B + "1")
        l_contenu.append(l_charactere_str[0])
        l_charactere_str, self.m_Solution['3'] = self.ChoisirChar(l_colonne_chiffre) 
        l_contenu.append(s_Id_Chi_C + "0")
        l_contenu.append(l_charactere_str[1])
        l_contenu.append(s_Id_Chi_C + "1")
        l_contenu.append(l_charactere_str[0])
        l_charactere_str, self.m_Solution['4'] = self.ChoisirChar(l_colonne_chiffre) 
        l_contenu.append(s_Id_Chi_D + "0")
        l_contenu.append(l_charactere_str[1])
        l_contenu.append(s_Id_Chi_D + "1")
        l_contenu.append(l_charactere_str[0])
        
        self.m_ser.construireMessage(s_Chinois_Boutons,l_contenu)
        self.m_solution_tab  = sorted(self.m_Solution.items(), key=operator.itemgetter(1)) 
        print("Solution module chinois (pressez du plus petit au plus grand)", self.m_solution_tab )
        
    def ChoisirChar(self, p_Colonne):
        l_int = random.randint(0,6)
        while l_int in self.m_Solution.values():
            l_int = random.randint(0,6)
        l_charactere = p_Colonne [l_int]   
            
        if l_charactere < 10:
            l_charactere_str = "0" + str(l_charactere)
        else:
            l_charactere_str = str(l_charactere)
        return l_charactere_str , l_int

    def LireMessage(self, p_Buffer):
        """
        Lit le message de l'arduino
        """
        if not self.m_module_desamorce :
            for i, l_id in enumerate(p_Buffer):
                if l_id[0] == 'B':
                    if int(p_Buffer[i+1]) == 1:
                    # Un bouton a bouger$
                        if l_id[1] == self.m_solution_tab[self.m_sequence][0]:
                            self.m_sequence += 1
                            if self.m_sequence == 4:
                                self.Succes()
                        else:
                            self.m_Buzzer.SonnerErreur()
                            self.m_sequence = 0
                
    def Succes(self):
        """
        Méthode chargée d'envoyer le message de succes
        """
        l_contenu = []
        l_contenu.append(s_Succes)
        
        self.m_ser.construireMessage(s_Chinois_Boutons,l_contenu)
        
            