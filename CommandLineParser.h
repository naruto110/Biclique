#ifndef _COMMANDLINEPARSER_H
#define _COMMANDLINEPARSER_H


#include<vector>
#include<map>
#include<set>
#include<unordered_map>

class CommandLineParser
{
public:
	CommandLineParser(char** argv);
	std::map<std::string, std::string> current_max_bic;
	std::map<std::string, std::vector<std::set<int>>> current_max_bic_set;

private:
	int edge_size;
	std::string algorithm_1;
	std::string algorithm_2;
	int batch_size;
	int size_th;
	int stop_iteration_number;

	void parse_filename(std::string filename);
	std::vector<std::string> read_edge;
	std::vector<std::string> SplitStr(std::string& str);

	std::map<int, std::vector<std::set<int>>> new_max_bi;
	std::map<int, std::string> new_max_bi_string;
	std::map<std::string, std::string> hash_new_max_bi;
	std::map<int, std::vector<std::set<int>>> sub_max_bi;
	std::map<std::string, std::string> hash_sub_bi;
	void update_max_bic();

	void process_edge_deletions(std::unordered_map<int, std::vector<int>>& batchEdges);
	std::map<int, std::vector<std::set<int>>> sperated_bi;
	std::map<std::string, std::string> hash_sub_max_bi;
	std::map<std::string, std::vector<std::set<int>>> sub_max_bi_set;
	void updateMaxBic_addNewSubMax();
};


#endif // !_COMMANDLINEPARSER_H