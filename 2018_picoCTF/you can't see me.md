# you can't see me
General Skills, 200 points

## Description:
> '...reading transmission... Y.O.U. .C.A.N.'.T. .S.E.E. .M.E. ...transmission ended...' Maybe something lies in /problems/you-can-t-see-me_4_8bd1412e56df49a3c3757ebeb7ead77f. 



## Solution:

Listing the given directory shows one file:

```console
user@pico-2018-shell-3:/problems/you-can-t-see-me_4_8bd1412e56df49a3c3757ebeb7ead77f$ ls -la
total 60
drwxr-xr-x   2 root       root        4096 Sep 28 08:35 .
-rw-rw-r--   1 hacksports hacksports    57 Sep 28 08:35 .
drwxr-x--x 571 root       root       53248 Sep 30 03:52 ..
```

However, we can't `cat` it easily due to its name:
```console
user@pico-2018-shell-3:/problems/you-can-t-see-me_4_8bd1412e56df49a3c3757ebeb7ead77f$ cat .
cat: .: Is a directory
```

`grep`-ing an empty string worked instead:
```console
user@pico-2018-shell-3:/problems/you-can-t-see-me_4_8bd1412e56df49a3c3757ebeb7ead77f$ grep -r "" .
./.  :picoCTF{j0hn_c3na_paparapaaaaaaa_paparapaaaaaa_22f627d9}
```

Pressing [tab] after `cat` in order to autocomplete the file name would also do the trick:
```console
user@pico-2018-shell-3:/problems/you-can-t-see-me_4_8bd1412e56df49a3c3757ebeb7ead77f$ cat .\ \
picoCTF{j0hn_c3na_paparapaaaaaaa_paparapaaaaaa_22f627d9}
```