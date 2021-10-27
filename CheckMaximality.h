#ifndef _CHECKMAXIMALITY_H
#define _CHECKMAXIMALITY_H

#include"Vertex.h"

class CheckMaximality 
{
public:
	CheckMaximality(Vertex vertexset, int node_size);
	void check_bic(std::map<int, std::vector<std::set<int>>>& sub_bic);
	std::map<std::string, std::string> get_hash_new_bic();
	std::map<std::string, std::vector<std::set<int>>> get_sub_max_bic_set();

private:
	Vertex vertexSet;
	int nodeSize;
	void store_max_bic(std::vector<std::set<int>>);
	std::map<std::string, std::string> hash_new_bic;
	std::map<std::string, std::vector<std::set<int>>> sub_max_bic_set;
};


#endif // !_CHECKMAXIMALITY_H
