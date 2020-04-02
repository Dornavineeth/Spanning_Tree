class message
{
public:
    int root;
    int distance;
    int source;
    char lan;
    int destination;
    message(int root1, int distance1, int source1, int lan1)
    {
        root = root1;
        distance = distance1;
        source = source1;
        lan = lan1;
    }
    message()
    {
    }
};

class packet
{
public:
    int sourcehost;
    int desthost;
    char sourcelan;
    int bridge;
    packet()
    {
    }
};