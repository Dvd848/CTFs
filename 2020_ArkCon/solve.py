from z3 import *

s = Solver()

KEY_LEN = 41

key = [BitVec("{}".format(i), 32) for i in range(KEY_LEN)]

for i in range(KEY_LEN):
    s.add(key[i] >= ord('!'))
    s.add(key[i] <= ord('~'))

local288 = key[0]
local289 = key[1]
local300 = key[2]
local311 = key[3]
local322 = key[4]
local324 = key[5]
local325 = key[6]
local326 = key[7]
local327 = key[8]
local328 = key[9]
local290 = key[10]
local291 = key[11]
local292 = key[12]
local293 = key[13]
local294 = key[14]
local295 = key[15]
local296 = key[16]
local297 = key[17]
local298 = key[18]
local299 = key[19]
local301 = key[20]
local302 = key[21]
local303 = key[22]
local304 = key[23]
local305 = key[24]
local306 = key[25]
local307 = key[26]
local308 = key[27]
local309 = key[28]
local310 = key[29]
local312 = key[30]
local313 = key[31]
local314 = key[32]
local315 = key[33]
local316 = key[34]
local317 = key[35]
local318 = key[36]
local319 = key[37]
local320 = key[38]
local321 = key[39]
local323 = key[40]

def abs(x):
    return If(x < 0, 0 - x, x)

local452 = 0
local41 = local288
local370 = local41 ^ -1
local206 = local370 & 208
local42 = local288
local207 = local42 & -209
local411 = local206 | local207
local453 = local411 - 184
local329 = abs(local453)

local77 = local452
local165 = local77 + local329
local452 = local165
local88 = local289
local387 = local88 ^ -1
local266 = local387 & 146
local99 = local289
local271 = local99 & -147
local444 = local266 | local271
local487 = local444 - 230
local364 = 	abs(local487)

local110 = local452
local202 = local110 + local364
local452 = local202
local121 = local300
local409 = local121 ^ -1
local208 = local409 & 100
local132 = local300
local213 = local132 & -101
local415 = local208 | local213
local458 = local415 - 16
local335 = 	abs(local458)

local143 = local452
local173 = local143 + local335
local452 = local173
local154 = local311
local380 = local154 ^ -1
local230 = local380 & 203
local43 = local311
local235 = local43 & -204
local426 = local230 | local235
local469 = local426 - 187
local346 = 	abs(local469)

local54 = local452
local184 = local54 + local346
local452 = local184
local65 = local322
local392 = local65 ^ -1
local252 = local392 & 3
local70 = local322
local257 = local70 & -4
local437 = local252 | local257
local480 = local437 - 57
local357 = 	abs(local480)

local71 = local452
local194 = local71 + local357
local452 = local194
local72 = local324
local400 = local72 ^ -1
local267 = local400 & 61
local73 = local324
local268 = local73 & -62
local441 = local267 | local268
local483 = local441 - 18
local359 = 	abs(local483)

local74 = local452
local195 = local74 + local359
local452 = local195
local75 = local325
local401 = local75 ^ -1
local269 = local401 & 3
local76 = local325
local270 = local76 & -4
local442 = local269 | local270
local484 = local442 - 44
local360 = 	abs(local484)

local78 = local452
local196 = local78 + local360
local452 = local196
local79 = local326
local402 = local79 ^ -1
local272 = local402 & 166
local80 = local326
local273 = local80 & -167
local443 = local272 | local273
local485 = local443 - 206
local361 = 	abs(local485)

local81 = local452
local197 = local81 + local361
local452 = local197
local82 = local327
local403 = local82 ^ -1
local274 = local403 & 42
local83 = local327
local275 = local83 & -43
local445 = local274 | local275
local486 = local445 - 25
local362 = 	abs(local486)

local84 = local452
local198 = local84 + local362
local452 = local198
local85 = local328
local404 = local85 ^ -1
local276 = local404 & 151
local86 = local328
local277 = local86 & -152
local446 = local276 | local277
local488 = local446 - 244
local363 = 	abs(local488)

local87 = local452
local199 = local87 + local363
local452 = local199
local89 = local290
local405 = local89 ^ -1
local278 = local405 & 250
local90 = local290
local279 = local90 & -251
local447 = local278 | local279
local489 = local447 - 145
local365 = 	abs(local489)

