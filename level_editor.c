
#include "header.h"
#include "header_allegro.h"
/////////////////////////////////////////
// nom: afficher_regle_ennemi
//utilité: afficher l'intro de l'éditeur d'ennemis
//dernière modif: commentaire,06/12
//entrées: rien
////////////////////////////////////////
void afficher_regle_ennemi()
{
    BITMAP* buffer;
    int w=1;
    buffer=create_bitmap(19*TSPRITE, 15*TSPRITE);
    clear_bitmap(buffer);
    textprintf_ex (buffer, font, 0, 170+ w++*(LIGNE+3), makecol(255, 255, 255),-1," Regles :");
    textprintf_ex (buffer, font, 0, 170+ w++*(LIGNE+3), makecol(255, 255, 255),-1," Utilisez les touches z,q,s,d pour déplacer votre ennemi");
    textprintf_ex (buffer, font, 0, 170+ w++*(LIGNE+3), makecol(255, 255, 255),-1," Le mouvement doit former un cycle complet");
    textprintf_ex (buffer, font, 0, 170+ w++*(LIGNE+3), makecol(255, 255, 255),-1," Appuyez sur echap pour annuler tout le dessin");
    textprintf_ex (buffer, font, 0, 170+ w++*(LIGNE+3), makecol(255, 255, 255),-1," Appuyez sur entrée pour confirmer ou annuler le mouvement");
    blit(buffer, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    destroy_bitmap(buffer);
}
/////////////////////////////////////////
// nom: page_flip
//utilité: gérer un affichage a deux images virtuelles
//dernière modif: commentaire,06/12
//entrées: le tableau d'affichage, le pointeur sur le numero de l'image utilisée
////////////////////////////////////////
void page_flip(BITMAP* affiche[15][19], int* page)
{
    //0 declaration de variables
    BITMAP* buffer1;
    BITMAP* buffer2;
    scare_mouse(); // on cache la souris
    if(*page)
    {
        buffer1= affiche_buffer(affiche); // on charge l'image demandée
        blit(buffer1, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE); // on l'affiche
        destroy_bitmap(buffer1); // on détruis la mémoire
        *page=0; // on change la page
    }
    else
    {
        // idem
        buffer2= affiche_buffer(affiche);
        blit(buffer2, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
        destroy_bitmap(buffer2);
        *page=1;
    }
    unscare_mouse(); // on réaffiche la souris
}
/////////////////////////////////////////
// nom: ennemy_editor
//utilité: définition des mouvements des ennemis par l'utilisateur
//dernière modif: commentaire,06/12
//entrées: le tableau d'affichage, le tableau de valeurs, le nom du niveau
////////////////////////////////////////
int ennemy_editor(BITMAP* affiche[15][19], int tab[15][19], char niveau[50])
{
    //0 declaration de variables
    int i,j,a=0;
    int nb_ennemi = 0;
    int buffer_pos[2]
    int deplacements_ennemis[50]= {0};
    int direction_x;
    int direction_y;
    char buffert[60];
    BITMAP* affiche2[NBSPRITE];
    BITMAP* buffer;
    BITMAP* chemin;
    BITMAP* depart;
    FILE* fichier=NULL;
    //1 initialisations
    chemin = load_bitmap("passee.bmp", NULL); // indicateur qu'une case fais partie du chemin passé par l'ennemi
    depart = load_bitmap("case_depart.bmp", NULL); // indicateur de la case de départ
    init_mode_graphique(1, affiche2); // chargement des images du jeu
    buffer=create_bitmap(TSPRITE*19,TSPRITE*15);
    buffer=affiche_buffer(affiche); // chargement du niveau tel qu'il a été dessiné
    
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (tab[i][j]==66) nb_ennemi++; // Comptage des ennemis
        }
    }
    i=0;
    j=0;
    int coord_ennemis[nb_ennemi][2]; // tableau des coordonnées de tous les ennemis
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (tab[i][j]==66)
            {
                coord_ennemis[a][0]=j; // enregistrement des différentes coordonnées
                coord_ennemis[a++][1]=i;
            }
        }
    }
    for (i=0; i<nb_ennemi; i++)
    {
        for(j=0; j<nb_ennemi; j++)
        {
            affiche[coord_ennemis[j][1]][coord_ennemis[j][0]]=affiche2[7]; // on cache tous les ennemis par de la glace
        }

        if (strlen(niveau)<40) // si le nom permet de le supporter
        {
            sprintf(buffert, "ennemi.%s.%d.txt", niveau, i); //on ouvre le fichier de l'ennemi qui nous intéresse
            fopen(buffert, "w");
        }
        else
        {
            allegro_message("le nom est trop long!");
            destroy_bitmap(buffer);
            destroy_bitmap(chemin);
            destroy_bitmap(depart);
            return 0; // échec de l'édition
        }
        affiche[coord_ennemis[i][1]][coord_ennemis[i][0]]=affiche2[5]; // affichage de l'ennemi qui nous intéresse
        buffer=affiche_buffer(affiche); // on recharge le niveau avec ces nouvelles valeurs
        a=1;
        buffer_pos[0]= coord_ennemis[i][0];//on retiens les coordonées de départ de l'ennemi
        buffer_pos[1]= coord_ennemis[i][1];
        rest(100);
        while(!key[KEY_ENTER])
        {
            direction_x=0;
            direction_y=0;
            if(a<49) // le nombre max de déplacements
            {
                if(!key[KEY_W])
                {
                    if(!key[KEY_S])
                    {
                        if(!key[KEY_A])
                        {
                            if(!key[KEY_D])
                            {
                                if(key[KEY_ESC])
                                {
                                    destroy_bitmap(buffer);
                                    destroy_bitmap(chemin);
                                    destroy_bitmap(depart);
                                    return 1;// sortie immédiate
                                }
                            }
                            else
                            {
                                coord_ennemis[i][0]++; //incrément de la position de l'ennemi
                                deplacements_ennemis[a++]=1; // incrément de a et obtention de la valeurs a inscrire dans le fichier texte
                                direction_x=1; // sauvegarde de la direction
                                rest(100); // pause pour ralentir les mouvements 
                            }
                        }
                        else
                        {
                            // idem
                            coord_ennemis[i][0]--;
                            deplacements_ennemis[a++]=2;
                            direction_x=-1;
                            rest(100);
                        }
                    }
                    else
                    {
                        //idem
                        coord_ennemis[i][1]++;
                        deplacements_ennemis[a++]=3;
                        direction_y=1;
                        rest(100);
                    }
                }
                else
                {
                    //idem
                    coord_ennemis[i][1]--;
                    deplacements_ennemis[a++]=4;
                    direction_y=-1;
                    rest(100);
                }
                if ((coord_ennemis[i][0]>=0)&&(coord_ennemis[i][1]>=0)&&(coord_ennemis[i][0]<19)&&(coord_ennemis[i][1]<15)) //blindage de sortie de tableau
                {
                    if ((chemin==NULL)||(depart==NULL)) printf("echec du chargement!"); //blindage d'échec de chargement
                    if((coord_ennemis[i][1]-direction_y==buffer_pos[1])&&(coord_ennemis[i][0]-direction_x==buffer_pos[0]))draw_sprite(buffer, depart, TSPRITE*(coord_ennemis[i][0]-direction_x), TSPRITE*(coord_ennemis[i][1]-direction_y));// affiche la tuile de départ
                    else
                    {
                        // affiche la case précédente avec un marqueur transparent
                        draw_sprite(buffer, affiche2[7], TSPRITE*(coord_ennemis[i][0]-direction_x), TSPRITE*(coord_ennemis[i][1]-direction_y));
                        draw_sprite(buffer, chemin, TSPRITE*(coord_ennemis[i][0]-direction_x), TSPRITE*(coord_ennemis[i][1]-direction_y));
                    }
                    //affiche l'ennemi
                    draw_sprite(buffer, affiche2[5], TSPRITE*coord_ennemis[i][0], TSPRITE*coord_ennemis[i][1]);
                    blit(buffer, screen, 0,0,0,0, TSPRITE*19, TSPRITE*15);
                    //affiche le tout a l'écran
                }
                else
                {
                    // annulation du mouvement
                    coord_ennemis[i][1]-=direction_y;
                    coord_ennemis[i][0]-=direction_x;
                    a--;
                }
            }
        }
        switch(menu_oui_non(buffer, "mouvement")) //on demand esi l'utilisateur est satisfait
        {
        case 1: // si non
            for(a=0; a<50; a++)
            {
                deplacements_ennemis[a]=0;//remise des deplacements a 0
            }
            coord_ennemis[i][1]= buffer_pos[1]; //L'ennemi reprends sa position intiale
            coord_ennemis[i][0]= buffer_pos[0];
            i--; //on décrémente i pour qu ela boucle refasse le même tour
            allegro_message("Le mouvement a ete annule\nEntrez le mouvement de cet ennemi"); //on signifie a l'utilisateur que l'annulation a été prise en compte
            break;
        case 0: // si oui
            
            if ((coord_ennemis[i][1]==buffer_pos[1])&&(coord_ennemis[i][0]==buffer_pos[0])) //on vérifie que le mouvement fait bien un cycle complet
            {
                fprintf(fichier, "%d ", a-1); // on écris le nombre de déplacements dans le fichier
                a=0;
                for(a=0; a<50; a++)
                {
                    fprintf(fichier, "%d ", deplacements_ennemis[a]); //on écris les dépacements 1 à 1
                }
                allegro_message("mouvement enregistré!"); //on indique que l'enregistrement a réussi
                fclose(fichier); // on ferme le fichier
            }
            else
            {
                // on indique son erreur a l'utilisateur
                allegro_message("le mouvement ne forme pas une boucle!\n");
                allegro_message("Le mouvement a ete annule\nEntrez le mouvement de cet ennemi");
                // on réinitialise le tout
                coord_ennemis[i][1]= buffer_pos[1];
                coord_ennemis[i][0]= buffer_pos[0];
                i--;
            }
            break;
        case 2: //sortie brutale
            destroy_bitmap(buffer);
            destroy_bitmap(chemin);
            destroy_bitmap(depart);
            return 1;
        }
        buffer=affiche_buffer(affiche); // reaffichage du niveau
    }
    // sortie
    destroy_bitmap(buffer);
    destroy_bitmap(chemin);
    destroy_bitmap(depart);
    return 0;
}
/////////////////////////////////////////
// nom: sauvegarde_niveau
//utilité: enregistre le niveau dans un fichier texte
//dernière modif: commentaire, 06/12
//entrées:  le tableau de valeurs, le nom du niveau
// sortie: statut d'execution
////////////////////////////////////////
int sauvegarde_niveau(int valeurs[15][19], char mot[50])
{
    //0 déclaration de variables
    int nb,i=0,j=0;
    FILE* noms_sauvegardes;
    FILE* sauvegarde;
    //1 initialisation
    noms_sauvegardes= fopen("noms_sauvegardes.txt", "r+t");
    fscanf(noms_sauvegardes, "%d", &nb);// on scanne le nombre de niveaux enregistrés a ce jour
    if (strlen(mot)<44) //vérification de taille
    {
        strcat(mot, ".txt");// on ajoute .txt au nom
    }
    else
    {
        allegro_message("le nom est trop long!");
        return 1; // sortie d'erreur
    }
    sauvegarde = fopen(mot, "w+"); // on ouvre le fichier qui contiendras le niveau
    // écriture du prmier nombre incrémenté dans le fichier des noms, puis du nom du niveau a la fin
    fseek(noms_sauvegardes, 0, SEEK_SET);
    fprintf(noms_sauvegardes, "%d ", ++nb);
    fseek(noms_sauvegardes, 0, SEEK_END);
    fprintf(noms_sauvegardes, " %s", mot);
    i=0;
    j=0;
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            fprintf(sauvegarde, "%d ", valeurs[i][j]); // impression des 285 valeurs a la suite
        }
    }
    // sorite du sous-programme
    fclose(sauvegarde);
    fclose(noms_sauvegardes);
    return 0;
}
/////////////////////////////////////////
// nom: conversion
//utilité: associe la valeur du tableau de valeurs a partir de la valeur dans le table des images
//dernière modif: commentaire,06/12
//entrées: le numéro dans le tableau des images, le compteur des teleporteurs
// sorites: le numéro converti dans le tableau de valeurs
////////////////////////////////////////
int conversion(int num, int *compteur)
{
    switch (num)
    {
    case 0: // le joueur
        num= 2;
        break;
    case 1: // les murs
        num= -2;
        break;
    case 2: // les serrures
        num= -50;
        break;
    case 3: // les clef
        num= 100;
        break;
    case 4: // la glace double
        num= 1;
        break;
    case 5: // les ennemis
        num= 66;
        break;
    case 6: // l'eau
        num= -1;
        break;
    case 7: // la glace
        num= 0;
        break;
    case 8: // la sortie
        num= 3;
        break;
    case 9: // le rocher
        num= 5;
        break;
    case 10: // la potion de légertée
        num=4;
        break;
    case 11: // le tunnel
        num= 7;
        break;
    case 12: // le turbo
        num=8;
        break;
    case 13: // le marteau
        num=9;
        break;
    case 14: // les teleporteurs
        num=101+((*compteur)%2)*10+(*compteur)-(*compteur)%2; //on associe correctement les valeurs: 
        //les teleportzeurs sont liés dans l'ordre dans lequel on les mets et se lient par une différence de 10 entre les deux
        (*compteur)++;// incrément du compteur de téléporteurs
        break;
    default:
        num=-2;
    }
    return num; // on retourne la valeur transformée
}
/////////////////////////////////////////
// nom: menu_oui_non
//utilité: demande a l'utilisateur s'il veux enregistrer une chose par un menu oui ou non
//dernière modif: commentaire,06/12
//entrées: l'image de fond, le nom de l'objet a enregistrer
////////////////////////////////////////
int menu_oui_non(BITMAP* buffer, char* mot)
{
    //0 déclaration de variables
    BITMAP* sauvegarde;
    BITMAP* ecran_noir;
    int compteur=0;
    // 1 initialisation
    ecran_noir= create_bitmap(192+5*32, 160);
    clear_to_color(ecran_noir, makecol(0, 0, 0));
    sauvegarde= create_bitmap(19*TSPRITE, 15*TSPRITE);
    textprintf_ex(ecran_noir, font, 0, 0, makecol(255, 255, 255),-1, "voulez-vous sauvegarder ce %s?", mot);
    blit(ecran_noir, sauvegarde,0,0,0,0, 19*TSPRITE, 15*TSPRITE);// sauvegarde de l'état initial
    textprintf_ex(ecran_noir, font, 0, LIGNE+7, makecol(255, 255, 255),-1, "-> oui        non"); // état oui
    draw_sprite(buffer, ecran_noir, 192-2*32, 160);
    blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE); // affichage
    rest(200); // attentez pur éviter que le précédent "entree" interfère
    while(!key[KEY_ENTER])
    {
        if((key[KEY_LEFT])||(key[KEY_A]))
        {
            blit(sauvegarde, ecran_noir,0,0,0,0, 19*TSPRITE, 15*TSPRITE);// retour a l'état sauvegarde
            textprintf_ex(ecran_noir, font, 0, LIGNE+7, makecol(255, 255, 255),-1, "-> oui        non"); // état oui
            draw_sprite(buffer, ecran_noir, 192-2*32, 160);
            blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);// affichage
            compteur=0; //enregistrement de l'état
        }
        if((key[KEY_RIGHT]||key[KEY_D]))
        {
            //idem
            blit(sauvegarde, ecran_noir,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            textprintf_ex(ecran_noir, font, 0, LIGNE+7, makecol(255, 255, 255),-1, "   oui     -> non"); // etat non
            draw_sprite(buffer, ecran_noir, 192-2*32, 160);
            blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            compteur=1;// enregistrement de l'état
        }
        if (key[KEY_ESC]) // sortie brutale
        {
            clear_bitmap(screen);
            destroy_bitmap(ecran_noir);
            destroy_bitmap(sauvegarde);
            return 2;
        }
    }
    // sortie normale
    clear_keybuf();
    destroy_bitmap(ecran_noir);
    destroy_bitmap(sauvegarde);
    return compteur;
}
/////////////////////////////////////////
// nom: editeur_de_case
//utilité: permet de dessiner un niveau
//dernière modif: commentaire,06/12
//entrées: le tableau d'affichage, le tableau de valeurs
// retour: valeurs d'état
////////////////////////////////////////
int editeur_de_case(BITMAP* affiche[15][19], int valeurs[15][19])
{
    // 0 déclaration de variables
    int compteur_teleport=0, compteur_sortie, compteur_joueur, compteur_serrure, compteur_clef;
    int Case=0;
    int test=1,x=100,y=100;
    int page=0;
    int i,j;
    BITMAP* affiche2[NBSPRITE];
    // initialisatio 
    init_mode_graphique(1, affiche2);
    show_mouse(screen); // affichage de la souris a l'écran
    rest(500);// pause pour éviter les interferences
    page_flip(affiche, &page); // premier affichage
    while (test)
    {

        if(mouse_needs_poll())poll_mouse(); // certains systèmes requierent cette fonction pour gérer la position de la souris
        if ((mouse_b & 1)) //si  gauche appuyé
        {
            if (((int)(mouse_x-mouse_x%TSPRITE)/TSPRITE!=x)||((int)(mouse_y-mouse_y%TSPRITE)/TSPRITE!=y))// si la case est nouvelle
            {
                Case=0;// on recommence la liste des tuiles
                x = (int)(mouse_x-mouse_x%TSPRITE)/TSPRITE;//on fixe les coordonnées de la case
                y =(int)(mouse_y-mouse_y%TSPRITE)/TSPRITE;
                affiche[y][x] = affiche2[Case];// on change les valeurs d'affichage et de valeurs
                valeurs[y][x] = conversion(Case, &compteur_teleport);
                page_flip(affiche, &page); //affichage
                rest(100); //pause pour éviter de faire défiler toutes les possibilités en 1 fois
            }
            else // si on appuye a nouveau sur la même case
            {
                if (Case==14) // si le numéro de la tuile est maximal
                {
                    Case=-1; // on abaisse la tuile a -1
                    compteur_teleport--; // on a quitté un téléporteur, il faut décrémenté le compteur de teleporteurs
                }
                affiche[y][x]=affiche2[++Case];// on associe la tuile suivantee
                valeurs[y][x]= conversion(Case, &compteur_teleport);
                page_flip(affiche, &page; //affichage
                rest(100); //pause pour évitzr que toutes les tuiles défilent en 1 fois

            }

        }
        if (key[KEY_ESC]) // sortie brutale
        {
            return 1;
        }
        if (key[KEY_ENTER]) // si tentative d'enregistrement
        {
            //mise a 0 des compteurs
            compteur_joueur=0;
            compteur_sortie=0;
            compteur_clef=0;
            compteur_serrure=0;
            if (compteur_teleport%2==1)
            {
                // vérification de parité des téléporteurs
                allegro_message("attention!\n les telporteurs sont lies 2 a 2! il ne peut pas y en avoir un nombre impair!");
            }
            else
            {
                for(i=0; i<15; i++)
                {
                    for(j=0; j<15; j++)
                    {
                        switch (valeurs[i][j])
                        {
                            // décompte
                        case 7:
                            allegro_message("les tunnels ne sont pas autorisés pour l'instant!\n");
                            compteur_sortie=2;
                            break;
                        case 3:
                            compteur_sortie++;
                            break;

                        case 2:
                            compteur_joueur++;
                            break;
                        case -50:
                            compteur_serrure++;
                            break;
                        case 100:
                            compteur_clef++;
                        }
                    }
                }
                if (compteur_sortie!=1) allegro_message("il y a un nombre incorrect de sorties!\n"); // tests des valeurs
                else if (compteur_joueur!=1) allegro_message("il y un nombre incorrect de joueurs!\n");
                else if(compteur_clef!=compteur_serrure) allegro_message("il n'y a pas le meme nombre de serrures que de clef!");
                else return 0; // si autorisation de sauvegarde
            }
        }

        if(mouse_b & 2) // si bouton de droite
        {

            Case=7; // mise du compteur de tuile a "glace"
            x= (int)(mouse_x-mouse_x%TSPRITE)/TSPRITE;// récupération de la case
            y=(int)(mouse_y-mouse_y%TSPRITE)/TSPRITE;
            if(affiche[y][x]==affiche2[14]) compteur_teleport--; // si on supprime un teleporteur décrémentation du compteur
            affiche[y][x]=affiche2[Case]; // mise a "glace" de la glace
            valeurs[y][x]= conversion(Case, &compteur_teleport); conversion;
            page_flip(affiche, &page); //affichage
            rest(100); // délai
        }
    }
    return 0; // sortie 
}
/////////////////////////////////////////
// nom: worst_writing_ever
//utilité: équivalent de scanf sous allegro, adapté pour le cas présent
//dernière modif: commentaire,06/12
//entrées: le mot a renter, l'image de fond, l'image noire (avec ecritures) dans laquelle le mot est écrit, la ligne où il faut écrire, le tableau d'affichage
////////////////////////////////////////
int worst_writing_ever(char mot[50], BITMAP* buffer, BITMAP* ecran_noir, int line, BITMAP* affiche[15][19])
{
    //0 déclaration de variables
    int a=0;
    int key_allegro;
    char key_ascii;
    int i=0;
    //1 initialisation
    draw_sprite(buffer, ecran_noir, 192-2*32, 160);
    blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    rest(200);// pause pour éviter les interférences
    while(!key[KEY_ENTER])
    {
        if (key[KEY_ESC])// sortie brutale
        {
            clear_bitmap(screen);
            return 1;
        } 
        if (keypressed())
        {
            key_allegro=readkey();// récuperation de la touche
            key_ascii= key_allegro & 0xff; // récupération de son code ASCII

            if((key_ascii >= 32) && (key_ascii <= 126))// si la touche est utile
            {
                if (i<49) //si la capacité de la chaaîne n'est pas dépassée
                {
                    mot[i]= key_ascii;// on entre le caractère
                    // on affcihe la chaine
                    for(a=0; a<i+1; a++)textprintf_ex(ecran_noir, font, a*text_length(font, "O"), line*LIGNE+5, makecol(255, 255, 255),-1, "%c", mot[a]);
                    a=0;
                    draw_sprite(buffer, ecran_noir, 192-2*32, 160);// on affiche a l'écran
                    blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
                    mot[i+1]='\0'; // on place le \0
                    i++;
                }
            }
        }
    }

    mot[i]='\0';
    return 0; //sortie
}
/////////////////////////////////////////
// nom: affiche_buffer
//utilité: génère une image du jeu dan sl'état actuel
//dernière modif: commentaire,06/12
//entrées: le tableau d'affichage
// sortie: l'image
////////////////////////////////////////
BITMAP* affiche_buffer(BITMAP* affiche[15][19])
{
    //0 déclaration de l'image et des compteurs
    BITMAP* buffer;
    int i,j;
    //1 initialisation
    buffer= create_bitmap(TSPRITE*19, TSPRITE*15);
    

    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], TSPRITE*j, TSPRITE*i); // dessin de chaque image a sa place
        }
    }

    return buffer; // on renvoie le pointeur sur l'image ainsi formée
}
/////////////////////////////////////////
// nom: level_editor
//utilité: lance l'édition de niveau
//dernière modif: commentaire,06/12
//entrées: 
// sortie: 
////////////////////////////////////////
void level_editor()
{
    //0 déclaration des varibles
    int i,j;
    BITMAP* buffer;
    BITMAP* ecran_noir;
    char mot[50];
    mot[0]='c';
    mot[1]='\0';
    BITMAP* affiche[15][19];
    //installation des composants
    install_mouse();
    enable_hardware_cursor();
    ecran_noir= create_bitmap(192+5*32, 160);
    clear_to_color(ecran_noir, makecol(0, 0, 0));
    buffer= create_bitmap(TSPRITE*19, TSPRITE*15);
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, TSPRITE*19, TSPRITE*15, 0, 0)==0)// affichage de la fenêtre graphique
    {
        affiche[0][0]=load_bitmap("mur.bmp", NULL);
        for (i=0; i<15; i++)
        {
            for(j=0; j<19; j++)
            {
                affiche[i][j]=affiche[0][0];
                if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], TSPRITE*j, TSPRITE*i); //affichage d'un niveau 100% mur
            }
        }
        textprintf_ex(ecran_noir, font, 0, LIGNE, makecol(255,255,255),-1, "bienvenue sur l'editeur de niveau"); //message de bienvenue
        textprintf_ex(ecran_noir, font, 0,2* LIGNE+3, makecol(255,255,255),-1, "a tout moment appuyez sur escape pour quitter");
        draw_sprite(buffer, ecran_noir, 192-2*32, 160);
        blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);// affichage
        rest(1100);// laisse le temps de voir le message
        menu(buffer, affiche, mot);
        destroy_bitmap(buffer);
        destroy_bitmap(ecran_noir);
        clear_bitmap(screen);
        set_gfx_mode(GFX_TEXT,80,25,0,0);
        allegro_exit();//sortie
    }
}
/////////////////////////////////////////
// nom: menu
//utilité: programme maître dans l'édition de niveau
//dernière modif: commentaire,06/12
//entrées:  image actuelle buffer, tableau d'affichage, mot
// sortie: valeurs de statut
////////////////////////////////////////
int menu(BITMAP* buffer, BITMAP* affiche[15][19], char mot[50])
{
    //0 déclaration de variables
    int x=1,i,j;
    BITMAP* ecran_noir;
    BITMAP* sauvegarde[15][19];
    BITMAP* ecran_selec;
    int valeurs[15][19];
    int line;
    //1 initilaisations
    ecran_noir= create_bitmap(192+5*32, 160);
    ecran_selec= create_bitmap(192+5*32, 160);
    clear_to_color(ecran_noir, makecol(0, 0, 0));//message de démarrage du menu
    textprintf_ex(ecran_noir, font, 0, LIGNE, makecol(255,255,255),-1, "Que souhaitez-vous faire?");
    textprintf_ex(ecran_noir, font, 0, 2*LIGNE+3, makecol(255,255,255),-1, "'dessin' pour commencer a dessiner");
    textprintf_ex(ecran_noir, font, 0, 3*LIGNE+3, makecol(255,255,255),-1, "'aide' pour voir l'aide au dessin!important!");
    blit(ecran_noir, ecran_selec, 0,0,0,0, 192+5*32, 160 );// enregistrement de l'état de base du menu;
    line=4;
    affiche[0][0]=load_bitmap("mur.bmp", NULL);
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            valeurs[i][j]=-2;// initialisation du tableau de valeurs a "mur"
            affiche[i][j]=affiche[0][0];
            if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], TSPRITE*j, TSPRITE*i);
        }
    }
    draw_sprite(buffer, ecran_noir, 192-2*32, 160);
    blit(buffer, screen,0,0,0,0, 21*TSPRITE, 15*TSPRITE); // affichage
    while(x)
    {
        i=0;
        j=0;
        for (i=0; i<15; i++)
        {
            for(j=0; j<19; j++)
            {
                sauvegarde[i][j]=affiche[i][j]; // sauvegarde de l'état initial du niveau
            }
        }
        if(worst_writing_ever(mot, buffer, ecran_noir, line, affiche)) return 0; //écriture du choix
        if (strcmp(mot, "dessin"))
        {
            if (strcmp(mot, "aide"))
            {
                allegro_message("ce n'est pas une entree valable");
            }
            else
            {
                // affichage de l'aide au dessin
                clear_to_color(ecran_noir, makecol(0,0,0));
                textprintf_ex(ecran_noir, font, 0, LIGNE+3, makecol(255,255,255),-1, "Pour changer une case cliquez dessus.");
                textprintf_ex(ecran_noir, font, 0, 2*(LIGNE+3), makecol(255,255,255),-1, "un clic gauche affiche la dalle suivante");
                textprintf_ex(ecran_noir, font, 0, 3*(LIGNE+3), makecol(255,255,255),-1, "un clic droit met une glace a l'emplacement");
                textprintf_ex(ecran_noir, font, 0, 4*(LIGNE+3), makecol(255,255,255),-1, "appuyez sur entree quand vous avez termine");
                textprintf_ex(ecran_noir, font, 0, 5*(LIGNE+3), makecol(255,255,255),-1, "appuyez sur echap pour quitter");
                textprintf_ex(ecran_noir, font, 0, 6*(LIGNE+3), makecol(255,255,255),-1, "Pas d'espaces dans les noms de niveaux!");
                textprintf_ex(ecran_noir, font, 0, 7*(LIGNE+3), makecol(255,255,255),-1, "Soyez creatifs!!!");
                draw_sprite(buffer, ecran_noir, 192-2*32, 160);
                blit(buffer, screen,0,0,0,0, 21*TSPRITE, 15*TSPRITE);
                rest(300);
                while(!key[KEY_ENTER]) if (key[KEY_ESC]) // attente d'une action
                    {
                        clear_bitmap(screen);
                        destroy_bitmap(ecran_noir);
                        destroy_bitmap(ecran_selec);
                        return 0;// sortie
                    }
            }
        }
        else //si l'utilisateur choisis "dessin"
        {
            if (editeur_de_case(affiche, valeurs))// editeur de case
            {
                // si dema,de de sortie brutale
                clear_bitmap(screen);
                destroy_bitmap(ecran_noir);
                destroy_bitmap(ecran_selec);
                return 0;
            }
            buffer=affiche_buffer(affiche); // mise a niveau de l'image
            draw_sprite(buffer, ecran_noir, 192-2*32, 160);
            blit(buffer, screen,0,0,0,0, 21*TSPRITE, 15*TSPRITE);// affichage
            x=menu_oui_non(buffer, "dessin"); // récupération du choix oui- non sur le dessin
            line++;
            if (x==2)
            {
                clear_bitmap(screen);
                destroy_bitmap(ecran_noir);
                destroy_bitmap(ecran_selec);
                return 1; //sortie brutale
            }
            if(x==0) // si oui
            {
                clear_to_color(ecran_noir, makecol(0,0,0));
                textprintf_ex(ecran_noir, font, 0, 0, makecol(255,255,255),-1, "entrez le nom de votre niveau"); // demande le nom du niveau
                if ((worst_writing_ever(mot, buffer, ecran_noir, 1, affiche))||(ennemy_editor(affiche, valeurs, mot))) //exécute l'écriture puis l'éditeur de niveay
                {
                    // si sortie brutale sur un des deux programmes
                    clear_bitmap(screen);
                    destroy_bitmap(ecran_noir);
                    destroy_bitmap(ecran_selec);
                    return 0;
                }

                if (sauvegarde_niveau(valeurs, mot)); // sauvegarde du niveau
                else
                {
                    allegro_message("niveau enregistre!");
                    x=0;
                }
            }
            else //si le niveau n'est pas a sauvegrder
            {

                for (i=0; i<15; i++)
                {
                    for(j=0; j<19; j++)
                    {
                        affiche[i][j]=sauvegarde[i][j]; // remise a mur du tableau de valeurs
                    }
                }
            }
        }
        blit(ecran_selec, ecran_noir, 0,0,0,0, 192+5*32, 160 ); //remise a l'écran de base du menu
    }
    return 0; //sortie
}

