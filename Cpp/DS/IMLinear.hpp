#pragma once
#ifndef _IMLINEAR_HPP_
#define _IMLINEAR_HPP_

#include "IMDS.hpp"

#include "../IMC.h"
#include "../IMTS"
#include "../IMMrc"

namespace IM { namespace DS {
    //vector auxiliary
template <typename T>
int getIndex(const std::vector<T>& v, T K)
{
    auto it = find(v.begin(), v.end(), K);
  
    // If element was found
    if (it != v.end())
    {
      
        // calculating the index
        // of K
        int index = it - v.begin();
        return index;
    }
    else {
        // If the element is not
        // present in the vector
        return -1;
    }
}
}};

#endif