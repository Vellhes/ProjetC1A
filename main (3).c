#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define VMIN 5
#define VMAX 30

typedef struct T_Case
{
    int x;
    int y;
} T_Case;
typedef T_Case* T_Tab_Case; //T_Case[] ne fonctionne pas

int Contiens(T_Tab_Case table, int taille, T_Case c)
{
    int i;
    for (i = 0;i<taille;i++)
        if (table[i].x == c.x && table[i].y == c.y)
            return 1;
    return 0;
}

int Lire_Entier(int min, int max)
{
    int result;
    do
    {
        scanf("%d", &result);
        getchar();
    } while (result < min || result > max);
    return result;
}

int Hasard_Ban(T_Tab_Case ban, int nlig, int ncol)
{
    int nban = rand() % (ncol > nlig ? ncol : nlig), i;
    for (i = 0;i<nban;i++)
    {
        int x;
        int y;
        T_Case caseActuelle;
        while (1)
        {
            x = rand() % ncol;
            y = rand() % nlig;
            caseActuelle.x = x;
            caseActuelle.y = y;
            //si la case se trouve dans un des deux coins autorisés ou
            //si la case se trouve dans la "partie centrale" du plateau
            if ((x == 0 && y == nlig-1 ||
                    x == nlig-1 && y == 0)
                    ||
                    (x > 0 && x < ncol-1 &&
                    y > 0 && y < nlig-1))
            {
                //la case se trouve à un emplacement de plateau autorisé
                //on vérifie que la case ne soi pas déjà bannie
                if (!Contiens(ban, i, caseActuelle))
                {
                    T_Case caseHautDroite, caseBasGauche;
                    caseHautDroite.x = x+1;
                    caseHautDroite.y = y-1;
                    caseBasGauche.x = x-1;
                    caseBasGauche.y = y+1;
                    //s'il s'agit de la case en bas à gauche et qu'elle n'a pas de case bannnies dans la diagonale
                    if (x == 0 && !Contiens(ban, i, caseHautDroite))
                        break;
                    //s'il s'agit de la case en haut à droite et qu'elle n'a pas de case bannnies dans la diagonale
                    else if (y == 0 && !Contiens(ban, i, caseBasGauche))
                        break;
                    //sinon si aucune des cases dans sa diagonale (cases interdites) n'est occupée
                    else if (!Contiens(ban, i, caseBasGauche) && !Contiens(ban, i, caseHautDroite))
                        break;
                }
            }
        }
        ban[i] = caseActuelle;//on ajoute la case bannie
    }

    return nban;
}

void Parametres(int* nlig, int* ncol, int* niveau, int* next)
{
    printf("Nombre de colonnes de la grille (entre %d et %d) :", VMIN, VMAX);
    *ncol = Lire_Entier(VMIN, VMAX);
    printf("Nombre de lignes de la grille (entre %d et %d) :", VMIN, VMAX);
    *nlig = Lire_Entier(VMIN, VMAX);
    printf("Difficulte (entre 1 et 4) :");
    *niveau = Lire_Entier(1, 4);
    printf("1 si l'ordi commence, sinon 2 :");
    *next = Lire_Entier(1, 2);
}

void Calcul_Nimbers(int nim[][VMAX], int nlig, int ncol)
{
    int x, y;
    int nimber = 0;
    for (x = ncol - 1 ; x >= 0 ; x--)
    {
        int baseNimber = nimber;
        for (y = nlig - 1 ; y >= 0 ; y--)
        {
            nim[x][y] = nimber > 0 ? 1:0;
            nimber = (nimber+1)%3;
        }
        nimber = (baseNimber+1)%3;
    }
}

