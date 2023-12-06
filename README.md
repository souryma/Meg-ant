# Meg-ant
Repository for google ia ant project 

# Coding style

Tous les membres de classes (y compris les méthodes) commencent par une majuscule. En plus de ça les membres privés débuteront avec un underscore.

Par exemple

```c++
class Ant
{
private:
    bool _Attr1;
    
public:
    int Attr2;

protected:
    Ant();
};
```

## Stratégie projet ants :

Garder des fourmis proches de la fourmilière 
Quelques fourmis partent en expédition pour chercher la nourriture
Garder ratio fourmis exploratrice/fourmis défensive

Si une fourmis exploratrice se rapproche d'ennemis, elle fuis dans la direction opposée des ennemis

Rechercher formation idéale pour fourmis exploratrices (3 fourmis espacées ?)

Lorsqu'une fourmis éclaireuse détecte une fourmis ennemi, elle marque l'endroit ou est l'ennemi.

Si une fourmis détecte une fourmilière, elle marque sa position.


Groupe de fourmis attaquantes :
- 5 fourmis 
- formation idéale pour attaquer
- Attaque les endroits ou une fourmilière a été trouvée
-  

http://ants.aichallenge.org/strategy_guide.php


Classes de fourmis :
- Exploratrice : vas à la nourriture la plus proche OU va tout droit/suis les bords
- Gardienne : Reste autours de la fourmilière (en cercle) et *s'éloigne de plus en plus si il n'y a pas d'ennemis autours*
- Attaquante : Se me en formation avec d'autres attaquantes et va à la fourmilière ennemi la plus proche

Si nouvelle fourmis nait : elle devient gardien
Si il y a trop de gardien : un des gradien devient explorateur
Après un certain délais : On créé une formation de fourmis attaquantes depuis les fourmis gardiennes



