#include<bits/stdc++.h>
#include "bridge.h"
#include "bridgesim.h"
using namespace std;


void printtrace(int trace_flag,vector<pair<int,message>>& trace,int time,string s){
    if(trace_flag==1){
            sort(trace.begin(),trace.end(),[](pair<int,message> t1,pair<int,message> t2){
                return t1.first > t2.first;
            });
            int siz = trace.size();
            for(int i=siz-1;i>=0;i--){
                string l="";
                l.append(to_string(time));
                l.append(" ");
                l.append(s);
                l.append(" B");
                l.append(to_string(trace[i].first));
                l.append(" (B");
                l.append(to_string(trace[i].second.root));
                l.append(",");
                l.append(to_string(trace[i].second.distance));
                l.append(",B");
                l.append(to_string(trace[i].second.source));
                l.append(")");
                trace.pop_back();
                cout<<l<<endl;
            }
    }
}

void tracelearning(int trace_flag,vector<pair<int,pair<char,char>>>& trace,int time,string s){
    if(trace_flag == 1){
        sort(trace.begin(),trace.end(),[](pair<int,pair<int,int>> t1,pair<int,pair<int,int>>t2){
            return t1.first > t2.first;
        });
        int siz = trace.size();
        for(int i=siz-1;i>=0;i--){
            string l = "";
            l.append((to_string(time)));
            l.append(" ");
            l.append(s);
            l.append(" B");
            l.append(to_string(trace[i].first));
            l.append(" ");
            l.push_back(trace[i].second.first);
            l.append("-->");
            l.push_back(trace[i].second.second);
            trace.pop_back();
            cout<<l<<endl;
        }
    }
}

bool check_table(vector<bridge>& bridge_set,int bridgeid,int host){
    // cout<<"checking for host "<<host<<" at bridge "<<bridgeid<<endl;
   for(int i=0;i<bridge_set.size();i++){
       if(bridge_set[i].id == bridgeid){
            for(int j=0;j<bridge_set[i].forward_table.size();j++){
                if(bridge_set[i].forward_table[j].host == host){
                    return true;
                }
            }
            return false;
       }
   }
}

void update_table(vector<bridge>& bridge_set,vector<lan>& lan_set,int bridgeid,int host,char lan){
    // cout<<"host "<<host<<" lan "<<lan<<" bridge "<<bridgeid<<endl;
    for(int i=0;i<bridge_set.size();i++){
        if(bridge_set[i].id == bridgeid){
            for(int j=0;j<lan_set.size();j++){
                if(lan == lan_set[j].id){
                    table t;
                    t.host = host;
                    t.forward_port = lan_set[j]; 
                    bridge_set[i].forward_table.push_back(t);
                    // cout<<"added "<<host<<" "<<lan_set[j].id<<" to "<<bridgeid<<endl;
                    sort(bridge_set[i].forward_table.begin(),bridge_set[i].forward_table.end(),[](table t1,table t2){
                        return t1.host < t2.host;
                    });
                }
            }
        }
    }
}

void printtables(vector<bridge>& bridge_set){
    for(int i=0;i<bridge_set.size();i++){
        cout<<"B"<<i+1<<":"<<endl;
        cout<<"HOST ID | FORWARDING PORT"<<endl;
        for(int j=0;j<bridge_set[i].forward_table.size();j++){
            cout<<"H"<<bridge_set[i].forward_table[j].host<<" | "<<bridge_set[i].forward_table[j].forward_port.id<<endl;
        }
    }
}
