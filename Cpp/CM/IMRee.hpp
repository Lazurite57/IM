#pragma once
#ifndef _IMREE_HPP_
#define _IMREE_HPP_

#include "../IMC.h"

#include "../IMMrc"
#include "../IMDS"
#include "../IMStr"
#include "../IMCM"

namespace IM { namespace CM {
class pureREE : public IM::CM::DFA<short,short,std::string> {
    pureREE()=default;
    pureREE(const std::string s){ // compile regular expression
            
    }

    bool match(std::string input)
    {
        
    }

};
class extendedREE : public IM::CM::NFA<short,short,std::string> {

};

}}
#endif