local91 = local452
local200 = local91 + local365
local452 = local200
local92 = local291
local406 = local92 ^ -1
local280 = local406 & 209
local93 = local291
local281 = local93 & -210
local448 = local280 | local281
local490 = local448 - 252
local366 = 	abs(local490)

local94 = local452
local201 = local94 + local366
local452 = local201
local95 = local292
local407 = local95 ^ -1
local282 = local407 & 249
local96 = local292
local283 = local96 & -250
local449 = local282 | local283
local491 = local449 - 128
local367 = 	abs(local491)

local97 = local452
local203 = local97 + local367
local452 = local203
local98 = local293
local408 = local98 ^ -1
local284 = local408 & 138
local100 = local293
local285 = local100 & -139
local450 = local284 | local285
local492 = local450 - 185
local368 = 	abs(local492)

local101 = local452
local204 = local101 + local368
local452 = local204
local102 = local294
local410 = local102 ^ -1
local286 = local410 & 163
local103 = local294
local287 = local103 & -164
local451 = local286 | local287
local493 = local451 - 151
local369 = 	abs(local493)

local104 = local452
local205 = local104 + local369
local452 = local205
local105 = local295
local371 = local105 ^ -1
local209 = local371 & 144
local106 = local295
local210 = local106 & -145
local412 = local209 | local210
local454 = local412 - 248
local330 = 	abs(local454)

local107 = local452
local166 = local107 + local330
local452 = local166
local108 = local296
local372 = local108 ^ -1
local211 = local372 & 150
local109 = local296
local212 = local109 & -151
local413 = local211 | local212
local455 = local413 - 187
local331 = 	abs(local455)

local111 = local452
local167 = local111 + local331
local452 = local167
local112 = local297
local373 = local112 ^ -1
local214 = local373 & 66
local113 = local297
local215 = local113 & -67
local414 = local214 | local215
local456 = local414 - 116
local332 = 	abs(local456)

local114 = local452
local168 = local114 + local332
local452 = local168
local115 = local298
local374 = local115 ^ -1
local216 = local374 & 244
local116 = local298
local217 = local116 & -245
local416 = local216 | local217
local457 = local416 - 197
local333 = 	abs(local457)

local117 = local452
local169 = local117 + local333
local452 = local169
local118 = local299
local375 = local118 ^ -1
local218 = local375 & 58
local119 = local299
local219 = local119 & -59
local417 = local218 | local219
local459 = local417 - 87
local334 = 	abs(local459)

local120 = local452
local170 = local120 + local334
local452 = local170
local122 = local301
local376 = local122 ^ -1
local220 = local376 & 21
local123 = local301
local221 = local123 & -22
local418 = local220 | local221
local460 = local418 - 120
local336 = 	abs(local460)

local124 = local452
local171 = local124 + local336
local452 = local171
local125 = local302
local377 = local125 ^ -1
local222 = local377 & 150
local126 = local302
local223 = local126 & -151
local419 = local222 | local223
local461 = local419 - 165
local337 = 	abs(local461)

local127 = local452
local172 = local127 + local337
local452 = local172
local128 = local303
local378 = local128 ^ -1
local224 = local378 & 211
local129 = local303
local225 = local129 & -212
local420 = local224 | local225
local462 = local420 - 254
local338 = 	abs(local462)

local130 = local452
local174 = local130 + local338
local452 = local174
local131 = local304
local379 = local131 ^ -1
local226 = local379 & 229
local133 = local304
local227 = local133 & -230
local421 = local226 | local227
local463 = local421 - 208
local339 = 	abs(local463)

local134 = local452
local175 = local134 + local339
local452 = local175
local135 = local305
local381 = local135 ^ -1
local228 = local381 & 187
local136 = local305
local229 = local136 & -188
local422 = local228 | local229
local464 = local422 - 139
local340 = 	abs(local464)

local137 = local452
local176 = local137 + local340
local452 = local176
local138 = local306
local382 = local138 ^ -1
local231 = local382 & 163
local139 = local306
local232 = local139 & -164
local423 = local231 | local232
local465 = local423 - 206
local341 = 	abs(local465)

