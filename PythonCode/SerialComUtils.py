"""
Fichier contenant des méthodes utiles pour la com
Arduino -> Raspberry
"""

def conversionEntier(pCentaine, pDizaine, pUnite):
    return int(pCentaine) *100 + int(pDizaine) *10 + int(pUnite)
    
def conversionEntierToChar(pEntier):
    unite = int(pEntier%10)
    dizaine = int(((pEntier-unite)%100)/10)
    centaine = int((pEntier-10*dizaine-unite)/100)
    
    unite_char = chr((unite+48))
    dizaine_char = chr((dizaine+48))
    centaine_char = chr((centaine+48))
    
    return [centaine_char, dizaine_char, unite_char]
    
