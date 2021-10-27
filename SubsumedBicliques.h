#ifndef _SUBSUMEDBICLIQUES_H
#define _SUBSUMEDBICLIQUES_H

#include <unordered_map>
#include"Vertex.h"
using namespace std;

class SubsumedBicliques
{
public:
	SubsumedBicliques(Vertex vertexset, std::unordered_map<int, std::vector<int>>& edges, std::map<int, std::vector<std::set<int>>> new_max_bicliques, int node_size);
	void find_subsumed_bic(std::string algorithm);
	std::map<std::string, std::string> get_hash_subsumed_bic();
	std::map<int, std::vector<std::set<int>>> get_sub_max_bic();

	void clear_sub_bicliques();

private:
	Vertex vertexSet;
	std::unordered_map<int, std::vector<int>> batch_edges;
	std::map<int, std::vector<std::set<int>>> newMaxB;
	int nodeSize;

	void check_maximality(std::set<int> L, std::set<int> R);
	void store_sub_max_bic(std::set<int>& L_in, std::set<int>& R_in);
	std::map<int, std::vector<std::set<int>>> sub_max_bic;
	std::map<int, std::vector<std::string>> sub_max_bic_string;
	std::map<std::string, std::string> hash_subsumed_bic;
	bool check_sub_bic(std::set<int>& L, std::set<int>& R); 
	std::string signal_L;
	std::string signal_R;
};

#endif // _SUBSUMEDBICLIQUES_H

