# Where am I
Misc, 100 points

## Description:

> What is the name of the city where the server for tamuctf.com is located?
> 
> (Not in standard gigem{flag} format)

## Solution:

We can use a service such as `iplocation.com` to determine the location of the server.

```console
root@kali:/media/sf_CTFs/tamu/Where_am_I# curl "https://iplocation.com/"   --data "ip=$(host tamuctf.com  | awk '/has address/ { print $4 }')"
{"city":"Boardman","company":"Amazon.com","continent_code":"NA","country_code":"US","country_name":"United States","found":1,"ip":"52.33.57.247","ip_header":"IP address","isp":"Amazon.com","lat":45.8491,"lng":-119.7143,"metro_code":810,"postal_code":"97818","region":"OR","region_name":"Oregon","time_zone":"America\/Los_Angeles"}
```