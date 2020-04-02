using namespace std;

class lan
{
public:
    char id;
    vector<int> adj_bridges;
    vector<int> hosts;
    int designatedport;

    lan(char lanid)
    {
        id = lanid;
    }
    lan()
    {
    }
};

class table
{
public:
    int host;
    lan forward_port;
    table()
    {
    }
};

class bridge
{
public:
    int id;
    vector<char> adj_lans;
    vector<int> typeport;
    int root;
    int distance;
    char rootport;
    bool broadcast;
    int source;
    vector<table> forward_table;
    bridge(int bridgeid)
    {
        id = bridgeid;
        root = bridgeid;
        broadcast = true;
    }
    bridge()
    {
        broadcast = true;

    }
};