void Dessin_plateau(T_Case pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    printf("\n  ");
    if (nlig >= 10)
        printf(" ");
    int i;
    for (i = 1;i<=ncol;i++)
    {
        printf("%d", i);
        if (i < 10)
            printf(" ");
    }
    printf("\n");
    int ligne, colonne;
    for (ligne = 0;ligne<nlig;ligne++)
    {
        printf("%d", ligne+1);
        if (nlig >= 10 && ligne < 9)
            printf(" ");
        printf("|");
        for (colonne = 0;colonne<ncol;colonne++)
        {
            T_Case caseActuelle;
            caseActuelle.x = colonne;
            caseActuelle.y = ligne;
            if (pion.x == caseActuelle.x && pion.y == caseActuelle.y)
                printf("O");
            else if (Contiens(ban, nban, caseActuelle))
                printf("X");
            else
                printf("-");
            printf("|");
        }
        printf("\n");
    }
    printf("\n");
}
int Voisines(T_Tab_Case vois, T_Case pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    int nvois = 0;
    {
        T_Case droite;
        droite.x = pion.x+1;
        droite.y = pion.y;
        if (droite.x < ncol && !Contiens(ban, nban, droite))
        {
            vois[nvois++] = droite;
            droite.x++;
            if (droite.x < ncol && !Contiens(ban, nban, droite))
                vois[nvois++] = droite;
        }
    }
    {
        T_Case bas;
        bas.x = pion.x;
        bas.y = pion.y+1;
        if (bas.y < nlig && !Contiens(ban, nban, bas))
        {
            vois[nvois++] = bas;
            bas.y++;
            if (bas.y < nlig && !Contiens(ban, nban, bas))
                vois[nvois++] = bas;
        }
    }
    return nvois;
}
void Coup_joueur(T_Case* pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    printf("A toi de jouer !\n");
    T_Case vois[4];
    int nvois = Voisines(vois, *pion, ban, nban, nlig, ncol);
    int selection;
    if (nvois == 1)
    {
        printf("Seule possibilite : (%d,%d)\nvalider --->\n", vois[0].y+1, vois[0].x+1);
        getchar();
        selection = 0;
    }
    else
    {
        printf("choisir la destination");
        int i;
        for (i = 0;i<nvois;i++)
            printf(" %d:(%d,%d)", i+1, vois[i].y+1, vois[i].x+1);
        printf("\n---> ");
        selection = Lire_Entier(1, nvois+1)-1;
    }
    *pion = vois[selection];
}
void Coup_Ordi_Hasard(T_Case* pion, T_Tab_Case ban, int nban, int nlig, int ncol)
{
    T_Case vois[4];
    int nvois = Voisines(vois, *pion, ban, nban, nlig, ncol);
    int selection = rand()%nvois;
    *pion = vois[selection];
    printf("L'ordinateur deplace le pion en (%d,%d)\n", pion->y+1, pion->x+1);
}
void Coup_Ordi_Gagnant(T_Case* pion, T_Tab_Case ban, int nim[][VMAX], int nban, int nlig, int ncol)
{
    T_Case vois[4];
    int nvois = Voisines(vois, *pion, ban, nban, nlig, ncol);
    int i;
    for (i = 0;i<nvois;i++)
    {
        T_Case caseActuelle = vois[i];
        if (nim[caseActuelle.x][caseActuelle.y] == 0)
        {
            *pion = vois[i];
            printf("L'ordinateur deplace le pion en (%d,%d)\n", pion->y+1, pion->x+1);
            return;
        }
    }
    Coup_Ordi_Hasard(pion, ban, nban, nlig, ncol);
}

int main()
{
    srand(time(NULL));
    T_Case pion;
    pion.x = 0;
    pion.y = 0;
    T_Case ban[VMAX];
    int nim[VMAX][VMAX];
    int nlig, ncol, niveau, next;
    Parametres(&nlig, &ncol, &niveau, &next);
    int nban = Hasard_Ban(ban, nlig, ncol);
    Calcul_Nimbers(nim, nlig, ncol);
    printf("\nC'est parti !\n");

    while (pion.x < ncol-1 || pion.y < nlig-1)
    {
        Dessin_plateau(pion, ban, nban, nlig, ncol);
        if (next == 1)
        {
            switch(niveau)
            {
                case 1:
                    Coup_Ordi_Hasard(&pion, ban, nban, nlig, ncol);
                    break;
                case 2:
                    if (rand()%3 == 0)
                        Coup_Ordi_Gagnant(&pion, ban, nim, nban, nlig, ncol);
                    else
                        Coup_Ordi_Hasard(&pion, ban, nban, nlig, ncol);
                    break;
                case 3:
                    if (rand()%3 == 0)
                        Coup_Ordi_Hasard(&pion, ban, nban, nlig, ncol);
                    else
                        Coup_Ordi_Gagnant(&pion, ban, nim, nban, nlig, ncol);
                    break;
                case 4:
                    Coup_Ordi_Gagnant(&pion, ban, nim, nban, nlig, ncol);
                    break;
            }
            next = 2;
        }
        else
        {
            Coup_joueur(&pion, ban, nban, nlig, ncol);
            next = 1;
        }
        printf("%d   %d\n", pion.x, pion.y);
    }
    Dessin_plateau(pion, ban, nban, nlig, ncol);
    if (next == 1)
        printf("C'est termine. BRAVO TU AS GAGNE !\n");
    else
        printf("C'est termine. L'ordinateur a gagne :( !\n");
    return 0;
}