# nyanyanyan
Grab Bag, 50 points

## Description:

Nyayanayanayanayanayanayanayan

ssh ctf@138.247.13.114

## Solution:

Connecting to the SSH server displayed an ASCII-art animation of the Nyan cat (see [here](https://nyancat.dakko.us/)).

In order to get the flag, we needed to redirect the output to a file:
```console
ssh ctf@138.247.13.114 > out.txt
```

The output contains many characters needed for displaying the ASCII art:
```
root@kali:/media/sf_CTFs/mitre/cat# cat out.txt | xxd -g 1 | head
00000000: 6b 4e 79 61 6e 79 61 6e 79 61 6e 79 61 6e 79 61  kNyanyanyanyanya
00000010: 6e 79 61 6e 79 61 2e 2e 2e 1b 5c 1b 5d 31 3b 4e  nyanya....\.]1;N
00000020: 79 61 6e 79 61 6e 79 61 6e 79 61 6e 79 61 6e 79  yanyanyanyanyany
00000030: 61 6e 79 61 2e 2e 2e 07 1b 5d 32 3b 4e 79 61 6e  anya.....]2;Nyan
00000040: 79 61 6e 79 61 6e 79 61 6e 79 61 6e 79 61 6e 79  yanyanyanyanyany
00000050: 61 2e 2e 2e 07 1b 5b 48 1b 5b 32 4a 1b 5b 3f 32  a.....[H.[2J.[?2
00000060: 35 6c 1b 5b 48 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a  5l.[H...........
00000070: 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a  ................
00000080: 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a  ................
00000090: 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a  ................
```

We can use `cat -v` to turn non-printable characters into a printable form (otherwise the output is messed up), and `tr -d ' '` to delete spaces.

We get:
```console
root@kali:/media/sf_CTFs/mitre/cat# cat -v out.txt | grep MCA | tr -d ' '
Flagis:MCA{Airadaepohh8Sha}^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^[[1;37mYouhavenyanedfor0seconds!^[[J^[[0m^[[H
Flagis:MCA{Airadaepohh8Sha}^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^[[1;37mYouhavenyanedfor0seconds!^[[J^[[0m^[[H
Flagis:MCA{Airadaepohh8Sha}^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^[[1;37mYouhavenyanedfor0seconds!^[[J^[[0m^[[H
Flagis:MCA{Airadaepohh8Sha}^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^[[1;37mYouhavenyanedfor0seconds!^[[J^[[0m^[[H
Flagis:MCA{Airadaepohh8Sha}^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^[[1;37mYouhavenyanedfor1seconds!^[[J^[[0m^[[H
Flagis:MCA{Airadaepohh8Sha}^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^H^[[1;37mYouhavenyanedfor1seconds!^[[J^[[0m^[[H
```

The flag is: MCA{Airadaepohh8Sha}