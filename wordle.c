#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// functie care adauga intr-un sir
// toate cuvintele pe care le poate
// cere jocul de la jucator
void initializare(char **sir)
{
    strcpy(sir[0], "arici");
    strcpy(sir[1], "atent");
    strcpy(sir[2], "baiat");
    strcpy(sir[3], "ceata");
    strcpy(sir[4], "debut");
    strcpy(sir[5], "peste");
    strcpy(sir[6], "fixat");
    strcpy(sir[7], "hamac");
    strcpy(sir[8], "harta");
    strcpy(sir[9], "jalon");
    strcpy(sir[10], "jucam");
    strcpy(sir[11], "lacat");
    strcpy(sir[12], "magie");
    strcpy(sir[13], "nufar");
    strcpy(sir[14], "oaste");
    strcpy(sir[15], "perus");
    strcpy(sir[16], "rigle");
    strcpy(sir[17], "roman");
    strcpy(sir[18], "sanie");
    strcpy(sir[19], "scris");
    strcpy(sir[20], "sonda");
    strcpy(sir[21], "texte");
    strcpy(sir[22], "tipar");
    strcpy(sir[23], "titan");
    strcpy(sir[24], "zebra");
    strcpy(sir[25], "vapor");
    strcpy(sir[26], "vatra");
}

