#include<allegro.h>
#include <winalleg.h>

#include "header.h"
#include "header_allegro.h"
void afficher_menu_principal()
{////////////////////////////////////////////////
 //Nom:afficher_menu_principal
 //Entrées :+
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 2.0
 ////////////////////////////////////////////////
    int POS_ECRAN_X, POS_ECRAN_Y;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    gotoligcol(POS_ECRAN_Y, POS_ECRAN_X);//affichage du menu dans une fenêtre adaptée
        printf ("\t\t\t MENU \n");
        printf ("\tPour une meilleure jouabilite, mettez la fenetre en plein ecran\n");
        printf ("\t    ecrivez sans fautes d'orthographe! \n");
        printf ("Ecrivez 'jouer' pour commencer a jouer ! \n");
        printf ("Ecrivez 'reprendre' pour reprendre votre partie sauvegardee ! \n");
        printf ("Ecrivez 'tutoriel' si vous jouez pour la premiere fois ! \n");
        printf ("Ecrivez 'options' pour modifier vos options ! \n");
        printf ("Ecrivez 'editer' pour acceder a l'editeur de niveau\n");
        printf ("Ecrivez 'niveaux' pour acceder aux niveaux crees!\n");
        printf ("Ecrivez 'scores' pour acceder aux scores precedentd\n");
        printf ("Ecrivez 'quitter' pour quitter le jeu !\n");
}
void game_exit(int mode_graphique, int buff, int score, int niveau)
{////////////////////////////////////////////////
 //Nom:game_exit
 //Entrées :mode_graphique,buff,score
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings 
 //Version : 7.0
 ////////////////////////////////////////////////
    char input[50];
    int i;
    FILE* sauvegarde;
    sauvegarde= fopen("niveausauvegarde.txt", "w");
    fprintf(sauvegarde, "%d", buff);
    fclose(sauvegarde);
    if (mode_graphique)//mode graphique
            {
                BITMAP* buffer;
                buffer=create_bitmap(TSPRITE*19, TSPRITE*15);// image d ela taille de l'écran
                clear_to_color(buffer,makecol(0,0,0)); // passge de l'image au noir
                if(menu_oui_non(buffer, "score")); // menu oui non proposant de sauvegarder le "score"
                else
                {
                    saisie_nom(input); // saisie du nom
                    sauvegarde_score(input, score); // enregistrement du score
                }
                clear_bitmap(screen);
                destroy_bitmap(buffer);
                clear_keybuf();
                set_gfx_mode(GFX_TEXT,80,25,0,0);
                allegro_exit();// sortie d'allegro
            }
    else
     {
        system("cls");
        printf("voulez-vous sauvegarder ce score?\n entrez 0 pour non ou 1 pour oui\n");//Options de sauvegardes
        scanf("%d", &i);//choix de l'utilisateur
        if(i) sauvegarde_score(input, score);//appel de la fonction pour sauvegarder le score

     }
}
void sauvegarde_score(char imput[50], int score)
{////////////////////////////////////////////////
 //Nom:sauvegarde_score
 //Entrées :imput [50],score
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 4.0
 ////////////////////////////////////////////////
    FILE* fichier_score;//initialisation du fichier pour sauvegarder le score
    int i;
    fichier_score= fopen("scores.txt", "r+");//ouverture du fichier de sauvegarde
    fseek(fichier_score, 0, SEEK_SET);
    fscanf(fichier_score,"%d", &i);
    fseek(fichier_score, 0, SEEK_SET);
    fprintf(fichier_score,"%d\n", ++i);
    fseek(fichier_score, 0, SEEK_END);
    fprintf(fichier_score,"%s %d\n", imput, score);//le score est écrit dans le fichier, accompagné du nom de l'utilisateur
    fclose(fichier_score);//fermeture du fichier
}
void saisie_nom(char imput[50])
{////////////////////////////////////////////////
 //Nom:saisie_nom
 //Entrées :imput[50]
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 8.0
 ////////////////////////////////////////////////
    BITMAP* sauvegarde;//0 déclaration des variables locales
    BITMAP* buffer;
    int i;
    int read;
    buffer=create_bitmap(TSPRITE*19, TSPRITE*15);//création d'une image
    clear_to_color(buffer,makecol(0,0,0));//noire
    sauvegarde= create_bitmap(19* TSPRITE, 15*TSPRITE);
    textprintf_ex(buffer, font, 192-2*32,160+7*LIGNE,makecol(255,255,255),-1,"quel est votre pseudo?");//affiche une demande de pseudo
    sauvegarde= create_bitmap(19* TSPRITE, 15*TSPRITE);
    blit(buffer, sauvegarde, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    blit(sauvegarde, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    clear_keybuf();
    Sleep(200);
    while(!key[KEY_ENTER])//tant que l'utilisateur n'appuie pas sur entree: c'est àdire tant qu'il continue à écrire son pseudo
    {
        if (key[KEY_BACKSPACE])//gère l'effaçage
        {
           if (i>0) i--; //efface la dernière lettre de la chaîne de caractère (fonction effacer)
           imput[i]='\0';
           if (i=0)
           {
            imput [i]=' ';
            imput [i+1]='\0';
           }
           blit(sauvegarde, buffer, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);//remet le buffer à l'état initial
           textprintf_ex(buffer, font, 192-2*32,160+9*LIGNE,makecol(255,255,255),-1,"%s", imput);//écris le mot inclu dans imput à l'endroit désiré
        }
        blit(buffer, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
        read= readkey();
        imput[i]= read & 0xff;
        if ((imput[i]>=32)||(imput[i]>=126))//si le mot fait partie des caractères utiles de la table ASCII
        {
             if (i>48) //blindage sur le pseudo
        {
         allegro_message("Le mot est trop long.");
         
        }
            else //si le mot est pas trop long
            {
             imput[++i] = '\0';
             blit(sauvegarde, buffer, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);//remet le buffer à l'état initial
             textprintf_ex(buffer, font, 192-2*32,160+9*LIGNE,makecol(255,255,255),-1,"%s", imput);//écris le mot inclu dans imput à l'endroit désiré
            }
        }
    }
    clear_bitmap(screen);
    destroy_bitmap(buffer);
    destroy_bitmap(sauvegarde);
}
void allegro_demarre(int mode_graphique)
{////////////////////////////////////////////////
 //Nom:allegro_demarre
 //Entrées :mode_graphique
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 2.0
 ////////////////////////////////////////////////
    if (mode_graphique)//initialisation du mode graphique
    {
        allegro_init();
        install_keyboard();
        set_color_depth(16);
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, 19*TSPRITE, 15*TSPRITE, 0,0);
    }
}

void getconsole_size(int* POS_ECRAN_X, int* POS_ECRAN_Y)
{////////////////////////////////////////////////
 //Nom:getconsole_size
 //Entrées :*POS_ECRAN_X,*POS_ECRAN_Y
 //Sorties : +
 //Date de dernière modification : 06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 2.0
 ////////////////////////////////////////////////
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int colonnes, lignes;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
    colonnes=csbi.srWindow.Right-csbi.srWindow.Left+1;
    lignes = csbi.srWindow.Bottom-csbi.srWindow.Top+1;
    if (colonnes>87) *POS_ECRAN_X=colonnes/4;
    *POS_ECRAN_Y = lignes/4;
}
void gotoligcol( int lig, int col )
{////////////////////////////////////////////////
 //Nom:gotoligcol
 //Entrées :lig,col
 //Sorties : +
 //Date de dernière modification :+
 //Nature de la dernière modif :+
 //Version : 1.0
 ////////////////////////////////////////////////
// ressources
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}
void reprendre(int mode_graphique, int* score)
{////////////////////////////////////////////////
 //Nom:reprendre
 //Entrées :mode_graphique,*score
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 3.0
 ////////////////////////////////////////////////
    FILE* fichier;
    int buff, i;
    fichier= fopen("niveausauvegarde.txt", "r");//ouverture du fichier de sauvegarde du dernier niveau joué
    fscanf(fichier, "%d", &buff);//dans le fichier,il y a un nombre correspondant au dernier niveau joué, le lis
    for (i=buff; i<5; i++)
    {
        switch(jeu_graphique(i, score, mode_graphique, NULL))//la fonction jeu graphique retourne le choix de l'utilisateur pour arreter ou recommencer le niveau
        {
        case 1://retour de la fonction jeu_graphique, si 1--> recommencer le niveau
            i--;//refais la boucle
            break;
        case 3://si 3--> arreter de jouer
            i=5;//termine la boucle
            break;
        default:
            break;

        }
    }
}
void mes_niveaux(int* score, int mode_graphique)
{////////////////////////////////////////////////
 //Nom:mes_niveaux
 //Entrées :*score,mode_graphique
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 7.0
 ////////////////////////////////////////////////
    FILE* nom_des_fichiers;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);//défini la taille de la console
    int i,x=1;//0 initialisation des variables locales
    char mot[50];
    char chaine[50];
    nom_des_fichiers= fopen("noms_sauvegardes.txt", "r");//ouvre le fichier qui contiendra le nom des sauvegardes
    fscanf(nom_des_fichiers, "%d", &i);
    while(x)
    {
        printf("quel est le nom du niveau?\n");//le nom du niveau qui vient d'être édité
        scanf("%s", mot);
        if (strlen(mot)<44)//blindage si le nom du niveau est trop long
        {
            strcat(mot, ".txt");
            x=0;
        }
        else printf("ce nom est trop long\n");//message d'erreur
    }
    x=-1;
    while(i)
    {
        fscanf(nom_des_fichiers, "%s", chaine);
        if(strcmp(mot, chaine));//cherche le nom entré par l'utilisateur dans la liste de niveaux sauvegardés
        else
        {
            x=i;
            i=0;
            getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
            jeu_graphique(0, score, mode_graphique, mot );
        }
        i--;
    }
    if (x==-1) printf ("Ce n'est pas un nom valable.\n");//blindage si le nom entré n'est pas valide

}
void fonction_scores ()
{////////////////////////////////////////////////
 //Nom:fonction_scores
 //Entrées :+
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 3.0
 ////////////////////////////////////////////////
    FILE* fichier;
    int nb,i, score;
    char tab [50];
    fichier= fopen("scores.txt", "r+");//ouvre le fichier contenant les cores
    fscanf(fichier, "%d", &nb);
    for(i=0; i<nb; i++)//Parcours le fichier texte
    {
        fscanf(fichier, "%s", tab);//sauvegarde le nom du l'utilisateur
        fscanf(fichier, "%d", &score);//sauvegrade le nom de l'utilisateur
        printf("%s %d\n", tab, score);//Affiche dans le fichier les 2 paramètres précédents
    }
}
void fonction_options (int* score, int* mode_graphique, int *mode_son)
{////////////////////////////////////////////////
 //Nom:fonction_options
 //Entrées :*score,*mode_graphique,*mode_son
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 
 ////////////////////////////////////////////////
    int choix;
    printf ("souhaitez-vous:\n1 passer en mode graphique\n2 passer en mode graphique et rejoindre le cote obscur de la force\n3 passer en mode console");//on informe l'utilisateur des différentes options possibles
    scanf("%d", &choix );//enregistre le choix de l'utilisateur
    switch (choix)
    {
    case 1:
        *mode_graphique=1;//active le mode graphique
        break;
    case 2:
        *mode_graphique=2;//active le mode star wars
        break;
    case 3:
        *mode_graphique=0;// passe en mode console
    }

}
void fonction_admin (int mode_graphique, int* score, int niveau)
{////////////////////////////////////////////////
 //Nom:fonction_admin
 //Entrées :mode_graphique,*score
 //Sorties : +
 //Date de dernière modification :06/1202015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 5.0
 ////////////////////////////////////////////////
    int entre,i=0;
    int buff;
    char x=1;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    printf ("A quel niveau souhaitez-vous acceder?\n");//la fonction admin permet d'accéder à n'importe quel niveau
    while(x)
    {
        scanf("%d", &entre);
        switch (entre)
        {
        case 0://blindage qui assure que le niveau sélectionné par l'admin est faisable
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if(mode_graphique)allegro_demarre(mode_graphique);//active la fonction en mode graphique
            else getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);//initialise la console
            for(i=entre; i<=5; i++)
            {
             buff=i;
                switch(jeu_graphique(i-1, score, mode_graphique, NULL))
                {
                case 1://recommencer le niveau
                    i--;
                    break;
                case 3://arrete le niveau
                    i=5;
                }
            }
            game_exit(mode_graphique, buff, score);//appel de la fonction pour quitter le jeu
            x=0;
            break;
        default:
            printf("ce n'est pas une entree valide!\nveuillez essayer a nouveau:");//blindage
        }
    }
}

