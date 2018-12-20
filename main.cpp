#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib>
#include <set>
#include <list>
#include <cmath>
#include <sstream>
using namespace std;

/// HR VR TR TL BL BR

bool canTilt = false;

enum Line
{
    HR = -1, VR = 1, TL = 2, TR = 3, BL = -3 , BR = -2, CRV = 69
};

enum Color
{
    RED = 0, GREEN = 1, BLUE = 2
};

class Tile
{
    unsigned ID;
    int red, green, blue;
    vector<int> rgb;
public:
    vector<int> sides;       /// 0 = black, 1 = red, 10 = green, 11 = yellow, 100 = blue, 101 = magenta, 110 = cyan, 111 = white
    static unsigned counter;
    Tile(int r, int g, int b) : red(r), green(g), blue(b), rgb{r,g,b}
    {
        ID = counter++;
        vector<int> temp{r,g,b};
        sides = {0,0,0,0};
        for(int i = 0; i < 3; i++)
        {
            if(temp[i] == HR)
            {
                sides[1] += pow(10,i);
                sides[3] += pow(10,i);
            }
            if(temp[i] == VR)
            {
                sides[0] += pow(10,i);
                sides[2] += pow(10,i);
            }
            if(temp[i] == TL)
            {
                sides[0] += pow(10,i);
                sides[1] += pow(10,i);
            }
            if(temp[i] == TR)
            {
                sides[0] += pow(10,i);
                sides[3] += pow(10,i);
            }
            if(temp[i] == BR)
            {
                sides[1] += pow(10,i);
                sides[2] += pow(10,i);
            }
            if(temp[i] == BL)
            {
                sides[3] += pow(10,i);
                sides[2] += pow(10,i);
            }
        }
    }
    unsigned getID()
    {
        return ID;
    }
    int getR() const
    {
        return red;
    }
    int getG() const
    {
        return green;
    }
    int getB() const
    {
        return blue;
    }
    vector<int> getRGB()
    {
        return rgb;
    }
    bool operator< (const Tile& otherTile) const
    {
        if(otherTile.ID < this->ID)
        {
            return true;
        }
    }
    bool operator!= (const Tile& otherTile) const
    {
        if(otherTile.ID != this->ID)
        {
            return true;
        }
    }
    bool operator== (const Tile& otherTile) const
    {
        if(otherTile.ID == this->ID)
        {
            return true;
        }
    }
    bool canConnect(const Tile& otherTile, int Color) const /// 1 = red, 10 = green, 100 = blue
    {
        if(canTilt)
        {
            for(int i = 0; i < 4; i++)
            {
                cout << (otherTile.sides[i]/Color) % 10 << " ";
                for(int j = 0; j < 4; j++)
                {
                    if((otherTile.sides[i]/Color) % 10 > 0 && (this->sides[j]/Color) % 10 > 0 && otherTile.sides[i] == this->sides[j])
                        return true;
                }
            }
            return false;
        }
        for(int i = 0; i < 4; i++)
        {
            if((otherTile.sides[i]/Color) % 10 > 0 && (this->sides[(i+2)%4]/Color) % 10 > 0 && otherTile.sides[i] == this->sides[(i+2)%4])
                return true;
        }
        return false;
    }
};

unsigned Tile::counter = 0;

void inputTiles(set<Tile>& tiles)
{
    ifstream inputFile("keszlet2.txt");
    if(inputFile.fail())
    {
        cerr << "szar a file" << endl;
        return;
    }
    string canRotate;
    getline(inputFile, canRotate);
    if(canRotate == "yes")
        canTilt = true;
    while(inputFile.good())
    {
        stringstream ss;
        string s;
        int red, green, blue, amt;
        getline(inputFile,s,';');
        red = atoi(s.c_str());
        getline(inputFile,s,';');
        green = atoi(s.c_str());
        getline(inputFile,s,';');
        blue = atoi(s.c_str());
        getline(inputFile,s);
        amt = atoi(s.c_str());
        for(int i = 0; i < amt; i++)
        {
            Tile temp(red,green,blue);
            tiles.insert(temp);
            //cout << " map elso ertek: " << temp.getR() << " map masodik ertek: " << temp.getID() << " map merete: " << tiles.size() << "\n"  ;
        }
    }
}

class tileSet
{
    set<Tile> tiles;
public:
    tileSet(set<Tile> t)
    {
        tiles = t;
    }
    int countBy(Color C, Line shape)
    {
        int counter = 0;
        for(Tile a : tiles)
        {
            if(a.getRGB()[C] == shape || ((shape == CRV) && pow(a.getRGB()[C],2) > 1))
            {
                counter++;
            }
        }
        return counter;
    }
    bool quickCheck()
    {
        if(canTilt)
        {
            if(countBy(RED,CRV) >= 4 || countBy(GREEN,CRV) >= 4 || countBy(BLUE,CRV) >= 4)
            {
                return true;
            }
            return false;
        }
        else
        {
            if((countBy(RED,TR) > 0 && countBy(RED,TL) > 0 && countBy(RED,BL) > 0 && countBy(RED,BR) > 0)
               || (countBy(GREEN,TR) > 0 && countBy(GREEN,TL) > 0 && countBy(GREEN,BL) > 0 && countBy(GREEN,BR) > 0)
               || (countBy(BLUE,TR) > 0 && countBy(BLUE,TL) > 0 && countBy(BLUE,BL) > 0 && countBy(BLUE,BR) > 0))
            {
                return true;
            }
            return false;
        }
    }
};


class Graph
{
protected:
    map<Tile, vector<Tile> > points;
public:
    void connectPoints(Tile a, Tile b)
    {
        points[a].push_back(b);
        points[b].push_back(a);
    }
    vector<Tile> connectedTo(Tile T) const
    {
        return points.at(T);
    }
    map<Tile, vector<Tile> > getPoints() const
    {
        return points;
    }
};

bool isMember(vector<Tile> b, Tile a)
{
    bool ret = false;
    for(unsigned i = 0; i < b.size(); i++)
    {
        if(a == b[i])
        {
            ret = true;
        }
    }
    return ret;
}

void createBlueGraph(Graph& g, set<Tile> tiles)
{

    return;
}

ostream& operator<<(std::ostream& os, Graph g)
{

    map<Tile, vector<Tile> > points = g.getPoints();
    for(map<Tile, vector<Tile> >::iterator a = points.begin(); a != points.end(); a++)
    {
        os << "["<< a->first.getR() << a->first.getG() << a->first.getB() << "]" << "       ";
        for(vector<Tile>::iterator b = a->second.begin(); b != a->second.end(); b++)
        {
            os << "["<< b->getR() << b->getG() << b->getB() << "]" << " ";
        }
        os << endl;
    }
    return os;
}

int main()
{
    set<Tile> tiles;
    inputTiles(tiles);
    tileSet T(tiles);
    if(!T.quickCheck())
    {
        cout << "Nem lehetseges hurkot kesziteni.";
        return 0;
    }
//    Graph g;
//    createBlueGraph(g,tiles);
//    cout << g;
    return 0;
}
