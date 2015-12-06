#include<allegro.h>
#include <winalleg.h>

#include "header.h"
#include "header_allegro.h"
void afficher_menu_principal()
{
    int POS_ECRAN_X, POS_ECRAN_Y;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    gotoligcol(POS_ECRAN_Y, POS_ECRAN_X);
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
void game_exit(int mode_graphique, int buff, int score)
{
    char input[50];
    int i;
    if (mode_graphique)
            {
                BITMAP* buffer;
                buffer=create_bitmap(TSPRITE*19, TSPRITE*15);
                clear_to_color(buffer,makecol(0,0,0));
                if(menu_oui_non(buffer, "score"));
                else
                {
                    saisie_nom(input);
                    sauvegarde_score(input, score);
                }
                destroy_bitmap(buffer);
                clear_keybuf();
                set_gfx_mode(GFX_TEXT,80,25,0,0);
                allegro_exit();
            }
            else
            {
                system("cls");
                printf("voulez-vous sauvegarder ce score?\n entrez 0 pour non ou 1 pour oui\n");
                scanf("%d", &i);
                if(i) sauvegarde_score(input, score);

            }
}
void sauvegarde_score(char imput[50], int score)
{
    FILE* fichier_score;
    int i;
    fichier_score= fopen("scores.txt", "r+");
    fseek(fichier_score, 0, SEEK_SET);
    fscanf(fichier_score,"%d", &i);
    fseek(fichier_score, 0, SEEK_SET);
    fprintf(fichier_score,"%d\n", ++i);
    fseek(fichier_score, 0, SEEK_END);
    fprintf(fichier_score,"%s %d\n", imput, score);
    fclose(fichier_score);
}
void saisie_nom(char imput[50])
{
    BITMAP* sauvegarde;
    BITMAP* buffer;
    int i;
    int read;
    buffer=create_bitmap(TSPRITE*19, TSPRITE*15);
    clear_to_color(buffer,makecol(0,0,0));
    sauvegarde= create_bitmap(19* TSPRITE, 15*TSPRITE);
    textprintf_ex(buffer, font, 192-2*32,160+7*LIGNE,makecol(255,255,255),-1,"quel est votre pseudo?");
    sauvegarde= create_bitmap(19* TSPRITE, 15*TSPRITE);
    blit(buffer, sauvegarde, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    blit(sauvegarde, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    clear_keybuf();
    Sleep(200);
    while(!key[KEY_ENTER])
    {
        if (key[KEY_BACKSPACE])
        {
            if (imput[i]!='\0') imput[--i] = '\0';
            blit(sauvegarde, buffer, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            textprintf_ex(buffer, font, 192-2*32,160+9*LIGNE,makecol(255,255,255),-1,"%s", imput);
        }
        if (i>49) break;
        blit(buffer, screen, 0,0,0,0, 19*TSPRITE, 15*TSPRITE);
        read= readkey();
        imput[i]= read & 0xff;
        if ((imput[i]>=32)||(imput[i]>=126))
        {

            textprintf_ex(buffer, font, 192-2*32+i*text_length(font, "O"),160+9*LIGNE,makecol(255,255,255),-1,"%c", imput[i]);
            imput[++i] = '\0';
        }
    }
    destroy_bitmap(buffer);
    destroy_bitmap(sauvegarde);
}
void allegro_demarre(int mode_graphique)
{
    if (mode_graphique)
    {
        allegro_init();
        install_keyboard();
        set_color_depth(16);
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, 19*TSPRITE, 15*TSPRITE, 0,0);
    }
}

void getconsole_size(int* POS_ECRAN_X, int* POS_ECRAN_Y)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int colonnes, lignes;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
    colonnes=csbi.srWindow.Right-csbi.srWindow.Left+1;
    lignes = csbi.srWindow.Bottom-csbi.srWindow.Top+1;
    if (colonnes>87) *POS_ECRAN_X=colonnes/4;
    *POS_ECRAN_Y = lignes/4;
}
void gotoligcol( int lig, int col )
{
// ressources
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}
void reprendre(int mode_graphique, int* score)
{
    FILE* fichier;
    int buff, i;
    fichier= fopen("niveausauvegarde.txt", "r");
    fscanf(fichier, "%d", &buff);
    for (i=buff; i<5; i++)
    {
        switch(jeu_graphique(i, score, mode_graphique, NULL))
        {
        case 1:
            i--;
            break;
        case 3:
            i=5;
            break;
        default:
            break;

        }
    }
}
void mes_niveaux(int* score, int mode_graphique)
{
    FILE* nom_des_fichiers;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
    int i,x=1;
    char mot[50];
    char chaine[50];
    nom_des_fichiers= fopen("noms_sauvegardes.txt", "r");
    fscanf(nom_des_fichiers, "%d", &i);
    while(x)
    {
        printf("quel est le nom du niveau?\n");
        scanf("%s", mot);
        if (strlen(mot)<44)
        {
            strcat(mot, ".txt");
            x=0;
        }
        else printf("ce nom est trop long\n");
    }
    x=1;
    while(i)
    {
        x=1;

        while(x)
        {
            fscanf(nom_des_fichiers, "%s", chaine);
            x=0;
            if(strcmp(mot, chaine));
            else
            {
                i=0;
                getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
                jeu_graphique(0, score, mode_graphique, mot );
            }
        }
        i--;
    }

}
void fonction_scores ()
{
    FILE* fichier;
    int nb,i, score;
    char tab [50];
    fichier= fopen("scores.txt", "r+");
    fscanf(fichier, "%d", &nb);
    for(i=0; i<nb; i++)
    {
        fscanf(fichier, "%s", tab);
        fscanf(fichier, "%d", &score);
        printf("%s %d\n", tab, score);
    }
}
void fonction_options (int* score, int* mode_graphique, int *mode_son)
{
    int choix;
    printf ("souhaitez-vous:\n1 activer le son\n2 paser en mode graphique\n3 passer en mode graphique et rejoindre le cote obscur de la force\n");
    scanf("%d", &choix );
    switch (choix)
    {
    case 1:
        *mode_son=1;
        break;
    case 2:
        *mode_graphique=1;
        break;
    case 3:
        *mode_graphique=2;
    }

}
void fonction_admin (int mode_graphique, int* score)
{
    int entre,i=0;
    char x=1;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;
    printf ("A quel niveau souhaitez-vous acceder?\n");
    while(x)
    {
        scanf("%d", &entre);
        switch (entre)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if(mode_graphique)allegro_demarre(mode_graphique);
            else getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
            for(i=entre; i<=5; i++)
            {
                switch(jeu_graphique(i-1, score, mode_graphique, NULL))
                {
                case 1:
                    i--;
                    break;
                case 3:
                    i=5;
                }
            }
            x=0;
            break;
        default:
            printf("ce n'est pas une entree valide!\nveuillez essayer a nouveau:");
        }
    }
}

int main()
{
    int x=0, buff;
    int score=0, niveau=0;
    char imput [50];
    int mode_graphique=0, mode_son=0;
    int POS_ECRAN_Y=0;
    int POS_ECRAN_X=0;

    while (x==0)
    {
        afficher_menu_principal();
        scanf ("%s",imput);

        if (strcmp (imput,"jouer")==0)
        {
            getconsole_size(&POS_ECRAN_X, &POS_ECRAN_Y);
            allegro_demarre(mode_graphique);
            for(niveau=0; niveau<5; niveau++)
            {
                switch(jeu_graphique(niveau, &score, mode_graphique, NULL))
                {
                case 1:
                    niveau--;
                    break;
                case 3:
                    buff=niveau;
                    niveau=5;
                }
            }
            game_exit(mode_graphique, buff, score);
        }
        else if (strcmp (imput,"reprendre")==0)
        {
            allegro_demarre(mode_graphique);
            reprendre(mode_graphique, &score);

        }
        else if (strcmp (imput, "tutoriel")==0)
        {
            allegro_demarre(mode_graphique);
            strcat(imput, ".txt");
            jeu_graphique(0, &score, mode_graphique, imput);
            set_gfx_mode(GFX_TEXT,0,0,0,0);
            system("cls");

        }
        else if (strcmp (imput, "scores")==0)
        {
            fonction_scores();
            system("pause");
            system("cls");
        }
        else if (strcmp (imput, "options")==0)
        {
            fonction_options(&score, &mode_graphique, &mode_son);
            system("cls");
        }
        else if (strcmp (imput, "quitter")==0)
        {
            return 0;
        }
        else if (strcmp (imput, "admin")==0)
        {
            fonction_admin(mode_graphique, &score);
            x++;
        }
        else if(strcmp (imput, "editer")==0)
        {
            allegro_demarre(1);
            level_editor();
            system("cls");
        }
        else if(strcmp (imput, "niveaux")==0)
        {
            mes_niveaux(&score, mode_graphique);
        }
        else
        {
            printf ("Ce n'est pas un choix correct, veuillez recommencer\n");
            Sleep(700);
            system("cls");

        }
    }
    return 0;
}
END_OF_MAIN();
