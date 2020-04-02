#include<bits/stdc++.h>
#include "bridge.h"
#include "bridgesim.h"
using namespace std;

bool process(bridge& b,message m){
    if(m.root < b.root){
        // cout<<"bridges id: "<<b.id<<" root changes from "<<b.root<<" to "<<m.root<<endl;
        b.root = m.root;
        b.rootport = m.lan;
        b.distance = m.distance+1;
        b.source = m.source;
        b.broadcast = false;
        return true;
    }
    else if(m.root == b.root){
        if(m.distance + 1 < b.distance){
            // cout<<"bridges id: "<<b.id<<" root changes from "<<b.root<<" to "<<m.root<<endl;
            b.root = m.root;
            b.rootport = m.lan;
            b.distance = m.distance+1;
            b.broadcast = false;
            b.source = m.source;
            return true;
        }
        else if(m.distance + 1 == b.distance){
            if(m.source < b.source ){
                // cout<<"bridges id: "<<b.id<<" root changes from "<<b.root<<" to "<<m.root<<endl;
                b.root = m.root;
                b.rootport = m.lan;
                b.distance = m.distance+1;
                b.broadcast = false;
                b.source = m.source;
                return true;
            }
            else{
                return false;
            }
        }  
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

