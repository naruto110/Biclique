#ifndef _VERTEX_H
#define _VERTEX_H


#include <map>
#include<set>
#include <string>
#include <vector>

class Vertex
{
public:
	Vertex();
	Vertex(int& leftNode, int& rightNode);
	std::set<int> get_left_node_neighbor(int& left_node);
	std::set<int> get_right_node_neighbor(int& right_node);

	int num_nbr_of_v_in_part_of_L(int& v, std::set<int>& L_prime);

	void add_neighbor(int& left_node, int& right_node);
	void delete_neighbor(int& left_node, int& right_node);

private:
	int left_start = 0;
	int right_start = 0;
	std::map<int, int> left_node_label;
	std::map<int, int> right_node_label;
	std::vector<std::set<int>> left_node_nbr_set; 
	std::vector<std::set<int>> right_node_nbr_set;
	
};

#endif // !_VERTEX_H