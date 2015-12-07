#include "header.h"
#include "header_allegro.h"
#include <winalleg.h>

//////////////////////////////////////////
// Nom : Objets
// Fonctions : Gère les cases objets 
// Entrées Emplacement
//         Précedent
//         Potion
//         Tableau de valeurs
//         Tableau de changement
//         Direction en X
//         Direction en Y
//         Tableau d'affichage des BITMAPS
//         Marteau
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.3
////////////////////////////////////////
void objets(int *emplacement, int *precedent, int* potion, char tab[15][19], int changement [50][2], int direction_x, int direction_y, char affiche[15][19], int* marteau)
{
    int i,j;
    int buffer,buffer2;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    switch (*precedent) //Repère le case sur lequel le joueur vient d'arriver
    {
    case 4: //Potion 

        *potion= *potion+6;//Valable 6 cases d'affilés
        *precedent=0;
        break;
    case 5: // le rocher
        // récupération des coordonnées du rocher
        changement[*emplacement][0]=changement[*emplacement-1][0] ;
        changement[*emplacement][1]= changement[*emplacement-1][1];
        // déplacement das la direction où il est poussé
        changement[*emplacement-1][0]+= direction_x;
        changement[*emplacement-1][1]+= direction_y;
        while(tab[changement[*emplacement-1][1]+direction_y][changement[*emplacement-1][0]+direction_x]>=0) //tant que la case suivante est positive
        {
            // on fait avancer le rocher
            tab[changement[*emplacement-1][1]+direction_y][changement[*emplacement-1][0]+direction_x]=tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]];
            // on met de l'eau a la place du rocher
            tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]]=-1;
            // on se postionne pour réafficher en direct
            gotoligcol(POS_ECRAN_Y+changement[*emplacement-1][1],POS_ECRAN_X+3*(changement[*emplacement-1][0]));
            update_tab_affiche(tab, affiche);
            printf(" %c", affiche[changement[*emplacement-1][1]][changement[*emplacement-1][0]]);
            // on incrémente les cordonnées du rocher
            changement[*emplacement-1][0]+= direction_x;
            changement[*emplacement-1][1]+= direction_y;
        }
        tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]]=-1;
        // le rocher disparait au profit de l'eau
        *precedent=0;
        // enregistre que le joueur est au-dessus de la glace
        (*emplacement)++;
        // on retiens les cases a réafficher
        break;
    case 8: // le turbo (découvert par le rocher)

        changement[*emplacement][0]=changement[*emplacement-1][0] ;
        changement[*emplacement][1]= changement[*emplacement-1][1]; // on récupère les coordonées
        while(tab[changement[*emplacement-1][1]+direction_y][changement[*emplacement-1][0]+direction_x]>=0) // tant que le personnage ne rencontre pas de mur
        {
            buffer2=tab[changement[*emplacement-1][1]+direction_y][changement[*emplacement-1][0]+direction_x]; // on enregistre la case qui est passée
            tab[changement[*emplacement-1][1]+direction_y][changement[*emplacement-1][0]+direction_x]=tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]]; // on incrémente la position du personnage
            tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]]=buffer; // on remet la bonne case
            buffer=buffer2; // on remplace la case précédente
            // on se place pour afficher
            gotoligcol(POS_ECRAN_Y+changement[*emplacement-1][1],POS_ECRAN_X+2*changement[*emplacement-1][0]);
            update_tab_affiche(tab, affiche); 
            // on affiche
            printf(" %c", affiche[changement[*emplacement-1][1]][changement[*emplacement-1][0]]);
            // on incrémente la position
            changement[*emplacement-1][0]+= direction_x;
            changement[*emplacement-1][1]+= direction_y;
            // on place le joueur
            tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]]=2;

        }
        // on retiens que le joueur est sur une case de glace
        *precedent=0;
        (*emplacement)++;

        break;
    case 9://Marteau
        (*marteau)+=6; //Valabe 6 coups
        *precedent=0;
        break;
    case -2://Mur
        (*marteau)--;//Utilise un coup de marteau
        *precedent=0;
    }
    if (*precedent>100)//Téléporteur
    {
        i=0;
        j=0;
        for(i=0; i<15; i++)
        {
            for(j=0; j<19; j++)
            {

                if ((tab[i][j]==*precedent+10)||(tab[i][j]==*precedent-10))//Recherche deux cases qui ont 10 d'écarts
                {
                    tab[i][j]=2;//Y place ke joueur
                    tab[changement[*emplacement-1][1]][changement[*emplacement-1][0]]=-1;
                    changement[*emplacement][1]=i;
                    changement[*emplacement][0]=j;
                    (*emplacement)++;
                    *precedent=0;
                }
            }
        }
    }
}

