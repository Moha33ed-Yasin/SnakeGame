#include <iostream>
#include <cmath>
#include <windows.h>
#include "conio.h"
#include "Group.h"

using namespace std;

const int Duration = 150;
const string VBLOCKTYPE = "|";
const string HBLOCKTYPE = "$";
const string GROUNDTYPE_1 = " ";
const string GROUNDTYPE_2 = ".";
const char FRUITTYPE = '&';
bool run = true;

class AppWin
{
    private:
    int width = 60, height = 20;

    public:
    int FruitX = (rand() + 1) % width, FruitY = (rand() + 1) % height;
    string Map;

    int get_width()const{
        return width;
    }
    int get_height()const{
        return height;
    }

    void setup()
    {
        for (int i=0; i<height; i++)
        {
            for (int j=0; j<width; j++)
            {
                if(i==0 || i==height-1)
                    Map += HBLOCKTYPE;
                else if(j==0 || j==width-1)
                    Map += VBLOCKTYPE;
                else if (i % 2 != 0)
                {
                    if (j % 2 == 0)
                        Map += GROUNDTYPE_1;
                    else
                        Map += GROUNDTYPE_2;
                }else
                    if (j % 2 != 0)
                        Map += GROUNDTYPE_1;
                    else
                        Map += GROUNDTYPE_2;
                {
                    
                }
            }
            Map += "\n";
        }
    }
    void place_on_map(Point<int, int> pos, const char character)
    {
        Map[pos[1] * (width + 1) + pos[0]] = character;
    }
    void place_fruit()
    {
        FruitX = rand() % (width - 1), FruitY = rand() % (height - 1);
        if (FruitX == 0)
            FruitX = 1;
        if (FruitY == 0)
            FruitY = 1;
    }

    void display(int score)
    {
        string _;
        system("cls");
        place_on_map(Point<int, int>(FruitX, FruitY), FRUITTYPE);
        for (int i=0; i< (int) width / 2 - 4; i++)
            _ += " ";
        cout << Map << _ << VBLOCKTYPE << "score: " << score << VBLOCKTYPE << endl;
        cout << _;
        for(int i=0; i<10 + trunc(log10(score)); i++)
            cout << HBLOCKTYPE;
        cout << endl;
        Sleep(Duration);
    } 
};

class Snake
{
private:
    // LEFT = 'a', RIGHT = 'd', UP = 'w', DOWN = 's'
    const char TAIL = 'o';
    const char BODY = 'O';
    char HEAD = '>';
    int moveX = 0, moveY = 0, Length = 1;
    Point<int, int> pos = Point<int, int>(30, 10); 
    Point<int, int> tail = pos;
    Group<Point<int, int>> body;

public:
    Snake()
    {

    }
    ~Snake()
    {

    }

    auto get_pos()const
    {
        return pos;
    }
    int get_length()const
    {
        return Length;
    }
    
    void input()
    {
        switch (tolower(getch()))
        {
        case 'a':
        if (moveX == 0)
        {
            HEAD = '<';
            moveX = -1; moveY = 0;
        }
            break;
        case 'd':
        if (moveX == 0)
        {
            HEAD = '>';
            moveX = 1; moveY = 0;
        }
            break;
        case 'w':
        if (moveY == 0)
        {
            HEAD = '^';
            moveY = -1; moveX = 0;
        }
            break;
        case 's':
        if (moveY == 0)
        {
            HEAD = 'v';
            moveY = 1; moveX = 0;
        }
            break;
        case 'e':
            cout << "OK, \nTHANKS FOR PLAYING \nCOME BACK LATER" << endl;
            run = false;
            break;

        default:
            break;
        }
    }

    bool crash()
    {
        if (Length <= 4)
            return false;
        return body.find(pos, 0, -2) != -1;
    }
    void draw(AppWin& Surface)
    {
        body.append(pos);
        pos[0] += moveX; pos[1] += moveY;
        tail = body[0];

        if(pos[0] == Surface.FruitX && pos[1] == Surface.FruitY)
        {
            Surface.place_fruit();
            Length++;
        }else
        {
            pop(Surface);
        }
        Surface.place_on_map(pos, HEAD);

        if (Length > 1)
        Surface.place_on_map(body[0], TAIL);
        
        if (Length > 2)
        Surface.place_on_map(body[-1], BODY);
        
    }

    void pop(AppWin& Surface)
    {
        
        if (tail[1] % 2 != 0){
            if (tail[0] % 2 == 0)
                Surface.place_on_map(tail, GROUNDTYPE_1[0]);
            else
                Surface.place_on_map(tail, GROUNDTYPE_2[0]);
        }else
        {
            if (tail[0] % 2 != 0)
                Surface.place_on_map(tail, GROUNDTYPE_1[0]);
            else
                Surface.place_on_map(tail, GROUNDTYPE_2[0]);
        }
        
        body.remove_at(0);
    }
};


main()
{
    AppWin App;
    Snake snake;
    App.setup();
    while(run)
    {
        snake.draw(App);
        App.display(snake.get_length());
        if (_kbhit())
        { 
            snake.input();
        }
        if (
            snake.get_pos()[0] >= App.get_width() || 
            snake.get_pos()[0] <= 0 || 
            snake.get_pos()[1] >= App.get_height() || 
            snake.get_pos()[1] <= 0 ||
            snake.crash()
            )
        {
            cout << "SORRY, \nYOU HAVE LOST THE GAME \nBETTER LOTTORY IN THE NEXT." << endl;
            run = false;
        }
    }
    return EXIT_SUCCESS;
}