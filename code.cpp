/**
 * code.cpp
 * Submission for Summer of Bitcoin Code Challenge
 * by Suryashankar Das
 **/

#include <bits/stdc++.h>
using namespace std;
#define MAX_WEIGHT (int)4e6

class MemPoolTransactions
{
public:
	string txid;
	int fee;
	int weight;
	int num_parents;
	vector<string> parents, children;

	MemPoolTransactions(vector<string> &params)
	{
		txid = params[0];
		fee = stoi(params[1]);
		weight = stoi(params[2]);
		parents = parse_parents(params[3]);
		num_parents = parents.size();
	}

	// parses string to generate list
	vector<string> parse_parents(string &p)
	{
		stringstream ss(p);
		string parent;
		vector<string> res;

		while (getline(ss, parent, ';'))
			res.push_back(parent);
		return res;
	}

	// highest fees, lowest weight, lowest(none) parents
	bool operator<(const MemPoolTransactions &other) const
	{
		if (this->num_parents == other.num_parents)
		{
			if (this->fee == other.fee)
			{
				if (this->weight == other.weight)
					return this->txid < other.txid;
				return this->weight > other.weight;
			}
			return this->fee < other.fee;
		}
		return this->num_parents > other.num_parents;
	}
};

// read file and generate a mempool map
void parse_mempool_csv(unordered_map<string, MemPoolTransactions *> &mempool)
{
	ifstream file("mempool.csv");
	string line, col;
	vector<string> params;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			params.clear();
			stringstream ss(line);
			while (getline(ss, col, ','))
			{
				params.push_back(col);
			}
			if (params.size() == 3)
				params.push_back("");
			mempool[params[0]] = new MemPoolTransactions(params);
		}
	}
	file.close();
}

class Block
{
private:
	unordered_map<string, MemPoolTransactions *> mempool;
	set<MemPoolTransactions> st;

public:
	vector<string> final_transactions;
	int total_weight = 0;
	int total_fee = 0;

	Block()
	{
		// generate the mempool
		parse_mempool_csv(mempool);
		// add children to parents
		for (const pair<string, MemPoolTransactions *> &transaction : mempool)
		{
			for (const string &parent : transaction.second->parents)
			{
				mempool[parent]->children.push_back(transaction.first);
			}
			// insert all mempool transactions into the set
			st.insert(*transaction.second);
		}
		// generate final list of transactions
		while (!st.empty())
		{
			// last item of the set is the next optimal item to be added
			if ((*st.rbegin()).weight + total_weight > MAX_WEIGHT)
				break; // end if max weight reached
			final_transactions.push_back((*st.rbegin()).txid);
			total_weight += (*st.rbegin()).weight;
			total_fee += (*st.rbegin()).fee;

			// remove and re-add to reflect their new position in the set
			for (const string &child : (*st.rbegin()).children)
			{
				st.erase(*mempool[child]);
				mempool[child]->num_parents--;
				st.insert(*mempool[child]);
			}
			st.erase(*(st.rbegin()));
		}
	}
};

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	Block block;

	// write the final transactions to the file
	ofstream file("block.txt");
	if (file.is_open())
		for (int i = 0; i < block.final_transactions.size(); i++)
		{
			file << block.final_transactions[i];
			if (i != block.final_transactions.size() - 1)
				file << '\n';
		}
	file.close();

	cout << "### generated block ###\n";
	cout << "number of transactions: " << block.final_transactions.size() << "\n";
	cout << "fees: " << block.total_fee << " weight: " << block.total_weight << "\n";

	return 0;
}