//////////////////////////////////////////
// Nom : Initialisation Rec
// Fonction : Initialise les niveaux du mode graphique créé par l'utilisateur
// Entrées :  Fichier Niveau Rec
//            Tableau de valeurs
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.1
/////////////////////////////////////////
void intitialisation_rec(FILE* niveau_rec, char tab[15][19])
{
    int i,j;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            fscanf(niveau_rec, "%d", (int*)&tab[i][j]);//Lit le niveau enregistré case à case
        }
    }
}
//////////////////////////////////////////
// Nom : Recherche Joueur
// Fonction : Recherche la position du joueur
// Entrées : Tableau de valeurs
//           Pointeur sur position X
//           Pointeur sur position Y
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.1
/////////////////////////////////////////
void recherche_joueur(char valeurs[15][19], int* x, int*y)
{
    int i,j;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (valeurs[i][j]==2)//Trouve le joueur
            {
                *y=i;
                *x=j;
            }
        }
    }
}
//////////////////////////////////////////
// Nom : Recherche Sortie Et Ennemies
// Fonction : Recherche des sorties, des tunnels et des ennemies sur la carte
// Entrées : Tableau de valeurs
//           Pointeur sur Xsortie
//           Pointeur sur Ysortie
//           Pointeur sur nombre de tunnel
//           Pointeur sur nombre ennemis
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.7
/////////////////////////////////////////
void recherche_sortie_et_ennemis(char tab[15][19], int* xsortie, int* ysortie, int* tunnel, int* nb_ennemis)
{
    int i,j;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {

            if(tab[i][j]==3)// Trouve la sortie
            {
                *ysortie=i;
                *xsortie=j;
            }
            if (tab[i][j]==7)//trouve le tunnel
            {
                *ysortie=i;
                *xsortie=j;
                *tunnel=1;
            }
            if (tab[i][j]==66) (*nb_ennemis)++;//compte le nombre d'ennemies
        }
    }
}
//////////////////////////////////////////
// Nom : Update Cases Glaces
// Fonction : Compte le nombre de cases de glaces au départ
// Entrées : Tableau de valeurs
// Sorties : Nombre de cases non accessible au joueurs
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 1.1
/////////////////////////////////////////
int update_cases_glace(char tab[15][19])
{
    int i,j;
    int val=0;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if((tab[i][j]>=0)||(tab[i][j]==-50)) val++;//nombre de case où l'on peut aller
            if(tab[i][j]==9) val+=6;//Rajoute 6 car présence d'un marteau
        }
    }
    return val;
}
//////////////////////////////////////////
// Nom : Update Cases Eau
// Fonction : Compte a tout moment le nombre de cases d'eau, le joueur et les ennemies
// Entrées : Tableau de valeurs
// Sorties : Nombre de cases positives
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.1
/////////////////////////////////////////
int update_cases_eau(char tab[15][19])
{
    int i,j;
    int val=0;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if((tab[i][j]==-1)||(tab[i][j]==2)||(tab[i][j]==66)) val++;//Nombre de cases où l'on ne peut aller
        }
    }
    return val;
}