local140 = local452
local177 = local140 + local341
local452 = local177
local141 = local307
local383 = local141 ^ -1
local233 = local383 & 107
local142 = local307
local234 = local142 & -108
local424 = local233 | local234
local466 = local424 - 88
local342 = 	abs(local466)

local144 = local452
local178 = local144 + local342
local452 = local178
local145 = local308
local384 = local145 ^ -1
local236 = local384 & 174
local146 = local308
local237 = local146 & -175
local425 = local236 | local237
local467 = local425 - 131
local343 = 	abs(local467)

local147 = local452
local179 = local147 + local343
local452 = local179
local148 = local309
local385 = local148 ^ -1
local238 = local385 & 106
local149 = local309
local239 = local149 & -107
local427 = local238 | local239
local468 = local427 - 9
local344 = 	abs(local468)

local150 = local452
local180 = local150 + local344
local452 = local180
local151 = local310
local386 = local151 ^ -1
local240 = local386 & 34
local152 = local310
local241 = local152 & -35
local428 = local240 | local241
local470 = local428 - 18
local345 = 	abs(local470)

local153 = local452
local181 = local153 + local345
local452 = local181
local155 = local312
local388 = local155 ^ -1
local242 = local388 & 186
local156 = local312
local243 = local156 & -187
local429 = local242 | local243
local471 = local429 - 138
local347 = 	abs(local471)

local157 = local452
local182 = local157 + local347
local452 = local182
local158 = local313
local389 = local158 ^ -1
local244 = local389 & 225
local159 = local313
local245 = local159 & -226
local430 = local244 | local245
local472 = local430 - 141
local348 = 	abs(local472)

local160 = local452
local183 = local160 + local348
local452 = local183
local161 = local314
local390 = local161 ^ -1
local246 = local390 & 8
local162 = local314
local247 = local162 & -9
local431 = local246 | local247
local473 = local431 - 37
local349 = 	abs(local473)

local163 = local452
local185 = local163 + local349
local452 = local185
local164 = local315
local391 = local164 ^ -1
local248 = local391 & 12
local44 = local315
local249 = local44 & -13
local432 = local248 | local249
local474 = local432 - 57
local350 = 	abs(local474)

local45 = local452
local186 = local45 + local350
local452 = local186
local46 = local316
local393 = local46 ^ -1
local250 = local393 & 62
local47 = local316
local251 = local47 & -63
local433 = local250 | local251
local475 = local433 - 73
local351 = 	abs(local475)

local48 = local452
local187 = local48 + local351
local452 = local187
local49 = local317
local394 = local49 ^ -1
local253 = local394 & 228
local50 = local317
local254 = local50 & -229
local434 = local253 | local254
local476 = local434 - 208
local352 = 	abs(local476)

local51 = local452
local188 = local51 + local352
local452 = local188
local52 = local318
local395 = local52 ^ -1
local255 = local395 & 85
local53 = local318
local256 = local53 & -86
local435 = local255 | local256
local477 = local435 - 99
local353 = 	abs(local477)

local55 = local452
local189 = local55 + local353
local452 = local189
local56 = local319
local396 = local56 ^ -1
local258 = local396 & 110
local57 = local319
local259 = local57 & -111
local436 = local258 | local259
local478 = local436 - 64
local354 = 	abs(local478)

local58 = local452
local190 = local58 + local354
local452 = local190
local59 = local320
local397 = local59 ^ -1
local260 = local397 & 79
local60 = local320
local261 = local60 & -80
local438 = local260 | local261
local479 = local438 - 55
local355 = 	abs(local479)

local61 = local452
local191 = local61 + local355
local452 = local191
local62 = local321
local398 = local62 ^ -1
local262 = local398 & 227
local63 = local321
local263 = local63 & -228
local439 = local262 | local263
local481 = local439 - 154
local356 = 	abs(local481)

local64 = local452
local192 = local64 + local356
local452 = local192
local66 = local323
local399 = local66 ^ -1
local264 = local399 & 14
local67 = local323
local265 = local67 & -15
local440 = local264 | local265
local482 = local440 - 116
local358 = 	abs(local482)

local68 = local452
local193 = local68 + local358
local452 = local193
local69 = local452
#global$0 = local495
s.add(local69 == 0)

if s.check() == sat:
    model = s.model()
    res = ""
    for i in range(KEY_LEN):
        res += chr(model[key[i]].as_long())
    print(res)