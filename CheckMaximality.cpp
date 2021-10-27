#include"CheckMaximality.h"

#include <algorithm>

CheckMaximality::CheckMaximality(Vertex vertexset, int node_size)
{
	vertexSet = vertexset;
	nodeSize = node_size;
}

void CheckMaximality::check_bic(std::map<int, std::vector<std::set<int>>>& sub_bic)
{
	for (auto b = sub_bic.begin(); b != sub_bic.end(); b++)
	{
		std::vector<std::set<int>> bic = b->second;
		if (bic[0].size() <= bic[1].size())
		{
			std::set<int> common_nbr;
			for (auto i = bic[0].begin(); i != bic[0].end(); i++)
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

			if (common_nbr.size() == bic[1].size())
			{
				store_max_bic(bic); 
			}
		}
		else
		{
			std::set<int> common_nbr;
			for (auto i = bic[1].begin(); i != bic[1].end(); i++)
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

			if (common_nbr.size() == bic[0].size())
			{
				store_max_bic(bic);  
			}
		}
	}
}

std::map<std::string, std::string> CheckMaximality::get_hash_new_bic()
{
	return hash_new_bic;
}

std::map<std::string, std::vector<std::set<int>>> CheckMaximality::get_sub_max_bic_set()
{
	return sub_max_bic_set;
}

void CheckMaximality::store_max_bic(std::vector<std::set<int>> b)
{
	if (nodeSize == 1)
	{
		if (b[0].size() != 1 || b[1].size() != 1)
		{
			std::string signal_L;
			std::string signal_R;

			for (auto i = b[0].begin(); i != b[0].end(); i++)
			{
				std::string str = std::to_string(*i);
				signal_L = signal_L + str;
			}
			for (auto i = b[1].begin(); i != b[1].end(); i++)
			{
				std::string str = std::to_string(*i);
				signal_R = signal_R + str;
			}

			sub_max_bic_set.insert(make_pair(signal_L, b));
			hash_new_bic.insert(make_pair(signal_L, signal_R));
		}
	}
	else
	{
		if (b[0].size() >= nodeSize && b[1].size() >= nodeSize)
		{
			std::string signal_L;
			std::string signal_R;

			for (auto i = b[0].begin(); i != b[0].end(); i++)
			{
				std::string str = std::to_string(*i);
				signal_L = signal_L + str;
			}
			for (auto i = b[1].begin(); i != b[1].end(); i++)
			{
				std::string str = std::to_string(*i);
				signal_R = signal_R + str;
			}

			sub_max_bic_set.insert(make_pair(signal_L, b)); 
			hash_new_bic.insert(make_pair(signal_L, signal_R));
		}
	}
	
}