//////////////////////////////////////////
// Nom : Initialisation Console
// Fonction : Affiche tout le jeu
// Entrées : Tableau qui a les valeurs du jeu pour la console
//           Score général
//           Score du niveau
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 3.1
/////////////////////////////////////////
void initialisation_console(char affiche_console[15][19], int score, int score_niveau)
{
    int i,j;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    system("cls");
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    for(i=0; i<15; i++)
    {
        gotoligcol(POS_ECRAN_Y+i,POS_ECRAN_X);
        for(j=0; j<19; j++)
        {
            printf(" %c ", affiche_console[i][j]);// affiche le jeu
        }
        if (i==0) printf("score: %d", score);// affiche le score général sur la première ligne
        if (i==1) printf("score niveau: %d", score_niveau);//Affiche le score du niveau sur la 2e ligne
        printf("\n");
    }
}

//////////////////////////////////////////
// Nom : Affichage score
// Fonction : Affichage du score durant la partie
// Entrées : Pointeur sur score
//           Score niveau
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 1.8
///////////////////////////////////////////
void affichage_score(int* score, int score_niveau) //Affichage du score pendant la partie
{
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    gotoligcol (POS_ECRAN_Y+0, POS_ECRAN_X+64);//affiche score général
    printf ("     ");
    gotoligcol (POS_ECRAN_Y+0, POS_ECRAN_X+64);
    printf ("%d",*score);
    gotoligcol (POS_ECRAN_Y+1,POS_ECRAN_X+70);//affiche score niveau
    printf ("     ");
    gotoligcol (POS_ECRAN_Y+1,POS_ECRAN_X+70);
    printf ("%d",score_niveau);
}

//////////////////////////////////////////
// Nom : Initialisation Niveau
// Fonction : Initialise un niveau pédéfinis et change les valeurs du tableau de valeurs
// Entrées : Fichier du niveau
//           Tableau de valeurs
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.6
/////////////////////////////////////////
void initialisation_niveau(FILE* fichier, char valeurs[15][19])
{
    int i,j,k;
    int coorx_debut, coory_debut, coorx, coory;
    int cases_spe, val_spe, zones_glaces;
    k=0;
    j=0;
    while( k<15)//Met tout le tableau a la valeur de mur
    {
        j=0;
        while( j<19)
        {
            valeurs[k][j]=-2;
            j++;
        }
        k=k+1;
    }
 ;//Nombre de zones de glaces
    for(i=0; i<zones_glaces; i++   fscanf(fichier,"%d",&zones_glaces))
    {
        fscanf(fichier,"%d",&coorx_debut);//coordonnée x de la case de debut du rectangle
        fscanf(fichier,"%d",&coory_debut);//coordonnée y de la case de debut du rectangle
        fscanf(fichier,"%d",&coorx);//Longueur x du rectangle
        fscanf(fichier,"%d",&coory);//longueur y du rectangle


        j=0;
        k=0;
        while(abs(j)<abs(coorx))
        {
            k=0;
            while(abs(k)<abs(coory))
            {
                if((coory_debut+k <15) && (coorx_debut+j<19) && (coory-_ebut+k>=0) && (coorx_debut+j>=0)) valeurs[coory_debut+k][coorx_debut+j]=0;//Met la zone a glace
                k=k+abs(coory)/coory;//pour pouvoir tracer vers des chiffres négatifs
            }
            j=j+abs(coorx)/coorx;
        }
    }
    i=0;
    fscanf(fichier,"%d",&cases_spe);//nombre de case spéciales

    for(i=0; i<cases_spe; i++)
    {
        fscanf(fichier,"%d",&coorx);//coordonnée x de la case spéciale
        fscanf(fichier,"%d",&coory);// coordonnée y de la case spéciale
        fscanf(fichier,"%d",&val_spe);// valeur de la case
        if((coory<15) && (coorx<19) && (coory>=0) && (coorx >=0)) valeurs[coory][coorx]=val_spe;
    }
}

