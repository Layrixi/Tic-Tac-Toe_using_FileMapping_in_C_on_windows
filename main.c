#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#define MEM_SIZE 1000
typedef struct {
    int x, y;            //y => i, x => j
    char* wiadomosc;
    char czyjakolej;    //przyjmuje jedynie 'x' lub 'o'
    int runda;
}ruch;

bool sprawdz_czy_wygral(char plansza[3][3],char gracz)
{
    if (plansza[0][0] == gracz && plansza[0][1] == gracz && plansza[0][2] == gracz)
        return 1;
    if (plansza[1][0] == gracz && plansza[1][1] == gracz && plansza[1][2] == gracz)
        return 1;
    if (plansza[3][0] == gracz && plansza[3][1] == gracz && plansza[3][2] == gracz)
        return 1;
    if (plansza[0][0] == gracz && plansza[1][0] == gracz && plansza[2][0] == gracz)
        return 1;
    if (plansza[0][1] == gracz && plansza[1][1] == gracz && plansza[2][1] == gracz)
        return 1;
    if (plansza[0][1] == gracz && plansza[1][1] == gracz && plansza[2][1] == gracz)
        return 1;
    if (plansza[0][0] == gracz && plansza[1][1] == gracz && plansza[2][2] == gracz)
        return 1;
    if (plansza[2][0] == gracz && plansza[1][1] == gracz && plansza[0][2] == gracz)
        return 1;
    return 0;
}

void print_plansza(char plansza[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf(" %c |", plansza[i][j]);
        printf("\n---+---+---\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s name\n", argv[0]);
        return 1;
    }
    ruch mojruch;
    ruch *contr;//'x' zaczyna
    HANDLE MapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        argv[1]);               // name of mapping object
    char gracz,gracz2;
    if (MapFile == NULL) {
        fprintf(stderr, "Mapka nie istnieje wiec tworze mapke\n");
        mojruch.czyjakolej = 'x';
        gracz = 'x';
        gracz2 = 'o';
        MapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            MEM_SIZE,                // maximum object size (low-order DWORD)
            argv[1]);                // name of mapping object

        if (MapFile == NULL)
        {

            fprintf(stderr, "Nie udalo siê stworzyæ?o.o\n");
            return 1;
        }
    }
    else
    {
        fprintf(stdout, "mapka juz stworzona;)))\n");
        mojruch.czyjakolej = 'o';
        gracz = 'o';
        gracz2 = 'x';
    }
    printf("jestem: %c\n", gracz);
    //----------------mapa stworzona--------------lub do³¹czona 
    
    void* Buf = MapViewOfFile(
        MapFile,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        MEM_SIZE);

    if (Buf == NULL)
    {
        fprintf(stderr, "Buffer nie stworzyl sb\n");
        CloseHandle(MapFile);
        return 3;
    }
    mojruch.runda = 0;
    //------------bffer--------------
    char plansza[3][3] = { {' ',' ',' '}, {' ',' ',' '}, {' ',' ',' '}};
    //-------------Petla glowna---------//
    
    bool koniec = 0;

    contr = (ruch*)(Buf);
    if (contr->czyjakolej == 'x' || contr->czyjakolej == 'o')
    {
        mojruch.runda = contr->runda;
        //buffor nie jest pusty
    }
    else
    {
        memcpy(Buf, &mojruch, sizeof(ruch));
        //bufor jest pusty
    }
    printf("Runda w pamieci: %d\n", contr->runda);
    if (mojruch.runda != contr->runda)
        mojruch.runda = contr->runda;
    while (koniec==0)// sprawdzic czy wszystko sie zgadza zgodnie z wymaganiami. Ja teraz ide spac.
    {
        if (sprawdz_czy_wygral(plansza, gracz) == 1)
        {
            printf("GRACZ %C WYGRAL!!!!\n", gracz);
            break;
        }
        else if (sprawdz_czy_wygral(plansza, gracz2) == 1)
        {
            printf("GRACZ %C WYGRAL!!!!\n", gracz2);
            break;
        }
        
        contr = (ruch*)(Buf);
        printf("---------\ngracz to: %c\t teraz jest kolej:%c\tRunda nr: %d\n-----------------\n", gracz,contr->czyjakolej,mojruch.runda);
        if (contr->czyjakolej == gracz)//wykonaj swoj ruch
        {
            //wykonaj ruch
            mojruch.runda++;
            system("CLS");
            print_plansza(plansza);
            while (1 == 1) {
                printf("Podaj wiersz (1 do 9)w ktorym postawisz %c : ",gracz);
                scanf_s("%d", &mojruch.y);
                getchar();
                mojruch.y--;
                mojruch.x = mojruch.y % 3;
                mojruch.y /= 3;
                if (mojruch.x >= 0 && mojruch.x <= 2 && mojruch.y >= 0 && mojruch.y <= 2 && plansza[mojruch.y][mojruch.x] != gracz2 && plansza[mojruch.y][mojruch.x] != gracz)
                    break;
                else
                    printf("Koordynaty nieprawid³owe lub przeciwnik ju¿ tam stoi. Spróbuj ponownie\n");
            }
            printf("Wykonuje ruch: %d, %d\n", mojruch.x+1, mojruch.y+1);
            plansza[mojruch.y][mojruch.x] = gracz;
            mojruch.wiadomosc = "wykonalem ruch";
            if (gracz == 'o')
                mojruch.czyjakolej = 'x';
            else
                mojruch.czyjakolej = 'o';
            
            memcpy(Buf, &mojruch, sizeof(ruch));
        }
        else                          //czekaj na swoj ruch
        {
            printf("czekam\n");
            while (1==1)
            {
                contr = (ruch*)(Buf);
                if (contr->czyjakolej == gracz)
                    break;
            }
            //odswiez plansze, odczytaj ruch i wgl
            printf("Przeciwnik wykonal ruch: %d ,%d\n", contr->x, contr->y);
            mojruch.runda=contr->runda;
            plansza[contr->y][contr->x] = gracz2;
        }            
        system("CLS");
        print_plansza(plansza);
        if (mojruch.runda == 9)
        {
            koniec = 1;
            printf("REMIS!\n");
        }
    }
    puts("Koniec gry. Wciœnij cokolwiek by wyjsc :)");
    getch();
    //-------------!Petla glowna--------//
    //-----------------zamykanie handle'a---------------
    if (!UnmapViewOfFile(Buf)) {
        fprintf(stderr, "Nie udalo sie odmapowac widoku mapki\n");
        CloseHandle(MapFile);
        return 4;
    }
    CloseHandle(MapFile);
    return 0;
}