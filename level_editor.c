#include<allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include<string.h>
#include "header.h"
#include "header_allegro.h"
#include <allegro.h>
#define LIGNE text_height(font)
void page_flip(BITMAP* affiche[15][19], int* page)
{
    BITMAP* buffer1;
    BITMAP* buffer2;
    scare_mouse();
    if(*page)
    {
        buffer1= affiche_buffer(affiche);
        blit(buffer1, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
        destroy_bitmap(buffer1);
        *page=0;
    }
    else
    {
        buffer2= affiche_buffer(affiche);
        blit(buffer2, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
        destroy_bitmap(buffer2);
        *page=1;
    }
    unscare_mouse();
}
int ennemy_editor(BITMAP* affiche[15][19], int tab[15][19], char niveau[50])
{
    int i,j,a=0;
    int nb_ennemi = 0;
    int deplacements_ennemis[50]= {0};
    int direction_x;
    int direction_y;
    char buffert[60];
    BITMAP* affiche2[NBSPRITE];
    BITMAP* buffer;
    BITMAP* chemin;
    BITMAP* depart;
    chemin = load_bitmap("passee.bmp", NULL);
    depart = load_bitmap("case_depart.bmp", NULL);
    init_mode_graphique(1, affiche2);
    buffer=create_bitmap(TSPRITE*19,TSPRITE*15);
    buffer=affiche_buffer(affiche);
    FILE* fichier=NULL;
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (tab[i][j]==66) nb_ennemi++;
        }
    }
    i=0;
    j=0;
    int coord_ennemis[nb_ennemi][2];
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (tab[i][j]==66)
            {
                coord_ennemis[a][0]=j;
                coord_ennemis[a++][1]=i;
            }
        }
    }
    for (i=0; i<nb_ennemi; i++)
    {
        j=0;
        for(j=0; j<nb_ennemi; j++)
        {
            affiche[coord_ennemis[j][1]][coord_ennemis[j][0]]=affiche2[7];
        }

        if (strlen(niveau)<40)
        {
            sprintf(buffert, "ennemi.%s.%d.txt", niveau, i);
            fopen(buffert, "w");
        }
        else
        {
            allegro_message("le nom est trop long!");
            return 0;
        }
        affiche[coord_ennemis[i][1]][coord_ennemis[i][0]]=affiche2[5];
        buffer=affiche_buffer(affiche);
        a=1;
        deplacements_ennemis[0]=49;
        rest(100);
        while(!key[KEY_ENTER])
        {
            direction_x=0;
            direction_y=0;
            if(a<49)
            {
                if(!key[KEY_W])
                {
                    if(!key[KEY_S])
                    {
                        if(!key[KEY_A])
                        {
                            if(!key[KEY_D])
                            {
                                if(key[KEY_ESC]) return 1;
                            }
                            else
                            {
                                coord_ennemis[i][0]++;
                                deplacements_ennemis[a++]=1;
                                direction_x=1;
                                rest(100);
                            }
                        }
                        else
                        {
                            coord_ennemis[i][0]--;
                            deplacements_ennemis[a++]=2;
                            direction_x=-1;
                            rest(100);
                        }
                    }
                    else
                    {
                        coord_ennemis[i][1]++;
                        deplacements_ennemis[a++]=3;
                        direction_y=1;
                        rest(100);
                    }
                }
                else
                {
                    coord_ennemis[i][1]--;
                    deplacements_ennemis[a++]=4;
                    direction_y=-1;
                    rest(100);
                }
                if ((coord_ennemis[i][0]>=0)&&(coord_ennemis[i][1]>=0)&&(coord_ennemis[i][0]<19)&&(coord_ennemis[i][1]<15))
                {
                if ((chemin==NULL)||(depart==NULL)) printf("echec du chargement!");
                if(a==2)draw_sprite(buffer, depart, TSPRITE*(coord_ennemis[i][0]-direction_x), TSPRITE*(coord_ennemis[i][1]-direction_y));
                else if (a>2)
                {
                    draw_sprite(buffer, affiche2[7], TSPRITE*(coord_ennemis[i][0]-direction_x), TSPRITE*(coord_ennemis[i][1]-direction_y));
                    draw_sprite(buffer, chemin, TSPRITE*(coord_ennemis[i][0]-direction_x), TSPRITE*(coord_ennemis[i][1]-direction_y));
                }
                draw_sprite(buffer, affiche2[5], TSPRITE*coord_ennemis[i][0], TSPRITE*coord_ennemis[i][1]);
                blit(buffer, screen, 0,0,0,0, TSPRITE*19, TSPRITE*15);
                }
                else
                {
                    coord_ennemis[i][1]-=direction_y;
                    coord_ennemis[i][0]-=direction_x;
                }
            }
        }
        switch(menu_oui_non(buffer, "mouvement"))
        {
        case 1:
            for(a=0; a<50; a++)
            {
                deplacements_ennemis[a]=0;

            }
            i--;
            allegro_message("Le mouvement a ete annule\nEntrez le mouvement de cet ennemi");
            break;
        case 0:
            fprintf(fichier, "%d ", a-1);
            a=0;
            for(a=0; a<50; a++)
            {
                fprintf(fichier, "%d ", deplacements_ennemis[a++]);
            }
            allegro_message("mouvement enregistré!");
            fclose(fichier);
            break;
        case 2:
            return 1;
        }
        buffer=affiche_buffer(affiche);
    }
    return 0;
}
int sauvegarde_niveau(int valeurs[15][19], char mot[50])
{
    int nb,i=0,j=0;
    FILE* noms_sauvegardes;
    FILE* sauvegarde;
    noms_sauvegardes= fopen("noms_sauvegardes.txt", "r+t");
    fscanf(noms_sauvegardes, "%d", &nb);
    if (strlen(mot)<44)
    {
        strcat(mot, ".txt");
    }
    else
    {
        allegro_message("le nom est trop long!");
        return 1;
    }
    sauvegarde = fopen(mot, "w+");

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
            fprintf(sauvegarde, "%d ", valeurs[i][j]);
        }
    }
    fclose(sauvegarde);
    fclose(noms_sauvegardes);
    return 0;
}
int conversion(int num, int *compteur)
{
    switch (num)
    {
    case 0:
        num= 2;
        break;
    case 1:
        num= -2;
        break;
    case 2:
        num= -50;
        break;

    case 3:
        num= 100;
        break;
    case 4:
        num= 1;
        break;
    case 5:
        num= 66;
        break;
    case 6:
        num= -1;
        break;
    case 7:
        num= 0;
        break;
    case 8:
        num= 3;
        break;
    case 9:
        num= 5;
        break;
    case 10:
        num=4;
        break;
    case 11:
        num= 7;
        break;
    case 12:
        num=8;
        break;
    case 13:
        num=9;
    break;
    case 14:
        num=101+((*compteur)%2)*10+(*compteur)-(*compteur)%2;
        (*compteur)++;
        break;
    default:
        num=-2;
    }
    return num;
}