//////////////////////////////////////////
// Nom : Deplacement
// Fonction : Repère le joueur et détecte l'appuis sur une tocuhe et enregistre donc les valeurs permettant le déplacement du joueur
// Entrées : Tableau de valeurs
//           Pointeur sur  nombre clef
//           Score général
//           Pointeur sur score
//           Pointeur sur valeur de la case précédente
//           Tableau de changements
//           Pointeur sur emplacement qui compte le nombre de cases ayant changé de valeurs
//           Pointeur sur nombre de potions
//           Tableau d'affichage des valeurs pour BITMAPS
//           Niveau
//           Pointeur sur nombre de marteau
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 10.9
////////////////////////////////////////// 
void deplacement(char tab[15][19], int *clef,int score_general, int* score, int *precedent, int changement[50][2], int* emplacement, int* potion, char affiche[15][19], int niveau, int* marteau)
{
    int i,j,x,y,z=1;
    char move;
    update_tab_affiche(tab, affiche);
    int test=0;
//on repère la case avec le joueur
    recherche_joueur(tab,&x,&y);
    int direction_x=0, direction_y=0;
    char case_suivante_joueur;
//si l'utilisateur appuie sur une case
    if (kbhit())//évenementiel
    {

        changement[*emplacement][0]=x;
        changement[*emplacement][1]=y;
        *emplacement= *emplacement +1;

        move=getch();
        switch (move)
        {
        case 's'://pour aller en bas
        case '2':
            case_suivante_joueur=tab[y+1][x];
            changement[*emplacement][0]=x;
            changement[*emplacement][1]=y+1;
            direction_y=1;
            test++;
            break;

        case 'z'://pour aller en haut
        case '8':
            case_suivante_joueur= tab[y-1][x];
            changement[*emplacement][0]=x;
            changement[*emplacement][1]=y-1;
            direction_y=-1;
            test++;
            break;

        case 'q'://pour aller à gauche
        case '6':
            case_suivante_joueur= tab[y][x-1];
            changement[*emplacement][0]=x-1;
            changement[*emplacement][1]=y;
            direction_x=-1;
            test++;
            break;

        case 'd'://pour aller à droite
        case '4':
            case_suivante_joueur= tab[y][x+1];
            changement[*emplacement][0]=x+1;
            changement[*emplacement][1]=y;
            direction_x=-1;
            test++;
            break;

        case 'x'://quitte le jeu
            system("cls");
            fin_de_niveau(niveau);
            break;
        case 'h'://affiche le menu d'aide
            afficher_menu();
            system("pause");
            initialisation_console(affiche, score_general, *score);
        default :
            (*emplacement)--;
        }
        if ((changement[*emplacement][0]>=0)&&(changement[*emplacement][0]<19)&&(changement[*emplacement][1]>=0)&&(changement[*emplacement][1]<15)&&(test))//vérification des conditions pour un potentiel déplacement(cases suivantes dans le tableau et bouton appuyer valable)
        {
            if ((case_suivante_joueur>=0)||((case_suivante_joueur==-2)&&(*marteau)))
            {
                if(*potion>0)
                {
                    z=0;
                    (*potion)--;
                }
                //on décale le personage et diminue la glace derrière lui
                i=case_suivante_joueur;
                tab[changement[*emplacement][1]][changement[*emplacement][0]]=tab[y][x];
                *precedent= *precedent-z;
                tab[y][x]=*precedent;
                *precedent=i;
                *emplacement= *emplacement +1;
                // test de la présence d'une clef
                if (*precedent== 100)
                {
                    *clef= *clef+1;
                    *precedent=0;
                }
            }
            if ((case_suivante_joueur==-50)&&(*clef>0))
            {
                (*clef)--;
                tab[changement[*emplacement][1]][changement[*emplacement][0]]=tab[y][x];
                tab[y][x]=*precedent-z;
                *precedent=0;
                *emplacement= *emplacement +1;

            }
            objets(emplacement, precedent, potion, tab, changement, direction_x, direction_y, affiche, marteau);
        }

    }
}

