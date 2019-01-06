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
int redValue, greenValue, blueValue;

enum Line
{
    HR = -1, VR = 1, TL = 2, TR = 3, BR = 4 , BL = 5, CRV = 69, LN = 420
};

enum Color
{
    RED = 0, GREEN = 1, BLUE = 2
};

class Tile
{
    int red, green, blue;

public:
    int ID;
    vector<int> rgb;
    vector<int> sides;       /// 0 = black, 1 = red, 10 = green, 11 = yellow, 100 = blue, 101 = magenta, 110 = cyan, 111 = white
    static int counter;
    Tile()
    {
        ID = -1;
    }
    Tile(int r, int g, int b) : red(r), green(g), blue(b), rgb{r,g,b}
    {
        vector<int> temp{r,g,b};
        ID = counter++;
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
                sides[3] += pow(10,i);
            }
            if(temp[i] == TR)
            {
                sides[0] += pow(10,i);
                sides[1] += pow(10,i);
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
    bool operator< (const Tile& otherTile) const
    {
        if(otherTile.ID > this->ID)
        {
            return true;
        }
        return false;
    }
    bool operator!= (const Tile& otherTile) const
    {
        if(otherTile.ID != this->ID)
        {
            return true;
        }
        return false;
    }
    bool operator== (const Tile& otherTile) const
    {
        if(otherTile.ID == this->ID)
        {
            return true;
        }
        return false;
    }
    bool canConnect(const Tile& otherTile, int Color) const /// RED GREEN BLUE
    {

        Color = pow(10,Color);
        if(canTilt)
        {
            for(int i = 0; i < 4; i++)
            {
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
    void tilt(int color, int shape) /// RED GREEN BLUE ||| HR VR TL TR BR BL
    {
        int ctr = 0;
        while(rgb[color] != shape && ctr < 4)
        {
            for(int i = 0; i < 3; i++)
            {
                if(rgb[i] < 2)
                {
                    rgb[i] *= -1;
                }
                else
                {
                    if(rgb[i] == 5)
                    {
                        rgb[i] = 2;
                    }
                    else
                    {
                        rgb[i]++;
                    }
                }
            }
            int last = sides[3];
            for(int i = 3; i > 0; i--)
            {
                sides[i] = sides[i-1];
            }
            sides[0] = last;
            red = rgb[0];
            green = rgb[1];
            blue = rgb[2];
            ctr++;
        }
    }
};

Tile nullTile;

int Tile::counter = 0;

void inputTiles(set<Tile>& tiles)
{
    ifstream inputFile("keszlet2.txt");
    if(inputFile.fail())
    {
        cerr << "szar a file" << endl;
        return;
    }
    string cValue;
    getline(inputFile,cValue,';');
    redValue = atoi(cValue.c_str());
    getline(inputFile,cValue,';');
    greenValue = atoi(cValue.c_str());
    getline(inputFile,cValue);
    blueValue = atoi(cValue.c_str());
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
public:
    set<Tile> tiles;
    tileSet(set<Tile> t)
    {
        tiles = t;
    }
    Tile Find(Color C, int shape)
    {
        for(auto a : tiles)
        {
            if(canTilt)
            {
                a.tilt(C,shape);
                if(a.rgb[C] == shape)
                {
                    return a;
                }
            }
            if(!canTilt && a.rgb[C] == shape)
            {
                return a;
            }
        }
        return nullTile;
    }
    Tile findBy(Color C, int shape, int color, int Loc)
    {
        for(auto a : tiles)
        {
            if(canTilt)
            {

                a.tilt(C,shape);
                if(a.rgb[C] == shape && a.sides[Loc] == color)
                {
                    return a;
                }
            }
            if(!canTilt && a.rgb[C] == shape && a.sides[Loc] == color)
            {
                return a;
            }
        }
        return nullTile;
    }
    int countBy(Color C, Line shape)
    {
        int counter = 0;
        for(Tile a : tiles)
        {
            if(a.rgb[C] == shape || ((shape == CRV) && a.rgb[C] > 1) || ((shape == LN) && pow(a.rgb[C],2) == 1))
            {
                counter++;
            }
        }
        return counter;
    }
    vector<Tile> fourTileLoop(Color C)
    {
        vector<Tile> TRT,TLT,BLT,BRT;
        for(auto a : tiles)
        {
            if(canTilt && a.rgb[C] > 1)
            {
                a.tilt(C,TR);
                TRT.push_back(a);
                a.tilt(C,TL);
                TLT.push_back(a);
                a.tilt(C,BR);
                BRT.push_back(a);
                a.tilt(C,BL);
                BLT.push_back(a);
            }
            else
            {
                switch(a.rgb[C])
                {
                    case 2 : {TLT.push_back(a);break;}
                    case 3 : {TRT.push_back(a);break;}
                    case 4 : {BRT.push_back(a);break;}
                    case 5 : {BLT.push_back(a);break;}
                }
            }
        }
        for(Tile a : TLT)
        {
            for(Tile b : BLT)
            {
                if(a.sides[0] == b.sides[2] && a.sides[0] > 0)
                {

                    for(Tile c : BRT)
                    {
                        if(b.sides[3] == c.sides[1] && b.sides[3] > 0)
                        {
                            for(Tile d : TRT)
                            {
                                if(c.sides[2] == d.sides[0] && c.sides[2] > 0)
                                {
                                    if(d.sides[1] == a.sides[3] && d.sides[1] > 0)
                                    {
                                        vector<Tile> ret{a,b,c,d};
                                        return ret;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        vector<Tile> ret{nullTile};
        return ret;
    }
    bool quickCheck()
    {
        if(canTilt)
        {
            if(countBy(RED,CRV) >= 4 || countBy(GREEN,CRV) >= 4 || countBy(BLUE,CRV) >= 4)
            {
                vector<Tile> ret{nullTile};
                if(fourTileLoop(RED) != ret)
                {
                    return true;
                }
                if(fourTileLoop(GREEN) != ret)
                {
                    return true;
                }
                if(fourTileLoop(BLUE) != ret)
                {
                    return true;
                }
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
    void removeTile(const Tile RT)
    {
        for(set<Tile>::iterator is = tiles.begin(); is != tiles.end(); is++)
        {
            if(is->ID == RT.ID)
            {
                tiles.erase(is);
                break;
            }
        }
    }
};

class loopTile
{
public:
    Tile T, leftT = nullTile, rightT = nullTile;
    int leftLoc = -1, rightLoc = -1; /// 0 = top, 1 = right, 2 = bot, 3 = left
    loopTile(const Tile& asd)
    {
        T = asd;
    }
    void addLeft(Tile left, int Loc)
    {
        leftT = left;
        leftLoc = Loc;
    }
    void add(Tile right, int Loc)
    {
        rightT = right;
        rightLoc = Loc;
    }
};


class loopPart
{
protected:
    list<loopTile> addedTiles;
public:
    loopPart(const Tile T, int Loc)
    {
        loopTile LT(T);
        LT.rightLoc = Loc;
        if(Loc == 0)
        {
            Loc = 4;
        }
        LT.leftLoc = Loc-1;
        addedTiles.push_back(LT);
    }
    bool isLoop()
    {
        for(auto a : addedTiles)
        {
            if(a.leftLoc == -1 || a.rightLoc == -1)
            {
                return false;
            }
        }
        return true;
    }
    int getStartColor()
    {
        return addedTiles.front().T.sides[addedTiles.front().leftLoc]; ///kezdocsempe szelene szine
    }
    int getEndColor()
    {
        return addedTiles.back().T.sides[addedTiles.back().rightLoc]; ///utso csempe szelenek szine
    }
    void addBackTile(const Tile TT)
    {
        loopTile temp(TT);
        temp.addLeft(addedTiles.back().T,addedTiles.back().rightLoc);
        addedTiles.push_back(temp);
    }
    loopPart operator+(const loopPart otherTile)
    {
        list<loopTile> fromOther = otherTile.addedTiles;
        for(auto a : fromOther)
        {
            this->addBackTile(a.T);
        }
        return *this;
    }
};

//void createRedLoop(vector<int> data, tileSet tempSet) /// KANYAROK EGYENESEK  HR VR TR TL BL BR
//{
//    tileSet testSet = tempSet;
//    bool canRun = true;
//    int curves = 1;
//    int horLines = 0;
//    int vertLines = 0;
//    Tile starter = testSet.Find(RED,2);
//    testSet.removeTile(starter);
//    data[0]--; data[4]--;
//    vector<loopPart> parts;
//    parts[0] = loopPart(starter,0);
//    while(canRun)
//    {
//        int lineShape = 2;
//        int theWay = 0;
//        int otherWay = 3;
//        for(int i = 1; i < 4; i++)
//        {
//            lineShape--;
//            if(lineShape < 2)
//            {
//                lineShape = 5;
//            }
//            theWay--;
//            if(theWay < 0)
//            {
//                theWay = 3;
//            }
//            otherWay--;
//            if(otherWay < 0)
//            {
//                otherWay = 3;
//            }
//            loopPart a(testSet.findBy(RED,lineShape,parts[i-1].getEndColor(),theWay),theWay);
//            for(int c = curves-1; c != 0; c--)
//            {
//                lineShape--;
//                if(lineShape < 2)
//                {
//                    lineShape = 5;
//                }
//                lineShape--;
//                if(lineShape < 2)
//                {
//                    lineShape = 5;
//                }
//                Tile one = testSet.findBy(RED,lineShape,a.getEndColor(),otherWay);
//                lineShape--;
//                if(lineShape < 2)
//                {
//                    lineShape = 5;
//                }
//                lineShape--;
//                if(lineShape < 2)
//                {
//                    lineShape = 5;
//                }
//                //Tile one = testSet.findBy(RED,lineShape,a.getEndColor(),otherWay);
//            }
//        }
//    }
//}


bool loop4Tile(Color C,Tile aa)
{
    if(aa.rgb[C] < 2)
        return false;
    vector<Tile> TRT,TLT,BLT,BRT;
    aa.tilt(C,TR);
    TRT.push_back(aa);
    aa.tilt(C,TL);
    TLT.push_back(aa);
    aa.tilt(C,BR);
    BRT.push_back(aa);
    aa.tilt(C,BL);
    BLT.push_back(aa);
    for(Tile a : TLT)
    {
        for(Tile b : BLT)
        {
            if(a.sides[0] == b.sides[2] && a.sides[0] > 0)
            {

                for(Tile c : BRT)
                {
                    if(b.sides[3] == c.sides[1] && b.sides[3] > 0)
                    {
                        for(Tile d : TRT)
                        {
                            if(c.sides[2] == d.sides[0] && c.sides[2] > 0)
                            {
                                if(d.sides[1] == a.sides[3] && d.sides[1] > 0)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}


vector<Tile> calculateBestLoop(vector<int> stats, Color C, tileSet allSet, vector<pair<int,int> >& coordz) /// KANYAROK EGYENESEK  HR VR TR TL BL BR
{                                                                                                          ///      0       1       2  3  4  5  6  7
    vector<Tile> elso;
    cout << C+1 << ". hurok keszitese..." << endl;
//    cout << "szinkalk" << endl;
    if(stats[0] < 4 || (!canTilt && (stats[4] == 0 || stats[5] == 0 || stats[6] == 0 || stats[7] == 0)))
    {
        elso.push_back(nullTile);
        return elso;
    }
    int canDoFour = 1;
    if(canTilt && loop4Tile(C,*allSet.tiles.begin()))
    {
        canDoFour = 0;
    }
    Tile starter = allSet.Find(C,TL);
    allSet.removeTile(starter);
    int HORI = (stats[2]-stats[2]%2)/2;
    int VERTI = (stats[3]-stats[3]%2)/2;
    int TTR = ((stats[4]-1)-(stats[4]-1)%2)/2;
    int TBL = ((stats[6]-1)-(stats[6]-1)%2)/2;
    int TTL = ((stats[5]-1)-(stats[5]-1)%2)/2;
    int TBR = ((stats[7]-1)-(stats[7]-1)%2)/2;
    if(canTilt)
    {
        TTR = ((stats[0]-stats[0]%4)-canDoFour*(((stats[0]-stats[0]%4)-4)%8))/4-1;
        TTL = ((stats[0]-stats[0]%4)-canDoFour*(((stats[0]-stats[0]%4)-4)%8))/4-1;
        TBL = ((stats[0]-stats[0]%4)-canDoFour*(((stats[0]-stats[0]%4)-4)%8))/4-1;
        TBR = ((stats[0]-stats[0]%4)-canDoFour*(((stats[0]-stats[0]%4)-4)%8))/4-1;
        VERTI = (stats[1]-(stats[1]%2))/2;
        HORI = 0;
        if(!canDoFour)
        {
            TTL += 1;
            TBR += 1;
            TTR -= 1;
            TBL -= 1;
        }
    }
 //   cout << HORI << " " << VERTI << " " << TTL << " " << TTR << " " << TBR << " " << TBL << endl;
    if(!canTilt && ((TTR+TBL)%2>0 || (TTL+TBR)%2>0) && allSet.fourTileLoop(C) == vector<Tile>{nullTile})
    {
        elso.push_back(nullTile);
        return elso;
    }
    elso.push_back(starter);
    int hori = HORI;
    int verti = VERTI;
    int bl = TBL;
    int tr = TTR;
    int tl = TTL;
    int br = TBR;
    int koniKoli = starter.sides[0];
    int koniLoki = 2;
    bool canDo1 = (tl > 0 && br > 0);
    bool canDo2 = (tr > 0 && bl > 0);
    int XX = 0, YY = 0;
    coordz.push_back(pair<int,int>(XX,YY));
 //   cout << elso[elso.size()-1].rgb[C] << endl;
    while((tl > 0 && br > 0 && canDo2) || verti > 0)
    {
       // cout << "tl br " << tl << " " << br << endl;
        //cout << "elso " << endl;
        Tile secNextTile = allSet.findBy(C,BR,koniKoli,koniLoki);
        if(secNextTile != nullTile && br > 0)
        {
            allSet.removeTile(secNextTile);
            elso.push_back(secNextTile);
            koniKoli = secNextTile.sides[1];
            koniLoki = 3;
            br--;
            if(canTilt)
            tl--;
            YY++;
            coordz.push_back(pair<int,int>(XX,YY));
 //   cout << elso[elso.size()-1].rgb[C] << endl;
        }
        Tile thirdNextTile = allSet.findBy(C,TL,koniKoli,koniLoki);
        if(thirdNextTile != nullTile && tl > 0)
        {
            allSet.removeTile(thirdNextTile);
            elso.push_back(thirdNextTile);
            koniKoli = thirdNextTile.sides[0];
            koniLoki = 2;
            tl--;
            if(canTilt)
            br--;
            XX++;
            coordz.push_back(pair<int,int>(XX,YY));
  //  cout << elso[elso.size()-1].rgb[C] << endl;
        }
        Tile nextTile = allSet.findBy(C,VR,koniKoli,koniLoki);
        if(verti == VERTI && verti > 0 && nextTile == nullTile)
        {
            hori = verti;
            HORI = verti;
            verti = 0;
            VERTI = 0;
        }
        if(nextTile != nullTile && verti > 0)
        {
            allSet.removeTile(nextTile);
            elso.push_back(nextTile);
            koniKoli = nextTile.sides[0];
            verti--;
            if(canTilt)
            hori--;
            YY++;
            coordz.push_back(pair<int,int>(XX,YY));
//    cout << elso[elso.size()-1].rgb[C] << endl;
        }
    }
//    cout << hori << " " << verti << " " << tl << " " << tr << " " << br << " " << bl << endl;
    Tile starter2 = allSet.findBy(C,BL,koniKoli,koniLoki);
    allSet.removeTile(starter2);
    koniLoki = 1;
    koniKoli = starter2.sides[3];
    elso.push_back(starter2);
   // cout << elso[elso.size()-1].rgb[C] << endl;
    YY++;
    coordz.push_back(pair<int,int>(XX,YY));
    while(hori > 0 || (bl > 0 && tr >0 && canDo1))
    {
  //      cout << "masodik" << endl;
        Tile secNextTile = allSet.findBy(C,TR,koniKoli,koniLoki);
        if(secNextTile != nullTile && tr > 0)
        {
            allSet.removeTile(secNextTile);
            elso.push_back(secNextTile);
  //  cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = secNextTile.sides[0];
            koniLoki = 2;
            tr--;
            if(canTilt)
            bl--;
            XX--;
            coordz.push_back(pair<int,int>(XX,YY));
        }
        Tile thirdNextTile = allSet.findBy(C,BL,koniKoli,koniLoki);
        if(thirdNextTile != nullTile && bl > 0)
        {
            allSet.removeTile(thirdNextTile);
            elso.push_back(thirdNextTile);
  //  cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = thirdNextTile.sides[3];
            koniLoki = 1;
            bl--;
            if(canTilt)
            tr--;
            YY++;
            coordz.push_back(pair<int,int>(XX,YY));
        }
        Tile nextTile = allSet.findBy(C,HR,koniKoli,koniLoki);
        if(nextTile != nullTile && hori > 0)
        {
            allSet.removeTile(nextTile);
            elso.push_back(nextTile);
//    cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = nextTile.sides[3];
            hori--;
            if(canTilt)
            verti--;
            XX--;
            coordz.push_back(pair<int,int>(XX,YY));
        }
    }
//    cout << hori << " " << verti << " " << tl << " " << tr << " " << br << " " << bl << endl;
    Tile starter3 = allSet.findBy(C,BR,koniKoli,koniLoki);
    allSet.removeTile(starter3);
    koniLoki = 0;
    koniKoli = starter3.sides[2];
    elso.push_back(starter3);
 //   cout << elso[elso.size()-1].rgb[C] << "   " << allSet.tiles.size() << endl;
    XX--;
    coordz.push_back(pair<int,int>(XX,YY));
    Tile thirdNextTile = allSet.findBy(C,TL,koniKoli,koniLoki);
//    cout << HORI << " " << VERTI << " " << TTL << " " << TTR << " " << TBR << " " << TBL << endl;
    while((TTL > 0 && TBR > 0 && canDo2) || verti > 0)
    {


        Tile thirdNextTile = allSet.findBy(C,TL,koniKoli,koniLoki);
        if(thirdNextTile != nullTile && TTL > 0)
        {
            allSet.removeTile(thirdNextTile);
            elso.push_back(thirdNextTile);
   // cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = thirdNextTile.sides[3];
            koniLoki = 1;
            TTL--;
            if(canTilt)
            TBR--;
            YY--;
            coordz.push_back(pair<int,int>(XX,YY));
        }
        Tile secNextTile = allSet.findBy(C,BR,koniKoli,koniLoki);
        if(secNextTile != nullTile && TBR > 0)
        {
            allSet.removeTile(secNextTile);
            elso.push_back(secNextTile);
  //  cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = secNextTile.sides[2];
            koniLoki = 0;
            TBR--;
            if(canTilt)
            TTL--;
            XX--;
            coordz.push_back(pair<int,int>(XX,YY));
        }
        Tile nextTile = allSet.findBy(C,VR,koniKoli,koniLoki);
        if(nextTile != nullTile && verti > 0)
        {
            allSet.removeTile(nextTile);
            elso.push_back(nextTile);
//    cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = nextTile.sides[2];
            VERTI--;
            if(canTilt)
            HORI--;
            YY--;
            coordz.push_back(pair<int,int>(XX,YY));
        }
    }
    Tile starter4 = allSet.findBy(C,TR,koniKoli,koniLoki);
    allSet.removeTile(starter4);
    koniLoki = 3;
    koniKoli = starter4.sides[1];
    elso.push_back(starter4);
  //  cout << elso[elso.size()-1].rgb[C] << endl;
    YY--;
    coordz.push_back(pair<int,int>(XX,YY));
    while(HORI > 0 || (TBL > 0 && TTR >0 && canDo1))
    {
 //       cout << HORI << " " << VERTI << " " << TTL << " " << TTR << " " << TBR << " " << TBL << endl;

        Tile thirdNextTile = allSet.findBy(C,BL,koniKoli,koniLoki);
        if(thirdNextTile != nullTile && TBL > 0)
        {
            allSet.removeTile(thirdNextTile);
            elso.push_back(thirdNextTile);
 //   cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = thirdNextTile.sides[2];
            koniLoki = 0;
            TBL--;
            if(canTilt)
            TTR--;
            XX++;
            coordz.push_back(pair<int,int>(XX,YY));
        }
        Tile secNextTile = allSet.findBy(C,TR,koniKoli,koniLoki);
        if(secNextTile != nullTile && TTR > 0)
        {
            allSet.removeTile(secNextTile);
            elso.push_back(secNextTile);
 //   cout << elso[elso.size()-1].rgb[C] << endl;
            koniKoli = secNextTile.sides[1];
            koniLoki = 3;
            TTR--;
            if(canTilt)
            TBL--;
            YY--;
            coordz.push_back(pair<int,int>(XX,YY));
        }
        Tile nextTile = allSet.findBy(C,HR,koniKoli,koniLoki);
        if(nextTile != nullTile && HORI > 0)
        {
            allSet.removeTile(nextTile);
            elso.push_back(nextTile);
            koniKoli = nextTile.sides[1];
            HORI--;
            if(canTilt)
            VERTI--;
            XX++;
            coordz.push_back(pair<int,int>(XX,YY));
 //   cout << elso[elso.size()-1].rgb[C] << endl;
        }
    }
    return elso;

}

long long int scoreCheck(vector<Tile> vt, vector<pair<int,int>> crd)
{
    vector<int> scores{1,1,1};
    if(vt.size() < 4)
    {
        return 0;
    }
    vector<int> values{redValue,greenValue,blueValue};
    long long fiboScore;
    long long int n = vt.size()-3, t1 = 0, t2 = 1;
    for(int i = 1; i <= n; i++)
    {
        fiboScore = t1 + t2;
        t1 = t2;
        t2 = fiboScore;
    }
    for(unsigned i = 1; i < crd.size(); i++)
    {
        int way = (crd[i].first-crd[i-1].first)*2 + (crd[i].second-crd[i-1].second); ///-2 ->3/1   -1 ->0/2    2->1/3   1->2/0
        switch (way)
        {
        case -2 :
            {
                for(int j = 0; j < 3; j++)
                {
                    if(scores[j] > 0 && ((vt[i].sides[1]/int(pow(10,j)))%10 == 0 || (vt[i-1].sides[3]/int(pow(10,j)))%10 == 0))
                    {
                        scores[j] = 0;
                    }
                }
                break;
            }
        case -1 :
            {
                for(int j = 0; j < 3; j++)
                {
                    if(scores[j] > 0 && ((vt[i].sides[0]/int(pow(10,j)))%10 == 0 || (vt[i-1].sides[2]/int(pow(10,j)))%10 == 0))
                    {
                        scores[j] = 0;
                    }
                }
                break;
            }
        case 2 :
            {
                for(int j = 0; j < 3; j++)
                {
                    if(scores[j] > 0 && ((vt[i].sides[3]/int(pow(10,j)))%10 == 0 || (vt[i-1].sides[1]/int(pow(10,j)))%10 == 0))
                    {
                        scores[j] = 0;
                    }
                }
                break;
            }
        case 1 :
            {
                for(int j = 0; j < 3; j++)
                {
                    if(scores[j] > 0 && ((vt[i].sides[2]/int(pow(10,j)))%10 == 0 || (vt[i-1].sides[0]/int(pow(10,j)))%10 == 0))
                    {
                        scores[j] = 0;
                    }
                }
                break;
            }
        }
    }
    long long int result = 0;
    for(unsigned i = 0; i < scores.size(); i++)
    {
        result += fiboScore*scores[i]*values[i];
    }
    return result;

}

long long int RESULT = 0;
vector<pair<int,int> > coordz;
vector<Tile> bestFunctionEver(vector<int> redStats, vector<int> greenStats, vector<int> blueStats, tileSet allSet)
{
    vector<pair<int,int> > redCoord, greenCoord, blueCoord;
    vector<Tile> bestRED = calculateBestLoop(redStats,RED,allSet,redCoord);
    vector<Tile> bestGREEN = calculateBestLoop(greenStats,GREEN,allSet,greenCoord);
    vector<Tile> bestBLUE = calculateBestLoop(blueStats,BLUE,allSet,blueCoord);
    if(bestRED.size() < 4 && bestGREEN.size() < 4 && bestBLUE.size() < 4)
    {

        return vector<Tile>{nullTile};
    }
    cout << "A kulonbozo szinu hurkok meretei: ";
    cout << endl << "Piros: " << bestRED.size() << " Zold: " << bestGREEN.size() << " Kek: " << bestBLUE.size() << endl;
    int red = scoreCheck(bestRED, redCoord);
    int green = scoreCheck(bestGREEN, greenCoord);
    int blue = scoreCheck(bestBLUE, blueCoord);
    red = fabs(red);
    green = fabs(green);
    blue = fabs(blue);
    cout << "Es a hurkokhoz tartozo pontok: " << endl;
    cout << "Piros: " << red << " Zold: " << green << " Kek: " << blue << endl;
    if(red >= green && red >= blue)
    {
        coordz = redCoord;
        RESULT = red;
        return bestRED;
    }
    if(green >= red && green >= blue)
    {
        coordz = greenCoord;
        RESULT = green;
        return bestGREEN;
    }
    if(blue >= red && blue >= green)
    {
        coordz = blueCoord;
        RESULT = blue;
        return bestBLUE;
    }
    return vector<Tile>{nullTile};
}

ostream& operator<<(std::ostream& os, const vector<Tile>& asd)
{

    for(unsigned i = 0; i < asd.size(); i++)
    {
        for(unsigned j = 0; j < 3; j++)
        {
            os << asd[i].rgb[j] << " ";
        }
        os << "    " << coordz[i].first << " " << coordz[i].second << endl;
    }
    return os;
}

int main()
{
    set<Tile> tiles;
    inputTiles(tiles);
    cout << "Csempek beolvasva..." << endl;
    tileSet T(tiles);
    if(!T.quickCheck())
    {
        cout << "Nem lehetseges hurkot kesziteni.";
        return 0;
    }
    vector<int> redz{T.countBy(RED,CRV),T.countBy(RED,LN),T.countBy(RED,HR),T.countBy(RED,VR),T.countBy(RED,TR),T.countBy(RED,TL),T.countBy(RED,BL),T.countBy(RED,BR)};
    vector<int> greenz{T.countBy(GREEN,CRV),T.countBy(GREEN,LN),T.countBy(GREEN,HR),T.countBy(GREEN,VR),T.countBy(GREEN,TR),T.countBy(GREEN,TL),T.countBy(GREEN,BL),T.countBy(GREEN,BR)};
    vector<int> bluez{T.countBy(BLUE,CRV),T.countBy(BLUE,LN),T.countBy(BLUE,HR),T.countBy(BLUE,VR),T.countBy(BLUE,TR),T.countBy(BLUE,TL),T.countBy(BLUE,BL),T.countBy(BLUE,BR)};
    /// KANYAROK EGYENESEK  HR VR TR TL BL BR
//    if(T.tiles.size() < 40)
//    {
//        vector<Tile> asd{nullTile};
//        vector<Tile> basicR = fourTileLoop(T,RED);
//        if(basicR == asd)
//        {
//            redz[0] = 0;
//            cout << "R" << endl;
//        }
//        vector<Tile> basicG = fourTileLoop(T,GREEN);
//        if(basicG == asd)
//        {
//            greenz[0] = 0;
//            cout << "G" << endl;
//        }
//        vector<Tile> basicB = fourTileLoop(T,BLUE);
//        if(basicB == asd)
//        {
//            bluez[0] = 0;
//            cout << "B" << endl;
//        }
//    }
//

    vector<Tile> eredmeny = bestFunctionEver(redz,greenz,bluez,T);
    if(eredmeny.size() == 1)
    {
        cout << "Nem lehet hurkot kesziteni.";
        return 0;
    }
    cout << "Az utvonal: " << endl;
    cout << eredmeny << endl;
    cout << "A hurok hossza: ";
    cout << coordz.size() << " csempe" << endl;
    cout << "A pontszam: " << RESULT << endl;
    //cout << basicB.size();
//    cout << basicR;
//    createRedLoop(redz,T);
//    Graph g;
//    createBlueGraph(g,tiles);
//    cout << g;
    return 0;
}
