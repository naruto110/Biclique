#include <string>
#include<iostream>
#include<fstream>
#include "CommandLineParser.h"
#include "Vertex.h"
#include "BicliqueFinder.h"
#include "SubsumedBicliques.h"
#include "CheckMaximality.h"
#include<unordered_map>
#include <algorithm>
#include <sys/time.h>
using namespace std;


CommandLineParser::CommandLineParser()
{
	string arg1 = "marvel.txt"; 
	edge_size = 96662; 
	algorithm_1 = "improvedNew"; 
	algorithm_2 = "improvedSub"; 
	batch_size = 20; 
	size_th = 1; 

	parse_filename(arg1);
}


void CommandLineParser::parse_filename(std::string filename)
{
	std::ifstream file;
	std::string line;
	file.open(filename, std::ifstream::in);
	Vertex input_edge= Vertex();

	struct timeval step_start; 
	struct timeval step_end;    
	struct timeval step_start_aBatch;       
	struct timeval step_end_aBatch;         
	struct timeval step_middle_1;           
	struct timeval step_middle_2;           


	float alltime = 0;        
	float aBatch_time = 0;      
	float total_new = 0;       
	float total_subsumed = 0;   

	string outfile_name;
	outfile_name.append(filename);
	outfile_name.append("_");
	outfile_name.append(algorithm_1);
	outfile_name.append("_");
	if (batch_size == 10)
	{
		outfile_name.append("10");
		outfile_name.append("_");
	}
	else if (batch_size == 100)
	{
		outfile_name.append("100");
		outfile_name.append("_");
	}
	else
	{
		outfile_name.append("1000");
		outfile_name.append("_");
	}
	
	if (size_th == 1)
	{
		outfile_name.append("1");
		outfile_name.append("_");
	}
	else if (size_th == 4)
	{
		outfile_name.append("4");
		outfile_name.append("_");
	}
	else if (size_th == 8)
	{
		outfile_name.append("8");
		outfile_name.append("_");
	}
	else
	{
		outfile_name.append("12");
		outfile_name.append("_");
	}
	ofstream out1(outfile_name + "out1.txt"); 
	ofstream out2(outfile_name + "out2.txt");  
	ofstream out3(outfile_name + "out3.txt"); 
	ofstream out4(outfile_name + "out4.txt");  
	
	int stop_num_edges = 0;

	int iteration_number = 0;
	if (file.is_open())
	{
		gettimeofday(&step_start, NULL);  
		int index = 1; 
		unordered_map<int, vector<int>> batch_edges; 

		for (auto i = 0; i < edge_size; i++)
		{
			stop_num_edges = stop_num_edges + 1;

			getline(file, line);
			read_edge = SplitStr(line);
			int a = atoi(read_edge[0].c_str());
			int b = atoi(read_edge[1].c_str());
			input_edge.add_neighbor(a, b);

			vector<int> edge;
			edge.push_back(a);
			edge.push_back(b);
			batch_edges[index] = edge;

			if (index == batch_size) 
			{
				iteration_number = iteration_number + 1;
				gettimeofday(&step_start_aBatch, NULL);

				BicliqueFinder finder = BicliqueFinder(input_edge, batch_edges, size_th);
				finder.find_new_maximal_bicliques(algorithm_1);
				new_max_bi = finder.get_new_max_bic();
				new_max_bi_string = finder.get_new_max_bic_string();
				hash_new_max_bi = finder.get_hash_new_max_bic();
				finder.clear_maximal_bicliques();

				gettimeofday(&step_middle_1, NULL);

				SubsumedBicliques finder_sub = SubsumedBicliques(input_edge, batch_edges, new_max_bi, size_th);
				finder_sub.find_subsumed_bic(algorithm_2);
				sub_max_bi = finder_sub.get_sub_max_bic();
				hash_sub_bi = finder_sub.get_hash_subsumed_bic();
				finder_sub.clear_sub_bicliques();

				gettimeofday(&step_middle_2, NULL);
				total_new = (step_middle_1.tv_sec * 1e6 + step_middle_1.tv_usec) - (step_start_aBatch.tv_sec * 1e6 + step_start_aBatch.tv_usec) + total_new;
				total_subsumed = (step_middle_2.tv_sec * 1e6 + step_middle_2.tv_usec) - (step_middle_1.tv_sec * 1e6 + step_middle_1.tv_usec) + total_subsumed;

				update_max_bic();

				index = 1;
				batch_edges.clear();

				gettimeofday(&step_end_aBatch, NULL);
				aBatch_time = (step_end_aBatch.tv_sec * 1e6 + step_end_aBatch.tv_usec) - (step_start_aBatch.tv_sec * 1e6 + step_start_aBatch.tv_usec);
				out2 << (long long)(aBatch_time / 1000) << std::endl;
			}
			else
			{
				index = index + 1;
			}


			gettimeofday(&step_end, NULL);
			alltime = ((step_end.tv_sec * 1e6 + step_end.tv_usec) - (step_start.tv_sec * 1e6 + step_start.tv_usec)) / 1e6; 

		}

		out1 << "alltime: "<< alltime << std::endl;
		out1 << "stop_edge_size: " << stop_num_edges << std::endl; 
		out1 << "iteration_number: " << iteration_number << std::endl;
		out1 << "total_new: " << total_new / 1e6 << std::endl;
		out1 << "total_subsumed: " << total_subsumed / 1e6 << std::endl;

	}
	file.close();
	
	//struct timeval deletion_step_start;  
	//struct timeval deletion_step_end;   
	//struct timeval deletion_step_start_aBatch;
	//struct timeval deletion_step_end_aBatch; 
	//struct timeval deletion_step_middle_1; 
	//struct timeval deletion_step_middle_2;

	//float del_alltime = 0;     
	//float total_del_bi = 0;
	//float total_add_new_bi = 0;

	////batch edge deletions
	//file.open(filename, std::ifstream::in);
	//if(file.is_open())
	//{
	//	gettimeofday(&deletion_step_start, NULL);
	//	int index = 1; //index batch edges
	//	unordered_map<int, vector<int>> batch_edges; 

	//	for (auto i = 0; i < edge_size; i++)
	//	{
	//		getline(file, line);
	//		read_edge = SplitStr(line);
	//		int a = atoi(read_edge[0].c_str());
	//		int b = atoi(read_edge[1].c_str());

	//		//read file and delete the edge
	//		input_edge.delete_neighbor(a, b);

	//		//store the batch edges
	//		vector<int> edge;
	//		edge.push_back(a);
	//		edge.push_back(b);
	//		batch_edges[index] = edge;

	//		if (index == batch_size)
	//		{
	//			gettimeofday(&deletion_step_start_aBatch, NULL);

	//			process_edge_deletions(batch_edges);

	//			gettimeofday(&deletion_step_middle_1, NULL);

	//			CheckMaximality finder_new = CheckMaximality(input_edge, size_th);
	//			finder_new.check_bic(sperated_bi);//add new biclique
	//			hash_sub_max_bi = finder_new.get_hash_new_bic();
	//			sub_max_bi_set = finder_new.get_sub_max_bic_set();

	//			gettimeofday(&deletion_step_middle_2, NULL);
	//			total_del_bi = (deletion_step_middle_1.tv_sec * 1e6 + deletion_step_middle_1.tv_usec) - (deletion_step_start_aBatch.tv_sec * 1e6 + deletion_step_start_aBatch.tv_usec) + total_del_bi;
	//			total_add_new_bi = (deletion_step_middle_2.tv_sec * 1e6 + deletion_step_middle_2.tv_usec) - (deletion_step_middle_1.tv_sec * 1e6 + deletion_step_middle_1.tv_usec) + total_add_new_bi;

	//			updateMaxBic_addNewSubMax();

	//			index = 1;
	//			batch_edges.clear();

	//			gettimeofday(&deletion_step_end_aBatch, NULL);
	//			aBatch_time = (deletion_step_end_aBatch.tv_sec * 1e6 + deletion_step_end_aBatch.tv_usec) - (deletion_step_start_aBatch.tv_sec * 1e6 + deletion_step_start_aBatch.tv_usec);
	//			out4 << (long long)(aBatch_time / 1000) << std::endl;
	//		}
	//		else
	//		{
	//			index = index + 1;
	//		}

	//		gettimeofday(&deletion_step_end, NULL);
	//		del_alltime = ((deletion_step_end.tv_sec * 1e6 + deletion_step_end.tv_usec) - (deletion_step_start.tv_sec * 1e6 + deletion_step_start.tv_usec)) / 1e6;

	//		if (i== stop_num_edges-1)
	//		{
	//			break;
	//		}
	//	}



	//	out3 << "deletion-alltime: " << del_alltime << std::endl;
	//	//out3 << "deletion-input_edge_size: " << edge_size << std::endl;
	//	out3 << "deletion-stop_edge_size: " << stop_num_edges << std::endl; 
	//	out3 << "deletion-total_del_bi: " << total_del_bi / 1e6 << std::endl;
	//	out3 << "deletion-total_add_new_bi: " << total_add_new_bi / 1e6 << std::endl;
	//}
}


