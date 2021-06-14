#include <bits/stdc++.h>
using namespace std;
#define MAX_WEIGHT (int)4e5

pair<uint64_t, vector<string *>> dp[MAX_WEIGHT + 1];

vector<string> parse_parents(string &p)
{
	stringstream ss(p);
	string parent;
	vector<string> res;

	while (getline(ss, parent, ';'))
		res.push_back(parent);
	return res;
}

class MemPoolTransactions
{
public:
	string txid;
	int fee;
	int weight;
	vector<string> parents;

	MemPoolTransactions(vector<string> &params)
	{
		this->txid = params[0];
		this->fee = stoi(params[1]);
		this->weight = stoi(params[2]);
		this->parents = parse_parents(params[3]);
	}
};

void parse_mempool_csv(vector<MemPoolTransactions> &mempool)
{
	ifstream file("mempool.csv");
	string line, col;
	vector<string> params;

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
		mempool.push_back(MemPoolTransactions(params));
	}
}

pair<uint64_t, vector<string *>> knapsack(vector<MemPoolTransactions> &mempool)
{
	int n = mempool.size();
	for (int i = 0; i < n; i++)
	{
		for (int w = MAX_WEIGHT; w >= mempool[i].weight; w--)
		{
			if (dp[w].first < dp[w - mempool[i].weight].first + mempool[i].fee)
			{
				dp[w] = make_pair(dp[w - mempool[i].weight].first + mempool[i].fee,
								  dp[w - mempool[i].weight].second);
				// dp[w].second.push_back(&mempool[i].txid);
			}
		}
	}
	return dp[MAX_WEIGHT];
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	vector<MemPoolTransactions> mempool;

	parse_mempool_csv(mempool);
	cout << knapsack(mempool).first << "\n";

	return 0;
}