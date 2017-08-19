#ifndef BULLET_H
#define BULLET_H


class Bullet
{
public:
    Bullet();
    Bullet(int x,int y,int state);


    int bx;
    int by;
    int state;
    int destroy;

};

#endif // BULLET_H
