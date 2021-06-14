import sys

sys.setrecursionlimit(int(float('inf')))


class MempoolTransaction():

    def __init__(self, txid, fee, weight, parents):
        self.txid = txid
        self.fee = int(fee)
        self.weight = int(weight)
        self.parents = parents.strip().split(';')

    def __repr__(self):
        return self.txid


def parse_mempool_csv():
    """Parse the CSV file and return a list of MempoolTransactions."""
    with open('mempool.csv') as f:
        return([MempoolTransaction(*line.strip().split(',')) for line in f.readlines()])


def knapSack(W, trans, n, vis, val):

    if n == 0 or W == 0:
        return {'vis': vis, 'val': val}

    if (trans[n-1].weight > W):
        return knapSack(W, trans, n-1, vis, val)

    else:
        a = knapSack(W, trans, n-1, vis, val)
        vis.append(trans[n-1].txid)
        val += trans[n-1].fee
        b = knapSack(W, trans, n-1, vis, val)
        return a if (a['val'] > b['val']) else b


def main():
    transactions = parse_mempool_csv()
    block = knapSack(4000000, transactions, len(transactions), [], 0)['val']
    print(block)


main()