int menu_oui_non(BITMAP* buffer, char* mot)
{
    BITMAP* sauvegarde;
    BITMAP* ecran_noir;
    int compteur=0;
    ecran_noir= create_bitmap(192+5*32, 160);
    clear_to_color(ecran_noir, makecol(0, 0, 0));
    sauvegarde= create_bitmap(19*TSPRITE, 15*TSPRITE);
    textprintf_ex(ecran_noir, font, 0, 0, makecol(255, 255, 255),-1, "voulez-vous sauvegarder ce %s?", mot);
    blit(ecran_noir, sauvegarde,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    textprintf_ex(ecran_noir, font, 0, LIGNE+7, makecol(255, 255, 255),-1, "-> oui        non");
    draw_sprite(buffer, ecran_noir, 192-2*32, 160);
    blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    rest(200);
    while(!key[KEY_ENTER])
    {
        if((key[KEY_LEFT])||(key[KEY_A]))
        {
            blit(sauvegarde, ecran_noir,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            textprintf_ex(ecran_noir, font, 0, LIGNE+7, makecol(255, 255, 255),-1, "-> oui        non");
            draw_sprite(buffer, ecran_noir, 192-2*32, 160);
            blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            compteur=0;
        }
        if((key[KEY_RIGHT]||key[KEY_D]))
        {
            blit(sauvegarde, ecran_noir,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            textprintf_ex(ecran_noir, font, 0, LIGNE+7, makecol(255, 255, 255),-1, "   oui     -> non");
            draw_sprite(buffer, ecran_noir, 192-2*32, 160);
            blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
            compteur=1;
        }
        if (key[KEY_ESC]) return 2;
    }
    clear_keybuf();
    return compteur;
}

int editeur_de_case(BITMAP* affiche[15][19], int valeurs[15][19])
{
    int compteur_teleport=0, compteur_sortie, compteur_joueur, compteur_serrure, compteur_clef;
    int Case=0;
    int test=1,x=100,y=100;
    int page=0;
    int i,j;
    BITMAP* affiche2[NBSPRITE];
    init_mode_graphique(1, affiche2);
    show_mouse(screen);
    rest(500);
    page_flip(affiche, &page);
    while (test)
    {


        if(mouse_needs_poll())poll_mouse();
        if ((mouse_b & 1))
        {
            if (((int)(mouse_x-mouse_x%TSPRITE)/TSPRITE!=x)||((int)(mouse_y-mouse_y%TSPRITE)/TSPRITE!=y))
            {
                Case=0;
                x = (int)(mouse_x-mouse_x%TSPRITE)/TSPRITE;
                y =(int)(mouse_y-mouse_y%TSPRITE)/TSPRITE;
                affiche[y][x] = affiche2[Case];
                valeurs[y][x] = conversion(Case, &compteur_teleport);
                page_flip(affiche, &page);
                rest(100);
            }
            else
            {
                if (Case==14)
                {
                    Case=-1;
                    compteur_teleport--;
                }
                affiche[y][x]=affiche2[++Case];
                valeurs[y][x]= conversion(Case, &compteur_teleport);
                page_flip(affiche, &page);
                rest(100);

            }

        }
        if (key[KEY_ESC]) return 1;
        if (key[KEY_ENTER])
        {
            compteur_joueur=0;
            compteur_sortie=0;
            compteur_clef=0;
            compteur_serrure=0;
            if (compteur_teleport%2==1)
            {
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
                        case 7:
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
                if (compteur_sortie>1) allegro_message("il y a plus d'un point de sortie!\n");
                else if (compteur_joueur>1) allegro_message("il y a plus d'un joueur!");
                else if(compteur_clef!=compteur_serrure) allegro_message("il n'y a pas le meme nombre de serrures que de clef!");
                else return 0;
            }

        }

        if(mouse_b & 2)
        {

            Case=7;
            x= (int)(mouse_x-mouse_x%TSPRITE)/TSPRITE;
            y=(int)(mouse_y-mouse_y%TSPRITE)/TSPRITE;
            if(affiche[y][x]==affiche2[14]) compteur_teleport--;
            affiche[y][x]=affiche2[Case];
            valeurs[y][x]= conversion(Case, &compteur_teleport);
            page_flip(affiche, &page);
            rest(100);
        }
    }
    return 0;
}
int worst_writing_ever(char mot[50], BITMAP* buffer, BITMAP* ecran_noir, int line, BITMAP* affiche[15][19])
{
    int a=0;
    int key_allegro;
    char key_ascii;
    int i=0;
    draw_sprite(buffer, ecran_noir, 192-2*32, 160);
    blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
    rest(200);
    while(!key[KEY_ENTER])
    {
        if (key[KEY_ESC]) return 1;
        if (keypressed())
        {key_allegro=readkey();
        key_ascii= key_allegro & 0xff;

        if((key_ascii >= 32) && (key_ascii <= 126))
        {
            if (i<49)
            {
                mot[i]= key_ascii;
                for(a=0; a<i+1; a++)textprintf_ex(ecran_noir, font, a*text_length(font, "O"), line*LIGNE+5, makecol(255, 255, 255),-1, "%c", mot[a]);
                a=0;
                destroy_bitmap(buffer);
                buffer=affiche_buffer(affiche);
                draw_sprite(buffer, ecran_noir, 192-2*32, 160);
                blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
                mot[i+1]='\0';
                i++;
            }
        }
    }
    }

    mot[i]='\0';
    return 0;
}

BITMAP* affiche_buffer(BITMAP* affiche[15][19])
{
    BITMAP* buffer;
    buffer= create_bitmap(TSPRITE*19, TSPRITE*15);
    int i,j;

    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], TSPRITE*j, TSPRITE*i);
        }
    }

    return buffer;
}
void level_editor()
{
    int i,j;
    BITMAP* buffer;
    BITMAP* ecran_noir;
    char mot[50];
    mot[0]='c';
    mot[1]='\0';
    install_mouse();
    enable_hardware_cursor();
    install_timer();
    BITMAP* affiche[15][19]= {{load_bitmap("mur.bmp", NULL)}};
    ecran_noir= create_bitmap(192+5*32, 160);
    clear_to_color(ecran_noir, makecol(0, 0, 0));
    buffer= create_bitmap(TSPRITE*19, TSPRITE*15);
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, TSPRITE*19, TSPRITE*15, 0, 0)==0)
    {
        affiche[0][0]=load_bitmap("mur.bmp", NULL);
        for (i=0; i<15; i++)
        {
            for(j=0; j<19; j++)
            {
                affiche[i][j]=affiche[0][0];
                if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], TSPRITE*j, TSPRITE*i);
            }
        }
        textprintf_ex(ecran_noir, font, 0, LIGNE, makecol(255,255,255),-1, "bienvenue sur l'editeur de niveau");
        draw_sprite(buffer, ecran_noir, 192-2*32, 160);
        blit(buffer, screen,0,0,0,0, 19*TSPRITE, 15*TSPRITE);
        rest(1100);
        menu(buffer, affiche, mot);
        destroy_bitmap(buffer);
        destroy_bitmap(ecran_noir);
        allegro_exit();
        }
}
int menu(BITMAP* buffer, BITMAP* affiche[15][19], char mot[50])
{
    int x=1,i,j;
    BITMAP* ecran_noir;
    BITMAP* sauvegarde[15][19];
    BITMAP* ecran_selec;
    int valeurs[15][19];
    int line;
    ecran_noir= create_bitmap(192+5*32, 160);
    ecran_selec= create_bitmap(192+5*32, 160);
    clear_to_color(ecran_noir, makecol(0, 0, 0));
    textprintf_ex(ecran_noir, font, 0, LIGNE, makecol(255,255,255),-1, "Que souhaitez-vous faire?");
    textprintf_ex(ecran_noir, font, 0, 2*LIGNE+3, makecol(255,255,255),-1, "'dessin' pour commencer a dessiner");
    textprintf_ex(ecran_noir, font, 0, 3*LIGNE+3, makecol(255,255,255),-1, "'aide' pour voir l'aide au dessin!important!");
    blit(ecran_noir, ecran_selec, 0,0,0,0, 192+5*32, 160 );
    line=4;
    affiche[0][0]=load_bitmap("mur.bmp", NULL);
    for (i=0; i<15; i++)
    {
        for(j=0; j<19; j++)
        {
            valeurs[i][j]=-2;
            affiche[i][j]=affiche[0][0];
            if (affiche[i][j]!=NULL) draw_sprite(buffer, affiche[i][j], TSPRITE*j, TSPRITE*i);
        }
    }
    draw_sprite(buffer, ecran_noir, 192-2*32, 160);
    blit(buffer, screen,0,0,0,0, 21*TSPRITE, 15*TSPRITE);
    while(x)
    {
        i=0;
        j=0;
        for (i=0; i<15; i++)
        {
            for(j=0; j<19; j++)
            {

                sauvegarde[i][j]=affiche[i][j];
            }
        }
        if(worst_writing_ever(mot, buffer, ecran_noir, line, affiche)) return 0;
        if (strcmp(mot, "dessin"))
        {
            if (strcmp(mot, "aide"))
            {
                allegro_message("ce n'est pas une entree valable");
            }
            else
            {
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
                while(!key[KEY_ENTER]) if (key[KEY_ESC])
                    {
                destroy_bitmap(ecran_noir);
                destroy_bitmap(ecran_selec);
                return 0;
                }
            }
        }
        else
        {
            if (editeur_de_case(affiche, valeurs))
            {
                destroy_bitmap(ecran_noir);
                destroy_bitmap(ecran_selec);
                return 0;
            }
            buffer=affiche_buffer(affiche);
            draw_sprite(buffer, ecran_noir, 192-2*32, 160);
            blit(buffer, screen,0,0,0,0, 21*TSPRITE, 15*TSPRITE);
            x=menu_oui_non(buffer, "dessin");
            line++;
            if (x==2)
            {
                destroy_bitmap(ecran_noir);
                destroy_bitmap(ecran_selec);
                return 1;
            }
            if(x==0)
            {
                clear_to_color(ecran_noir, makecol(0,0,0));
                textprintf_ex(ecran_noir, font, 0, 0, makecol(255,255,255),-1, "entrez le nom de votre niveau");
                if ((worst_writing_ever(mot, buffer, ecran_noir, 1, affiche))||(ennemy_editor(affiche, valeurs, mot)))
                {
                    destroy_bitmap(ecran_noir);
                    destroy_bitmap(ecran_selec);
                    return 0;
                }

                if (sauvegarde_niveau(valeurs, mot));
                else
                {
                    allegro_message("niveau enregistre!");
                    x=0;
                }
            }
            else
            {

                for (i=0; i<15; i++)
                {
                    for(j=0; j<19; j++)
                    {
                        affiche[i][j]=sauvegarde[i][j];
                    }
                }
            }
        }
        blit(ecran_selec, ecran_noir, 0,0,0,0, 192+5*32, 160 );
    }
    return 0;
}

