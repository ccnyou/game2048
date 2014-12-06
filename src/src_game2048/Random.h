#pragma once

class Random
{
public:
    Random(void);
    ~Random(void);

    BOOL Init(int nSeed = 0);
    
    double  GetDouble() const;
    int     GetInt(int nMax)  const;
};
