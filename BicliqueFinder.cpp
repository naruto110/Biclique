#include "BicliqueFinder.h"
#include<algorithm> 
#include<iostream>
#include<string>
 

BicliqueFinder::BicliqueFinder(Vertex vertexset, std::unordered_map<int, std::vector<int>>& add_edges, int node_size)
{
	batch_edges = add_edges;
	vertexSet = vertexset;
	nodeSize = node_size;
}

void BicliqueFinder::find_new_maximal_bicliques(std::string algorithm)
{
	if (algorithm == "standardNew")
	{
		for (auto i = batch_edges.begin(); i != batch_edges.end(); i++) 
		{
			std::vector<int> e = i->second;
			L_initial_ = vertexSet.get_right_node_neighbor(e[1]);
			P_initial_ = vertexSet.get_left_node_neighbor(e[0]);

			R_initial_.clear();
			Q_initial_.clear();

			biclique_find_base(L_initial_, R_initial_, P_initial_, Q_initial_);
		}
	}
	else if (algorithm == "improvedNew")
	{
		for (auto i = batch_edges.begin(); i != batch_edges.end(); i++)
		{
			std::vector<int> e = i->second;

			L_initial_ = vertexSet.get_right_node_neighbor(e[1]);
			P_initial_ = vertexSet.get_left_node_neighbor(e[0]);

			R_initial_.clear();
			Q_initial_.clear();

			biclique_find_improved(L_initial_, R_initial_, P_initial_, Q_initial_);
		}

	}
}

void BicliqueFinder::add_to_processed_edges(int& u, int& v)
{
	if (processed_edges_index.find(v) == processed_edges_index.end())
	{
		int index = processed_edges_index.size();
		processed_edges_index[v] = index;

		std::set<int> nbr;
		nbr.insert(u);
		processed_edges_set.push_back(nbr);
	}
	else
	{
		processed_edges_set[processed_edges_index.at(v)].insert(u);
	}
}

void BicliqueFinder::biclique_find_improved(std::set<int>& L_in, std::set<int>& R_in, std::set<int>& P_in, std::set<int>& Q_in)
{
	std::multimap<int, int> P = sort_P(L_in, P_in); 

	std::set<int> L = L_in;
	std::set<int> R = R_in;
	std::set<int> Q = Q_in;

	while (!P.empty())
	{
		std::multimap<int, int>::iterator x = P.begin();
		std::set<int> R_prime = R;
		R_prime.insert(x->second); 


		std::vector<int> LPrime;
		std::set<int> x_all_nbr = vertexSet.get_right_node_neighbor(x->second);
		set_intersection(x_all_nbr.begin(), x_all_nbr.end(), L.begin(), L.end(), back_inserter(LPrime));
		std::set<int> L_prime;
		for (auto i = LPrime.begin(); i != LPrime.end(); i++)
		{
			L_prime.insert(*i);
		}

		std::vector<int> overline_LPrime;
		std::set<int> C;
		set_difference(L.begin(), L.end(), LPrime.begin(), LPrime.end(), back_inserter(overline_LPrime));
		std::set<int> overline_L_prime;
		for (auto i = overline_LPrime.begin(); i != overline_LPrime.end(); i++)
		{
			overline_L_prime.insert(*i);
		}
		C.insert(x->second);


		std::set<int> P_prime;
		std::set<int> Q_prime;

		bool is_maximal = true;

		for (auto i = Q.begin(); i != Q.end(); i++)
		{
			int i_label = *i;
			int num_L_prime_nbr = vertexSet.num_nbr_of_v_in_part_of_L(i_label, L_prime);

			if (num_L_prime_nbr == L_prime.size())
			{
				is_maximal = false;
				break;
			}
			else if (num_L_prime_nbr > 0)
			{
				Q_prime.insert(*i);
			}
		}

		if (is_maximal)
		{
			for (auto i = P.begin(); i != P.end(); i++)
			{
				if (i->second == x->second)
				{
					continue;
				}

				int num_L_prime_nbr = vertexSet.num_nbr_of_v_in_part_of_L(i->second, L_prime);
				if (num_L_prime_nbr == L_prime.size())
				{
					R_prime.insert(i->second);

					int num_overline_L_prime_nbr = vertexSet.num_nbr_of_v_in_part_of_L(i->second, overline_L_prime);
					if (num_overline_L_prime_nbr == 0)
					{
						C.insert(i->second);
					}
				}
				else if (num_L_prime_nbr > 0)
				{
					P_prime.insert(i->second);
				}
			}

			new_maiximal_bicliques(L_prime, R_prime);
			

			if (!P_prime.empty())
			{
				biclique_find_improved(L_prime, R_prime, P_prime, Q_prime);
			}
		}

		for (auto i = P.begin(); i != P.end();)
		{
			if (C.find(i->second) != C.end())
			{
				Q.insert(i->second);
				auto iter = i;
				i++;
				P.erase(iter);
			}
			else
			{
				i++;
			}
		}
	}
}

std::multimap<int, int> BicliqueFinder::sort_P(std::set<int>& L_in, std::set<int>& P_in)
{
	std::set<int> L = L_in;
	std::multimap<int, int> P_sorted; 
	for (auto i = P_in.begin(); i != P_in.end(); i++)
	{
		int lable = *i;
		std::set<int> nbr_set = vertexSet.get_right_node_neighbor(lable);
		std::vector<int> common_nbr;
		set_intersection(nbr_set.begin(), nbr_set.end(), L.begin(), L.end(), back_inserter(common_nbr));
		P_sorted.insert(std::make_pair(common_nbr.size(), lable)); 
	}
	return P_sorted;
}

