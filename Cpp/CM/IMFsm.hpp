#pragma once
#ifndef _IMDFA_HPP_
#define _IMDFA_HPP_

#include "../IMC.h"

#include "../IMTS"
#include "../IMMrc"
#include "../IMDS"
#include "../IMStr"

namespace IM { namespace CM {
template <typename Ord,typename Attr,typename Rel> requires order<Ord>
class NFA : public IM::DS::undirectedGraph<Ord,Attr,Rel> {
};

template <typename Ord,typename Attr,typename Rel> requires order<Ord>
class DFA : public IM::DS::directedGraph<Ord,Attr,Rel> {
public:

    typedef typename IM::DS::directedGraph<Ord,Attr,Rel>::vertex state;
    _ALIAS_TEMPLATE_FUNCTION_(add_state,(IM::DS::directedGraph<Ord,Attr,Rel>::add_vertex));
    _ALIAS_TEMPLATE_FUNCTION_(add_transfer,(IM::DS::directedGraph<Ord,Attr,Rel>::add_edge));
    
    DFA(const std::string s) : IM::DS::directedGraph<Ord,Attr,Rel>(s){
    }

    const Ord& _s(){return this->s;}
    const unsigned long& _t(){return this->t;}
    const std::vector<state>& _states(){return this->states;}
    const std::unordered_map<Ord,std::unordered_map<Rel,Ord>>& _transfers(){return this->transfers;}

    bool run_DFA(Ord start_state,const std::vector<Ord>&final_states,std::vector<Rel>input){
        //args check
            //!remember to add wrong input message handle!alert the info of the error
        int size_final_states=final_states.size();
        if(start_state>=this->s||size_final_states==0||(input.size()==0&&size_final_states!=0&&final_states[0]!=start_state)) 
        {
            std::cout<<"args error!DFA stops."<<std::endl;
            return false;
        }
            //final state ord out of range check
        for(int i=0;i<size_final_states;i++){
            if(final_states[i]>=this->s) 
            {
                std::cout<<"final state serial number out of range!DFA stops."<<std::endl;
                return false;
            }
        }

        //run DFA
        std::reverse(input.begin(), input.end());
        Rel cur_read;
        Ord cur_state=start_state;
        auto iter_fst=this->transfers.begin();
        auto iter_snd=(iter_fst->second).begin();
        while(input.size()!=0)
        {
            cur_read=input.back();
            input.pop_back();
            iter_fst=this->transfers.find(cur_state);
            iter_snd=(iter_fst->second).find(cur_read);
            if(iter_snd==(iter_fst->second).end())
            {
                return false;
            }
            cur_state=iter_snd->second;
        }
        if(std::find(std::begin(final_states), std::end(final_states), cur_state) != std::end(final_states))
        {
            return true;
        }
        else
        {
            return false;
        } 
    }

protected:
    Ord& s=this->v;
    unsigned long& t=this->e;
    std::vector<state>&states=this->vertices;
    std::unordered_map<Ord,std::unordered_map<Rel,Ord>>&transfers=this->edges;
};

}}
#endif


