Veuillez renseigner votre avancement à chaque étape achevée et testée et par défaut à la fin de chaque séance de TP

NOMs & prénoms du groupe étudiants : 
- Aude JEAN-BAPTISTE 
- Joel IMBERGAMO GUASCH 

# Avancement sur la Partie TP AIPS - Mise en oeuvre de l'application tsock

## Version 1 de tsock
L'objectif principal de la 1ère séance est de développer la v1 de tsock: coder une version de la source et une version du puit capables d'échanger des messages en utilisant le service UDP.  Vous pouvez décomposer le travail en deux parties successives: la version "source" de "tsock" puis celle du "puit". Vous pouvez tester chaque étape de manière isolée, en utilisant la version enseignant de tsock (en la configurant comme puit, pour tester votre source ou en tant que sourcenetcat , pour votre version du puit).  Cela néanmoins suppose que vous soyez connecté(e) sur une machine INSA via le client VPN. A défaut, vous pouvez  utiliser l'utilitaire nc (netcat), disponible sous les différents systèmes d'exploitation.

### Avancement sur la partie SOURCE UDP
veuillez cocher les seules cases qui correspondent aux étapes validées.

[x] la gestion des options -s, -u,  le nombre et Taille par défaut des messages , nom de la machine destinataire,  numéro de port du puit est opérationnelle

[x] la création du socket UDP local et la construction de l'adresse du socket distant est correcte

[x] l’envoi de  messages en utilisant l'appel système sendto (qui retourne une valeur >0 correspondant à la taille de chaque message envoyé) est fonctionnelle

[x] la réception côté puit (sur nc ou la version "enseignant" de tsock) des messages envoyés par votre client est correcte

### Avancement Point sur la partie puit UDP.

[x] la prise en compte de l' option -p est correcte

[x] la création du socket UDP local du puit et la construction de son adresse est correcte

[x] la réception et l'affichage des messages reçus sont corrects

[x] l'échange de messages tq décrits dans le cahier des charges entre vos versions source et puit de la version 1 de "tsock" est opérationnel

## Avancement sur la version 2 de tsock
La version 2 intègre l’utilisation de TCP pour l’échange des messages. Veuillez renseigner les étapes que vous avez traitées.

[x] la création du socket TCP local du puit et la construction de son adresse est correcte

[x] l'acceptation de la demande d'établissement de connexion de la primitive "accept" est effective

[x] la réception et l'affichage des messages reçus  (en utilisant nc ou la version "enseignant" de tsock comme source)  sont corrects

[x] l'échange de messages entre vos versions source et puit de la version 1 de "tsock" est opérationnel

## Avancement version 3 de tsock

La version v3 de tsock intègre les fonctions de formatage et d'affichage des messages émis et reçus (selon le cahier des charges) ainsi que la gestion des options restantes : -n  et -l. Si toutes les fonctionnalités additionnelles de la version 3 sont validées par vos tests, vous devez cocher toutes les cases suivantes. Si ce n'est pas le cas, veuillez cocher les seules cases qui correspondent aux étapes validées.

[x] Le formattage et affichage des messages selon le cahier des charges sont opérationnels

[x] la prise en compte de l'option -n en émission aussi bien avec UDP et TCP est correcte

[x] la prise en compte de l'option -n en réception avec UDP est correcte

[x] la prise en compte de l'option -n en réception avec TCP est correcte

[x] la prise en compte de l'option -l est correcte

