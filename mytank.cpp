#include "mytank.h"

MyTank::MyTank()
{

}

MyTank::MyTank(int x, int y, int id)
{
    this->tx=x;
    this->ty=y;
    this->id=id;
    this->state=1;
    this->death=false;
}
