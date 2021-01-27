# Data Science 2
Category: Data Science, 150 points


## Description

See [Data_Science_1.md](Data Science 1)

## Solution:

For this flag, we'll investigate who's sending out a significant amount of data.

Let's start by getting statistics for the amount of bytes sent from an internal host to an external host:

```python
df[df['src_int'] & ~df['dst_int']]\
  .groupby('src_ip')\
  .payload.sum()\
  .pipe(lambda x: x[x > 0])\
  .sort_values(ascending=False).head()
```

Output:
```
src_ip
120.18.53.84     24795472
120.18.215.38    11774718
120.18.231.65     8447175
120.18.15.208     8047102
120.18.138.6      7872829
Name: payload, dtype: int64
```

We can see that `120.18.53.84` is sending lots of data compared to the rest of the hosts.

Who's he talking to outside the corporation?

```python
df[(df['src_ip'] == "120.18.53.84") & ~df['dst_int']][['dst_ip']].drop_duplicates("dst_ip").reset_index()
```

Output:
```
      index    dst_ip
0     1896     8.8.8.8
```

`8.8.8.8` is a DNS server. Can this be [DNS Tunneling](https://www.paloaltonetworks.com/cyberpedia/what-is-dns-tunneling)?

Let's find the IPs that are talking to our suspect within the network:

```python
df[
   df['src_int'] & df['dst_int'] &
   (df['dst_ip'] == '120.18.53.84')
].sort_values(by="payload",ascending=False).drop_duplicates('src_ip')[["src_ip","payload"]].head(20)
```

Output:
```

    src_ip     payload
17372911     120.18.212.32      450
5344676      120.18.252.47      450
8272425      120.18.179.89      449
337259       120.18.7.139       449
22821948     120.18.85.233      448
16895409     120.18.70.11       447
20668204     120.18.56.143      446
12639850     120.18.24.144      442
6659136      120.18.88.125      440
21480711     120.18.109.224     435
14669212     120.18.107.52      250
15580481     120.18.31.136      250
7722221      120.18.212.63      250
15651258     120.18.64.46       250
1333362      120.18.64.93       250
7723299      120.18.172.179     250
9784261      120.18.23.132      250
140100       120.18.39.9        250
15586973     120.18.92.185      250
1335597      120.18.115.245     250
```

We can see that at some point the number drops from `435` to `250`. Let's round up everyone with a high number of requests:

```python
df[
   df['src_int'] & df['dst_int'] &
   (df['dst_ip'] == '120.18.53.84')
].sort_values(by="payload",ascending=False).drop_duplicates('src_ip')[["src_ip","payload"]].head(10)["src_ip"].to_csv("flag_b.csv", index = False, header = ["ip"])
```

Output:
```
ip
120.18.212.32
120.18.252.47
120.18.179.89
120.18.7.139
120.18.85.233
120.18.70.11
120.18.56.143
120.18.24.144
120.18.88.125
120.18.109.224
```

And we get our second flag:
```
Congratulations! You achieved the goal. Here is your flag: 

flag{sjgdgyweo3983483742lk2$%^344}
```