//////////////////////////////////////////
// Nom : Test die
// Fonction : Vérifie si le déplacement est possible
// Entrées : Tableau de valeurs
//           Clef
//           Niveau
//           Pointeur sur score
//           Précedent
//           Mode-graphique si 0 mode console, 1 mode graphique normal et 2 star wars
//           Marteau
// Sorties : Retourne une valeur en fonction du mode souhaité
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 2.5
/////////////////////////////////////////
int TEST_DIE( char tab[15][19],int clef, int lvl, int *score, int precedent, int mode_graphique, int marteau)
{
    int i,j,x, y;
//on repère la case avec le joueur
    recherche_joueur(tab,&x,&y);

//test de la condition de mort: perso entouré de cases a valeur négatives comme mur,eau,serrure sans clé,...
    if (((tab[y+1][x]==-2)||(tab[y-1][x]==-2)||(tab[y][x+1]==-2)||(tab[y][x-1]==-2))&&(marteau)) return 0;
    if (((((tab[y+1][x]<0)||(y+1>14))&&((tab[y-1][x]<0)||(y-1)<0)&&((tab[y][x+1]<0)||(x+1>18))&&((tab[y][x-1]<0)||(x-1<0))&&(clef==0))||
         ((((tab[y+1][x]<0)||(y+1>14))&&((tab[y-1][x]<0)||(y-1)<0)&&((tab[y][x+1]<0)||(x+1>18))&&((tab[y][x-1]<0)||(x-1<0)))&&
          (((tab[y+1][x]!=-50)||(y+1>14))&&((tab[y-1][x]!=-50)||(y-1<0))&&((tab[y][x+1]!=-50)||(x+1>18))&&((tab[y][x-1]!=-50)||(x-1<0)))))
    ||(precedent==66))
    {
        switch(afficher_mort(score, lvl,  mode_graphique))
        {
        case 1:
            return 1;
        case 3:
            return 3;
        }

    }
    return 0;
}
//////////////////////////////////////////
// Nom : Afficher Mort
// Fonction : Affiche le message de mort en mode console et en mode graphique
// Entrées : Pointeur sur score
//           Niveau
//           Mode graphique
// Sorties : Retourne des valeurs en fonction de ce que je veux 0 rien ne se passe, 3 quitte le jeu et 1 recommence le niveau
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 3.2
/////////////////////////////////////////
int afficher_mort(int* score, int niveau, int mode_graphique)// 42 est le sens de la vie
{
    int j=1;
    int i=0;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y); // MLP c'est bien !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (mode_graphique)// affiche le message de mort en mode graphique
    {

        BITMAP* buffer;
        buffer=create_bitmap(TSPRITE*19, TSPRITE*15);
        clear_to_color(buffer, makecol(0,0,0));
        textprintf_ex(buffer, font, TSPRITE*9, TSPRITE*7, makecol(255,255,255),-1,"vous avez perdu! Votre score est de %d", *score );
        textprintf_ex(buffer, font, TSPRITE*9, TSPRITE*7+LIGNE+3,makecol(255,255,255),-1,"voulez-vous:");
        textprintf_ex(buffer, font, TSPRITE*9, TSPRITE*7+2*(LIGNE+3), makecol(255,255,255),-1,"1 quitter");
        textprintf_ex(buffer, font, TSPRITE*9, TSPRITE*7+3*(LIGNE+3), makecol(255,255,255),-1,"ou");
        textprintf_ex(buffer, font, TSPRITE*9, TSPRITE*7+4*(LIGNE+3), makecol(255,255,255),-1,"2 recommencer le niveau?");
        blit(buffer, screen, 0,0,0,0,TSPRITE*19, TSPRITE*15);

        while (j)
        {
            if(!((key[KEY_1])||(key[KEY_1_PAD])))//recommence le niveau
            {
                if(!((key[KEY_2])||(key[KEY_2_PAD])));
                else
                {
                    return 1;
                }

            }
            else// quitte le jeu
            {
                textprintf_ex(buffer, font, TSPRITE*9, TSPRITE*7+5*(LIGNE+3),makecol(255,255,255),-1,"au revoir!");
                blit(buffer, screen, 0,0,0,0,TSPRITE*19, TSPRITE*15);
                rest(200);
                return 3;
            }
        }

    }
    else
    {
        gotoligcol(POS_ECRAN_Y+15, POS_ECRAN_X);
        printf("\t\t Vous avez perdu! Votre score est de %d\n", *score);
        Sleep(300);
        j=1;
        while(j)
        {
            getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
            gotoligcol(POS_ECRAN_Y+16, POS_ECRAN_X);// Affiche le choix lors de la mort
            printf("Voulez-vous:");
            gotoligcol(POS_ECRAN_Y+17, POS_ECRAN_X);
            printf("1 quitter");
            gotoligcol(POS_ECRAN_Y+18, POS_ECRAN_X);
            printf("ou");
            gotoligcol(POS_ECRAN_Y+19, POS_ECRAN_X);
            printf("2 recommencer le niveau?");
            gotoligcol(POS_ECRAN_Y+20, POS_ECRAN_X);
            scanf("%d", &i);
            if (i==1)// quitte le jeu
            {
                printf("Au revoir!\n");
                Sleep(200);
                j=0;
                return 3;
            }
            if(i==2)//recommence le niveau
            {

                return 1;
            }
        }
    }
    return 0;//Ne fais rien #trempette
}

