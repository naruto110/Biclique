#ifndef _BICLIQUEFINDER_H
#define _BICLIQUEFINDER_H

#include <unordered_map>
#include <map>
#include<set>
#include"Vertex.h"
using namespace std;

class BicliqueFinder
{
public:
	BicliqueFinder(Vertex vertexset, std::unordered_map<int, std::vector<int>>& edges, int node_size);

	void find_new_maximal_bicliques(std::string algorithm); 

	std::map<string, string> get_hash_new_max_bic();
	std::map<int, std::vector<std::set<int>>> get_new_max_bic();
	std::map<int, std::string> get_new_max_bic_string();

	void clear_maximal_bicliques();

private:
	Vertex vertexSet;
	std::unordered_map<int, std::vector<int>> batch_edges;
	int nodeSize;

	void add_to_processed_edges(int& u, int& v);
	std::unordered_map<int, int> processed_edges_index; 
	std::vector<std::set<int>> processed_edges_set; 

	std::set<int> L_initial_;
	std::set<int> R_initial_;
	std::set<int> P_initial_;
	std::set<int> Q_initial_;

	void biclique_find_improved(std::set<int>& L_in, std::set<int>& R_in, std::set<int>& P_in, std::set<int>& Q_in);
	std::multimap<int, int> sort_P(std::set<int>& L_in, std::set<int>& P_in);
	bool check_maximal_bicliques(std::set<int>& L, std::set<int>& R); 
	bool check_processed_edges(int& node_v, std::set<int>& L_prime); 
	void new_maiximal_bicliques(std::set<int>& L_in, std::set<int>& R_in);


	void biclique_find_base(std::set<int>& L_in, std::set<int>& R_in, std::set<int>& P_in, std::set<int>& Q_in);

	std::map<int, std::vector<std::set<int>>> new_max_bic;
	std::map<int, std::string> new_max_bic_string;
	std::map<std::string, std::string> hash_max_bic;
	std::string signal_L;
	std::string signal_R;
};




#endif // _BICLIQUEFINDER_H
