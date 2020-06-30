# TheTarget
* Category: Misc.
* 100 Points
* Solved by the JCTF Team

## Description
> EZ-PZ-LEMON-SQUIZZY

A file was attached.

## Solution

Let's check the attached file:

```console
root@kali:/media/sf_CTFs/bsidestlv/TheTarget/TheTarget# file TheTarget
TheTarget: Extensible storage engine DataBase, version 0x620, checksum 0xbb95e8e9, page size 8192, Windows version 6.3
```

It's a 75MB [ESE DB](https://en.wikipedia.org/wiki/Extensible_Storage_Engine). 

We can use [libesedb](https://github.com/libyal/libesedb) to work with it. We just need to download the latest version from github, and call `./configure` and `make` to built it from sources. (On Windows, we can use [ESEDatabaseView](https://www.nirsoft.net/utils/ese_database_view.html)).

First, we can check what are the tables contained within the DB:
```console
root@kali:/media/sf_CTFs/bsidestlv/TheTarget/TheTarget# ./libesedb-20200418/esedbtools/esedbinfo TheTarget | grep "Table: "
Table: 1                        MSysObjects (2)
Table: 2                        MSysObjectsShadow (3)
Table: 3                        MSysObjids (6)
Table: 4                        MSysLocales (7)
Table: 5                        datatable (8)
Table: 6                        hiddentable (185)
Table: 7                        link_history_table (186)
Table: 8                        link_table (194)
Table: 9                        quota_rebuild_progress_table (207)
Table: 10                       quota_table (208)
Table: 11                       sdpropcounttable (209)
Table: 12                       sdproptable (210)
Table: 13                       sd_table (214)
```

`datatable` sounds the most interesting, so let's dump it:
```console
root@kali:/media/sf_CTFs/bsidestlv/TheTarget/TheTarget# ./libesedb-20200418/esedbtools/esedbexport -T datatable TheTarget
esedbexport 20200418

Opening file.
Database type: Unknown.
Exporting table 5 (datatable).
Export completed.
```

Now we just search for the flag format:
```console
root@kali:/media/sf_CTFs/bsidestlv/TheTarget/TheTarget/TheTarget.export# sed 's/\x09/\x00/g' datatable.4 | strings | egrep -i "BSidesTLV(2020)?{"
BSIDESTLV2020{ThisGoesDeeper}
```

(Note: For some reason, the export uses `0x09` as a filler character, we replace it with `0x00` for convenience).