//////////////////////////////////////////
// Nom : Update Tab Affiche
// Fonction : Applique les valeurs  tableau de valeurs au tableau d'affichage
// Entrées : Tableau de valeurs
//           Tableau d'affichage
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 7.5
/////////////////////////////////////////
void update_tab_affiche(char tab[15][19], char affiche[15][19])
{
// declaration des variables
    int i,j;
// boucle sur tout le tableau
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            //affectation de la valeur du caractère dans le tableau d'affichage
            switch (tab[i][j])
            {
            case -2://les murs
                affiche[i][j]= 207;
                break;
            case 1://glace double
                affiche[i][j]= 127;
                break;
            case 0:// la glace
                affiche[i][j]=223;
                break;
            case -1://l'eau
                affiche[i][j]= 177;
                break;
            case -50://les serrures
                affiche[i][j]= 206;
                break;
            case 100://les clés
                affiche[i][j]= 197;
                break;
            case 2://le personnage
                affiche[i][j]= 208;
                break; 
            case 3://la sortie
                affiche[i][j]=209;
                break;
            case 66: // l'ennemi
                affiche[i][j]= 245;
                break;
            case 4: // la potion
                affiche[i][j]= 244;
                break;
            case 5: // le rocher
                affiche[i][j]= 246;
                break;
            case 7: //le tunnel
                affiche[i][j]= 'O';
                break;
            case 8:// le turbo
                affiche[i][j]=175;
                break;
            case 9:// le marteau
                affiche[i][j]=173;
            }
            if (tab[i][j]>100) affiche[i][j]= 203;//téléporteur
        }

    }
}
//////////////////////////////////////////
// Nom : Affichage
// Fonction : Applique les changement d'affichage et positionne les modifications
// Entrées : Tableau de changement
//           Tableau affichage
//           Emplacement
// Sorties :
// Date de dernièere modifictaion : 06/12/2015
// Nature de la dernière modification : Correction des warnings
///Versoin : 4.1
/////////////////////////////////////////

void affichage(int changement[50][2], char affiche[15][19], int emplacement)
{
    int i;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
//on repère la case avec le joueur

    for(i=0; i<emplacement; i++)
    {
        gotoligcol(POS_ECRAN_Y+changement[i][1],POS_ECRAN_X+3*changement[i][0]+1);
        printf("%c", affiche[changement[i][1]][changement[i][0]]);

    }
}

