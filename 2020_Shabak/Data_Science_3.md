# Data Science 3
Category: Data Science, 200 points


## Description

See [Data_Science_1.md](Data Science 1)

## Solution:

For this flag, we'll look for the largest internal clique of hosts. In other words, we're searching for the largest "sub-network" in our corporation where all hosts are communicating with each other.

We'll use the reference code from [here](https://www.kaggle.com/hawkcurry/data-analysis-for-network-security-101-solution) to identify the clique.

```python
import networkx
from networkx.algorithms.approximation.clique import large_clique_size 
from collections import Counter

internal_edges_all = df[
  df['src_int'] & df['dst_int']
].drop_duplicates(['src_ip', 'dst_ip', 'dst_port'])
internal_ports = internal_edges_all.dst_port.unique()

port_upper_bounds = []
for p in internal_ports:
   internal_edges = internal_edges_all\
      .pipe(lambda x: x[x['dst_port'] == p])\
      .drop_duplicates(['src_ip', 'dst_ip'])
   edges = set()
   for l, r in zip(internal_edges.src_ip, internal_edges.dst_ip):
      k = min((l, r), (r, l))
      edges.add(k)
   degrees = Counter()
   for (l, r) in edges:
      degrees[l] += 1
      degrees[r] += 1
   max_clique_size = 0
   min_degrees = len(degrees)
   for idx, (node, degree) in enumerate(degrees.most_common()):
      min_degrees = min(min_degrees, degree)
      if min_degrees >= idx:
         max_clique_size = max(max_clique_size, idx+1)
      if min_degrees < max_clique_size:
         break
   port_upper_bounds.append((p, max_clique_size + 1))

max_port = 0
curr_max_clique = 0
max_clique_G = None
for p, max_clique_upper_bound in port_upper_bounds:
    if curr_max_clique > max_clique_upper_bound: break
    internal_edges = internal_edges_all\
        .pipe(lambda x: x[x['dst_port'] == p])\
        .drop_duplicates(['src_ip', 'dst_ip'])
    internal_nodes = set(internal_edges.src_ip) | set(internal_edges.dst_ip)
    G = networkx.Graph()
    G.add_nodes_from(internal_nodes)
    for l, r in zip(internal_edges.src_ip, internal_edges.dst_ip):
        G.add_edge(l, r)        
    _size = large_clique_size(G) 
    if curr_max_clique < _size:
        curr_max_clique = _size
        max_port = p
        max_clique_G = G

max_clique_ips = networkx.algorithms.approximation.clique.max_clique(max_clique_G)
max_clique_ips
```

Output:
```
{'120.18.123.198',
 '120.18.132.228',
 '120.18.14.179',
 '120.18.147.29',
 '120.18.15.122',
 '120.18.154.145',
 '120.18.158.121',
 '120.18.187.161',
 '120.18.201.129',
 '120.18.241.224',
 '120.18.254.217'}
```

We'll export to CSV:
```python
pd.DataFrame(max_clique_ips).to_csv("flag_c.csv", index = False, header = ["ip"])
```

And we got our third flag:
```
Congratulations! You achieved the goal. Here is your flag: 

flag{12124wdjhjh78882saslw@90817#4}
```