vector<std::string> CommandLineParser::SplitStr(std::string& each_line)
{
	vector<std::string> result;
	std::string tmp_str;
	int i = 0;
	for (; i < each_line.size(); i++) {
		if (each_line[i] == '\t' || each_line[i] == ' ' || each_line[i] == 0x20) {
			if (tmp_str.empty())
				continue;
			result.push_back(tmp_str);
			tmp_str.clear();
		}
		else {
			tmp_str += each_line[i];
		}
	}

	if (!tmp_str.empty())
		result.push_back(tmp_str);
	return result;
}

void CommandLineParser::update_max_bic()
{
	if (algorithm_2 == "standardSub")
	{
		for (auto i = hash_sub_bi.begin(); i != hash_sub_bi.end(); i++)
		{
			if (current_max_bic.find(i->first) != current_max_bic.end() && current_max_bic.at(i->first) == i->second)
			{
				current_max_bic.erase(i->first);
				current_max_bic_set.erase(i->first);
			}
		}
	}
	else if ("improvedSub")
	{
		for (auto i = hash_sub_bi.begin(); i != hash_sub_bi.end(); i++)
		{
			if (current_max_bic.find(i->first) != current_max_bic.end() && current_max_bic.at(i->first) == i->second)
			{
				current_max_bic.erase(i->first);
				current_max_bic_set.erase(i->first);
			}
		}
	}

	current_max_bic.insert(hash_new_max_bi.begin(), hash_new_max_bi.end());
	for (auto i = new_max_bi.begin(); i != new_max_bi.end(); i++)
	{
		current_max_bic_set.insert(make_pair(new_max_bi_string.at(i->first), i->second));
	}

	new_max_bi.clear();
	new_max_bi_string.clear();
	hash_new_max_bi.clear();
	sub_max_bi.clear();
	hash_sub_bi.clear();

}

