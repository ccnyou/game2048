#pragma once
#include "Object.h"

namespace cc
{
    class Random :
        public Object
    {
    public:
        Random(void);
        Random(int seed);
        virtual ~Random(void);

        //************************************
        // Method:    GetInt
        // FullName:  cc::Random::GetInt
        // Access:    public 
        // Returns:   if maxValue is not zero,return 
        //              a int value in [0, maxValue)
        //            else return a value in [0, RAND_MAX)
        // Parameter: int maxValue
        //************************************
        int GetInt(int maxValue = 0) const;

        //************************************
        // Method:    GetDouble
        // FullName:  cc::Random::GetDouble
        // Access:    public 
        // Returns:   a double value in [0.0,1.0)
        //************************************
        double GetDouble() const;
    };
}


