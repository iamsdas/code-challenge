import sys
from typing import List, Optional


class MempoolTransaction():

    def __init__(self, txid: str, fee: str, weight: str, parents: Optional[str]):
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


def knapSack(W: int, trans: List[MempoolTransaction]):
    n = len(trans)
    K = [[{'vis': [], 'val': 0} for x in range(W + 1)] for x in range(n + 1)]

    for i in range(n + 1):
        for w in range(W + 1):
            if i == 0 or w == 0:
                continue
            else:
                K[i][w] = K[i-1][w]
                if trans[i-1].weight <= w:
                    K[i][w] = K[i-1][w]
                    if (K[i-1][w-trans[i-1].weight]['val']+trans[i-1].fee <= K[i-1][w]['val']):
                        K[i][w]['val'] = K[i-1][w -
                                                trans[i-1].weight]['val'] + trans[i-1].fee

    return K[n][W]


def main():
    transactions = parse_mempool_csv()
    # block = knapSack(40000, transactions)['val']
    # print(block)
    print(transactions)


main()
