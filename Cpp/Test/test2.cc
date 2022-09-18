#include "../IMDS"
#include "../IMC.h"
#include "../IMCM"
#include <gtest/gtest.h>

#define _GLIBCXX_NO_ASSERT
using namespace std;
using namespace IM::CM;
using namespace IM::DS;
static default_random_engine gen;
static uniform_int_distribution<int> ID(0, 1);
static uniform_int_distribution<int> NID(0,100);

int main(int argc, char **argv)
{
    ifstream ifs("/root/code/IM/Cpp/Test/IMGraph.json");
    string s = "{\
    \"vertices\":[\
    0,0,0,0,0],\
	\"edges\":[\
    [0,1],[1,2],[2,3],[3,4]],\
	\"rels\":[\
	\"I\",\"L\",\"O\",\"V\"\
	]\
	}";
    directedGraph<unsigned char,unsigned char,string>g(s);
	cout<<"graph edges:"<<g._edges().size()<<endl;
	cout<<"graph vertices:"<<g._vertices().size()<<endl;
    return 0;
}