#include "enemytank.h"
#include<ctime>
#include<cstdlib>
#define W 36
EnemyTank::EnemyTank()
{
    srand((unsigned int)time(NULL));
    int i=rand()%3;
    this->ex=i*W/2-i;
    this->ey=0;
    this->eState=2;//下
    this->eDestroy=false;
}
