#include<bits/stdc++.h>
#include "bridge.h"
#include "bridgesim.h"
using namespace std;

//list of functions used
bool process(bridge& b,message m);
void printtrace(int trace_flag,vector<pair<int,message>>& trace,int time,string s);
void tracelearning(int trace_flag,vector<pair<int,pair<char,char>>>& trace,int time,string s);
bool check_table(vector<bridge>& bridge_set,int bridgeid,int host);
void update_table(vector<bridge>& bridge_set,vector<lan>& lan_set,int bridgeid,int host,char lan);
void printtables(vector<bridge>& bridge_set);

int main(){
    int trace_flag;
    int n;
    vector<bridge> bridge_set;
    vector<lan> lan_set;
    set<char> lanset;
    //*setup bridges*//
    string line;
    getline(cin,line);
    trace_flag = stoi(line);
    getline(cin,line);
    n = stoi(line);
    for(int i=0;i<n;i++){
        int id=0;
        int ind=0;
        bridge brdg;
        getline(cin,line);
        for(int j=1;j<line.size();j++){
            if(line[j+1]!=':'){
                id = 10*id + (int)(line[j])-48;
            }
            if(line[j+1]==':'){
                id = 10*id + (int) (line[j])-48;
                ind=j;
                break;
            }
        }
        brdg.id = id;
        for(int j=ind+3;j<line.size();j++){
            if(line[j]!=' '){
                brdg.adj_lans.push_back(line[j]);
                lanset.insert(line[j]);
                j++;
            }
        }
        sort(brdg.adj_lans.begin(),brdg.adj_lans.end());
        brdg.root = brdg.id;
        brdg.distance = 0; 
        bridge_set.push_back(brdg);
    }
    /*cout<<"======================="<<endl;
    for(int i=0;i<n;i++){//debug for bridge setup
        cout<<"bridge_id "<<bridge_set[i].id<<endl;
        cout<<"bridge adj lans: ";
        for(int j=0;j<bridge_set[i].adj_lans.size();j++){
            cout<<bridge_set[i].adj_lans[j]<<" ";
        }
        cout<<endl;
        cout<<"bridge's root: "<<bridge_set[i].root<<endl;
        cout<<"bridge distance: "<<bridge_set[i].distance<<endl;
        cout<<"-----------------------"<<endl;
    }
    cout<<"======================="<<endl;*/

    //setup lans
    while(!lanset.empty()){
        auto iter = lanset.begin();
        lan l(*iter);
        for(int i=0;i<n;i++){
            for(int j=0;j<bridge_set[i].adj_lans.size();j++){
                if(*iter==bridge_set[i].adj_lans[j]){
                    l.adj_bridges.push_back(bridge_set[i].id);
                }
            }
        }
        lan_set.push_back(l);
        lanset.erase(iter);
    }
    /*for(int i=0;i<lan_set.size();i++){//debug for lansetup
        cout<<lan_set[i].id<<endl;
        for(int j=0;j<lan_set[i].adj_bridges.size();j++){
            cout<<lan_set[i].adj_bridges[j]<<" ";
        }
        cout<<endl;
    }*/
    queue<message> transmit,receive;
    vector<pair<int,message>> trace;
    int time = 0;
    //initaializations of all messages
    for(int i=0;i<n;i++){
        for(int j=0;j<bridge_set[i].adj_lans.size();j++){
            message m;
            m.distance = bridge_set[i].distance;
            m.root = bridge_set[i].id;
            m.source = bridge_set[i].id;
            m.lan = bridge_set[i].adj_lans[j];
            transmit.push(m);
        }
    }
    bool steady = false;
    while(true){
        steady = false;
        while(!transmit.empty()){
            int source = transmit.front().source;
            char lan = transmit.front().lan;
            for(int j=0;j<lan_set.size();j++){
                if(lan_set[j].id==lan){
                    for(int k=0;k<lan_set[j].adj_bridges.size();k++){
                        if(source != lan_set[j].adj_bridges[k]){
                            // cout<<"t="<<time<<" transmitted from "<<source<<" to "<<lan_set[j].adj_bridges[k]<<endl;
                            transmit.front().destination = lan_set[j].adj_bridges[k];
                            receive.push(transmit.front());
                            trace.push_back(make_pair(source,transmit.front()));
                        }
                    }
                }
            }
            transmit.pop();
        }
        printtrace(trace_flag,trace,time,"s");
        time++;
        while(!receive.empty()){
            int source = receive.front().source;
            char lan = receive.front().lan;
            int destination = receive.front().destination;
            // cout<<"t="<<time<<" message is received at "<<destination<<" from "<<source<<"with root "<<receive.front().root<<endl;
            bool k3 = process(bridge_set[destination-1],receive.front());
            steady = steady + k3;
            trace.push_back(make_pair(destination,receive.front()));
            for(int j=0;j<bridge_set[destination-1].adj_lans.size();j++){
                message m;
                if(bridge_set[destination-1].adj_lans[j]!=lan){
                bool bo = bridge_set[destination-1].broadcast;
                if(!bo){
                    m.distance = receive.front().distance+1;
                    m.root = receive.front().root;
                    m.source = bridge_set[destination-1].id;
                    m.lan = bridge_set[destination-1].adj_lans[j];
                    transmit.push(m);
                }
                }
            }
            receive.pop();
        }
        printtrace(trace_flag,trace,time,"r");
        for(int i=0;i<n;i++){
            for(int j=0;j<bridge_set[i].adj_lans.size();j++){
                if(bridge_set[i].broadcast){
                    message m;
                    m.distance = bridge_set[i].distance;
                    m.root = bridge_set[i].root;
                    m.source = bridge_set[i].id;
                    m.lan = bridge_set[i].adj_lans[j];
                    transmit.push(m);
                }
            }
        }
        if(steady==false){
            break;
        }  
    }
    /*cout<<"======================="<<endl;//for checking final configurations
    for(int i=0;i<n;i++){//debug for bridge setup
        cout<<"bridge_id "<<bridge_set[i].id<<endl;
        cout<<"bridge adj lans: ";
        for(int j=0;j<bridge_set[i].adj_lans.size();j++){
            cout<<bridge_set[i].adj_lans[j]<<" ";
        }
        cout<<endl;
        cout<<"bridge's root: "<<bridge_set[i].root<<endl;
        cout<<"bridge distance: "<<bridge_set[i].distance<<endl;
        cout<<"root port: "<<bridge_set[i].rootport<<endl;
    cout<<"-----------------------"<<endl;
    }
    cout<<"======================="<<endl;*/

    for(int i=0;i<lan_set.size();i++){
        int distance = n+1;
        int bridgeid=n+1;
        for(int j=0;j<lan_set[i].adj_bridges.size();j++){
            if(bridge_set[lan_set[i].adj_bridges[j]-1].distance < distance){
                distance = bridge_set[lan_set[i].adj_bridges[j]-1].distance;
                bridgeid = lan_set[i].adj_bridges[j];
            }
            if(lan_set[i].adj_bridges[j] == distance){
                if(bridgeid < bridge_set[lan_set[i].adj_bridges[j]-1].id){
                    distance = bridge_set[lan_set[i].adj_bridges[j]-1].distance;
                    bridgeid = lan_set[i].adj_bridges[j];
                }
            }
        }
        lan_set[i].designatedport = bridgeid;
    }
    /*for(int i=0;i<lan_set.size();i++){//debug for lansetup
        cout<<lan_set[i].id<<endl;
        for(int j=0;j<lan_set[i].adj_bridges.size();j++){
            cout<<"adjacent bridges are "<<lan_set[i].adj_bridges[j]<<" ";
        }
        cout<<endl;
        cout<<"designated port "<<lan_set[i].designatedport<<endl;
    }*/

    for(int i=0;i<n;i++){
        string l="B";
        l.append(to_string(i+1));
        l.append(":");
        for(int j=0;j<lan_set.size();j++){
            if(bridge_set[i].rootport == lan_set[j].id){
                l.append(" ");
                l.push_back(lan_set[j].id);
                l.append("-RP");
            }
            else if(lan_set[j].designatedport-1 == i){
                l.append(" ");
                l.push_back(lan_set[j].id);
                l.append("-DP");

            }
            else{
                for(int k=0;k<bridge_set[i].adj_lans.size();k++){
                    if(bridge_set[i].adj_lans[k] == lan_set[j].id){
                        l.append(" ");
                        l.push_back(lan_set[j].id);
                        l.append("-NP");
                        break;
                    }
                }
            }

        }
        cout<<l<<endl;

    }
    ///////////////////////////////////////////implementing LEARNING BRIDGES////////////////////////////////////////////////
    
    //////taking input from hosts//////////////////////
    for(int i=0;i<lan_set.size();i++){
        bool hostinput = false; 
        getline(cin,line);
        int indexof;
        for(int j=0;j<line.size();j++){
            if(line[j]==':'){
                indexof = j;
                break;
            }
        }
        int j = indexof+2;
        while(j<line.size()){
        int start = j+1;
            while(j<line.size()&&line[j]!=' '){j++;}
            j++;
            int hostid = stoi(line.substr(start,j-2-start+1));
            lan_set[i].hosts.push_back(hostid);
        }
        sort(lan_set[i].hosts.begin(),lan_set[i].hosts.end());    
    }

    /*for(int i=0;i<lan_set.size();i++){// debug hosts setup
        cout<<lan_set[i].id<<endl<<"hosts are: ";
        for(int j=0;j<lan_set[i].hosts.size();j++){
            cout<<lan_set[i].hosts[j]<<" ";
        }
        cout<<endl;
    }*/
    /////////////setup for packet exchanges/////////////////////
    int m;
    getline(cin,line);
    m = stoi(line);
    // vector<pair<int,int>> transfer;
    for(int iter=0;iter<m;iter++){
        int sourcehost=0;
        int desthost=0;
        getline(cin,line);
        int p=1;
        while(line[p] != ' '){
            // sourcehost = 10*sourcehost + (int) line[p] -48;
            p++;
        }
        sourcehost = stoi(line.substr(1,p-1));
        desthost = stoi(line.substr(p+2,line.length()-1-p)); 

        queue<packet> transmit,receive;
        vector<pair<int,pair<char,char> > > trace;
        time = 0;
        lan sourcelan;
        lan finalsourcelan;
        lan finaldestlan;
        for(int i=0;i<lan_set.size();i++){
            for(int j=0;j<lan_set[i].hosts.size();j++){
                if(sourcehost == lan_set[i].hosts[j]){
                    sourcelan.id = lan_set[i].id;
                    sourcelan.designatedport = lan_set[i].designatedport;
                    finalsourcelan.id = lan_set[i].id;
                }
                if(desthost == lan_set[i].hosts[j]){
                    finaldestlan.id = lan_set[i].id;
                }
            }
        }
        packet initial_packet;
        initial_packet.bridge = sourcelan.designatedport;
        initial_packet.sourcehost = sourcehost;
        initial_packet.desthost = desthost;
        initial_packet.sourcelan = sourcelan.id;
        transmit.push(initial_packet);
        trace.push_back(make_pair(initial_packet.bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
        // cout<<"source initial host is "<<sourcehost<<endl;
        // cout<<"destination initial host is "<<desthost<<endl;
        // cout<<"host initial lan is "<<sourcelan.id<<endl;
        // cout<<"packet transfered from port "<<initial_packet.sourcelan<<" to    "<<initial_packet.bridge<<endl;

        for(int i=0;i<bridge_set.size();i++){
            if(bridge_set[i].rootport == sourcelan.id){
                packet p;
                p.bridge = bridge_set[i].id;
                p.sourcehost = sourcehost;
                p.desthost = desthost;
                p.sourcelan = sourcelan.id;
                transmit.push(p);
                trace.push_back(make_pair(p.bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
                // cout<<"packet transfered from port "<<p.sourcelan<<" to      "<<p.bridge<<endl;
            }
        }
        tracelearning(trace_flag,trace,time,"r");
        while(true){
        while(!transmit.empty()){
            int bridgeid = transmit.front().bridge;
            char sourcelan = transmit.front().sourcelan;
            int sourcehost = transmit.front().sourcehost;
            int desthost = transmit.front().desthost;
            if(!check_table(bridge_set,bridgeid,sourcehost)){
                // cout<<bridgeid<<" "<<sourcehost<<" "<<sourcelan<<endl;
                update_table(bridge_set,lan_set,bridgeid,sourcehost,sourcelan);
            }
            if(!check_table(bridge_set,bridgeid,desthost)){
                for(int i=0;i<bridge_set.size();i++){
                    if(bridge_set[i].id == bridgeid){
                        if(sourcelan != bridge_set[i].rootport && bridge_set[i].rootport != '('){
                            transmit.front().sourcelan = bridge_set[i].rootport;
                            receive.push(transmit.front());
                            trace.push_back(make_pair(transmit.front().bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
                            // cout<<"packet transfered from bridge "<<transmit.front().bridge<<" to "<<transmit.front().sourcelan<<endl;
                        }
                    }
                }
                for(int i=0;i<lan_set.size();i++){
                    if(lan_set[i].designatedport == bridgeid){
                        if(sourcelan != lan_set[i].id){
                            transmit.front().sourcelan = lan_set[i].id;
                            receive.push(transmit.front());
                            trace.push_back(make_pair(transmit.front().bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
                            // cout<<"packet transfered from bridge "<<transmit.front().bridge<<" to "<<transmit.front().sourcelan<<endl;
                        }
                    }
                }
            }
            else{
                for(int i=0;i<bridge_set.size();i++){
                    if(bridgeid == bridge_set[i].id){
                        for(int j=0;j<bridge_set[i].forward_table.size();j++){
                            if(desthost == bridge_set[i].forward_table[j].host && bridge_set[i].forward_table[j].forward_port.id != sourcelan){
                                transmit.front().sourcelan = bridge_set[i].forward_table[j].forward_port.id;
                                receive.push(transmit.front());
                                trace.push_back(make_pair(transmit.front().bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
                                // cout<<"packet transfered from bridge "<<transmit.front().bridge<<" to "<<transmit.front().sourcelan<<endl;
                                break;
                            }
                        }
                    }
                }
            }
            transmit.pop();
        }
        tracelearning(trace_flag,trace,time,"s");
        time++;
        while(!receive.empty()){
            int bridgeid = receive.front().bridge;
            char sourcelan = receive.front().sourcelan;
            int sourcehost = receive.front().sourcehost;
            int desthost = receive.front().desthost;
            for(int i=0;i<lan_set.size();i++){
                if(lan_set[i].id == sourcelan){
                    if(lan_set[i].designatedport != bridgeid){
                        receive.front().bridge = lan_set[i].designatedport;
                        transmit.push(receive.front());
                        trace.push_back(make_pair(receive.front().bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
                        // cout<<"packet transfered from port "<<receive.front().sourcelan<<" to "<<receive.front().bridge<<endl;
                    }
                }
            }

            for(int i=0;i<bridge_set.size();i++){
                if(bridge_set[i].rootport == sourcelan){
                    if(bridge_set[i].id != bridgeid){
                        receive.front().bridge = bridge_set[i].id;
                        transmit.push(receive.front());
                        trace.push_back(make_pair(receive.front().bridge,make_pair(finalsourcelan.id,finaldestlan.id)));
                        // cout<<"packet transfered from port "<<receive.front().sourcelan<<" to "<<receive.front().bridge<<endl;
                    }
                }
            }

        receive.pop();
        }
        tracelearning(trace_flag,trace,time,"r");
        if(transmit.size() == 0 && receive.size()==0){
            printtables(bridge_set);
            cout<<endl;
            break;
        }
        }

    }

}