# b00tl3gRSA3
Cryptography, 450 points

## Description:
> Why use p and q when I can use more?


## Solution: 

This challenge is similar to last year's [Super Safe RSA 3](/2018_picoCTF/Super%20Safe%20RSA%203.md).

Last year we used [an online service](https://www.alpertron.com.ar/ECM.HTM) to factorize `n`, and had to copy the result manually to our script. This year we'll provide a fully automated solution (using either the [FactorDB](http://factordb.com/api/index.php?query=123) [Python API](https://github.com/ryosan-470/factordb-pycli) or [YAFU](https://sourceforge.net/projects/yafu/)).

```python
from pwn import *
import functools
import gmpy2
import re

class FactorizationWrapper(object):

    def __init__(self, verbose = False):
        self.verbose = verbose
        self.factor_functions = []

    def _print(self, msg):
        if self.verbose:
            print msg

    @classmethod
    def _factordb_factor_list(cls, num):
        from factordb.factordb import FactorDB

        f = FactorDB(num)
        if f.connect().status_code != 200:
            raise Exception("Can't connect")
        status = f.get_status()
        if status == 'C':
            raise Exception("Composite, no factors known")
        elif status == 'P' or status == 'Prp':
            raise Exception("Prime or probably prime")
        elif status == 'CF':
            raise Exception("Composite, factors known")
        elif status == 'FF':
            # Composite, fully factored
            result = f.get_factor_list()
            return result
        else:
            raise Exception("Unexpected status: '{}'".format(status))

    @classmethod
    def _yafu_factor_list(cls, num):
        import subprocess
        try:
            with open('/dev/null') as devnull:
                output = subprocess.check_output(
                    ["yafu", "factor({})".format(num)], stderr=devnull)

                result = []
                
                if "***factors found***" not in output:
                    raise Exception("Factors not found")

                for line in output.splitlines():
                    match = re.search(r'^P[\d]+\s=\s([\d]+)$', line)
                    if match:
                        result.append(int(match.group(1)))
                    elif re.match(r'^C[\d]+\s=\s([\d]+)$', line):
                        raise Exception("Composite number found")
                return result
        except:
            raise

    def get_factor_list(self, num):
        
        functions = {"FactorDB": self._factordb_factor_list, "YAFU": self._yafu_factor_list}
        for engine, function in functions.items():
            try:
                res = function(num)
                self._print("{} found {} factors".format(engine, len(res)))
                assert(num == reduce(lambda x, y: x * y, res))
                return res
            except:
                self._print("{} failed, error = {}".format(engine, str(e)))    

        raise Exception("Can't find factor list")


args = {}
r = remote("2019shell1.picoctf.com", 45122)
output = r.recvall()
for line in output.split("\n"):
    line = line.rstrip()
    if line != "":
        var_name, var_value = line.split(": ")
        args[var_name] = int(var_value)
        log.info("{}: {}".format(var_name, var_value))

assert('c' in args)
assert('n' in args)
assert('e' in args)

factors = FactorizationWrapper().get_factor_list(args['n'])
log.info("factors: {}".format(factors))

phi_n = 1
for i in range(len(factors)):
    phi_n *= (factors[i] - 1)
log.info("phi_n: {}".format(phi_n))

d = gmpy2.invert(args['e'], phi_n)
log.info("d: {}".format(d))

plaintext = pow(args['c'], d, args['n'])
log.info("plaintext: {}".format(str(plaintext)))

plaintext_decoded = (format(plaintext, 'x')).decode("hex")

log.success("Flag: {}".format(plaintext_decoded))
```

Output:
```
root@kali:/media/sf_CTFs/pico/b00tl3gRSA3# python solve.py
[+] Opening connection to 2019shell1.picoctf.com on port 45122: Done
[+] Receiving all data: Done (703B)
[*] Closed connection to 2019shell1.picoctf.com port 45122
[*] c: 2119596483582558480583929659164309083296660442838854968468759928450353346297950871181143180716588952234498810113225169410718136242100409285815716755217693534767226132381024632868961033843749108040758183809500760771427677425657433392478512746836236199961443235703574582372712785398185681080705839658430626639368029475166150632444902109452965798
[*] n: 3672464541648239369227217909364198602719180450540051030110460920213922520849808202127087033512632253320345577574894749232930157121564026701599179431818977806524931182803454664976110867812150776157861188489120752987953472318021253967490594518158670003474778638352965323617178267413359412650997652695798964583048530279735817175448628179108047701
[*] e: 65537
[*] factors: [10438103437, 8876550257, 14571618619, 9420392389, 12231531233, 13428635837, 10300611119, 16377790381, 12181725953, 9537930719, 14189030617, 15192969613, 16684859311, 16368342083, 13620022499, 8597216993, 9652490711, 11484109213, 9005703103, 15153984289, 12174300283, 9225912887, 10553881621, 11444035081, 15366336439, 15079064467, 16647469627, 11875342261, 10673013397, 10998953323, 10597722019, 8621804209, 8697999641, 14833004983]
[*] phi_n: 3672464530906332488432082249165216851377847923651297590278024508373963363528659179894961129609641707593597322229333347449561947088457884177661713010595676672203892210858490176841374908093136240739934097469984024153766471267503685210882917632086523432305165922649096576946475880711321709723247437882748283364393023156793255184761188515840000000
[*] d: 204757394997203700394141149861142596928981435113322877075177404421906131350744169603982299580292518722965723414651022347386962397748220223464240037546982659569907413193721456676051450542019314641556970751717680459249930360124181237477549796720084175681570353866667666999747056900974556774474665273411389404664420199504441070618389350090473473
[*] plaintext: 13016382529449106065933618925167173598170118383294989999418819014052201555703933
[+] Flag: picoCTF{too_many_fact0rs_8024768}
```

Note: The prebuilt YAFU crashed with a `Segmentation Fault` on my machine (Kali), so I used a version compiled from the sources.
Applied the following patch to `factor_common.c` in order to have YAFU reach a deeper refactor depth (I suppose the alternative is to call the program recursively for each number that couldn't be factorized to primes within 3 levels):
```
1158c1158
< 					if (fobj->refactor_depth > 20)
---
> 					if (fobj->refactor_depth > 3)
```