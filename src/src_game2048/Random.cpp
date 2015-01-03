#include "StdAfx.h"
#include <ctime>
#include <cstdlib>
#include "Random.h"

Random::Random(void)
{
}

Random::~Random(void)
{
}

BOOL Random::Init( int nSeed /*= 0*/ )
{
    if (nSeed == 0)
    {
        nSeed = static_cast<int>(time(0));
    }

    srand(nSeed);

    return TRUE;
}

int Random::GetInt( int nMax ) const
{
    int nRet = 0;

    nRet = rand();
    if (nMax != 0)
    {
        nRet %= nMax;
    }
    
    return nRet;
}

double Random::GetDouble() const
{
    double fRet = 0;

    fRet = rand();
    fRet /= RAND_MAX;

    return fRet;
}
