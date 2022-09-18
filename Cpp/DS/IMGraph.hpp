#pragma once
#ifndef _IMGRAPH_HPP_
#define _IMGRAPH_HPP_

#include "IMDS.hpp"

#include "../IMC.h"
#include "../IMTS"
#include "../IMMrc"

namespace IM { namespace DS {

template <typename Ord,typename Attr,typename Rel> requires order<Ord>
class undirectedGraph{

};

template <typename Ord,typename Attr,typename Rel> requires order<Ord>
/*
    Ord:graph's element Ord
    Attr:graph's element Attr
    Rel:graph's edge hit value
*/
class directedGraph{
public:
    class vertex : public elementEntity<Ord,Attr>{
    public:
        vertex(Ord ordinal,Attr attr):elementEntity<Ord,Attr>(ordinal,attr){}
        ~vertex()=default;
    };
    
    directedGraph()=default;
    directedGraph(const std::string s):vertices(),edges(){
        json j = json::parse(s);
        //v,e
        this->v=j["vertices"].size();
        this->e=j["edges"].size(); 
        bool has_rels=j.count("rels")&&j["rels"].size()==e;
        for(Ord i=0;i<this->v;i++) this->vertices.push_back(vertex(i,j["vertices"][i]));
        //rel default:char,represent mapped info,initialized to '\0'.
        for(unsigned long i=0;i<e;i++){
            if(static_cast<Ord>(j["edges"][i][0])>=v||static_cast<Ord>(j["edges"][i][1])>=v) continue;
            this->edges[j["edges"][i][0]].emplace(std::make_pair(has_rels?j["rels"][i]:"\0",j["edges"][i][1]));//if doesn't have rels,then fill with char '\0' 
        }
    }
    ~directedGraph()=default;

    const Ord& _v(){return this->v;}
    const unsigned long& _e(){return this->e;}
    const std::vector<vertex>& _vertices(){return this->vertices;}
    const std::unordered_map<Ord,std::unordered_map<Rel,Ord>>& _edges(){return this->edges;}
    
    //add_edge(no rel)
    bool add_edge(Ord from_ordinal,Ord to_ordinal){
        if(from_ordinal>=this->v||to_ordinal>=this->v||from_ordinal<0||to_ordinal<=0) return false;
        else{this->edges[from_ordinal].emplace(std::make_pair("\0",to_ordinal));
        return true;
        }
    }
    
    // add_edge:have rel
    bool add_edge(Ord from_ordinal,Ord to_ordinal,Rel rel){
        if(from_ordinal>=this->v||to_ordinal>=this->v||from_ordinal<0||to_ordinal<=0) return false;
        else{this->edges[from_ordinal].emplace(std::make_pair(rel,to_ordinal));
        return true;
        }
    }

    bool add_vertex(Attr attr){
        if(this->v+1>=std::numeric_limits<Ord>::max())return false;
        this->vertices.push_back(vertex(this->v,attr));
        this->v++;
        return true;
    }

    // bool dfs(std::vector<Ord>&res,Ord start_ord){
    //     res.clear();
    //     if(start_ord>=this->v||start_ord<0) return false;

    // }
    // bool bfs(std::vector<Ord>&res,Ord start_ord){
    //     res.clear();
    //     if(start_ord>=this->v||start_ord<0) return false;

    // }
    // unsigned long in_degree(Ord v_ordinal){
        
    //     return 1;
    // }
    // unsigned long out_degree(Ord v_ordinal){
    //     return 1;
    // }
    // void strong_connected_components(){
        
    // }

    // void topological_sort(){
        
    // }
protected:
    std::vector<vertex>vertices;
    std::unordered_map<Ord,std::unordered_map<Rel,Ord>>edges;
    Ord v;
    unsigned long e;

private:
};

}}
#endif