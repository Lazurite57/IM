#include "../IMC.h"
using namespace std;
int main()
{
    std::ifstream f("IMGraph.json");
    json data = json::parse(f);
    int a=1;
    cout<<data["vertices"][1].type_name()<<endl;
    // cout<<data["a"][1].type_name()<<endl;
    return 0;
}