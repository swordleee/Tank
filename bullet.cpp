#include "bullet.h"


Bullet::Bullet()
{

}

Bullet::Bullet(int x,int y,int state)
{
    this->bx=x;
    this->by=y;
    this->state=state;
    this->destroy=false;
}
