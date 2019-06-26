# Redis in the wild
* Category: OSINT
* 300 Points
* Solved by the JCTF Team

## Description
> In order to evaluate your OSINT skills, we hid a server on the internet!
> 
> All we share is the following things:
> 
>     Our server is located in Frankfurt and belongs to DigitalOcean.
>     Our server has been indexed in shodan.
>     Our server is running a Redis service that contains the flag by the key "flag"
> 
> Your mission is to figure out and find our server, and then submit the flag!

## Solution

This one was pretty simple.

Search [Shodan](https://www.shodan.io/) for '[org:"DigitalOcean" city:Frankfurt Redis flag"](https://www.shodan.io/search?query=org%3A%22DigitalOcean%22+city%3AFrankfurt+Redis+flag)'.

The only result is:
```
46.101.175.108

Property Name 	Value
area_code 	null
asn 	AS200130
city 	Frankfurt
country_code 	DE
country_code3 	DEU
country_name 	Germany
data.0._shodan.crawler 	545144fc95e7a7ef13ece5dbceb98ee386b37950
data.0._shodan.id 	null
data.0._shodan.module 	ssh
data.0._shodan.ptr 	True
data.0.cpe 	['cpe:/a:openbsd:openssh:7.6p1 Ubuntu-4ubuntu0.3']
data.0.data 	SSH-2.0-OpenSSH_7.6p1 Ubuntu-4ubuntu0.3 Key type: ssh-rsa Key: AAAAB3NzaC1yc2EAAAADAQABAAABAQC2d54zxb0xRv9HlqnMp1Qe/elxyhw9q7+jm/cEg2Fq+oQo hDKyrhRbXr+YOf/+QxeZdFNj6xHHRB9v+C2p9rArEI9k1QwTUgQFwhpqNGKLhDhdXWkxWoVmomRl elSVKK2pbq0URZlFscfgesZgUySoER8OXCJ2WS2mmY1CE0ho0u2MaATRk+7akTlGwn2RA/3x2gU0 wTPBOCQ8UyhHSEXiVbDWC0fXHkF8G6l18rXqKheHM913UY6AedHqhHpG0WRbOSt5p8J0pvDGOn5v teLhqfLz3WL6QBrQzzYSqLD9+qz6WJKIfUhkECLmPqwm4k5ZLly5ru2gA+v0aYls1HFB Fingerprint: 4e:9a:71:fc:a6:2b:4b:d5:ef:ec:cf:73:1d:02:e1:a2 Kex Algorithms: curve25519-sha256 curve25519-sha256@libssh.org ecdh-sha2-nistp256 ecdh-sha2-nistp384 ecdh-sha2-nistp521 diffie-hellman-group-exchange-sha256 diffie-hellman-group16-sha512 diffie-hellman-group18-sha512 diffie-hellman-group14-sha256 diffie-hellman-group14-sha1 Server Host Key Algorithms: ssh-rsa rsa-sha2-512 rsa-sha2-256 ecdsa-sha2-nistp256 ssh-ed25519 Encryption Algorithms: chacha20-poly1305@openssh.com aes128-ctr aes192-ctr aes256-ctr aes128-gcm@openssh.com aes256-gcm@openssh.com MAC Algorithms: umac-64-etm@openssh.com umac-128-etm@openssh.com hmac-sha2-256-etm@openssh.com hmac-sha2-512-etm@openssh.com hmac-sha1-etm@openssh.com umac-64@openssh.com umac-128@openssh.com hmac-sha2-256 hmac-sha2-512 hmac-sha1 Compression Algorithms: none zlib@openssh.com
data.0.domains 	[]
data.0.hash 	436255690
data.0.hostnames 	[]
data.0.info 	protocol 2.0
data.0.port 	22
data.0.product 	OpenSSH
data.0.ssh.cipher 	aes128-ctr
data.0.ssh.fingerprint 	4e:9a:71:fc:a6:2b:4b:d5:ef:ec:cf:73:1d:02:e1:a2
data.0.ssh.hassh 	b12d2871a1189eff20364cf5333619ee
data.0.ssh.kex.compression_algorithms 	['none', 'zlib@openssh.com']
data.0.ssh.kex.encryption_algorithms 	['chacha20-poly1305@openssh.com', 'aes128-ctr', 'aes192-ctr', 'aes256-ctr', 'aes128-gcm@openssh.com', 'aes256-gcm@openssh.com']
data.0.ssh.kex.kex_algorithms 	['curve25519-sha256', 'curve25519-sha256@libssh.org', 'ecdh-sha2-nistp256', 'ecdh-sha2-nistp384', 'ecdh-sha2-nistp521', 'diffie-hellman-group-exchange-sha256', 'diffie-hellman-group16-sha512', 'diffie-hellman-group18-sha512', 'diffie-hellman-group14-sha256', 'diffie-hellman-group14-sha1']
data.0.ssh.kex.kex_follows 	False
data.0.ssh.kex.languages 	['']
data.0.ssh.kex.mac_algorithms 	['umac-64-etm@openssh.com', 'umac-128-etm@openssh.com', 'hmac-sha2-256-etm@openssh.com', 'hmac-sha2-512-etm@openssh.com', 'hmac-sha1-etm@openssh.com', 'umac-64@openssh.com', 'umac-128@openssh.com', 'hmac-sha2-256', 'hmac-sha2-512', 'hmac-sha1']
data.0.ssh.kex.server_host_key_algorithms 	['ssh-rsa', 'rsa-sha2-512', 'rsa-sha2-256', 'ecdsa-sha2-nistp256', 'ssh-ed25519']
data.0.ssh.kex.unused 	0
data.0.ssh.key 	AAAAB3NzaC1yc2EAAAADAQABAAABAQC2d54zxb0xRv9HlqnMp1Qe/elxyhw9q7+jm/cEg2Fq+oQo hDKyrhRbXr+YOf/+QxeZdFNj6xHHRB9v+C2p9rArEI9k1QwTUgQFwhpqNGKLhDhdXWkxWoVmomRl elSVKK2pbq0URZlFscfgesZgUySoER8OXCJ2WS2mmY1CE0ho0u2MaATRk+7akTlGwn2RA/3x2gU0 wTPBOCQ8UyhHSEXiVbDWC0fXHkF8G6l18rXqKheHM913UY6AedHqhHpG0WRbOSt5p8J0pvDGOn5v teLhqfLz3WL6QBrQzzYSqLD9+qz6WJKIfUhkECLmPqwm4k5ZLly5ru2gA+v0aYls1HFB
data.0.ssh.mac 	hmac-sha2-256
data.0.ssh.type 	ssh-rsa
data.0.tags 	['cloud']
data.0.timestamp 	2019-06-18T18:19:00.323936
data.0.transport 	tcp
data.0.version 	7.6p1 Ubuntu-4ubuntu0.3
data.1._shodan.crawler 	d264629436af1b777b3b513ca6ed1404d7395d80
data.1._shodan.id 	f9d51810-0115-4ab3-9d9c-efa82ec28b47
data.1._shodan.module 	redis
data.1._shodan.ptr 	True
data.1.data 	# Server redis_version:5.0.5 redis_git_sha1:00000000 redis_git_dirty:0 redis_build_id:7983a619928f1f2d redis_mode:standalone os:Linux 4.15.0-50-generic x86_64 arch_bits:64 multiplexing_api:epoll atomicvar_api:atomic-builtin gcc_version:6.3.0 process_id:9 run_id:62dcdf7717bb79e873dd972f8b2a749bce0bbf57 tcp_port:6379 uptime_in_seconds:636175 uptime_in_days:7 hz:10 configured_hz:10 lru_clock:15941456 executable:/tmp/redis-server config_file: # Clients connected_clients:2 client_recent_max_input_buffer:2 client_recent_max_output_buffer:0 blocked_clients:0 # Memory used_memory:1923832 used_memory_human:1.83M used_memory_rss:5947392 used_memory_rss_human:5.67M used_memory_peak:1984640 used_memory_peak_human:1.89M used_memory_peak_perc:96.94% used_memory_overhead:1906568 used_memory_startup:791272 used_memory_dataset:17264 used_memory_dataset_perc:1.52% allocator_allocated:1893776 allocator_active:2064384 allocator_resident:4460544 total_system_memory:1033347072 total_system_memory_human:985.48M used_memory_lua:37888 used_memory_lua_human:37.00K used_memory_scripts:0 used_memory_scripts_human:0B number_of_cached_scripts:0 maxmemory:0 maxmemory_human:0B maxmemory_policy:noeviction allocator_frag_ratio:1.09 allocator_frag_bytes:170608 allocator_rss_ratio:2.16 allocator_rss_bytes:2396160 rss_overhead_ratio:1.33 rss_overhead_bytes:1486848 mem_fragmentation_ratio:3.20 mem_fragmentation_bytes:4086464 mem_not_counted_for_evict:0 mem_replication_backlog:1048576 mem_clients_slaves:0 mem_clients_normal:66616 mem_aof_buffer:0 mem_allocator:jemalloc-5.1.0 active_defrag_running:0 lazyfree_pending_objects:0 # Persistence loading:0 rdb_changes_since_last_save:0 rdb_bgsave_in_progress:0 rdb_last_save_time:1559364040 rdb_last_bgsave_status:ok rdb_last_bgsave_time_sec:-1 rdb_current_bgsave_time_sec:-1 rdb_last_cow_size:0 aof_enabled:0 aof_rewrite_in_progress:0 aof_rewrite_scheduled:0 aof_last_rewrite_time_sec:-1 aof_current_rewrite_time_sec:-1 aof_last_bgrewrite_status:ok aof_last_write_status:ok aof_last_cow_size:0 # Stats total_connections_received:90 total_commands_processed:63543 instantaneous_ops_per_sec:0 total_net_input_bytes:896638 total_net_output_bytes:24769302 instantaneous_input_kbps:0.00 instantaneous_output_kbps:0.05 rejected_connections:0 sync_full:0 sync_partial_ok:0 sync_partial_err:0 expired_keys:0 expired_stale_perc:0.00 expired_time_cap_reached_count:0 evicted_keys:0 keyspace_hits:4 keyspace_misses:0 pubsub_channels:0 pubsub_patterns:0 latest_fork_usec:0 migrate_cached_sockets:0 slave_expires_tracked_keys:0 active_defrag_hits:0 active_defrag_misses:0 active_defrag_key_hits:0 active_defrag_key_misses:0 # Replication role:slave master_host:redis-master master_port:6380 master_link_status:up master_last_io_seconds_ago:8 master_sync_in_progress:0 slave_repl_offset:887572 slave_priority:100 slave_read_only:1 connected_slaves:0 master_replid:cb2d2104e3a00cddb597c7076c3beec4022ff2f6 master_replid2:0000000000000000000000000000000000000000 master_repl_offset:887572 second_repl_offset:-1 repl_backlog_active:1 repl_backlog_size:1048576 repl_backlog_first_byte_offset:1 repl_backlog_histlen:887572 # CPU used_cpu_sys:562.952297 used_cpu_user:476.872706 used_cpu_sys_children:0.000000 used_cpu_user_children:0.000000 # Cluster cluster_enabled:0 # Keyspace db0:keys=1,expires=0,avg_ttl=0 # Keys flag # Connected Clients id=3 addr=172.18.0.2:6380 fd=9 name= age=636175 idle=8 flags=M db=0 sub=0 psub=0 multi=-1 qbuf=0 qbuf-free=0 obl=0 oll=0 omem=0 events=r cmd=ping id=93 addr=82.98.254.57:55094 fd=10 name= age=0 idle=0 flags=N db=0 sub=0 psub=0 multi=-1 qbuf=26 qbuf-free=32742 obl=0 oll=0 omem=0 events=r cmd=client
data.1.domains 	[]
data.1.hash 	601215482
data.1.hostnames 	[]
data.1.port 	6379
data.1.product 	Redis key-value store
data.1.redis.clients 	[{u'sub': 0, u'obl': 0, u'addr': [u'172.18.0.2', 6380], u'omem': 0, u'db': 0, u'age': 636175, u'cmd': u'ping', u'oll': 0, u'multi': -1, u'id': 3, u'qbuf': 0, u'psub': 0, u'fd': 9, u'qbuf-free': 0, u'flags': u'M', u'events': u'r', u'idle': 8, u'name': u''}, {u'sub': 0, u'obl': 0, u'addr': [u'82.98.254.57', 55094], u'omem': 0, u'db': 0, u'age': 0, u'cmd': u'client', u'oll': 0, u'multi': -1, u'id': 93, u'qbuf': 26, u'psub': 0, u'fd': 10, u'qbuf-free': 32742, u'flags': u'N', u'events': u'r', u'idle': 0, u'name': u''}]
data.1.redis.cluster.cluster_enabled 	0
data.1.redis.cpu.used_cpu_sys 	562.952297
data.1.redis.cpu.used_cpu_sys_children 	0.0
data.1.redis.cpu.used_cpu_user 	476.872706
data.1.redis.cpu.used_cpu_user_children 	0.0
data.1.redis.keys.data 	['flag']
data.1.redis.keys.more 	False
data.1.redis.keyspace.db0 	keys=1,expires=0,avg_ttl=0
data.1.redis.memory.active_defrag_running 	0
data.1.redis.memory.allocator_active 	2064384
data.1.redis.memory.allocator_allocated 	1893776
data.1.redis.memory.allocator_frag_bytes 	170608
data.1.redis.memory.allocator_frag_ratio 	1.09
data.1.redis.memory.allocator_resident 	4460544
data.1.redis.memory.allocator_rss_bytes 	2396160
data.1.redis.memory.allocator_rss_ratio 	2.16
data.1.redis.memory.lazyfree_pending_objects 	0
data.1.redis.memory.maxmemory 	0
data.1.redis.memory.maxmemory_human 	0B
data.1.redis.memory.maxmemory_policy 	noeviction
data.1.redis.memory.mem_allocator 	jemalloc-5.1.0
data.1.redis.memory.mem_aof_buffer 	0
data.1.redis.memory.mem_clients_normal 	66616
data.1.redis.memory.mem_clients_slaves 	0
data.1.redis.memory.mem_fragmentation_bytes 	4086464
data.1.redis.memory.mem_fragmentation_ratio 	3.2
data.1.redis.memory.mem_not_counted_for_evict 	0
data.1.redis.memory.mem_replication_backlog 	1048576
data.1.redis.memory.number_of_cached_scripts 	0
data.1.redis.memory.rss_overhead_bytes 	1486848
data.1.redis.memory.rss_overhead_ratio 	1.33
data.1.redis.memory.total_system_memory 	1033347072
data.1.redis.memory.total_system_memory_human 	985.48M
data.1.redis.memory.used_memory 	1923832
data.1.redis.memory.used_memory_dataset 	17264
data.1.redis.memory.used_memory_dataset_perc 	1.52%
data.1.redis.memory.used_memory_human 	1.83M
data.1.redis.memory.used_memory_lua 	37888
data.1.redis.memory.used_memory_lua_human 	37.00K
data.1.redis.memory.used_memory_overhead 	1906568
data.1.redis.memory.used_memory_peak 	1984640
data.1.redis.memory.used_memory_peak_human 	1.89M
data.1.redis.memory.used_memory_peak_perc 	96.94%
data.1.redis.memory.used_memory_rss 	5947392
data.1.redis.memory.used_memory_rss_human 	5.67M
data.1.redis.memory.used_memory_scripts 	0
data.1.redis.memory.used_memory_scripts_human 	0B
data.1.redis.memory.used_memory_startup 	791272
data.1.redis.persistence.aof_current_rewrite_time_sec 	-1
data.1.redis.persistence.aof_enabled 	0
data.1.redis.persistence.aof_last_bgrewrite_status 	ok
data.1.redis.persistence.aof_last_cow_size 	0
data.1.redis.persistence.aof_last_rewrite_time_sec 	-1
data.1.redis.persistence.aof_last_write_status 	ok
data.1.redis.persistence.aof_rewrite_in_progress 	0
data.1.redis.persistence.aof_rewrite_scheduled 	0
data.1.redis.persistence.loading 	0
data.1.redis.persistence.rdb_bgsave_in_progress 	0
data.1.redis.persistence.rdb_changes_since_last_save 	0
data.1.redis.persistence.rdb_current_bgsave_time_sec 	-1
data.1.redis.persistence.rdb_last_bgsave_status 	ok
data.1.redis.persistence.rdb_last_bgsave_time_sec 	-1
data.1.redis.persistence.rdb_last_cow_size 	0
data.1.redis.persistence.rdb_last_save_time 	1559364040
data.1.redis.replication.connected_slaves 	0
data.1.redis.replication.master_host 	redis-master
data.1.redis.replication.master_last_io_seconds_ago 	8
data.1.redis.replication.master_link_status 	up
data.1.redis.replication.master_port 	6380
data.1.redis.replication.master_repl_offset 	887572
data.1.redis.replication.master_replid 	cb2d2104e3a00cddb597c7076c3beec4022ff2f6
data.1.redis.replication.master_replid2 	0
data.1.redis.replication.master_sync_in_progress 	0
data.1.redis.replication.repl_backlog_active 	1
data.1.redis.replication.repl_backlog_first_byte_offset 	1
data.1.redis.replication.repl_backlog_histlen 	887572
data.1.redis.replication.repl_backlog_size 	1048576
data.1.redis.replication.role 	slave
data.1.redis.replication.second_repl_offset 	-1
data.1.redis.replication.slave_priority 	100
data.1.redis.replication.slave_read_only 	1
data.1.redis.replication.slave_repl_offset 	887572
data.1.redis.server.arch_bits 	64
data.1.redis.server.atomicvar_api 	atomic-builtin
data.1.redis.server.config_file 	
data.1.redis.server.configured_hz 	10
data.1.redis.server.executable 	/tmp/redis-server
data.1.redis.server.gcc_version 	6.3.0
data.1.redis.server.hz 	10
data.1.redis.server.lru_clock 	15941456
data.1.redis.server.multiplexing_api 	epoll
data.1.redis.server.os 	Linux 4.15.0-50-generic x86_64
data.1.redis.server.process_id 	9
data.1.redis.server.redis_build_id 	7983a619928f1f2d
data.1.redis.server.redis_git_dirty 	0
data.1.redis.server.redis_git_sha1 	0
data.1.redis.server.redis_mode 	standalone
data.1.redis.server.redis_version 	5.0.5
data.1.redis.server.run_id 	62dcdf7717bb79e873dd972f8b2a749bce0bbf57
data.1.redis.server.tcp_port 	6379
data.1.redis.server.uptime_in_days 	7
data.1.redis.server.uptime_in_seconds 	636175
data.1.redis.stats.active_defrag_hits 	0
data.1.redis.stats.active_defrag_key_hits 	0
data.1.redis.stats.active_defrag_key_misses 	0
data.1.redis.stats.active_defrag_misses 	0
data.1.redis.stats.evicted_keys 	0
data.1.redis.stats.expired_keys 	0
data.1.redis.stats.expired_stale_perc 	0.0
data.1.redis.stats.expired_time_cap_reached_count 	0
data.1.redis.stats.instantaneous_input_kbps 	0.0
data.1.redis.stats.instantaneous_ops_per_sec 	0
data.1.redis.stats.instantaneous_output_kbps 	0.05
data.1.redis.stats.keyspace_hits 	4
data.1.redis.stats.keyspace_misses 	0
data.1.redis.stats.latest_fork_usec 	0
data.1.redis.stats.migrate_cached_sockets 	0
data.1.redis.stats.pubsub_channels 	0
data.1.redis.stats.pubsub_patterns 	0
data.1.redis.stats.rejected_connections 	0
data.1.redis.stats.slave_expires_tracked_keys 	0
data.1.redis.stats.sync_full 	0
data.1.redis.stats.sync_partial_err 	0
data.1.redis.stats.sync_partial_ok 	0
data.1.redis.stats.total_commands_processed 	63543
data.1.redis.stats.total_connections_received 	90
data.1.redis.stats.total_net_input_bytes 	896638
data.1.redis.stats.total_net_output_bytes 	24769302
data.1.tags 	['cloud']
data.1.timestamp 	2019-06-02T03:15:28.641722
data.1.transport 	tcp
data.1.version 	5.0.5
dma_code 	null
hostnames 	[]
ip 	778415980
ip_str 	46.101.175.108
isp 	DigitalOcean
last_update 	2019-06-18T18:19:00.323936
latitude 	50.1155
longitude 	8.6842
org 	DigitalOcean
os 	null
ports 	[22, 6379]
postal_code 	60313
region_code 	05
tags 	['cloud']
```

Connect to the server and request the flag:
```console
root@kali:/media/sf_CTFs/bsidestlv/Redis_in_the_wild# nc 46.101.175.108 6379
get flag
$26
BSidesTLV{L00ksL1k30s1nt!}
```