void CommandLineParser::process_edge_deletions(std::unordered_map<int, std::vector<int>>&  batchEdges)
{
	std::set<std::string> delete_id;
	for (auto i = batchEdges.begin(); i != batchEdges.end(); i++)
	{
		for (auto j = current_max_bic_set.begin(); j != current_max_bic_set.end(); j++)
		{
			std::vector<int> edge = i->second;
			std::vector<std::set<int>> bicliques = j->second;
			if (bicliques[0].find(edge[0]) != bicliques[0].end() && bicliques[1].find(edge[1]) != bicliques[1].end())
			{
				delete_id.insert(j->first);
				if (bicliques[0].size() == 1 && bicliques[1].size() != 1)
				{
					set<int> s1 = bicliques[1];
					s1.erase(edge[1]);
					if (s1.size() != 1)
					{
						std::vector<std::set<int>> s2; 
						s2.push_back(bicliques[0]);
						s2.push_back(s1);
						int num = sperated_bi.size();
						sperated_bi.insert(make_pair(num + 1, s2));
					}
				}
				else if (bicliques[0].size() != 1 && bicliques[1].size() == 1)
				{
					set<int> s1 = bicliques[0];
					s1.erase(edge[0]);
					if (s1.size() != 1)
					{
						std::vector<std::set<int>> s2;
						s2.push_back(s1);
						s2.push_back(bicliques[1]);
						int num = sperated_bi.size();
						sperated_bi.insert(make_pair(num + 1, s2));
					}
				}
				else if (bicliques[0].size() != 1 && bicliques[1].size() != 1)
				{
					set<int> s1 = bicliques[0];
					s1.erase(edge[0]);
					std::vector<std::set<int>> s2;
					s2.push_back(s1);
					s2.push_back(bicliques[1]);
					int num = sperated_bi.size();
					sperated_bi.insert(make_pair(num + 1, s2));

					s1.insert(edge[0]);
					set<int> s3 = bicliques[1];
					s3.erase(edge[1]);
					std::vector<std::set<int>> s4;
					s4.push_back(s1);
					s4.push_back(s3);
					sperated_bi.insert(make_pair(num + 2, s4));
				}
			}
		}
	}

	for (auto k = delete_id.begin(); k != delete_id.end(); k++)
	{
		current_max_bic.erase(*k);
		current_max_bic_set.erase(*k);
	}
	delete_id.clear();
}

void CommandLineParser::updateMaxBic_addNewSubMax()
{
	current_max_bic.insert(hash_sub_max_bi.begin(), hash_sub_max_bi.end());
	current_max_bic_set.insert(sub_max_bi_set.begin(), sub_max_bi_set.end());

	sperated_bi.clear();
	hash_sub_max_bi.clear();
	sub_max_bi_set.clear();
}

