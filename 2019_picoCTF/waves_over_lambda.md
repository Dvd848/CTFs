# waves over lambda
Cryptography, 300 points

## Description:
> We made alot of substitutions to encrypt this. Can you decrypt it?


## Solution: 

We connect to the service and receive the following output:

```console
root@kali:/media/sf_CTFs/pico/waves_over_lambda# nc 2019shell1.picoctf.com 37925
-------------------------------------------------------------------------------
eymxicfz tbib uz dyhi pvcx - pibghbmed_uz_e_ykbi_vcojac_ocizjlnyni
-------------------------------------------------------------------------------
tckumx tca zyob fuob cf od auznyzcv ltbm um vymaym, u tca kuzufba ftb jiufuzt ohzbho, cma ocab zbciet coymx ftb jyyqz cma ocnz um ftb vujicid ibxciaumx ficmzdvkcmuc; uf tca zfiheq ob ftcf zyob pyibqmylvbaxb yp ftb eyhmfid eyhva tciavd pcuv fy tckb zyob uonyifcmeb um abcvumx luft c myjvbocm yp ftcf eyhmfid. u puma ftcf ftb auzfiuef tb mcoba uz um ftb brfibob bczf yp ftb eyhmfid, whzf ym ftb jyiabiz yp ftibb zfcfbz, ficmzdvkcmuc, oyvackuc cma jhqykumc, um ftb ouazf yp ftb ecincftucm oyhmfcumz; ymb yp ftb luvabzf cma vbczf qmylm nyifuymz yp bhiynb. u lcz myf cjvb fy vuxtf ym cmd ocn yi lyiq xukumx ftb brcef vyecvufd yp ftb eczfvb aicehvc, cz ftbib cib my ocnz yp ftuz eyhmfid cz dbf fy eyoncib luft yhi ylm yiamcmeb zhikbd ocnz; jhf u pyhma ftcf juzfiufs, ftb nyzf fylm mcoba jd eyhmf aicehvc, uz c pcuivd lbvv-qmylm nvceb. u ztcvv bmfbi tbib zyob yp od myfbz, cz ftbd ocd ibpibzt od oboyid ltbm u fcvq ykbi od fickbvz luft oumc.
```

[This site](https://www.guballa.de/substitution-solver) can easily break the substitution cipher using frequency analysis.

The key:
```
abcdefghijklmnopqrstuvwxyz
cjeabpxtuwqvomyngizfhklrds
```

The result:

```
-------------------------------------------------------------------------------
congrats here is your flag - frequency_is_c_over_lambda_marsbwpopr
-------------------------------------------------------------------------------
having had some time at my disposal when in london, i had visited the british museum, and made search among the books and maps in the library regarding transylvania; it had struck me that some foreknowledge of the country could hardly fail to have some importance in dealing with a nobleman of that country. i find that the district he named is in the extreme east of the country, just on the borders of three states, transylvania, moldavia and bukovina, in the midst of the carpathian mountains; one of the wildest and least known portions of europe. i was not able to light on any map or work giving the exact locality of the castle dracula, as there are no maps of this country as yet to compare with our own ordnance survey maps; but i found that bistritz, the post town named by count dracula, is a fairly well-known place. i shall enter here some of my notes, as they may refresh my memory when i talk over my travels with mina.
```
