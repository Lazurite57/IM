#pragma once
#ifndef _IMDS_HPP_
#define _IMDS_HPP_

#include "../IMC.h"
#include "../IMTS"

namespace IM { namespace DS {

//element entity with a serial number & a datum
template <typename Ord,typename Attr> requires order<Ord>
class elementEntity{
public:
    elementEntity(Ord ordinal,Attr attr){
        this->ordinal=ordinal;
        this->attr=attr;
    }
    ~elementEntity()=default;

    const Ord& _ordinal() const{return this->ordinal;}
    const Attr& _data() const{return this->attr;}
    void set_ordinal(Ord ordinal){this->ordinal=ordinal;}
    void set_data(Attr attr){this->attr=attr;}
protected:
    Ord ordinal;
    Attr attr;
};

}}
#endif