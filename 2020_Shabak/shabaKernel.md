# shabaKernel
Category: Reversing, 350 points


## Description

> This binary is not an elf. So what is it?
> 
> Load the .ko file and find out...
> 
> Use the image:
> 
> http://uec-images.ubuntu.com/releases/focal/release-20201210/
> 
> Good luck!

Two binary files were attached.

## Solution:


Let's check the binary files we've received:
```console
root@kali:/media/sf_CTFs/shabak/shabaKernel# file app.magen
app.magen: data
root@kali:/media/sf_CTFs/shabak/shabaKernel# file loader.ko
loader.ko: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), BuildID[sha1]=dd09ff411b9362d587cf727385d072f0f3a03565, not stripped
```

So we have at least one ELF - let's check it in Ghidra:

```c
undefined8 init_module(void)
{
  __fentry__();
  __register_binfmt(magen_fmt,0);
  return 0;
}


void cleanup_module(void)
{
  unregister_binfmt(magen_fmt);
  return;
}

// ...

undefined8 load_magen_binary(long param_1)
{
  long lVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  ulong uVar7;
  uint uVar8;
  long in_GS_OFFSET;
  uint local_44;
  long local_40;
  long local_38;
  
  __fentry__();
  local_38 = *(long *)(in_GS_OFFSET + 0x28);
  local_40 = 0;
  if (8 < *(uint *)(param_1 + 0x8d)) {
    set_binfmt(magen_fmt);
    flush_old_exec(param_1);
    setup_new_exec(param_1);
    uVar6 = 0x7ffffffff000;
    if (((**(ulong **)(&current_task + in_GS_OFFSET) & 0x20000000) != 0) &&
       (uVar6 = 0xc0000000,
       (*(byte *)((long)*(ulong **)(&current_task + in_GS_OFFSET) + 0x87b) & 8) == 0)) {
      uVar6 = 0xffffe000;
    }
    setup_arg_pages(param_1,uVar6,0);
    uVar3 = vm_mmap(0,0,0x2000,3,2,0);
    *(ulong *)(param_1 + 0x18) = uVar3;
    if (uVar3 < 0xfffffffffffff001) {
      lVar4 = __kmalloc((ulong)*(uint *)(param_1 + 0x8d),0xcc0);
      if (lVar4 != 0) {
        puVar5 = (undefined8 *)kmem_cache_alloc_trace(_DAT_001010d0,0xcc0,0x12);
        if (puVar5 != (undefined8 *)0x0) {
          uVar6 = build_key((char *)(param_1 + 0x91),puVar5,(ulong)(*(int *)(param_1 + 0x8d) - 9),
                            (int *)&local_44);
          if ((int)uVar6 != -1) {
            local_40 = (ulong)local_44 + 9;
            uVar8 = (-9 - local_44) + *(int *)(param_1 + 0x8d);
            uVar7 = (ulong)uVar8;
            uVar3 = kernel_read(*(undefined8 *)(param_1 + 0x30),lVar4,uVar7,&local_40);
            if (uVar7 == uVar3) {
              stateless_rc4(puVar5,0x12,lVar4,(ulong)uVar8);
              uVar3 = vm_mmap(0,0,uVar7,7,2,0);
              if (uVar3 < 0xfffffffffffff001) {
                if ((int)uVar8 < 0) {
                  do {
                    invalidInstructionException();
                  } while( true );
                }
                __check_object_size(lVar4,uVar7,1);
                _copy_to_user(uVar3,lVar4,uVar7);
                lVar1 = *(long *)(&current_task + in_GS_OFFSET);
                lVar2 = *(long *)(lVar1 + 0x810);
                *(ulong *)(lVar2 + 0x100) = uVar3;
                *(long *)(lVar2 + 0x108) = uVar7 + uVar3;
                uVar6 = *(undefined8 *)(param_1 + 0x18);
                *(undefined8 *)(lVar2 + 0x110) = 0;
                *(undefined8 *)(lVar2 + 0x130) = uVar6;
                finalize_exec(param_1);
                start_thread(*(long *)(lVar1 + 0x18) + 0x3f58,uVar3,*(undefined8 *)(param_1 + 0x18))
                ;
                kfree(puVar5);
                kfree(lVar4);
                uVar6 = 0;
                goto LAB_00100610;
              }
            }
          }
          kfree(puVar5);
        }
        kfree(lVar4);
      }
      vm_munmap(*(undefined8 *)(param_1 + 0x18),0x2000);
    }
  }
  uVar6 = 0xfffffff8;
LAB_00100610:
  if (local_38 != *(long *)(in_GS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar6;
}
```

We can see that this loader registers the `magen` format, then starts loading it somehow while calling functions such as `build_key` and `stateless_rc4`. Maybe we're supposed to reverse the loading process in order to recover the key and decrypt the app. We'll take a shortcut to the flag instead. But first, we need to setup the Ubuntu image we were requested to use.

