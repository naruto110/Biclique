#include "SubsumedBicliques.h"
#include <algorithm>
#include<iostream>
#include<string>

SubsumedBicliques::SubsumedBicliques(Vertex vertexset, std::unordered_map<int, std::vector<int>>& edges, std::map<int, std::vector<std::set<int>>> new_max_bicliques, int node_size)
{
	batch_edges = edges;
	vertexSet = vertexset;
	newMaxB = new_max_bicliques; 
	nodeSize = node_size;
}

void SubsumedBicliques::find_subsumed_bic(std::string algorithm)
{
	if (algorithm == "standardSub")
	{
		for (auto i = newMaxB.begin(); i != newMaxB.end(); i++)
		{
			std::vector<std::set<int>> b = i->second;  

			for (auto j = batch_edges.begin(); j != batch_edges.end(); j++)
			{
				std::vector<int> e = j->second;  

				if (b[0].find(e[0]) != b[0].end() && b[1].find(e[1]) != b[1].end()) 
				{
					if (b[0].size() != 1)
					{
						b[0].erase(e[0]);
						store_sub_max_bic(b[0], b[1]);
					}

					if (b[1].size() != 1)
					{
						b[0].insert(e[0]);
						b[1].erase(e[1]);
						store_sub_max_bic(b[0], b[1]);
					}
				}
			}
		}
	}
	else if (algorithm == "improvedSub")
	{
		for (auto i = newMaxB.begin(); i != newMaxB.end(); i++)
		{
			std::vector<std::set<int>> b = i->second;  

			for (auto j = batch_edges.begin(); j != batch_edges.end(); j++)
			{
				std::vector<int> e = j->second;  

				if (b[0].find(e[0]) != b[0].end() && b[1].find(e[1]) != b[1].end())
				{
					if (b[0].size() != 1)
					{
						b[0].erase(e[0]);
						check_maximality(b[0], b[1]);
					}
					
					if (b[1].size() != 1)
					{
						b[0].insert(e[0]);
						b[1].erase(e[1]);
						check_maximality(b[0], b[1]);
					}
				}
			}
		}
	}
}

std::map<std::string, std::string> SubsumedBicliques::get_hash_subsumed_bic()
{
	return hash_subsumed_bic;
}

std::map<int, std::vector<std::set<int>>> SubsumedBicliques::get_sub_max_bic()
{
	return sub_max_bic;
}

void SubsumedBicliques::clear_sub_bicliques()
{
	sub_max_bic.clear();
	sub_max_bic_string.clear();
	hash_subsumed_bic.clear();
}

void SubsumedBicliques::check_maximality(std::set<int> L, std::set<int> R)
{
	bool result;
	if (L.size() <= R.size())
	{
		std::set<int> common_nbr;
		for (auto i = L.begin(); i != L.end(); i++)
		{
			int node = *i;
			std::set<int> node_nbr = vertexSet.get_left_node_neighbor(node);
			std::vector<int> current_com;
			set_intersection(node_nbr.begin(), node_nbr.end(), common_nbr.begin(), common_nbr.end(), back_inserter(current_com));
			for (auto j = current_com.begin(); j != current_com.end(); j++)
			{
				common_nbr.insert(*j);
			}
		}

		if (common_nbr.size() > R.size())
		{
			result = false; 
		}
		else      
		{
			result = true;
			store_sub_max_bic(L, R);
		}
	}
	else
	{
		std::set<int> common_nbr;
		for (auto i = R.begin(); i != R.end(); i++)
		{
			int node = *i;
			std::set<int> node_nbr = vertexSet.get_right_node_neighbor(node);
			std::vector<int> current_com;
			set_intersection(node_nbr.begin(), node_nbr.end(), common_nbr.begin(), common_nbr.end(), back_inserter(current_com));
			for (auto j = current_com.begin(); j != current_com.end(); j++)
			{
				common_nbr.insert(*j);
			}
		}

		if (common_nbr.size() > L.size())
		{
			result = false;  
			
		}
		else     
		{
			result = true;
			store_sub_max_bic(L, R);
		}
	}
}

void SubsumedBicliques::store_sub_max_bic(std::set<int>& L_in, std::set<int>& R_in)
{
	if (nodeSize == 1)
	{
		if (L_in.size() != 1 || R_in.size() != 1)
		{
			bool re = check_sub_bic(L_in, R_in);

			if (re == false)  
			{
				std::vector<std::set<int>> bi;
				bi.push_back(L_in);
				bi.push_back(R_in);
				int num = sub_max_bic.size();
				sub_max_bic.insert(make_pair(num + 1, bi)); 

				hash_subsumed_bic.insert(make_pair(signal_L, signal_R));

			}

			signal_L.clear();
			signal_R.clear();
		}
	}
	else
	{
		if (L_in.size() >= nodeSize && R_in.size() >= nodeSize)
		{
			bool re = check_sub_bic(L_in, R_in);

			if (re == false)  
			{
				std::vector<std::set<int>> bi;
				bi.push_back(L_in);
				bi.push_back(R_in);
				int num = sub_max_bic.size();
				sub_max_bic.insert(make_pair(num + 1, bi)); 

				hash_subsumed_bic.insert(make_pair(signal_L, signal_R));

			}

			signal_L.clear();
			signal_R.clear();
		}
	}
	
}

bool SubsumedBicliques::check_sub_bic(std::set<int>& L, std::set<int>& R)
{
	bool result = false;

	for (auto i = L.begin(); i != L.end(); i++)
	{
		std::string str = std::to_string(*i);
		signal_L = signal_L + str;
	}
	for (auto i = R.begin(); i != R.end(); i++)
	{
		std::string str = std::to_string(*i);
		signal_R = signal_R + str;
	}

	if (hash_subsumed_bic.find(signal_L) != hash_subsumed_bic.end())
	{
		if (hash_subsumed_bic[signal_L] == signal_R)
		{
			result = true;
		}
	}

	return result;
}
