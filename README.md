# Escape_bomb
Ce projet contient le code source d'un jeu d'escape game basé sur le jeu vidéo "keep talking and nobody explodes".

Les joueurs forment 2 équipes:
* Un démineur
* Une équipe d'ingénieur

Le démineur se place d'un coté de la table avec la bombe, les ingénieurs de l'autre côté avec le manuel expliquant comment désarmorcer le dispositif.

Dès que le démineur ouvre la malette, le chrono commence : l'équipe a 10 min pour désarmorser les 5 modules en faisant moins de 3 fautes. Mais de nombreuses surprises attende les joueurs au cours de la partie...

Un visuel du dispositif terminé est disponible ici : https://youtu.be/4wBnja9f7Bo

# Architecture

Le dispositif contient une rasperry pi qui execute le code en python qui communique via un protocole série avec 2 arduino méga. Celle-ci adresse ensuite tous les capteurs et remontent les informations à la raspberry pi.

Le code PythonCode est à déposer sur la rasberry, le arduino code est à compiler puis injecter sur les arduinos mégas
