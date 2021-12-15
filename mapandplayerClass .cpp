//Алексеева Полина
#include <TXLib.h>
#include <stdlib.h>

void GetTextNumber(int n,char* st); // функция преобразования числа в строку
void FinalMusic(); // проигрывание музки в конце игры
void NextLevelMusic(); // музыка для следующего уровня
void GetCristall();

class Map
{
private:
    int n;  //столбец
    int m;  //строка
    HDC sprite;
    //HDC mineral;
public:
    int karta[9][9]={{0,0,0,0,0,0,0,0,0},
                     {0,4,4,4,1,1,1,0,0},
                     {0,1,1,1,1,0,1,1,0},
                     {0,4,0,0,1,1,4,4,0},
                     {0,4,0,1,1,0,4,1,0},
                     {0,1,0,0,0,1,1,1,0},
                     {0,4,4,0,1,1,1,1,0},
                     {0,1,1,1,1,0,1,1,0},
                     {0,0,0,0,0,0,0,0,0}
                     };

    int level2[9][9]= {{0,0,0,0,0,0,0,0,0},
                       {0,4,1,1,4,0,4,1,0},
                       {0,1,0,0,0,0,4,4,0},
                       {0,1,4,4,0,0,1,1,0},
                       {0,1,1,1,1,1,1,0,0},
                       {0,4,0,1,0,0,1,0,0},
                       {0,1,4,1,1,1,4,1,0},
                       {0,0,0,0,0,0,0,0,0},
                       };
    int level3[9][9]= {{0,0,0,0,0,0,0,0,0},
                       {0,4,0,0,1,1,1,1,0},
                       {0,1,1,1,4,1,4,4,0},
                       {0,4,1,4,4,1,1,1,0},
                       {0,4,1,0,1,1,0,0,0},
                       {0,1,1,0,1,1,0,4,0},
                       {0,1,1,4,1,1,1,1,0},
                       {0,0,0,0,0,0,0,0,0},
                       };


    Map()    //конструктор
    {
        n=9;
        m=9;
        sprite = txLoadImage("texture.bmp");

    }

    //void init();
    void draw();
    int  get_cell(int x, int y);
    void set_cell(int x, int y,int type);
    void check_mouse();
    void tabu_block();

};

class player
{
private:
    int h;    //высота кадра
    int w;    //ширина кадра
    int xpl;  //координата какрда
    int ypl;  //координата какрда
    int x;    //где рисовать
    int y;    //где рисовать
public:
    player()
    {
        x = 100, y = 80;
        h = 64;
        w = 64;
        xpl = 0;
        ypl = 192;
        sprite = txLoadImage("sprite.bmp");
        score=0;
        playerlevel=1; // ставим 1 уровень
    }
    HDC sprite;
    void Draw();
    void Right();
    void Left();
    void Up();
    void Down();
    void check_keys();
    void SetLevel(int level); // устанавливаем новый уровень !

    COLORREF tabupixel;  //красный фон, на который нельзя
    Map* mp; //тип переменных Map в player, чтобы player имел доступ к Map
    int score; // очки за кристаллы
    int playerlevel;
};

int main()
{
    txCreateWindow(800, 600);
    txSetColor(TX_BLACK);
    player pl;
    Map m;
    char scr[4]="";
    char lvl[2]="";
    int exit=0;

    pl.mp=&m;  //чтобы персонаж пользовался картой

    while (!GetAsyncKeyState(VK_ESCAPE) && exit==0)
    {
        GetTextNumber(pl.score,scr);
        GetTextNumber(pl.playerlevel,lvl);
        txBegin();
        txClear();
        m.draw();
        pl.check_keys();
        m.check_mouse();
        pl.Draw();

        txTextOut(600, 10,"Level: " );
        txTextOut(750, 10,lvl );

        if (pl.score<10)
        {
            txTextOut(600, 40,"Cristalls: " );
            txTextOut(750, 40,scr );
        }
        else
        {

           if (pl.playerlevel==1)
           {
               pl.SetLevel(2);
               NextLevelMusic();
           }
           else if (pl.playerlevel==2)
           {
               pl.SetLevel(3);
               NextLevelMusic();
           }
           else
           {
               // уровни кончились
               txTextOut(600, 40,"Game over!!!!!!" );
               Sleep(1000);
               FinalMusic();
               break;
           }

           txTextOut(600, 40,"You win!!!!" );
           Sleep(100);
        }
        Sleep(10);
        //m.init();
        txEnd();
    }
    txDeleteDC(pl.sprite);
}

/*void Map::init()
{
    n=9;
    m=9;
    sprite = txLoadImage("texture.bmp");
    /* for(int i=0; i<n; i++)
    {
       for(int j=0; j<m; j++)
        {
            karta[i][j]=rand()%3;
        }
    }
}*/

void Map::draw()
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            if(karta[i][j]==0)
            {
                txTransparentBlt(txDC(), j*50, i*50, 50, 50, sprite, 0, 0, TX_WHITE);
            }
            else if(karta[i][j]==1)
            {
                txTransparentBlt(txDC(), j*50, i*50, 50, 50, sprite, 50, 50, TX_WHITE);
            }
            else if(karta[i][j]==4)
            {
                txTransparentBlt(txDC(), j*50, i*50, 50, 50, sprite, 50, 0, TX_WHITE);
            }

            else
            {
                txTransparentBlt(txDC(), j*50, i*50, 50, 50, sprite, 0, 50, TX_WHITE);
            }
        }
    }

}

int Map::get_cell(int x, int y)
{
    if(x<=n*50 && y<=m*50)
    {
        return karta[y/50][x/50];
    }
    return -1;
}

