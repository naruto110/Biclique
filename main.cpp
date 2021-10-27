#include<iostream>
#include<string>
#include"CommandLineParser.h"
using namespace std;


int main(int argc, char* argv[]) {

	if (argc == 7)
	{
		CommandLineParser CLP = CommandLineParser(argv);
	}
	else
	{
		std::cout << "input argv is wrong.";
	}
	

	return 0;
}