int main()
{
    int i, cuv_ales, gasit = 0, j, ch, k = 0, ok, culori[10], corect, joc = 1;
    int max_y, max_x, cx, cy;
    // declaram si initializam tot sirul de cuvinte posibile
    char **cuvinte;
    char cuv[5];
    cuvinte = (char **)malloc(30 * sizeof(char *));
    for (i = 0; i < 30; i++)
    {
        cuvinte[i] = (char *)malloc(10 * sizeof(char));
    }
    initializare(cuvinte);
    // alegem un cuvant random penru joc
    srand(time(NULL));
    cuv_ales = rand() % 27;
    // intram in modul curses
    initscr();
    // construim perechile de culori
    start_color();
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    noecho(); // nu afisam tastele pe ecran
    raw();    // tastele nu trimit si semnale la program
    // aflam dimensiunea ferestrei si o potrivim
    // pentru a putea aseza chenarul bine
    getmaxyx(stdscr, max_y, max_x);
    max_y = max_y - (max_y % 2);
    max_x = max_x - (max_x % 2);
    // incepem jocul
    while (joc)
    {
        // alegem un cuant random pentru joc
        srand(time(NULL));
        cuv_ales = rand() % 27;
        // afisam numele jocului si tabela de joc
        mvprintw(0, (max_x / 2) - 2, "WORDLE");
        for (i = 2; i <= 14; i = i + 2)
        {
            mvprintw(i, (max_x / 2) - 4, "-----------");
        }
        for (i = 3; i <= 13; i = i + 2)
        {
            mvprintw(i, (max_x / 2) - 4, "| | | | | |");
        }
        move(3, max_x / 2 - 3);
        // incepe introducerea cuvintelor
        for (j = 3; j <= 13 && gasit == 0; j = j + 2)
        {
            move(j, max_x / 2 - 3);
            i = -3;
            ok = 0;
            k = 0;
            corect = 0;
            while (!ok)
            {
                // preluam fiecare tasta apasata de utilizator
                ch = getch();
                switch (ch)
                {
                // daca tasta e delete
                case 127:
                    if (i != -3)
                    {
                        // mutam cursorul si afisam spatiu pe locul literei pe care o stergem
                        mvprintw(j, max_x / 2 + i - 2, "%c", ' ');
                        // i merge cu o pozitie mai in urma
                        i = i - 2;
                        // mutam cursorul pe pozitia pe care urmeaza sa introduce,m urmatoarea litera
                        move(j, max_x / 2 + i);
                        i = i - 2;
                        // sterge litera din sirul pe care il vom compara cu sirul ales random
                        k--;
                    }
                    break;
                // daca utilizatorul apasa ENTER
                case 10:
                {
                    if (i == 7)
                    {
                        // daca cuvantul are 5 literea
                        // trecem la faza de verificare a cuvantului
                        ok = 1;
                        // stergem potentialul mesaj de eroare
                        getyx(stdscr, cy, cx);
                        mvprintw(j, max_x / 2 + 10, "%s", "                        ");
                        move(cy, cx);
                    }
                    else
                    {
                        // daca nu, i ramane la valoarea actuala si tasta este ignorata
                        i = i - 2;
                        getyx(stdscr, cy, cx);
                        // apare mesajul care ii spune jucatorului despre lungimea cuvintelor
                        mvprintw(j, max_x / 2 + 10, "%s", "CUVANTUL NU ARE 5 LITERE");
                        move(cy, cx);
                    }
                }
                break;
                default:
                    if (ch != 127 && ch != 10)
                    {
                        // daca este introdusa o litera
                        // verificam sa nu avem deja 5 litere introduse
                        if (i >= -3 && i <= 5)
                        {
                            // daca nu, o adaugam la cuvant
                            mvprintw(j, max_x / 2 + i, "%c", ch);
                            move(j, max_x / 2 + i + 2);
                            cuv[k] = (char)ch;
                            k++;
                        }
                        else
                            // daca da, o ignoram
                            i = i - 2;
                    }
                    break;
                }
                i = i + 2;
                refresh();
            }
            // incepe faza de verificare
            cuv[5] = '\0';
            // initializam sirul ce contine perechea de culori
            // coresunzatoare fiecarei litere din cuvant
            for (i = 0; i < 5; i++)
            {
                culori[i] = 0;
            }
            // incepem verificarea
            for (i = 0; i < 5; i++)
            {
                culori[i] = 1; // initial toate literele sunt normale
                if (cuv[i] == cuvinte[cuv_ales][i])
                {
                    // daca literele corespund fix pe fix le coloram in verde
                    //si marcam ca a fost corecta
                    culori[i] = 3;
                    corect++;
                }
                else
                {
                    //daca litera exista in cuvant o coloram cu galben
                    if (strchr(cuvinte[cuv_ales], cuv[i]))
                        culori[i] = 2;
                }
            }
            //afisam literele in culorile pe care
            //le-am determinat mai devreme
            for (i = 0; i < 5; i++)
            {
                attron(COLOR_PAIR(culori[i]));
                mvprintw(j, max_x / 2 + (-3 + 2 * i), "%c", cuv[i]);
                attroff(COLOR_PAIR(culori[i]));
                refresh();
            }
            //verificam daca cuvantul introdus este cel cautat
            if (corect == 5)
                gasit = 1;
            refresh();
        }
        //daca cuvantul a fost gasit sau daca jucatorul nu mai are incercari,
        //trecem la faza de sfarsit a jocului
        if (gasit)
        {
            //daca cuvantul a fost gasit, jucatorul a castigat
            mvprintw(15, max_x / 2 - 3, "AI CASTIGAT");
            mvprintw(16, max_x / 2 - 3, "                                  ");
        }
        else
        {
            //daca nu, a pierdut si se afisaza cuvantul care trebuia gasit
            mvprintw(15, max_x / 2 - 3, "AI PIERDUT");
            mvprintw(16, max_x / 2 - 3, "CUVANTUL ERA %s", cuvinte[cuv_ales]);
        }
        //apoi afisam optiunile de la sfarsitul unui joc
        mvprintw(18, max_x / 2 - 11, "%s", "APASA ESC PENTRU A IESI DIN JOC");
        mvprintw(19, max_x / 2 - 11, "%s", "APASA ORICE ALTA TASTA PENTRU UN JOC NOU");
        refresh();
        ch = getch();
        //daca jucatorul apasa ESC, jocul se termina si se inchide fereasta
        if (ch == 27)
        {
            joc = 0;
        }
        else
        {
            //daca este apasa orice alta tasta, atunci se golestre ecranul
            //de tot scrisul si jocul o ia de la inceput
            //(adica de la alegera cuavntului si generarea tabelei de joc)
            for (i = i; i <= max_x; i++)
                for (j = 1; j < max_y; j++)
                {
                    mvprintw(j, i, "%c", ' ');
                }
        }
    }
    //inchidem fereastra
    endwin();
    return 0;
}
