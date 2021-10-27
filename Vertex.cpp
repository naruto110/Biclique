#include "Vertex.h"
#include<algorithm>
#include<iostream>

Vertex::Vertex()
{
}

Vertex::Vertex(int& leftNode, int& rightNode)
{
	int left_node = leftNode;
	int right_node = rightNode;
}

std::set<int> Vertex::get_left_node_neighbor(int& left_node)
{
	int label1 = left_node_label.at(left_node);
	return left_node_nbr_set.at(label1);
}

std::set<int> Vertex::get_right_node_neighbor(int& right_node)
{
	int label2 = right_node_label.at(right_node);
	return right_node_nbr_set.at(label2);
}

int Vertex::num_nbr_of_v_in_part_of_L(int& v, std::set<int>& part_of_L)
{
	int label = right_node_label.at(v);
	std::set<int> v_nbr = right_node_nbr_set.at(label);
	std::vector<int> inter_set;
	set_intersection(v_nbr.begin(), v_nbr.end(), part_of_L.begin(), part_of_L.end(), back_inserter(inter_set));
	return inter_set.size();
}

void Vertex::add_neighbor(int& left_node, int& right_node)
{
	if (left_node_label.find(left_node) == left_node_label.end())
	{
		left_node_label[left_node] = left_start;
		std::set<int> nbr;
		left_node_nbr_set.push_back(nbr);

		left_start = left_start + 1;
	}

	if (right_node_label.find(right_node) == right_node_label.end())
	{
		right_node_label[right_node] = right_start;
		std::set<int> nbr;
		right_node_nbr_set.push_back(nbr);

		right_start = right_start + 1;
	}

	int label1 = left_node_label.at(left_node);
	left_node_nbr_set.at(label1).insert(right_node);

	int label2 = right_node_label.at(right_node);
	right_node_nbr_set.at(label2).insert(left_node);

}

void Vertex::delete_neighbor(int& left_node, int& right_node)
{
	int index_left = left_node_label.at(left_node);
	left_node_nbr_set.at(index_left).erase(right_node);
	int index_right = right_node_label.at(right_node);
	right_node_nbr_set.at(index_right).erase(left_node);
}



