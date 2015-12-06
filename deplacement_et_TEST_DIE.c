#include <allegro.h>
#include <winalleg.h>
#include "header_allegro.h"
#include "header.h"
///////////////////////////////////////////////////////////////////////
// Nom: afficher_intro
// utilité: affiche l'introduction du turotiel en mode graphique
// nombre de modifications: 2
// dernière modification: Commentaire, 06/12/2015
// entrées et sortie: la case sur laquelle le joueur est, le pointeur de potion, la direction du joueur,
// les tableaux de valeurs et d'affichage, les coordonées de la case suivante, le mode graphique et les scores
// sortie: la case par-dessus laquelle le joueur est après son déplacement
////////////////////////////////////////////////////////////////////////
int objets_graphique(int buffer, int* potion, int direction_x, int direction_y,BITMAP* affiche[15][19], char tab[15][19], int coord_case_suiv[2],int mode_graphique, int score_general, int* score, BITMAP* affiche2[NBSPRITE])
{
    //0 déclaration des variables
    int buffer1=0, buffer2; 
    switch (buffer)
        {

        case 4:
            // On incrémente le compteur de potion
            *potion= *potion+6;
            return 0;
            break;
        case 5:
            // le rocher se déplace vers là où il est poussé jusqu'à ce qu'il rencontre un mur
            coord_case_suiv[0]+= direction_x;
            coord_case_suiv[1]+= direction_y;
            // récupération du sens de déplacement
            while(tab[coord_case_suiv[1]+direction_y][coord_case_suiv[0]+direction_x]>=0) //tant que la case suivante est positive
            {
                tab[coord_case_suiv[1]+direction_y][coord_case_suiv[0]+direction_x]=tab[coord_case_suiv[1]][coord_case_suiv[0]];
                
                tab[coord_case_suiv[1]][coord_case_suiv[0]]=-1;
                // on met de l'eau a la place du rocher
                coord_case_suiv[0]+= direction_x;
                coord_case_suiv[1]+= direction_y;
                // on récupère la case suivante
                tab[coord_case_suiv[1]][coord_case_suiv[0]]=5;
                // on place le rocher
                update_affichage_graphique(tab, affiche, affiche2, mode_graphique);
                affichage_graphique(affiche, mode_graphique, score_general, *score, affiche2, tab);
                // on affiche les changements
                rest(50);
                // on pause pour ralentir le mouvement
            }
            tab[coord_case_suiv[1]][coord_case_suiv[0]]=-1;
            //le rocher laisse sa place a de l'eau
            return 0;
            break;

        case 8:
            // Le turbo, découvert par erreur en codant le rocher, projète le joueur contre le mur dans la direction qu'il a pris
            while(tab[coord_case_suiv[1]+direction_y][coord_case_suiv[0]+direction_x]>=0) //tant que la case suivante est positive
            {
                buffer2=tab[coord_case_suiv[1]+direction_y][coord_case_suiv[0]+direction_x]; //on sauvegarde la case suivante
                tab[coord_case_suiv[1]+direction_y][coord_case_suiv[0]+direction_x]=tab[coord_case_suiv[1]][coord_case_suiv[0]];
                //on fait avancer le joueur
                tab[coord_case_suiv[1]][coord_case_suiv[0]]=buffer1;
                // on remet la case précédente
                buffer1=buffer2;
                // on retiens la valeur de la case sauvegardée
                coord_case_suiv[0]+= direction_x;
                coord_case_suiv[1]+= direction_y;
                // on obtiens les coordonées de la case suivante
                tab[coord_case_suiv[1]][coord_case_suiv[0]]=2;
                // on met le personnage sur la case suivante
                update_affichage_graphique(tab, affiche, affiche2, mode_graphique);
                affichage_graphique(affiche, mode_graphique, score_general, *score, affiche2, tab);
                // on affiche
                rest(50);
                // on pause pour ralentir le mouvement
            }
            return 0;
            // on met la case précédente a "glace"
            break;
            case 100: // si c'est une clé
              (*clef)++;
              // incrément du nombre de clefs
               return 0;
               // la case précédente deviens de la glace
        }
        if (buffer>100)
        {
            // si c'est un téléporteur
            i=0;
            j=0;
            for(i=0; i<15; i++)
            {
                for(j=0; j<19; j++)
                {
        
                    if ((tab[i][j]==buffer+10)||(tab[i][j]==buffer-10)) // on cherche le teleporteur de sortie
                    {
                        tab[i][j]=2;
                        tab[coord_case_suiv[1]][coord_case_suiv[0]]=-1;
                        // on déplace le joueur et on détruis le téléporteur précédent
                        return 0;
                        // on met la case précédente a "glace"
                    }
                }
            }
        }
    return buffer;
    // on retourne la valeur de sauvegarde
}
///////////////////////////////////////////////////////////////////////
// Nom: afficher_intro
// utilité: affiche l'introduction du turotiel en mode graphique
// nombre de modifications: 2
// dernière modification: Commentaire, 06/12/2015
// entrées et sortie: +
//////////////////////////////////////////////////////////////////////
void afficher_intro()
{
    // 0 declaration de variables
    BITMAP* buffer;
    //1 initialisation
    buffer=create_bitmap(19*TSPRITE, 15*TSPRITE);
    clear_bitmap(buffer);
    //1 ecriture sur buffer et affichage
    textprintf_ex (buffer, font, 152, 170+ 0*(LIGNE+3), makecol(255, 255, 255),-1,"Bienvenue dans le tutoriel!");
    textprintf_ex (buffer, font, 152, 170+ 1*(LIGNE+3), makecol(255, 255, 255),-1,"Pendant le jeu appuyez sur h pour acceder aux regles");
    blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    while(!key[KEY_ENTER]);
}
///////////////////////////////////////////////////////////////////////
// Nom: afficher_menu_graphique
// utilité: affiche les regles en mode graphique
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 2
// entrées et sortie: +
///////////////////////////////////////////////////////////////////////

