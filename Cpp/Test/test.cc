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
	std::stringstream strStream;
    strStream << ifs.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file

    std::cout << str << "\n"; //you can do anything with the string!!!
	
	//json test
	// auto j=json::parse(ifs);
	// cout<<j.dump()<<endl;
	// cout<<j.type_name()<<endl;
	// if(ifs.is_open())
	// {
	// 	cout<<ifs.rdbuf()<<endl;
	// }
	// directedGraph<unsigned char,unsigned char,string>g(str);
	DFA<unsigned short,unsigned short,string>dfa(str);
	cout<<dfa._states().size()<<endl;
	cout<<dfa._transfers().size()<<endl;
	// cout<<"graph edges:"<<g._edges().size()<<endl;
	// cout<<"graph vertices:"<<g._vertices().size()<<endl;
	vector<unsigned short>final_states{5};
	vector<string>input{"W","J"};
	auto res = dfa.run_DFA(0,final_states,input);
	cout<<res<<endl;
	
	// check the unordered map correctness
		// for(auto iter=dfa._transfers().begin();iter!=dfa._transfers().end();iter++){
		// 	cout << "hash layer:" <<iter->first << endl;
		// 	for(auto iter2=iter->second.begin();iter2!=iter->second.end();iter2++){
		// 		cout <<iter2->first<<" "<<endl;
		// 	}
		// }
		// cout<<endl;

	// for(int i=0;i<g._v();i++){
	// 	cout<<"the "<<i<<": "<<g._vertices()[i]._data()<<endl;
	// }
	// for(int i=0;i<g._v();i++){
	// 	cout<<"the "<<i<<": "<<g._vertices()[i]._data()<<endl;
	// }
	// bool a = true;
	// char b = 'a';
	// cout<<sizeof(a)<<endl;
	// cout<<sizeof(b)<<endl;

	
	//std::cout<<j.dump()<<std::endl;
	// std::vector<std::string> sv;
	// std::string s = "1 2,3 4,5 6";
	// json j = json::parse(s);
	// vector<string>v = static_cast<vector<string>>(j["vertices"]);
	// for(auto& i : v) cout << i << " ";
	// cout << typeid((vector<string>)j["vertices"]).name() <<endl;
	// cout<< j.dump() << endl;
	
	// split(s, sv,',');
	// for (auto &i : sv) {
	// 	for(auto &j:i){
	// 		if(j==' '){
	// 			continue;
	// 		}

	// 	}
	// }
    // int times = 50;
	// binTree<double>bt(1);
	// binTree<double>bt_2;
	// bt.assign_val(1);
    // bt_2.assign_val(2);
	// for (int i = 0; i < times; i++)
	// {
	// 	//v.push_back(NID(gen));
	// 	bt.add_node(NID(gen),ID(gen));
	// 	bt_2.add_node(NID(gen), ID(gen));
	// }
	// cout << endl;
	// cout << endl;
	// cout << "fst:" << endl;
	// bt.route_front();
	// cout << "snd" << endl;
	// bt_2.route_front();
    return 0;
//    testing::InitGoogleTest(&argc, argv);
}