void Map::set_cell(int x, int y,int type)
{
    if(x<=n*50 && y<=m*50)
    {
        karta[y/50][x/50]=type;
    }

}

void Map::check_mouse()
{
    int x;
    int y;
    if (txMouseButtons() & 1)
    {
        x=txMouseX();
        y=txMouseY();
        int res;
        txTextOut(450, 50, "left mouse");
        res=get_cell(x, y);

        char snum[2]; //строка, куда записывается перевод из int числа (res)
        itoa(res, snum, 10);  //функция перевода из int (res)  в строку(snum)
        txTextOut(450, 20, snum);  //вывод строки snum на экран
    }
    if (txMouseButtons() & 2)
    {
        x=txMouseX();
        y=txMouseY();
        txTextOut(450, 90, "right mouse");
        set_cell(x, y, 0);
        txTextOut(450, 70, "0");
    }
    if (txMouseButtons() == 3)
    {
        x=txMouseX();
        y=txMouseY();
        txTextOut(450, 120, "both mouse");
        set_cell(x, y, 1);

    }
}

void player::Draw()
{
    txTransparentBlt(txDC(), x, y, w, h, sprite, xpl, ypl, TX_WHITE);
}

void player::Right()
{

    ypl = 192;
    xpl = xpl + 64;

    if (xpl >= 576)
    {
        xpl = 0;
    }


    if (mp->get_cell(x+38, y+64)!=-1 && mp->get_cell(x+38, y+64)!=0)
    {
        if (mp->get_cell(x+38, y+64)==4) // проверяем если кристалл то заменяем на пустую и увеличиваем очки
        {
            mp->set_cell(x+38, y+64,1);
            GetCristall();
            score++;
        }
        x = x + 3;

    }

    if (x >= 800)
    {
        x = 0;
    }


}

void player::Left()
{
    ypl = 64;
    xpl = xpl + 64;

    if (xpl >= 576)
    {
        xpl = 0;
    }
    if (mp->get_cell(x+26, y+64)!=-1 && mp->get_cell(x+26, y+64)!=0)
    {

        if (mp->get_cell(x+26, y+64)==4) // проверяем если кристалл то заменяем на пустую и увеличиваем очки
        {
            mp->set_cell(x+26, y+64,1);
            GetCristall();
            score++;
        }

        x = x - 3;
    }
    if (x < 0)
    {
        x = 0;
    }
}

void player::Up()
{

    ypl = 0;
    xpl = xpl + 64;

    if (xpl >= 576)
    {
        xpl = 64;
    }

    if (mp->get_cell(x+32, y+55)!=-1 && mp->get_cell(x+32, y+55)!=0 )
    {

        if (mp->get_cell(x+32, y+55)==4) // проверяем если кристалл то заменяем на пустую и увеличиваем очки
        {
            mp->set_cell(x+32, y+55,1);
            GetCristall();
            score++;
        }

        y = y - 3;
    }

    if (y < 0)
    {
        y = 0;
    }
}

void player::Down()
{
    ypl = 128;
    xpl = xpl + 64;

    if (xpl >= 576)
    {
        xpl = 64;
    }

    if (mp->get_cell(x+32, y+67)!=-1 && mp->get_cell(x+32, y+67)!=0)
    {
        if (mp->get_cell(x+32, y+67)==4) // проверяем если кристалл то заменяем на пустую и увеличиваем очки
        {
            mp->set_cell(x+32, y+67,1);
            GetCristall();
            score++;
        }

        y = y + 3;
    }

    if (y > 600)
    {
        y = 0;
    }
}

void player::check_keys()
{
    if (GetAsyncKeyState(VK_RIGHT))
    {
        Right();
    }
    if (GetAsyncKeyState(VK_LEFT))
    {
        Left();
    }
    if (GetAsyncKeyState(VK_UP))
    {
        Up();
    }
    if (GetAsyncKeyState(VK_DOWN))
    {
        Down();
    }

}

void player::SetLevel(int level)
{
    score=0;

        for(int i=0;i<9;i++)
        {
            for (int j=0;j<9;j++)
            {
               if (level==2)
               {
                    mp->karta[i][j]=mp->level2[i][j];   // копируем уровень2
                    playerlevel=2;
               }
               else if(level==3)
               {
                    mp->karta[i][j]=mp->level3[i][j];   // копируем уровень2
                    playerlevel=3;
               }
            }

        }



}


void GetTextNumber(int n,char* st) // функция преобразования числа в строку
{
    char snum[4]=""; //строка, куда записывается перевод из int числа (res)
    itoa(n, st, 10);
}

void FinalMusic()
{
    Beep(247, 500);
    Beep(417, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(417, 500);
    Beep(329, 500);
    Beep(247, 500);
    Beep(247, 500);
    Beep(247, 500);
    Beep(417, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(417, 500);
    Beep(497, 500);
    Sleep(500);
    Beep(497, 500);
    Beep(277, 500);
    Beep(277, 500);
    Beep(440, 500);
    Beep(440, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(329, 500);
    Beep(247, 500);
    Beep(417, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(417, 500);
    Beep(329, 500);

}

void NextLevelMusic()
{
    Beep (659.26,200);
    Beep (659.26,200);
    Sleep (200);
    Beep (659.26,200);
    Sleep (100);
    Beep (523.26,200);
    Beep (659.26,200);
    Sleep(200);
    Beep (783.98,200);
    Sleep(400);
    Beep (391.99,200);

}
void GetCristall()
{
    Beep (329, 100);
}