The link points to an `Ubuntu Cloud` image which requires some special setup to access. We chose to download the `*.vmdk` hard-drive file and plug it into VirtualBox. To work with it, there are two things that need to be done after creating a default virtual machine. First - we need to change the network adapter to "Bridged" in order to be able to SSH into the machine. In addition, we need to create an ISO image which will provision the machine and setup credentials. This is done with:

```console
root@kali:/media/sf_CTFs/shabak/shabaKernel/config# cat user-data
#cloud-config

password: 123456
ssh_pwauth: True
chpasswd: { expire: False }

ssh_authorized_keys:
  - ssh-rsa AAAAB[...]FF root@kali # <-- Replace with your own key from ~/.ssh/id_rsa.pub

root@kali:/media/sf_CTFs/shabak/shabaKernel/config# cat meta-data
instance-id: cloud-host-0
local-hostname: cloudhost
root@kali:/media/sf_CTFs/shabak/shabaKernel/config#  genisoimage -output out.iso  -volid cidata -joliet -rock user-data meta-data
I: -input-charset not specified, using utf-8 (detected in locale settings)
Total translation table size: 0
Total rockridge attributes bytes: 331
Total directory bytes: 0
Path table size(bytes): 10
Max brk space used 0
183 extents written (0 MB)
```

We insert the ISO into the virtual machine and boot it. We can then either log into it with `ubuntu:123456` or SSH into it:

```console
root@kali:/media/sf_CTFs/shabak/shabaKernel# ssh ubuntu@192.168.1.115
Welcome to Ubuntu 20.04.1 LTS (GNU/Linux 5.4.0-54-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

 System information disabled due to load higher than 1.0


1 update can be installed immediately.
0 of these updates are security updates.
To see these additional updates run: apt list --upgradable


The list of available updates is more than a week old.
To check for new updates run: sudo apt update

Last login: Tue Jan 19 22:10:32 2021 from 192.168.1.108
ubuntu@cloudhost:~$
```

Now, to our plan. Instead of reversing the program, we'll launch it and dump its memory, hoping that the flag will be there. We'll use a Python script for that:

```python
#!/usr/bin/env python

# from https://stackoverflow.com/a/23001686/1935787

import re
import sys

def print_memory_of_pid(pid, only_writable=True):
    """
    Run as root, take an integer PID and return the contents of memory to STDOUT
    """
    memory_permissions = 'rw' if only_writable else 'r-'
    sys.stderr.write("PID = %d" % pid)
    with open("/proc/%d/maps" % pid, 'r') as maps_file,   \
         open("/proc/%d/mem" % pid, 'rb', 0) as mem_file, \
         open("memory.bin", "wb") as output:
            for line in maps_file.readlines():  # for each mapped region
                m = re.match(r'([0-9A-Fa-f]+)-([0-9A-Fa-f]+) ([-r][-w])', line)
                if m.group(3) == memory_permissions:
                    sys.stderr.write("\nOK : \n" + line+"\n")
                    start = int(m.group(1), 16)
                    if start > 0xFFFFFFFFFFFF:
                        continue
                    end = int(m.group(2), 16)
                    sys.stderr.write( "start = " + str(start) + "\n")
                    mem_file.seek(start)  # seek to region start
                    chunk = mem_file.read(end - start)  # read region contents
                    output.write(chunk)
                else:
                    sys.stderr.write("\nPASS : \n" + line+"\n")

if __name__ == '__main__': # Execute this code when run from the commandline.
    try:
        assert len(sys.argv) == 2, "Provide exactly 1 PID (process ID)"
        pid = int(sys.argv[1])
        print_memory_of_pid(pid)
    except (AssertionError, ValueError) as e:
        print ("Please provide 1 PID as a command-line argument.")
        print ("You entered: %s" % ' '.join(sys.argv))
        raise e
```

We copy what we need to the virtual machine:

```console
root@kali:/media/sf_CTFs/shabak/shabaKernel# scp loader.ko app.magen ubuntu@192.168.1.115:/home/ubuntu/magen
loader.ko                                                                                                                           100% 8272     3.7MB/s   00:00
app.magen                                                                                                                           100% 1561     1.0MB/s   00:00
```

Run it:

```console
ubuntu@cloudhost:~/magen$ sudo python3 dump_mem.py 1280
PID = 1280
OK :
7ffff7ffa000-7ffff7ffc000 rw-p 00000000 00:00 0

start = 140737354113024

OK :
7ffff7ffc000-7ffff7ffd000 rwxp 00000000 00:00 0                          [stack]

start = 140737354121216

OK :
7ffff7ffd000-7ffff7fff000 rw-p 00000000 00:00 0                          [stack]

start = 140737354125312

OK :
7ffffffde000-7ffffffff000 rw-p 00000000 00:00 0

start = 140737488216064

PASS :
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
```

And we got the flag:
```console
ubuntu@cloudhost:~/magen$ strings memory.bin | grep -i flag
FLAG{magen_velo_yeraELF}
This is not the right flag, Buddy
Good work my friend, go submit the flag
```