void afficher_menu_graphique()
{
    //0 declaration de variables
    BITMAP* buffer;
    int a=1;
    //1 initialisation
    buffer=create_bitmap(19*TSPRITE, 15*TSPRITE);
    clear_bitmap(buffer);
    //2 ecriture sur buffer et affichage
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1,"Regles :");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," pour vous deplacer utilisez touches z,q,s,d");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Lorsque vous passez sur un bloc de glace, il disparait.");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Il laisse place a de l'eau.");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Vous ne pouvez pas passer sur l'eau, vous entourer d'eau vous fera perdre");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Certains niveaux contiennent des doubles glaces : ");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," lorsque vous passez dessus, ils deviennent des blocs de glace simple.");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Certains niveaux contiennent des bonus utilisables ! ");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Les potions vous permettent de passer par-dessus la glace pour 6 mouvements!");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Les marteaux vous autorisent a detruire 6 murs!");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Certains niveaux contiennent des serrures !");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Des clef seront disponibles pour les ouvrir et terminer le niveau.");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Les rochers sont mobiles, ils detruisent tout sur leur passage.");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Attention a ne pas detruire de clef!");
    textprintf_ex (buffer, font, 0, 170+ a++*(LIGNE+3), makecol(255, 255, 255),-1," Amusez-vous !");
    blit(buffer, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    // libération de mémoire
    destroy_bitmap(buffer);
}
///////////////////////////////////////////////////////////////////////
// Nom: afficher_menu
// utilité: affiche les regles
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 2
// entrées et sortie: +
///////////////////////////////////////////////////////////////////////
void afficher_menu()
{
    system("cls");
    printf ("\tRegles :\n");
    printf (" Le but du jeu est d'atteindre la sortie. \n");
    printf (" Utilisez les touches 2,4,6,8 du pave ou z,q,s,d pour vous deplacer.\n");
    printf (" Lorsque vous passez sur un bloc de glace %c, il disparait.\n",223);
    printf (" Il laisse place a de l'eau %c. \n", 177);
    printf (" Vous ne pouvez pas passer sur de l'eau %c.\n",177);
    printf (" Si vous vous entourez d'eau, vous perdez. \n");
    printf (" Certains niveaux contiennent des doubles glaces %c :  \n", 127);
    printf (" lorsque vous passez dessus, ils deviennent des blocs de glace simple.\n");
    printf (" Certains niveaux contiennent des bonus utilisables ! \n");
    printf (" Les potions %c vous permettent de passer par-dessu la glace pour 6 mouvements! \n", 244);
    printf (" Les martaux %c vous autorisent a detruire 6 murs!\n", 173);
    printf (" Les rochers %c sont mobiles et detruisent tout sur leur passage:\n", 246);
    printf (" attention aux clef!\n");
    printf (" Certains niveaux contiennent des serrures %c!\n", 206);
    printf (" Des clef %c seront disponibles pour les ouvrir et terminer le niveau.\n", 197);
    printf (" Amusez-vous!\n");
}
///////////////////////////////////////////////////////////////////////
// Nom: fin_de_niveau
// utilité: termine le jeu et enregistre le niveau auquel le jouer s'arrête
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 3
// entrées :niveau
// sorties: +
///////////////////////////////////////////////////////////////////////
void fin_de_niveau(int niveau)
{
    FILE* sauvegarde;
    sauvegarde= fopen("niveausauvegarde.txt", "w");
    fprintf(sauvegarde, "%d", niveau);
    fclose(sauvegarde);
    exit(0);
}
///////////////////////////////////////////////////////////////////////
// Nom: deplacement_graphique
// utilité: gère le déplacement de l'ennemi
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 8
// entrées: le tableau de valeurs, le pointeurs de score
// sortie: +
///////////////////////////////////////////////////////////////////////
int deplacement_graphique(char tab[15][19], int* score, int* clef, int precedent, int* potion, BITMAP* affiche[15][19], BITMAP* affiche2[NBSPRITE], int mode_graphique, int score_general, int niveau, int* marteau)
{
    // 0 declaration des variables et blindages de touches appuyées en trop
    clear_keybuf();
    int i,j,x,y;
    int z=1;
    int direction_x=0, direction_y=0;
    int test=1, buffer=precedent, buffer1=-1, buffer2;
    int coord_case_suiv[2]= {0};
    recherche_joueur(tab,&x,&y);
    if(!key[KEY_W])
    {
        if(!key[KEY_S])
        {
            if(!key[KEY_A]) //allegro gère le clavier en qwerty z=w et a=q
            {
                if(!key[KEY_D])
                {
                    if(!key[KEY_X]) test=0; //pas de touche
                    else fin_de_niveau(niveau); // si l'utilisateur tape X le jeu sors
                }
                else
                {
                    // on récupère les coordonnées de la case sur laquelle le joueur souhaite aller
                    coord_case_suiv[0]=x+1;
                    coord_case_suiv[1]=y;
                    // on récupère sa direction
                    direction_x=1;
                }
            }
            else
            {
                // idem
                coord_case_suiv[0]=x-1;
                coord_case_suiv[1]=y;
                // idem
                direction_x=-1;
            }
        }
        else
        {
            // idem
            coord_case_suiv[0]=x;
            coord_case_suiv[1]=y+1;
            //idem
            direction_y=1;
        }
    }
    else
    {
        //idem
        coord_case_suiv[0]=x;
        coord_case_suiv[1]=y-1;
        //idem
        direction_y=-1;
    }
    if ((coord_case_suiv[0]>=0)&&(coord_case_suiv[0]<19)&&(coord_case_suiv[1]>=0)&&(coord_case_suiv[1]<15)&&(test))//test des conditions autorisées pour un potentiel déplacement
    {
        if(*potion>0) //si la potion est active
        {
            // on décrémente la potion
            *potion=*potion-1;
            // on interdit la décrémentation de la case quittée
            z=0;
            
        }
        if ((tab[coord_case_suiv[1]][coord_case_suiv[0]]>=0)||((tab[coord_case_suiv[1]][coord_case_suiv[0]]==-2)&&(*marteau)))// test des conditions autorisées pour un déplacement effectif
        {
            buffer=tab[coord_case_suiv[1]][coord_case_suiv[0]];
            // on enregistre la case sur laquelle le personnage va
            tab[coord_case_suiv[1]][coord_case_suiv[0]]=tab[y][x];
            // on déplace le personnage
            tab[y][x]=precedent-z;
            // on rétabli la case par-dessus laquelle le personnage était en enlevant 1 si la potions est inactive
        }
        
        if((tab[coord_case_suiv[1]][coord_case_suiv[0]]==-50)&&(*clef>0)) // si c'est une serrure et qu'on a une clef
        {
            (*clef)--;
            // on décrémente la clef
            tab[coord_case_suiv[1]][coord_case_suiv[0]]=tab[y][x];
            // on avance sur la case
            tab[y][x]=precedent-z;
            // on remet en place la case
            return 0;
            // on enregisre la case en-dessous du joueur comme glace
        }
        // on appelle le test des objets
        return objets_graphique(buffer, potion,direction_x,direction_y,affiche, tab, coord_case_suiv,mode_graphique, score_general, score,affiche2)
    }
    // si aucune actio la case précédente reste la même
    else return precedent;
}
///////////////////////////////////////////////////////////////////////
// Nom: affichage_graphique
// utilité: gère l'affichage graphique
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 3
// entrées: le tableau d'affichage, le mode graphique, le score général et celui du niveau,
// le tableau des différentes images, le tableau de valeurs
// sortie: +
///////////////////////////////////////////////////////////////////////
void affichage_graphique(BITMAP* affiche[15][19], int mode_graphique, int score, int score_niveau, BITMAP* affiche2[NBSPRITE], char tab[15][19])
{
    //0 declaration de variables
    int i,j;
    BITMAP* buffer;
    BITMAP* fond;

    buffer= create_bitmap(19*TSPRITE, 15*TSPRITE);// image de la taille de l'écran
    if (mode_graphique==2) // si mode star_wars
    {
        fond=load_bitmap("fond_star_wars.bmp", NULL);
        draw_sprite(buffer, fond,0,0);
        // le fond deviens l'image de fond de star Wars
        destroy_bitmap(fond);
    }

    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], j*TSPRITE, i*TSPRITE); //dessin du niveau sur le buffer
        }
    }
    textprintf_ex(buffer, font, 15*TSPRITE-20, 7, makecol(255, 0,0),-1, "Score: %d", score);
    textprintf_ex(buffer, font, 15*TSPRITE-20, 10+LIGNE, makecol(255, 0,0),-1, "Score niveau: %d", score_niveau);
    // affichage des scores
    blit(buffer, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE); // copie sur l'écran
    destroy_bitmap(buffer);
}
///////////////////////////////////////////////////////////////////////
// Nom: init_mode_graphique
// utilité: charge les images
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 4
// entrées: le mode graphique, le tableau des différentes images
// sortie:+
///////////////////////////////////////////////////////////////////////
void init_mode_graphique(int mode_graphique,BITMAP* affiche2[NBSPRITE])
{
    // chargement des différents bitmaps
    switch(mode_graphique)
    {
    case 1:
        affiche2[0]=load_bitmap("Bmp/pingouin.bmp", NULL);
        // le personnage
        affiche2[1]=load_bitmap("Bmp/mur.bmp", NULL);
        // les murs
        affiche2[2]=load_bitmap("Bmp/serrure.bmp", NULL);
        // les serrures
        affiche2[3]=load_bitmap("Bmp/clef.bmp", NULL);
        // les clef
        affiche2[4]=load_bitmap("Bmp/glace_double.bmp", NULL);
        // la glace double
        affiche2[5]=load_bitmap("Bmp/ghost.bmp", NULL);
        // les ennemis
        affiche2[6]=load_bitmap("Bmp/eau.bmp", NULL);
        // l'eau
        affiche2[7]=load_bitmap("Bmp/glace.bmp", NULL);
        // la glace
        affiche2[8]=load_bitmap("Bmp/sortie.bmp", NULL);
        // la porte de sortie
        affiche2[9]=load_bitmap("Bmp/rocher1.bmp", NULL);
        // les rochers
        affiche2[10]=load_bitmap("Bmp/potion.bmp", NULL);
        // les potions
        affiche2[11]=load_bitmap("Bmp/tunnel1.bmp", NULL);
        // les tunnels
        affiche2[12]=load_bitmap("Bmp/turbo.bmp", NULL);
        // le turbo
        affiche2[14]=load_bitmap("Bmp/teleport.bmp", NULL);
        affiche2[15]=load_bitmap("Bmp/teleport2.bmp", NULL);
        affiche2[16]=load_bitmap("Bmp/teleport3.bmp", NULL);
        affiche2[17]=load_bitmap("Bmp/teleport4.bmp", NULL);
        //Les teleporteurs
        affiche2[13]=load_bitmap("Bmp/marteau1.bmp", NULL);
        // le marteau
        break;
    case 2:
        // idem avec les images star_wars
        affiche2[0]=load_bitmap("Bmp/Darth-Vader.bmp", NULL);
        affiche2[1]=NULL;
        affiche2[2]=load_bitmap("Bmp/master_yoda.bmp", NULL);
        affiche2[3]=load_bitmap("Bmp/sabre_laser18.bmp", NULL);
        affiche2[4]=load_bitmap("Bmp/star_wars_double.bmp", NULL);
        affiche2[5]=load_bitmap("Bmp/X-wing.bmp", NULL);
        affiche2[6]=NULL;
        affiche2[7]=load_bitmap("Bmp/star_wars_floor.bmp", NULL);
        affiche2[8]=load_bitmap("Bmp/Death_Star.bmp", NULL);
        affiche2[9]=load_bitmap("Bmp/deathstar_fire1.bmp", NULL);
        //affiche2[10]=load_bitmap(".bmp", NULL);
        //affiche2[11]=load_bitamp(".bmp", NULL);
        break;
    }
}
///////////////////////////////////////////////////////////////////////
// Nom: update_affichage_graphique
// utilité: gère l'affichage graphique
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 5
// entrées: le tableau de valeurs,le tableau d'affichage, le tableau des différentes images
// sortie: +
///////////////////////////////////////////////////////////////////////
void update_affichage_graphique(char tab[15][19], BITMAP* affiche[15][19], BITMAP* affiche2[NBSPRITE])
{
    int i,j, k,l;
    int teleport=0;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            switch (tab[i][j])
            {
            case -2: // les murs

                affiche[i][j]=affiche2[1];
                break;
            case -1: // l'eau
                affiche[i][j]=affiche2[6];
                break;
            case 0: // la glace
                affiche[i][j]=affiche2[7];
                break;
            case 1: // la glace double
                affiche[i][j]=affiche2[4];
                break;
            case 2: // le personnage
                affiche[i][j]=affiche2[0];
                break;
            case 3: // la sortie
                affiche[i][j]=affiche2[8];
                break;
            case 4: // les potions
                affiche[i][j]=affiche2[10];
                break;
            case 5: // les rochers
                affiche[i][j]=affiche2[9];
                break;
            case 100: // les clef
                affiche[i][j]=affiche2[3];
                break;
            case -50: // les serrures
                affiche[i][j]=affiche2[2];
                break;
            case 66:
                affiche[i][j]=affiche2[5];
                // les ennemis
                break;
            case 7:
                affiche[i][j]= affiche2[11];
                // les tunnels
                break;
            case 8:
                affiche[i][j]= affiche2[12];
                // le turbo
                break;
            case 9:
                affiche[i][j]= affiche2[13];
                // le marteau
            }
            if (tab[i][j]>100)
            {// les téléporteurs
                for(k=0; k<15; k++)
                {
                    for(l=0; l<19; l++)
                    {
                        if ((tab[k][l]==tab[i][j]+10)&&(teleport<4))
                        {
                            affiche[k][l]=affiche2[14+teleport];
                            affiche[i][j]=affiche2[14+teleport];
                            teleport++;
                        }


                    }
                }
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////
// Nom: affichage_graphique
// utilité: gère l'affichage graphique
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 8
// entrées: le tableau de valeurs, le nombre d'ennemis, les coordonées des ennemis, le niveau, les compteurs des ennemis, le nombre de mouvements de chaque ennemis, l'hypothétique nom du niveau
// sortie: +
///////////////////////////////////////////////////////////////////////
void init_move_ennemis(char tab[15][19], int nb_ennemis, int coord_ennemis[nb_ennemis][2], FILE* ennemis[nb_ennemis], int niveau, int compteurs_ennemis[nb_ennemis], int compteur_max[nb_ennemis], int precedent_ennemi[nb_ennemis], char mot[50])
{
    //0 déclaration des variables
    int i,j, a=0;
    char nom[30];
    for(i=0; i<nb_ennemis; i++) //pour chaque ennemi
    {
        if ((mot!=NULL)&&(strcmp(mot, "tutoriel.txt")))sprintf(nom, "ennemi.%s.%d.txt",mot, i); //génération du nom du fichier ennemi si le niveau est un niveau non régulier
        else sprintf(nom, "ennemi.niveau%d.%d.txt", niveau+1, i+1); // génération du nom de fichier pour les ennemis des niveaux réguliers
        ennemis[i]=fopen(nom, "r");//ouverture du fichier
        compteurs_ennemis[i]=1; // initialisation des compteur
        precedent_ennemi[i]=0; // la case de départ d'un ennemi, toujours de la glace
        fscanf(ennemis[i], "%d", &compteur_max[i]); // on lis le nombre de mouvements dans un cycle.
    }
    i=0;
    for(i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (tab[i][j]==66)
            {
                //on récupère toutes les coordonnées des ennemis et on les stock
                coord_ennemis[a][0]=j;
                coord_ennemis[a][1]=i;
                a++;
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////
// Nom: move_ennemis
// utilité: gère le déplacement des ennemis
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 10
// entrées: le fichier de l'ennemi, le tableau de valeurs, la case par-dessus laquelle est l'ennemi,son compteur de mouvement, ses coordonnées, le tableau de changements, le compteur de changements
// sorties: +
///////////////////////////////////////////////////////////////////////
int move_ennemis(FILE* ennemis, char tab[15][19], int precedent_ennemi, int coord[2], int compteur, int changement[50][2], int* emplacement )
{
    //0 déclaration de variables
    int move;
    int direction_x=0, direction_y=0;
    if(ennemis!=NULL)// blindage
    {
        fseek(ennemis, 2*compteur, SEEK_SET); // on cherche le compteur-ième mouvement de l'ennemi
        fscanf(ennemis, "%d", &move);// on le range dans move
    }
    switch(move)
    {
    case 1: //a droite
        direction_x=1;
        break;
    case 2: // a gauche
        direction_x=-1;
        break;
    case 3: // en bas
        direction_y=1;
        break;
    case 4: // en haut
        direction_y=-1;
        break;
    }
    move= tab[coord[1]+direction_y][coord[0]+direction_x]; // on enregistre la case suivante par-dessus laquelle l'ennemi va
    tab[coord[1]+direction_y][coord[0]+direction_x]= tab[coord[1]][coord[0]]; // on met l'ennemi a ses nouvelles coordonnées
    tab[coord[1]][coord[0]]= precedent_ennemi; // on remet la case que l'ennemi quitte a sa valeur
    changement[*emplacement][0]=coord[0]; // on enregistre que la case où etais l'ennemi a été modifiée pour la réafficher ultérieurement
    changement[(*emplacement)++][1]=coord[1];
    coord[1]+= direction_y; // on enregistre les nouvelles coordonnéesde l'ennemi
    coord[0]+=direction_x;
    changement[*emplacement][0]=coord[0]; // on enregistre que la case où est l'ennemi a changée pour la réafficher ultérieurement
    changement[(*emplacement)++][1]=coord[1];
    return move; // on retourne la case par-dessus laquelle l'ennemi est
}
///////////////////////////////////////////////////////////////////////
// Nom: jeu_graphique
// utilité: gère le jeu
// dernière modification: Commentaire, 06/12/2015
// nombre de modifications: 25
// entrées: le niveau, un pointeur sur le score général, potentiellement le nom du niveau
// sorties: un statut d'exécution (recommencer niveau, interrompre, execution normale)
///////////////////////////////////////////////////////////////////////
int jeu_graphique(int niveau, int* score, int mode_graphique, char mot[50])
{
    //0 déclaration de variables
    BITMAP* affiche[15][19];
    BITMAP* affiche2[NBSPRITE]= {NULL};
    FILE *niveau_rec;
    FILE *fichier[6]= {NULL};

    time_t debut;
    time_t mesure;
    int precedent=0;
    int i;
    int x,y;
    char valeurs[15][19];
    int xsortie,ysortie;
    int score_niveau=0;
    int cle=0, potion=0;
    int nb_ennemis=0,nb_cases_glace=0, nb_cases_eau;
    char affiche_console[15][19];
    int changement[50][2];
    int emplacement=0;
    int tunnel=0;
    int boucle=1;
    int marteau=0;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    //1 initialisation
    fichier[0] = fopen("niveau1.txt","r");
    fichier[1] = fopen("niveau2.txt","r");
    fichier[2] = fopen("niveau3.txt","r");
    fichier[3] = fopen("niveau4.txt","r");
    fichier[4] = fopen("niveau5.txt","r");
    fichier[5] = fopen("niveausauvegarde.txt","r");
    
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    if (niveau==5)
    {
        fscanf(fichier[5],"%d", &i);
        niveau=i;
    }

    i=0;
    system("cls");

    if (fichier[niveau]!=NULL) initialisation_niveau(fichier[niveau], valeurs); //initialisation du tableau de valeurs

    if ((mot!= NULL)&&(strcmp(mot, "tutoriel")))
    {
        // si le niveau demandé est un niveau spécial
        niveau_rec= fopen(mot, "r");
        intitialisation_rec(niveau_rec, valeurs); //méthode d'initialisation spécifique aux niveaux enregistrés

    }
    if (mot!=NULL)
    {
        if (strcmp(mot, "tutoriel.txt")==0)
        {
            // si le niveau demandé est le tutoriel
            fichier[niveau]= fopen(mot, "r");
            if (fichier[niveau]!=NULL)initialisation_niveau(fichier[niveau], valeurs); //initialisation classique

        }
    }
    if (mode_graphique) // mode graphique
    {
        init_mode_graphique(mode_graphique, affiche2); // chargement des images
        update_affichage_graphique(valeurs, affiche, affiche2); //rafraichissement du tableau d'affichage graphique
        if ((mot!=NULL)&&(strcmp(mot, "tutoriel.txt")==0))
        {
            afficher_intro();// affiche l'introduction du tutoriel
        }

    }
    else // mode console
    {
        update_tab_affiche(valeurs, affiche_console);// rafraichissement du tableau d'affichage console
        initialisation_console(affiche_console, *score, 0); // affichage du tableau complet
        if ((mot!=NULL)&&(strcmp(mot, "tutoriel.txt")==0)) // affichage de l'intro du tutoriel
        {
            gotoligcol(POS_ECRAN_Y/2,POS_ECRAN_X/3);
            printf("Bienvenue dans le niveau tutoriel!");
            gotoligcol(1+POS_ECRAN_Y/2,POS_ECRAN_X/3);
            printf("En jeu appuyez sur h pour voir l'aide!");
            Sleep(1500);
        }
    }


    time(&debut); // on note l'heure de début du niveau
    recherche_sortie_et_ennemis(valeurs, &xsortie, &ysortie, &tunnel, &nb_ennemis); // on compte les ennemis et on retient les coordonnées de la sorite/tunnel
    recherche_joueur(valeurs, &x, &y); // on retient les coordonnées du joueur
    nb_cases_glace= update_cases_glace(valeurs);// on compte le nombre de cases sur lesquelles le joueur peut et doit passer
    i=0;
    // déclaration des variables relatives aux ennemis (nombre variable)
    int coord_ennemis[nb_ennemis][2];
    FILE* ennemis[nb_ennemis];
    int compteurs_ennemis[nb_ennemis];
    int compteur_max[nb_ennemis];
    int precedent_ennemi[nb_ennemis];
    int compteur=0;
    init_move_ennemis(valeurs, nb_ennemis, coord_ennemis, ennemis, niveau, compteurs_ennemis, compteur_max, precedent_ennemi, mot); //initialisation de toutes les valeurs des variables spécifiques aux ennemis
    while(boucle) // boucle plus générale permettant la gestion du tunnel
    {
        while(((x!=xsortie)||(y!=ysortie))||(nb_cases_eau!=nb_cases_glace)) //test des conditions de fin du niveau
        {

            emplacement=POS_ECRAN_X;
            getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y); // récupération de la position du jeu a l'écran
            if (emplacement!= POS_ECRAN_X)
            {
                system("cls");
                initialisation_console(affiche_console,*score, score_niveau); // en cas de changement de position du jeu (consome-> plein écran ou inverse) réaffichage de tout le tableau
            }
            // remise a 0 des compteurs
            emplacement=0;
            i=0;
            nb_cases_eau=0;
            if (mode_graphique!=0)
            {
                // déplacement en mode graphique et retenue du la variable precedent
                precedent=deplacement_graphique(valeurs, &score_niveau, &cle, precedent, &potion, affiche, affiche2, mode_graphique, *score, niveau, &marteau);
            }
            else
            {
                // deplacement en mode console
                deplacement(valeurs, &cle,*score, &score_niveau, &precedent, changement, &emplacement, &potion, affiche_console, niveau, &marteau);
            }

            if (compteur==5) // les ennemis ne bougent qu'un tour de boucle sur 5
            {
                // pour chaque ennemi
                for(i=0; i<nb_ennemis; i++)
                {
                    // deplacement de l'ennemi
                    precedent_ennemi[i]=move_ennemis(ennemis[i], valeurs, precedent_ennemi[i], coord_ennemis[i], compteurs_ennemis[i], changement, &emplacement);

                    if (compteurs_ennemis[i]>=compteur_max[i]) compteurs_ennemis[i]=1; // gestion du nombre de mouvements de chaque ennemi, le mouvement est cyclique
                    else compteurs_ennemis[i]++;
                    if (precedent_ennemi[i]==2) // vérification: si l'ennemi passe sur le joueur
                    {
                        // affichage de la situation
                        if (mode_graphique)
                        {
                            update_affichage_graphique(valeurs, affiche, affiche2, mode_graphique);
                            affichage_graphique(affiche, mode_graphique, *score, score_niveau, affiche2, valeurs);
                        }
                        else
                        {
                            update_tab_affiche(valeurs, affiche_console);
                            affichage(changement, affiche_console, emplacement);
                        }
                        // Menu de mort
                        switch(afficher_mort(score, niveau, mode_graphique))
                        {
                            // retour du statut(recommencer ou quitter)
                        case 1:
                            return 1;
                        case 3:
                            return 3;
                        }
                    }
                }
                compteur=0;
            }

            Sleep(80); // delai pour limiter la vitesse du jeu
            if (mode_graphique!=0)
            {
                // réaffichage
                update_affichage_graphique(valeurs, affiche, affiche2, mode_graphique);
                affichage_graphique(affiche, mode_graphique, *score, score_niveau, affiche2, valeurs);
                if (key[KEY_H]) // affichage des regles
                {
                    afficher_menu_graphique();
                    while (key[KEY_H]); // le menu est affiché tant que l'utilisateur maintiens h
                }
            }
            else
            {
                update_tab_affiche(valeurs, affiche_console); // reafraichit le tableau d'affichage console
                affichage(changement, affiche_console, emplacement); // réaffiche uniquement les cases qui ont changé pendant ce tour de boucle
                affichage_score(score, score_niveau); // réaffiche le score

            }
            nb_cases_eau= update_cases_eau(valeurs); // on recompte le nombre de cases passées par le joueur
            recherche_joueur(valeurs, &x, &y);// on trouve le joueur
            time(&mesure); // on obtiens le temps
            if(difftime(mesure, debut))score_niveau= ((int)difftime(mesure, debut)+1000)*nb_cases_eau/(int)difftime(mesure, debut); //on calcule le score en fonction du temps et du nombre de cases passées
            if((x!=xsortie)||(y!=ysortie))
            {
                switch (TEST_DIE(valeurs, cle, niveau, score, precedent, mode_graphique, marteau)) // test si le joueur est mort
                {
                    // valeurs de retour
                case 1:
                    return 1;
                case 3:
                    return 3;
                }
            }
            if ((x==xsortie)&&(y==ysortie)&&(nb_cases_eau!=nb_cases_glace)) //si le joueur est a la sortie mais a manqué des cases
            {
                //system("cls");
                if(mode_graphique) allegro_message("Vous avez oublie %d %d-%d cases!",niveau,nb_cases_glace,nb_cases_eau); // message signifiant l'échec
                else {
                printf("\n\nVous avez oublie %d - %d cases!", nb_cases_glace, nb_cases_eau); // idem
                Sleep(1000);
                }
                switch(afficher_mort(score, niveau, mode_graphique)) // affichage de mort
                {
                    // valeurs de retour
                case 1:
                    return 1;
                case 3:
                    return 3;
                }

            }
            compteur++; // incrément du compteur des ennemis
        }
        if(tunnel) // compteur vérifiant la présence d'un tunnel
        {
            tunnel=0; // remise a 0
            initialisation_niveau(fichier[niveau], valeurs); // le tableau suivant se trouve directement a la suite dans le fichier texte, il suffit de rappeler la fonction
            precedent=0; // remise a 0 de la case sur laquelle le presonnage est
            if(mode_graphique==0) // cas spécial pour la console affichage du tableau complet
            {
                getsonsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
                gotoligcol(POS_ECRAN_Y,POS_ECRAN_X); // vérification d ela pos. du jeu sur la fenêtre
                update_tab_affiche(valeurs, affiche_console);
                initialisation_console(affiche_console, *score, score_niveau); // affichage du tableau complet
            }
            nb_cases_glace= update_cases_glace(valeurs); // comptage des cases a détruire sur le niveau
            nb_cases_eau= 0; // réinistialisation du nombre de cases d'eau
            recherche_sortie_et_ennemis(valeurs, &xsortie, &ysortie, &tunnel, &nb_ennemis); // recherche de la sortie et des potentiels ennemis
            recherche_joueur(valeurs, &x, &y); //recherche du joueur
        }
        else // pas de tunnel
        {
            boucle=0; // sorite de la boucle
        }
    }
    for (i=0; i<NBSPRITE; i++) if(affiche2[i]!=NULL)destroy_bitmap(affiche2[i]); // destruction de toutes les images
    (*score)+=score_niveau; // incrément du score général
    return 0; // retour normal d'exécution
}
