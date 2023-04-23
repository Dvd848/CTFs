
# Wild Beasts

## Description

> > “if you will not let My people go, behold, I will send swarms of flies on you and your servants, on your people and into your houses. The houses of the Egyptians shall be full of swarms of flies, and also the ground on which they stand. And in that day I will set apart the land of Goshen, in which My people dwell, that no swarms of flies shall be there, in order that you may know that I am the Lord in the midst of the land’” (Exodus 8:21-22).
> 
> A swarm of wild beasts came across the whole land of Egypt, roaming the ports, fileds and territories! Can you uncover the hidden sequence and gain a deeper insight into the lives of these wild beasts that once dominated the Egyptian landscape?
> 
> Note: This is not a reverse challenage

A Windows executable was attached.

## Solution

Running the executable in the console just seems to "hang" with no output. We can use a utility such as "Process Monitor" to see what the executable is doing. 
Eventually we'll see many logs that seem related to network traffic. We can launch "Wireshark" and wait for some outgoing packets, but nothing seems to be happening so the program might
be listening on the network instead.

We can use `netstat` to verify this:

```console
E:\CTFs\basmach\wild_beasts>tasklist | findstr script.exe
script.exe                    6812 Console                    1      4,056 K
script.exe                    2980 Console                    1     12,300 K

E:\CTFs\basmach\wild_beasts>netstat -ano | findstr 2980
  TCP    127.0.0.1:65           0.0.0.0:0              LISTENING       2980
```

So we see that it's listening on port `65`. But what if we call netstat again shortly after?

```console
E:\CTFs\basmach\wild_beasts>netstat -ano | findstr 2980
  TCP    127.0.0.1:125          0.0.0.0:0              LISTENING       2980
```

Now it's listening on port `125`. One last try:

```console
E:\CTFs\basmach\wild_beasts>netstat -ano | findstr 2980
  TCP    127.0.0.1:123          0.0.0.0:0              LISTENING       2980
```

This time it's `123`. The program keeps changing ports. Let's write a short Powershell script to try and track it:

```powershell
PS E:\CTFs\basmach\wild_beasts> while ($true) {
>>     Get-Process -Name script | Select-Object -ExpandProperty Id | ForEach-Object { Get-NetTCPConnection -OwningProcess $_ -ErrorAction SilentlyContinue } | Select-Object LocalPort,State
>>     Start-Sleep -Seconds 1
>> }

LocalPort  State
---------  -----
       83 Listen
       77 Listen
       67 Listen
       72 Listen
      123 Listen
       83 Listen
       65 Listen
       77 Listen
       65 Listen
       84 Listen
       79 Listen
      125 Listen
       66 Listen
       83 Listen
       77 Listen
       67 Listen
       72 Listen
      123 Listen
       83 Listen
       65 Listen
```

Looks like there's a sequence that repeats itself, and moreover - looks like the ports are in the ASCII range! Let's try to modify the script to decode them:

```powershell
PS E:\CTFs\basmach\wild_beasts> $flag = ""
>> while ($true) {
>>     Get-Process -Name script -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Id | ForEach-Object { Get-NetTCPConnection -OwningProcess $_ -ErrorAction SilentlyContinue } | Select-Object LocalPort,State | ForEach-Object {
>>         $localPort = $_.LocalPort
>>         $state = $_.State
>>         $char = [char][int]$localPort
>>         Write-Host "$localPort $state $char"
>>         $flag += $char
>>         if ($char -eq '}') {
>>             break
>>         }
>>     }
>>     Start-Sleep -Seconds 1
>> }
>> Write-Host "Flag: $flag"
66 Listen B
83 Listen S
77 Listen M
67 Listen C
72 Listen H
123 Listen {
83 Listen S
65 Listen A
77 Listen M
65 Listen A
76 Listen L
84 Listen T
79 Listen O
125 Listen }
Flag: BSMCH{SAMALTO}
```