#ifndef MYTANK_H
#define MYTANK_H


class MyTank
{
public:
    MyTank();
    MyTank(int x,int y,int state);

    int tx;
    int ty;
    int state;
    int id;
    bool death;

};

#endif // MYTANK_H