int main()//main
{////////////////////////////////////////////////
 //Nom:main
 //Entrées :+
 //Sorties : +
 //Date de dernière modification :06/12/2015
 //Nature de la dernière modif :Commentaire et correction des warnings
 //Version : 9.0
 ////////////////////////////////////////////////
    int x=0, buff;//0 déclaration des variables
    int score=0, niveau=0;
    char imput [50];
    int mode_graphique=0, mode_son=0;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;

    while (x==0)//boucle infinie pour l'affichage du menu
    {
        afficher_menu_principal();//appel de la fonction d'affichage du menu
        scanf ("%s",imput);

        if (strcmp (imput,"jouer")==0)//lance le jeu si l'utilisateur à choisi de jouer dans le menu
        {
            getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);//initialise la console
            allegro_demarre(mode_graphique);//initialise le mode graphique
            for(niveau=0; niveau<5; niveau++)//lance les niveaux
            {
                switch(jeu_graphique(niveau, &score, mode_graphique, NULL))
                {
                case 1://recommence le niveau
                    niveau--;
                    break;
                case 3://arrete le niveau
                    buff=niveau;
                    niveau=5;
                }
            }
            game_exit(mode_graphique, buff, score, niveau);//appel de la fonction pour quitter le jeu
        }
        else if (strcmp (imput,"reprendre")==0)//lance le dernier niveau sauvegardé si l'utilisateur a choisi reprendre dans le menu
        {
            allegro_demarre(mode_graphique);//initialise le mode graphique
            reprendre(mode_graphique, &score);//appel de la fonction qui reprend le dernier niveau fait

        }
        else if (strcmp (imput, "tutoriel")==0)//démarre le tutoriel si l'utilisateur a choisi le tutoriel dans le menu
        {
            allegro_demarre(mode_graphique);//initialise le mode graphique
            strcat(imput, ".txt");
            jeu_graphique(0, &score, mode_graphique, imput);
            set_gfx_mode(GFX_TEXT,0,0,0,0);
            system("cls");

        }
        else if (strcmp (imput, "scores")==0)//Affiche les scores sauvegardés
        {
            fonction_scores();//appel de la fonction qui affiche les scores et le spseudos des joueurs ayant sauvegardé leur progression
            system("pause");
            system("cls");
        }
        else if (strcmp (imput, "options")==0)//affiche les options
        {
            fonction_options(&score, &mode_graphique, &mode_son);/// fonction chargée d'afficher et de lancer les options
            system("cls");
        }
        else if (strcmp (imput, "quitter")==0)//quitte le jeu
        {
            fin_de_niveau(niveau);
        }
        else if (strcmp (imput, "admin")==0)//fonction admin pour accéder a tous les niveaux
        {
            fonction_admin(mode_graphique, &score);//appel de la fonction admin
        }
        else if(strcmp (imput, "editer")==0)//lance le level editor sous allegro
        {
            allegro_demarre(1);//lance allegro
            level_editor();//appel de la fonction du level editor
            system("cls");
        }
        else if(strcmp (imput, "niveaux")==0)//lance la recharge de niveaux édités
        {
            mes_niveaux(&score, mode_graphique);//appel de la fonction qui recharge le sniveaux édités
        }
        else
        {
            printf ("Ce n'est pas un choix correct, veuillez recommencer\n");//blindage
            Sleep(700);
            system("cls");

        }
    }
    return 0;
}
END_OF_MAIN();//fin