bool BicliqueFinder::check_maximal_bicliques(std::set<int>& L, std::set<int>& R)
{
	bool result = false;
	
	for (auto i = L.begin(); i != L.end(); i++)
	{
		string str = to_string(*i);
		signal_L = signal_L + str;
	}
	for (auto i = R.begin(); i != R.end(); i++)
	{
		string str = to_string(*i);
		signal_R = signal_R + str;
	}

	if (hash_max_bic.find(signal_L) != hash_max_bic.end())
	{
		if (hash_max_bic[signal_L] == signal_R)
		{
			result = true;
		}
	}

	return result;
}

bool BicliqueFinder::check_processed_edges(int& node, std::set<int>& L_prime)
{
	bool result;
	
	if (processed_edges_index.find(node) == processed_edges_index.end())
	{
		result = false;
	}
	else
	{
		int index = processed_edges_index.at(node);
		std::vector<int> processed;
		set_intersection(L_prime.begin(), L_prime.end(), processed_edges_set[index].begin(), processed_edges_set[index].end(), back_inserter(processed));
		if (processed.size() != 0)
		{
			result = true;
		}
		else
		{
			result = false;
		}
	}
	
	return result;
}

void BicliqueFinder::new_maiximal_bicliques(std::set<int>& L_in, std::set<int>& R_in)
{
	if (nodeSize == 1)
	{
		if (L_in.size() != 1 || R_in.size() != 1)
		{
			bool re = check_maximal_bicliques(L_in, R_in);

			if (re == false) 
			{
				std::vector<std::set<int>> bi;
				bi.push_back(L_in);
				bi.push_back(R_in);
				int num = new_max_bic.size();
				new_max_bic.insert(make_pair(num + 1, bi)); 

				new_max_bic_string.insert(make_pair(num + 1, signal_L));

				hash_max_bic.insert(make_pair(signal_L, signal_R));
			}

			signal_L.clear();
			signal_R.clear();
		}
	}
	else
	{
		if (L_in.size() >= nodeSize && R_in.size() >= nodeSize)
		{
			bool re = check_maximal_bicliques(L_in, R_in);

			if (re == false)  
			{
				std::vector<std::set<int>> bi;
				bi.push_back(L_in);
				bi.push_back(R_in);
				int num = new_max_bic.size();
				new_max_bic.insert(make_pair(num + 1, bi)); 

				new_max_bic_string.insert(make_pair(num + 1, signal_L));

				hash_max_bic.insert(make_pair(signal_L, signal_R));

			}

			signal_L.clear();
			signal_R.clear();
		}
	}
}

void BicliqueFinder::biclique_find_base(std::set<int>& L_in, std::set<int>& R_in, std::set<int>& P_in, std::set<int>& Q_in)
{
	std::set<int> P = P_in;
	std::set<int> L = L_in;
	std::set<int> R = R_in;
	std::set<int> Q = Q_in;

	for (auto i = P.begin(); i != P.end(); i++)
	{
		std::set<int> R_prime = R;
		R_prime.insert(*i); 

		std::vector<int> LPrime;
		int node = *i;
		std::set<int> x_all_nbr = vertexSet.get_right_node_neighbor(node);
		set_intersection(x_all_nbr.begin(), x_all_nbr.end(), L.begin(), L.end(), back_inserter(LPrime));
		std::set<int> L_prime;
		for (auto i = LPrime.begin(); i != LPrime.end(); i++)
		{
			L_prime.insert(*i);
		}

		std::set<int> P_prime;
		std::set<int> Q_prime;

		bool is_maximal = true;

		for (auto i = Q.begin(); i != Q.end(); i++)
		{
			int i_label = *i;
			int num_L_prime_nbr = vertexSet.num_nbr_of_v_in_part_of_L(i_label, L_prime);

			if (num_L_prime_nbr == L_prime.size())
			{
				is_maximal = false;
				break;
			}
			else if (num_L_prime_nbr > 0)
			{
				Q_prime.insert(*i);
			}
		}

		if (is_maximal)
		{
			for (auto j = P.begin(); j != P.end(); j++)
			{
				if (*j == *i)
				{
					continue;
				}

				int n = *j;
				int num_L_prime_nbr = vertexSet.num_nbr_of_v_in_part_of_L(n, L_prime);
				if (num_L_prime_nbr == L_prime.size())
				{
					R_prime.insert(*j);
				}
			}

			new_maiximal_bicliques(L_prime, R_prime);

			if (!P_prime.empty())
			{
				biclique_find_improved(L_prime, R_prime, P_prime, Q_prime);
			}
		}
	}
}

std::map<string, string> BicliqueFinder::get_hash_new_max_bic()
{
	return hash_max_bic;
}

std::map<int, std::vector<std::set<int>>> BicliqueFinder::get_new_max_bic()
{
	return new_max_bic;
}

std::map<int, std::string> BicliqueFinder::get_new_max_bic_string()
{
	return new_max_bic_string;
}

void BicliqueFinder::clear_maximal_bicliques()
{
	new_max_bic.clear();
	hash_max_bic.clear();
}
