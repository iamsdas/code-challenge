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

	vector<string> parse_parents(string &p)
	{
		stringstream ss(p);
		string parent;
		vector<string> res;

		while (getline(ss, parent, ';'))
			res.push_back(parent);
		return res;
	}

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
		parse_mempool_csv(mempool);
		for (const pair<string, MemPoolTransactions *> &transaction : mempool)
		{
			for (const string &parent : transaction.second->parents)
			{
				mempool[parent]->children.push_back(transaction.first);
			}
			st.insert(*transaction.second);
		}

		while (!st.empty())
		{
			if ((*st.rbegin()).weight + total_weight > MAX_WEIGHT)
				break;
			final_transactions.push_back((*st.rbegin()).txid);
			total_weight += (*st.rbegin()).weight;
			total_fee += (*st.rbegin()).fee;

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

	ofstream file("block.txt");
	if (file.is_open())
		for (const string &txid : block.final_transactions)
			file << txid << "\n";
	file.close();

	cout << "### generated block ###\n";
	cout << "number of transactions: " << block.final_transactions.size() << "\n";
	cout << "fees: " << block.total_fee << " weight: " << block.total_weight << "\n";

	return 0;
}