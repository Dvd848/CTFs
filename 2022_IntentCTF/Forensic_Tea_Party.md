# Forensic Tea Party
* Category: Forensics, Reverse Engineering
* 400 Points
* Solved by the JCTF Team

## Description

> Analyze the memory dump to find any suspicious processes and find the flag.

A memory dump was attached.

## Solution

Let's start analyzing our memory dump using `volatility`. We start by extracting the process tree:

```console
┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ alias vol="python3 ~/utils/forensics/volatility3/vol.py"

┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ export memdump=/media/sf_CTFs/intent/Forensic_Tea_Party/Windows10x64_AliceInWonderland-b4365e16.vmem/Windows10x64_AliceInWonderland-b4365e16.vmem

┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ vol -f $memdump windows.pstree
Volatility 3 Framework 2.4.1
Progress:  100.00               PDB scanning finished
PID     PPID    ImageFileName   Offset(V)       Threads Handles SessionId       Wow64   CreateTime      ExitTime

4       0       System  0xa08ce048c440  123     -       N/A     False   2022-09-02 22:01:11.000000      N/A
* 280   4       smss.exe        0xa08ce1772040  3       -       N/A     False   2022-09-02 22:01:11.000000      N/A
** 476  280     smss.exe        0xa08ce1bb4580  0       -       1       False   2022-09-02 22:01:12.000000      2022-09-02 22:01:12.000000
*** 576 476     winlogon.exe    0xa08ce1ae6480  6       -       1       False   2022-09-02 22:01:12.000000      N/A
**** 760        576     fontdrvhost.ex  0xa08ce1a46080  5       -       1       False   2022-09-02 22:01:13.000000      N/A
**** 2460       576     userinit.exe    0xa08ce284b340  0       -       1       False   2022-09-02 22:01:15.000000      2022-09-02 22:01:37.000000
***** 2700      2460    explorer.exe    0xa08ce2853580  77      -       1       False   2022-09-02 22:01:15.000000      N/A
****** 3232     2700    MSASCuiL.exe    0xa08ce2db8580  4       -       1       False   2022-09-02 22:01:29.000000      N/A
****** 5912     2700    vmtoolsd.exe    0xa08ce304e080  8       -       1       False   2022-09-02 22:01:29.000000      N/A
****** 4484     2700    TeaParty.exe    0xa08ce26aa080  9       -       1       True    2022-09-02 22:26:18.000000      N/A
****** 6100     2700    OneDrive.exe    0xa08ce2eb1080  23      -       1       False   2022-09-02 22:01:31.000000      N/A
******* 6164    6100    Microsoft.Shar  0xa08ce30c1580  0       -       1       False   2022-09-02 22:01:32.000000      2022-09-02 22:01:43.000000
**** 960        576     dwm.exe 0xa08ce20d4080  11      -       1       False   2022-09-02 22:01:13.000000      N/A
*** 492 476     csrss.exe       0xa08ce1fbb080  12      -       1       False   2022-09-02 22:01:12.000000      N/A
* 1356  4       MemCompression  0xa08ce23e9280  18      -       N/A     False   2022-09-02 22:01:13.000000      N/A
```

We can see an interesting process labeled `TeaParty`, let's dump it:

```console
┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ vol -f $memdump windows.pslist --pid 4484 --dump
Volatility 3 Framework 2.4.1
Progress:  100.00               PDB scanning finished
PID     PPID    ImageFileName   Offset(V)       Threads Handles SessionId       Wow64   CreateTime      ExitTime        File output

4484    2700    TeaParty.exe    0xa08ce26aa080  9       -       1       True    2022-09-02 22:26:18.000000      N/A     pid.4484.0x420000.dmp

┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ file pid.4484.0x420000.dmp
pid.4484.0x420000.dmp: PE32 executable (GUI) Intel 80386 Mono/.Net assembly, for MS Windows
```

It's a .NET executable, we can decompile it with dotPeek. 

<details>
  <summary>Full source code</summary>

  ### Some Code
  ```csharp
// Decompiled with JetBrains decompiler
// Type: TeaParty.TeaParty
// Assembly: TeaParty, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: C5228E40-9DB9-4613-9B66-CF1FA1BC1DA7
// Assembly location: E:\CTFs\intent\FTP\pid.4484.0x420000.exe

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace TeaParty
{
  public class TeaParty : Form
  {
    private const int WH_KEYBOARD_LL = 13;
    private const int WM_KEYDOWN = 256;
    private static TeaParty.TeaParty.HookProc hookProc = new TeaParty.TeaParty.HookProc(TeaParty.TeaParty.HookCallback);
    private static IntPtr hookId = IntPtr.Zero;
    private static int passcodeLength = 17;
    private static List<string> buffers = new List<string>();
    private const uint SERVICE_NO_CHANGE = 4294967295;
    private const uint SC_MANAGER_ALL_ACCESS = 983103;
    private const uint SERVICE_ALL_ACCESS = 983551;
    private IContainer components = (IContainer) null;
    private PictureBox pictureBox1;

    [DllImport("user32.dll")]
    private static extern IntPtr SetWindowsHookEx(
      int idHook,
      TeaParty.TeaParty.HookProc lpfn,
      IntPtr hMod,
      uint dwThreadId);

    [DllImport("user32.dll")]
    private static extern bool UnhookWindowsHookEx(IntPtr hhk);

    [DllImport("user32.dll")]
    private static extern IntPtr CallNextHookEx(
      IntPtr hhk,
      int nCode,
      IntPtr wParam,
      IntPtr lParam);

    [DllImport("kernel32.dll")]
    private static extern IntPtr GetModuleHandle(string lpModuleName);

    [DllImport("kernel32.dll")]
    private static extern bool DeviceIoControl(
      IntPtr hDevice,
      uint dwIoControlCode,
      IntPtr lpInBuffer,
      uint nInBufferSize,
      IntPtr lpOutBuffer,
      uint nOutBufferSize,
      out int lpBytesReturned,
      IntPtr lpOverlapped);

    [DllImport("kernel32.dll")]
    private static extern IntPtr CreateFileA(
      string lpFileName,
      uint dwDesiredAccess,
      uint dwSharedMode,
      IntPtr lpSecurityAttributes,
      uint dwCreationDisposition,
      uint dwFlagsAndAttributes,
      IntPtr hTemplate);

    [DllImport("kernel32.dll")]
    private static extern bool CloseHandle(IntPtr hHandle);

    [DllImport("kernel32.dll")]
    private static extern bool IsDebuggerPresent();

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    private static extern IntPtr OpenSCManager(
      string lpMachineName,
      string lpDatabaseName,
      uint dwDesiredAccess);

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    private static extern IntPtr OpenService(
      IntPtr hSCManager,
      string lpServiceName,
      uint dwDesiredAccess);

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    private static extern IntPtr CreateServiceW(
      IntPtr hSCManager,
      string lpServiceName,
      string lpDisplayName,
      uint dwDesiredAccess,
      uint dwServiceType,
      uint dwStartType,
      uint dwErrorControl,
      string lpBinaryPathName,
      string lpLoadOrderGroup,
      IntPtr lpdwTagId,
      string lpDependencies,
      string lpServiceStartName,
      string lpPassword);

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool QueryServiceConfig(
      IntPtr hService,
      IntPtr lpServiceConfig,
      uint cbBufSize,
      out uint pcbBytesNeeded);

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool ChangeServiceConfig(
      IntPtr hService,
      uint dwServiceType,
      uint dwStartType,
      uint dwErrorControl,
      string lpBinaryPathName,
      string lpLoadOrderGroup,
      IntPtr lpdwTagId,
      string lpDependencies,
      string lpServiceStartName,
      string lpPassword,
      string lpDisplayName);

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool CloseServiceHandle(IntPtr hSCObject);

    [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    private static extern bool StartServiceW(
      IntPtr hService,
      uint dwNumServiceArgs,
      IntPtr lpServiceArgVectors);

    [DllImport("kernel32.dll")]
    private static extern uint GetLastError();

    private static string GetHash1()
    {
      IntPtr fileA = TeaParty.TeaParty.CreateFileA("\\\\.\\TeaParty", 1073741824U, 2U, IntPtr.Zero, 3U, 0U, IntPtr.Zero);
      IntPtr num = Marshal.AllocHGlobal(32);
      int lpBytesReturned = 0;
      TeaParty.TeaParty.DeviceIoControl(fileA, 2236424U, IntPtr.Zero, 0U, num, 32U, out lpBytesReturned, IntPtr.Zero);
      byte[] numArray = new byte[32];
      Marshal.Copy(num, numArray, 0, 32);
      Marshal.FreeHGlobal(num);
      TeaParty.TeaParty.CloseHandle(fileA);
      return Encoding.ASCII.GetString(numArray);
    }

    private static bool CheckDebugging() => Environment.ProcessorCount < 2 || (TeaParty.TeaParty.IsDebuggerPresent() || Debugger.IsAttached);

    private static string GetHash2()
    {
      if (TeaParty.TeaParty.CheckDebugging())
        Environment.FailFast("Suspecting Anti-Analysis Environment");
      IntPtr fileA = TeaParty.TeaParty.CreateFileA("\\\\.\\TeaParty", 1073741824U, 2U, IntPtr.Zero, 3U, 0U, IntPtr.Zero);
      IntPtr num = Marshal.AllocHGlobal(32);
      int lpBytesReturned = 0;
      TeaParty.TeaParty.DeviceIoControl(fileA, 2236428U, IntPtr.Zero, 0U, num, 32U, out lpBytesReturned, IntPtr.Zero);
      byte[] numArray = new byte[32];
      Marshal.Copy(num, numArray, 0, 32);
      Marshal.FreeHGlobal(num);
      TeaParty.TeaParty.CloseHandle(fileA);
      return Encoding.ASCII.GetString(numArray);
    }

    private static string GetFlag(string passcode)
    {
      IntPtr fileA = TeaParty.TeaParty.CreateFileA("\\\\.\\TeaParty", 1073741824U, 2U, IntPtr.Zero, 3U, 0U, IntPtr.Zero);
      IntPtr num1 = Marshal.AllocHGlobal(512);
      int lpBytesReturned = 0;
      IntPtr num2 = Marshal.AllocHGlobal(passcode.Length + 1);
      Marshal.Copy(Encoding.ASCII.GetBytes(passcode + "\0"), 0, num2, passcode.Length + 1);
      bool flag = TeaParty.TeaParty.DeviceIoControl(fileA, 2236416U, num2, (uint) (passcode.Length + 1), IntPtr.Zero, 0U, out lpBytesReturned, IntPtr.Zero);
      flag = TeaParty.TeaParty.DeviceIoControl(fileA, 2236420U, IntPtr.Zero, 0U, num1, 512U, out lpBytesReturned, IntPtr.Zero);
      byte[] numArray = new byte[lpBytesReturned];
      Marshal.Copy(num1, numArray, 0, lpBytesReturned);
      int num3 = (int) MessageBox.Show(Encoding.ASCII.GetString(numArray));
      Marshal.FreeHGlobal(num1);
      Marshal.FreeHGlobal(num2);
      TeaParty.TeaParty.CloseHandle(fileA);
      return Encoding.ASCII.GetString(numArray);
    }

    private static string CalculateMd5HexDigest(string data)
    {
      using (MD5 md5 = MD5.Create())
        return BitConverter.ToString(md5.ComputeHash(Encoding.ASCII.GetBytes(data))).Replace("-", string.Empty).ToLower();
    }

    private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
    {
      if (nCode >= 0 && wParam == (IntPtr) 256)
      {
        int num1 = Marshal.ReadInt32(lParam);
        for (int index = 0; index < TeaParty.TeaParty.passcodeLength; ++index)
          TeaParty.TeaParty.buffers[index] += ((char) num1).ToString();
        if (TeaParty.TeaParty.CalculateMd5HexDigest(TeaParty.TeaParty.buffers.Last<string>()) == TeaParty.TeaParty.GetHash1() || TeaParty.TeaParty.CalculateMd5HexDigest(TeaParty.TeaParty.buffers.Last<string>()) == TeaParty.TeaParty.GetHash2())
        {
          TeaParty.TeaParty.UnhookWindowsHookEx(TeaParty.TeaParty.hookId);
          int num2 = (int) MessageBox.Show("You Got the FLAG!");
          TeaParty.TeaParty.GetFlag(TeaParty.TeaParty.buffers.Last<string>().ToLower());
          Application.Exit();
        }
        TeaParty.TeaParty.buffers.RemoveAt(TeaParty.TeaParty.buffers.Count - 1);
        TeaParty.TeaParty.buffers.Insert(0, "");
      }
      return TeaParty.TeaParty.CallNextHookEx(TeaParty.TeaParty.hookId, nCode, wParam, lParam);
    }

    private static IntPtr SetHook(TeaParty.TeaParty.HookProc hookProc)
    {
      IntPtr moduleHandle = TeaParty.TeaParty.GetModuleHandle(Process.GetCurrentProcess().MainModule.ModuleName);
      return TeaParty.TeaParty.SetWindowsHookEx(13, hookProc, moduleHandle, 0U);
    }

    public TeaParty() => this.InitializeComponent();

    public static bool ResourceExists(string resourceName) => ((IEnumerable<string>) Assembly.GetCallingAssembly().GetManifestResourceNames()).Contains<string>(resourceName);

    public static byte[] ReadResource(string resourceName)
    {
      byte[] buffer = (byte[]) null;
      try
      {
        using (Stream manifestResourceStream = Assembly.GetCallingAssembly().GetManifestResourceStream(resourceName))
        {
          buffer = new byte[(int) manifestResourceStream.Length];
          manifestResourceStream.Read(buffer, 0, (int) manifestResourceStream.Length);
        }
      }
      catch (NullReferenceException ex)
      {
        throw ex;
      }
      return buffer;
    }

    private static byte[] GetResource(string resourceName)
    {
      resourceName = Assembly.GetCallingAssembly().GetName().Name + "." + resourceName;
      return TeaParty.TeaParty.ResourceExists(resourceName) ? TeaParty.TeaParty.ReadResource(resourceName) : (byte[]) null;
    }

    private static byte[] decodeResource(byte[] data)
    {
      byte[] numArray = new byte[data.Length];
      for (int index = 0; index < data.Length; ++index)
        numArray[index] = (byte) ((uint) data[index] ^ 170U);
      return numArray;
    }

    private static void InstallDriver(string driverName, string driverPath)
    {
      IntPtr num1 = IntPtr.Zero;
      byte[] resource = TeaParty.TeaParty.GetResource(driverName + ".sys");
      if (resource != null)
      {
        byte[] bytes = TeaParty.TeaParty.decodeResource(resource);
        if (!File.Exists(driverPath))
        {
          if (!Directory.Exists(Path.GetDirectoryName(driverPath)))
            Directory.CreateDirectory(Path.GetDirectoryName(driverPath));
          File.WriteAllBytes(driverPath, bytes);
        }
      }
      IntPtr num2 = TeaParty.TeaParty.OpenSCManager((string) null, (string) null, 983103U);
      if (num2 != IntPtr.Zero)
      {
        num1 = TeaParty.TeaParty.OpenService(num2, driverName, 983551U);
        if (!(num1 != IntPtr.Zero))
          num1 = TeaParty.TeaParty.CreateServiceW(num2, nameof (TeaParty), "AliceInWonderlandTeaParty", 983103U, 1U, 3U, 1U, driverPath, (string) null, IntPtr.Zero, string.Empty, (string) null, string.Empty);
      }
      TeaParty.TeaParty.StartServiceW(num1, 0U, IntPtr.Zero);
      TeaParty.TeaParty.CloseServiceHandle(num1);
      TeaParty.TeaParty.CloseServiceHandle(num2);
    }

    private void Form1_Load(object sender, EventArgs e)
    {
      TeaParty.TeaParty.InstallDriver(nameof (TeaParty), "C:\\Program Files\\AliceInWonderlandTeaParty\\TeaParty.sys");
      if (TeaParty.TeaParty.CheckDebugging())
        Environment.FailFast("Suspecting Anti-Analysis Environment");
      TeaParty.TeaParty.hookId = TeaParty.TeaParty.SetHook(TeaParty.TeaParty.hookProc);
      for (int index = 0; index < TeaParty.TeaParty.passcodeLength; ++index)
        TeaParty.TeaParty.buffers.Add("");
    }

    private void Form1_Close(object sender, FormClosingEventArgs e) => TeaParty.TeaParty.UnhookWindowsHookEx(TeaParty.TeaParty.hookId);

    protected override void Dispose(bool disposing)
    {
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
      ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof (TeaParty.TeaParty));
      this.pictureBox1 = new PictureBox();
      ((ISupportInitialize) this.pictureBox1).BeginInit();
      this.SuspendLayout();
      this.pictureBox1.Image = (Image) componentResourceManager.GetObject("pictureBox1.Image");
      this.pictureBox1.Location = new Point(111, 54);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new Size(253, 199);
      this.pictureBox1.TabIndex = 0;
      this.pictureBox1.TabStop = false;
      this.AutoScaleDimensions = new SizeF(12f, 25f);
      this.AutoScaleMode = AutoScaleMode.Font;
      this.ClientSize = new Size(529, 320);
      this.Controls.Add((Control) this.pictureBox1);
      this.Margin = new Padding(6);
      this.Name = nameof (TeaParty);
      this.Text = "DownTheRabbitHole";
      this.FormClosing += new FormClosingEventHandler(this.Form1_Close);
      this.Load += new EventHandler(this.Form1_Load);
      ((ISupportInitialize) this.pictureBox1).EndInit();
      this.ResumeLayout(false);
    }

    private delegate IntPtr HookProc(int nCode, IntPtr wParam, IntPtr lParam);

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    private struct QUERY_SERVICE_CONFIG
    {
      public uint dwServiceType;
      public uint dwStartType;
      public uint dwErrorControl;
      public string lpBinaryPathName;
      public string lpLoadOrderGroup;
      public uint dwTagId;
      public string lpDependencies;
      public string lpServiceStartName;
      public string lpDisplayName;
    }
  }
}

  ```
</details>

The initialization method does two things:

```csharp
private void Form1_Load(object sender, EventArgs e)
{
    TeaParty.TeaParty.InstallDriver(nameof (TeaParty), "C:\\Program Files\\AliceInWonderlandTeaParty\\TeaParty.sys");
    if (TeaParty.TeaParty.CheckDebugging())
    Environment.FailFast("Suspecting Anti-Analysis Environment");
    TeaParty.TeaParty.hookId = TeaParty.TeaParty.SetHook(TeaParty.TeaParty.hookProc);
    for (int index = 0; index < TeaParty.TeaParty.passcodeLength; ++index)
    TeaParty.TeaParty.buffers.Add("");
}
```

The first thing the program does is install some driver. We can find it in the memory dump:

```console
┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ vol -f $memdump windows.modules | grep -i tea
0xa08ce3586a80.00xf80944e00000  0x7000anTeaParty.sys    \??\C:\Program Files\AliceInWonderlandTeaParty\TeaParty.sys     Disabled

┌──(user@kali)-[/media/sf_CTFs/intent/Forensic_Tea_Party/Windows10x64_AliceInWonderland-b4365e16.vmem]
└─$ vol -f $memdump windows.filescan | grep -i tea
0xa08ce28d2960.0\Windows\Prefetch\TEAPARTY.EXE-A85CB48A.pf      216
0xa08ce2930820  \Users\TheMadHatter\Desktop\TeaParty.exe        216
0xa08ce30512e0  \Users\TheMadHatter\Desktop\TeaParty.exe        216
0xa08ce34c31c0  \Users\TheMadHatter\Desktop\TeaParty.exe        216
0xa08ce38c74b0  \Program Files\AliceInWonderlandTeaParty\TeaParty.sys   216
```

Let's dump it (`moddump` is missing on latest volatility?):

```console
┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ vol -f $memdump windows.dumpfiles --virtaddr 0xa08ce38c74b0
Volatility 3 Framework 2.4.1
Progress:  100.00               PDB scanning finished
Cache   FileObject      FileName        Result

DataSectionObject       0xa08ce38c74b0  TeaParty.sys    file.0xa08ce38c74b0.0xa08ce2ebde50.DataSectionObject.TeaParty.sys.dat
ImageSectionObject      0xa08ce38c74b0  TeaParty.sys    file.0xa08ce38c74b0.0xa08ce3537de0.ImageSectionObject.TeaParty.sys.img
```

Using Ghidra, we can see that the driver sets up some handlers during initialization:

```c
undefined8 driver_init(longlong param_1)

{
  undefined local_28 [16];
  undefined local_18 [16];
  
  DbgPrint("Hello World!\n");
  RtlInitUnicodeString(local_28,L"\\Device\\TeaParty");
  RtlInitUnicodeString(local_18,L"\\DosDevices\\TeaParty");
  IoCreateDevice(param_1,0,local_28,0x22,0,0,&DAT_00013050);
  IoCreateSymbolicLink(local_18,local_28);
  *(code **)(param_1 + 0x70) = empty_handler;
  *(code **)(param_1 + 0x80) = empty_handler;
  *(code **)(param_1 + 0x88) = empty_handler;
  *(code **)(param_1 + 0x90) = empty_handler;
  *(code **)(param_1 + 0xe0) = message_handler;
  *(code **)(param_1 + 0x68) = unload_handler;
  return 0;
}
```

We'll return to what it does during runtime later. Back to the initialization method of the `.NET` application. The second thing it does is to setup some hook for `WH_KEYBOARD_LL` (`13`):

```csharp
private static IntPtr HookCallback(int nCode, IntPtr wParam, IntPtr lParam)
{
    if (nCode >= 0 && wParam == (IntPtr) 256)
    {
        int num1 = Marshal.ReadInt32(lParam);
        for (int index = 0; index < TeaParty.TeaParty.passcodeLength; ++index)
            TeaParty.TeaParty.buffers[index] += ((char) num1).ToString();
        if (TeaParty.TeaParty.CalculateMd5HexDigest(TeaParty.TeaParty.buffers.Last<string>()) == TeaParty.TeaParty.GetHash1() || TeaParty.TeaParty.CalculateMd5HexDigest(TeaParty.TeaParty.buffers.Last<string>()) == TeaParty.TeaParty.GetHash2())
        {
            TeaParty.TeaParty.UnhookWindowsHookEx(TeaParty.TeaParty.hookId);
            int num2 = (int) MessageBox.Show("You Got the FLAG!");
            TeaParty.TeaParty.GetFlag(TeaParty.TeaParty.buffers.Last<string>().ToLower());
            Application.Exit();
        }
        TeaParty.TeaParty.buffers.RemoveAt(TeaParty.TeaParty.buffers.Count - 1);
        TeaParty.TeaParty.buffers.Insert(0, "");
    }
    return TeaParty.TeaParty.CallNextHookEx(TeaParty.TeaParty.hookId, nCode, wParam, lParam);
}
```

If this is a `WM_KEYDOWN` (`256`) event, the program appends the key code to all 17 entries of `buffers`. In other words, `buffers.Last()` contains at every point the last `17` characters typed on the keyboard (assuming that at least 17 characters were already typed). At this point, the program takes the last 17 characters typed, calculates an MD5 hash on them and compares the result to the hash received from `GetHash1()` and `GetHash2()`. If one of them matches - we get the flag!

```csharp
private static string GetHash1()
    {
      IntPtr fileA = TeaParty.TeaParty.CreateFileA("\\\\.\\TeaParty", 1073741824U, 2U, IntPtr.Zero, 3U, 0U, IntPtr.Zero);
      IntPtr num = Marshal.AllocHGlobal(32);
      int lpBytesReturned = 0;
      TeaParty.TeaParty.DeviceIoControl(fileA, 2236424U, IntPtr.Zero, 0U, num, 32U, out lpBytesReturned, IntPtr.Zero);
      byte[] numArray = new byte[32];
      Marshal.Copy(num, numArray, 0, 32);
      Marshal.FreeHGlobal(num);
      TeaParty.TeaParty.CloseHandle(fileA);
      return Encoding.ASCII.GetString(numArray);
    }

    private static bool CheckDebugging() => Environment.ProcessorCount < 2 || (TeaParty.TeaParty.IsDebuggerPresent() || Debugger.IsAttached);

    private static string GetHash2()
    {
      if (TeaParty.TeaParty.CheckDebugging())
        Environment.FailFast("Suspecting Anti-Analysis Environment");
      IntPtr fileA = TeaParty.TeaParty.CreateFileA("\\\\.\\TeaParty", 1073741824U, 2U, IntPtr.Zero, 3U, 0U, IntPtr.Zero);
      IntPtr num = Marshal.AllocHGlobal(32);
      int lpBytesReturned = 0;
      TeaParty.TeaParty.DeviceIoControl(fileA, 2236428U, IntPtr.Zero, 0U, num, 32U, out lpBytesReturned, IntPtr.Zero);
      byte[] numArray = new byte[32];
      Marshal.Copy(num, numArray, 0, 32);
      Marshal.FreeHGlobal(num);
      TeaParty.TeaParty.CloseHandle(fileA);
      return Encoding.ASCII.GetString(numArray);
    }
```

The `GetHashX` methods get the hash from the `TeaParty` driver that we saw earlier. The following function handles it:

```c
void message_handler(undefined8 param_1,longlong param_2)

{
  int iVar1;
  undefined4 *puVar2;
  void *_Src;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint hash_idx;
  size_t _Size;
  longlong lVar6;
  undefined auStack152 [32];
  undefined4 reg_key_out [2];
  undefined local_70 [8];
  undefined4 uStack104;
  undefined4 uStack100;
  undefined local_60 [8];
  undefined4 uStack88;
  undefined4 uStack84;
  undefined local_50 [8];
  undefined4 uStack72;
  undefined4 uStack68;
  undefined local_40 [8];
  undefined4 uStack56;
  undefined4 uStack52;
  undefined4 reg_key;
  ulonglong local_10;
  uint i;
  undefined (*p_flag) [16];
  
  local_10 = DAT_00013000 ^ (ulonglong)auStack152;
  lVar6 = *(longlong *)(param_2 + 0xb8);
  reg_key_out[0] = 0;
  _local_50 = ZEXT816(0);
  _local_40 = ZEXT816(0);
  _local_70 = ZEXT816(0);
  _local_60 = ZEXT816(0);
  read_reg_key(L"\\Registry\\Machine\\SOFTWARE\\AliceInWonderlandTeaParty",L"CareForTea",
               (undefined *)reg_key_out);
  p_flag = g_flag;
  iVar1 = *(int *)(lVar6 + 0x18);
  reg_key = reg_key_out[0];
    if (iVar1 == 0x222000) {
    _Src = *(void **)(param_2 + 0x18);
    DbgPrint(_Src);
    _Size = 0xffffffffffffffff;
    _g_passcode = ZEXT816(0);
    _DAT_00013068 = ZEXT816(0);
    do {
      _Size = _Size + 1;
    } while (*(char *)((longlong)_Src + _Size) != '\0');
    memmove(&g_passcode,_Src,_Size);
    g_flag = decrypt_flag(&g_encrypted_flag,0x30);
    DbgPrint(g_flag);
  }
  else if (iVar1 == 0x222004) {
    puVar2 = *(undefined4 **)(param_2 + 0x18);
    *(undefined4 *)(lVar6 + 8) = 8;
    uVar3 = *(undefined4 *)(*p_flag + 4);
    uVar4 = *(undefined4 *)(*p_flag + 8);
    uVar5 = *(undefined4 *)(*p_flag + 0xc);
    *puVar2 = *(undefined4 *)*p_flag;
    puVar2[1] = uVar3;
    puVar2[2] = uVar4;
    puVar2[3] = uVar5;
    uVar3 = *(undefined4 *)(p_flag[1] + 4);
    uVar4 = *(undefined4 *)(p_flag[1] + 8);
    uVar5 = *(undefined4 *)(p_flag[1] + 0xc);
    puVar2[4] = *(undefined4 *)p_flag[1];
    puVar2[5] = uVar3;
    puVar2[6] = uVar4;
    puVar2[7] = uVar5;
    uVar3 = *(undefined4 *)(p_flag[2] + 4);
    uVar4 = *(undefined4 *)(p_flag[2] + 8);
    uVar5 = *(undefined4 *)(p_flag[2] + 0xc);
    puVar2[8] = *(undefined4 *)p_flag[2];
    puVar2[9] = uVar3;
    puVar2[10] = uVar4;
    puVar2[0xb] = uVar5;
    ExFreePoolWithTag(p_flag,0xaabbccdd);
  }
  else {
    if (iVar1 == 0x222008) {
      p_flag = *(undefined (**) [16])(param_2 + 0x18);
      *(undefined4 *)(lVar6 + 8) = 0x20;
      lVar6 = 0;
      i = 0;
      do {
        hash_idx = i + 1;
        local_50[lVar6] = *(byte *)((longlong)&reg_key + (ulonglong)(i & 3)) ^ (&g_hash1)[lVar6];
        lVar6 = lVar6 + 1;
        i = hash_idx;
      } while ((int)hash_idx < 0x20);
      local_70._0_4_ = local_50._0_4_;
      local_70._4_4_ = local_50._4_4_;
      uStack104 = uStack72;
      uStack100 = uStack68;
      local_60._0_4_ = local_40._0_4_;
      local_60._4_4_ = local_40._4_4_;
      uStack88 = uStack56;
      uStack84 = uStack52;
    }
    else {
      if (iVar1 != 0x22200c) goto LAB_00011469;
      p_flag = *(undefined (**) [16])(param_2 + 0x18);
      *(undefined4 *)(lVar6 + 8) = 0x20;
      lVar6 = 0;
      i = 0;
      do {
        hash_idx = i + 1;
        local_70[lVar6] = *(byte *)((longlong)&reg_key + (ulonglong)(i & 3)) ^ (&g_hash2)[lVar6];
        lVar6 = lVar6 + 1;
        i = hash_idx;
      } while ((int)hash_idx < 0x20);
    }
    *p_flag = CONCAT412(uStack100,CONCAT48(uStack104,CONCAT44(local_70._4_4_,local_70._0_4_)));
    p_flag[1] = CONCAT412(uStack84,CONCAT48(uStack88,CONCAT44(local_60._4_4_,local_60._0_4_)));
  }
LAB_00011469:
  *(undefined4 *)(param_2 + 0x30) = 0;
  *(undefined8 *)(param_2 + 0x38) = 0x30;
  IofCompleteRequest(param_2,0);
  FUN_000117a0(local_10 ^ (ulonglong)auStack152);
  return;
}

```

We can see that the function reads `\Registry\Machine\SOFTWARE\AliceInWonderlandTeaParty\CareForTea` from the registry, then uses it to decrypt both hashes using XOR.

The registry key can be extracted from the dump. In general, the following command should work:

```console
┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ vol -f $memdump windows.registry.printkey --key "AliceInWonderlandTeaParty" --recurse
Volatility 3 Framework 2.4.1
Progress:  100.00               PDB scanning finished
Last Write Time Hive Offset     Type    Key     Name    Data    Volatile

-       0xde88d0023000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d003d000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d0078000  Key     ?\AliceInWonderlandTeaParty     -               -
```

However version `2.4.1` probably has a bug. Running the same command on an older version of volatility provides the key:

```
Volatility 3 Framework 1.0.0
Progress:  100.00               PDB scanning finished
Last Write Time Hive Offset     Type    Key     Name    Data    Volatile

-       0xde88d0023000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d003d000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d0078000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d77e2000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d6119000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d7dc6000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d7d21000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d7d06000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d7bae000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d716f000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d6a54000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d692b000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d6335000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d62ff000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d6177000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d606c000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d5ff3000  Key     ?\AliceInWonderlandTeaParty     -               -
-       0xde88d0c29000  Key     ?\AliceInWonderlandTeaParty     -               -
2022-09-02 22:32:25.000000      0xde88d0ae4000  REG_DWORD       \SystemRoot\System32\Config\SOFTWARE\AliceInWonderlandTeaParty  CareForTea      2703026955      False
```

We can see that the key is `2703026955` (`0xa11ceb0b`).

We XOR it with the encrypted hashes to retrieve the plaintext hashes:

```python
>>> from pwn import *
>>> xor(p32(0xa11ceb0b), b'\x39\xd3\x2a\x98\x3e\xdf\x7a\xc3\x6e\xda\x25\xc0\x3f\x8f\x79\x99\x3d\xde\x2b\x99\x32\x8d\x78\xc7\x3c\xde\x7f\xc2\x6a\xde\x79\xc0')
b'286954fbe19a4de865789fdf75cca5ea'
>>> xor(p32(0xa11ceb0b), b'\x32\x89\x2d\xc7\x3a\xd3\x7e\xc2\x32\x8e\x29\x94\x3d\xd2\x7a\xc3\x3a\xdd\x2a\xc0\x39\xd9\x7f\xc0\x3d\x8e\x7e\x92\x38\xdc\x7d\x95')
b'9b1f18bc9e5569fb166a22ca6eb337a4'
```

During the CTF itself, we were just lucky and skipped the whole thing by searching for MD5 hashes in the memory:

```console
┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ vol -f $memdump windows.memmap --dump --pid 4484

┌──(user@kali)-[~/CTFs/intent/FTP]
└─$ strings -n 32 pid.4484.dmp | grep -iP '^[0-9a-f]+$' | head
286954fbe19a4de865789fdf75cca5ea
286954fbe19a4de865789fdf75cca5ea
9b1f18bc9e5569fb166a22ca6eb337a4
286954fbe19a4de865789fdf75cca5ea
9b1f18bc9e5569fb166a22ca6eb337a4
286954fbe19a4de865789fdf75cca5ea
9b1f18bc9e5569fb166a22ca6eb337a4
286954fbe19a4de865789fdf75cca5ea
9b1f18bc9e5569fb166a22ca6eb337a4
286954fbe19a4de865789fdf75cca5ea
```

Now that we have the hashes, we use a reverse hash lookup service such as [CrackStation](https://crackstation.net/) to get the original input to the hash function. For `286954fbe19a4de865789fdf75cca5ea` it's `whothefuckisalice`. The second hash wasn't cracked but we just need one.

Once we have the correct passcode, the program calls `GetFlag`:

```csharp
private static string GetFlag(string passcode)
{
    IntPtr fileA = TeaParty.TeaParty.CreateFileA("\\\\.\\TeaParty", 1073741824U, 2U, IntPtr.Zero, 3U, 0U, IntPtr.Zero);
    IntPtr num1 = Marshal.AllocHGlobal(512);
    int lpBytesReturned = 0;
    IntPtr num2 = Marshal.AllocHGlobal(passcode.Length + 1);
    Marshal.Copy(Encoding.ASCII.GetBytes(passcode + "\0"), 0, num2, passcode.Length + 1);
    bool flag = TeaParty.TeaParty.DeviceIoControl(fileA, 2236416U, num2, (uint) (passcode.Length + 1), IntPtr.Zero, 0U, out lpBytesReturned, IntPtr.Zero);
    flag = TeaParty.TeaParty.DeviceIoControl(fileA, 2236420U, IntPtr.Zero, 0U, num1, 512U, out lpBytesReturned, IntPtr.Zero);
    byte[] numArray = new byte[lpBytesReturned];
    Marshal.Copy(num1, numArray, 0, lpBytesReturned);
    int num3 = (int) MessageBox.Show(Encoding.ASCII.GetString(numArray));
    Marshal.FreeHGlobal(num1);
    Marshal.FreeHGlobal(num2);
    TeaParty.TeaParty.CloseHandle(fileA);
    return Encoding.ASCII.GetString(numArray);
}
```

This sends us back to the driver, to the following section:

```c
  if (iVar1 == 0x222000) {
    _Src = *(void **)(param_2 + 0x18);
    DbgPrint(_Src);
    _Size = 0xffffffffffffffff;
    _g_passcode = ZEXT816(0);
    _DAT_00013068 = ZEXT816(0);
    do {
      _Size = _Size + 1;
    } while (*(char *)((longlong)_Src + _Size) != '\0');
    memmove(&g_passcode,_Src,_Size);
    g_flag = decrypt_flag(&g_encrypted_flag,0x30);
    DbgPrint(g_flag);
  }
```

The actual decryption happens here:

```c
undefined (*) [16] decrypt_flag(PUCHAR encrypted_buffer,ULONG encrypted_buffer_size)

{
  NTSTATUS NVar1;
  undefined (*pbKeyObject) [16];
  undefined (*pbIV) [16];
  undefined (*pbOutput) [16];
  undefined (*pauVar2) [16];
  undefined (*pauVar3) [16];
  uint local_res18 [2];
  uint local_res20 [2];
  ULONG local_58;
  ULONG local_54;
  BCRYPT_HANDLE local_50;
  BCRYPT_KEY_HANDLE local_48 [2];
  
  pbIV = (undefined (*) [16])0x0;
  local_50 = (BCRYPT_HANDLE)0x0;
  local_48[0] = (BCRYPT_KEY_HANDLE)0x0;
  local_res20[0] = 0;
  local_54 = 0;
  local_58 = 0;
  local_res18[0] = 0;
  pauVar3 = (undefined (*) [16])0x0;
  pauVar2 = (undefined (*) [16])0x0;
  NVar1 = BCryptOpenAlgorithmProvider(&local_50,L"AES",(LPCWSTR)0x0,0);
  pbOutput = pbIV;
  pbKeyObject = pbIV;
  if (-1 < NVar1) {
    NVar1 = BCryptGetProperty(local_50,L"ObjectLength",(PUCHAR)&local_58,8,&local_54,0);
    pbIV = pauVar2;
    pbOutput = pauVar3;
    pbKeyObject = (undefined (*) [16])0x0;
    if (-1 < NVar1) {
      pbKeyObject = (undefined (*) [16])ExAllocatePoolWithTag(1,local_58,0xaabbccdd);
      if (pbKeyObject != (undefined (*) [16])0x0) {
        NVar1 = BCryptGetProperty(local_50,L"BlockLength",(PUCHAR)local_res18,8,&local_54,0);
        if ((-1 < NVar1) && (local_res18[0] < 0x11)) {
          pbIV = (undefined (*) [16])ExAllocatePoolWithTag(1);
          if (pbIV != (undefined (*) [16])0x0) {
            memmove(pbIV,&DAT_00012110,(ulonglong)local_res18[0]);
            NVar1 = BCryptSetProperty(local_50,L"ChainingMode",(PUCHAR)L"ChainingModeCBC",0x20,0);
            if (-1 < NVar1) {
              NVar1 = BCryptGenerateSymmetricKey
                                (local_50,local_48,(PUCHAR)pbKeyObject,local_58,&g_passcode,0x20,0);
              if (-1 < NVar1) {
                NVar1 = BCryptDecrypt(local_48[0],encrypted_buffer,encrypted_buffer_size,(void *)0x0
                                      ,(PUCHAR)pbIV,local_res18[0],(PUCHAR)0x0,0,local_res20,1);
                if (-1 < NVar1) {
                  pbOutput = (undefined (*) [16])ExAllocatePoolWithTag(1);
                  FUN_00011c00(pbOutput,0,(ulonglong)local_res20[0]);
                  if (pbOutput != (undefined (*) [16])0x0) {
                    BCryptDecrypt(local_48[0],encrypted_buffer,encrypted_buffer_size,(void *)0x0,
                                  (PUCHAR)pbIV,local_res18[0],(PUCHAR)pbOutput,local_res20[0],
                                  local_res20,1);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if (local_50 != (BCRYPT_ALG_HANDLE)0x0) {
    BCryptCloseAlgorithmProvider(local_50,0);
  }
  if (local_48[0] != (BCRYPT_KEY_HANDLE)0x0) {
    BCryptDestroyKey(local_48[0]);
  }
  if (pbKeyObject != (undefined (*) [16])0x0) {
    ExFreePoolWithTag(pbKeyObject,0xaabbccdd);
  }
  if (pbIV != (undefined (*) [16])0x0) {
    ExFreePoolWithTag(pbIV,0xaabbccdd);
  }
  return pbOutput;
}

```

We can "port" it to C:

<details>
  <summary>C Code</summary>
  
```c
#include <stdio.h>
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS

#include <winternl.h>
#include <ntstatus.h>
#include <winerror.h>
#include <stdio.h>
#include <bcrypt.h>
#include <sal.h>

#pragma comment(lib, "bcrypt.lib")

#define SECRET "whothefuckisalice"

void
ReportError(
    _In_	DWORD       dwErrCode
)
{
    wprintf(L"Error: 0x%08x (%d)\n", dwErrCode, dwErrCode);
}

int main()
{
    NTSTATUS            Status;
    BCRYPT_ALG_HANDLE   AesAlgHandle = NULL;
    SIZE_T              NumberOfBytes = 0;
    PUCHAR              KeyObject = NULL;
    SIZE_T              Size;
    BCRYPT_KEY_HANDLE   KeyHandle = NULL;
    UCHAR               secret[32] = { 0 };
    DWORD               PlainTextLength = 0;
    PUCHAR              PlainText = NULL;

    memset(secret, 0, sizeof(secret));
    strncpy_s(secret, sizeof(SECRET), SECRET, sizeof(secret));

    BYTE IV[] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    BYTE CipherText[] =
    {
        0x74, 0xF8, 0x14, 0x89, 0x7D, 0x5A, 0xC9, 0xC0, 0x53, 0x01, 0xFD, 0x99, 0x22, 0xC3, 0xAC, 0x84, 0xFD, 0xFB,
        0x43, 0x12, 0xFF, 0x39, 0xAB, 0x49, 0xEE, 0x39, 0xE5, 0x80, 0xC1, 0xF5, 0x16, 0x0C
    };

    Status = BCryptOpenAlgorithmProvider(
        &AesAlgHandle,              // Alg Handle pointer
        BCRYPT_AES_ALGORITHM,       // Cryptographic Algorithm name (null terminated unicode string)
        NULL,                       // Provider name; if null, the default provider is loaded
        0);                         // Flags
    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }

    Status = BCryptGetProperty(
        AesAlgHandle,               // Handle to a CNG object
        BCRYPT_OBJECT_LENGTH,       // Property name (null terminated unicode string)
        (PBYTE)&NumberOfBytes,      // Addr of the output buffer which recieves the property value
        sizeof(NumberOfBytes),      // Size of the buffer in the bytes
        (char*)&NumberOfBytes + 4,  // Number of bytes that were copied into the buffer
        0);                         // Flags

    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }

    
    KeyObject = malloc((unsigned int)NumberOfBytes);

    Status = BCryptGetProperty(
        AesAlgHandle,               // Handle to a CNG object
        BCRYPT_BLOCK_LENGTH,        // Property name (null terminated unicode string)
        (PBYTE)&Size,               // Addr of the output buffer which recieves the property value
        sizeof(NumberOfBytes),      // Size of the buffer in the bytes
        (char*)&NumberOfBytes + 4,  // Number of bytes that were copied into the buffer
        0);                         // Flags

    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }
    

    Status = BCryptSetProperty(
        AesAlgHandle,               // Handle to a CNG object          
        BCRYPT_CHAINING_MODE,       // Property name(null terminated unicode string)
        BCRYPT_CHAIN_MODE_CBC,      // Address of the buffer that contains the new property value 
        32,                         // Size of the buffer in bytes
        0);                         // Flags
    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }

    Status = BCryptGenerateSymmetricKey(
        AesAlgHandle,                // Algorithm provider handle
        &KeyHandle,                  // A pointer to key handle
        KeyObject,                   // A pointer to the buffer that recieves the key object;NULL implies memory is allocated and freed by the function
        (unsigned int)NumberOfBytes, // Size of the buffer in bytes
        (PBYTE)secret,               // A pointer to a buffer that contains the key material
        32,                          // Size of the buffer in bytes
        0);                          // Flags
    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }

    Status = BCryptDecrypt(
        KeyHandle,                  // Handle to a key which is used to encrypt 
        CipherText,                 // Address of the buffer that contains the ciphertext
        sizeof(CipherText),         // Size of the buffer in bytes
        NULL,                       // A pointer to padding info, used with asymmetric and authenticated encryption; else set to NULL
        IV,                         // Address of the buffer that contains the IV. 
        sizeof(IV),                 // Size of the IV buffer in bytes
        NULL,                       // Address of the buffer the recieves the plaintext
        0,                          // Size of the buffer in bytes
        &PlainTextLength,           // Variable that recieves number of bytes copied to plaintext buffer 
        BCRYPT_BLOCK_PADDING);      // Flags; Block padding allows to pad data to the next block size

    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }

    PlainText = malloc(PlainTextLength);

    Status = BCryptDecrypt(
        KeyHandle,                  // Handle to a key which is used to encrypt 
        CipherText,                 // Address of the buffer that contains the ciphertext
        sizeof(CipherText),         // Size of the buffer in bytes
        NULL,                       // A pointer to padding info, used with asymmetric and authenticated encryption; else set to NULL
        IV,                         // Address of the buffer that contains the IV. 
        sizeof(IV),                 // Size of the IV buffer in bytes
        PlainText,                  // Address of the buffer the recieves the plaintext
        PlainTextLength,            // Size of the buffer in bytes
        &PlainTextLength,           // Variable that recieves number of bytes copied to plaintext buffer 
        BCRYPT_BLOCK_PADDING);      // Flags; Block padding allows to pad data to the next block size

    if (!NT_SUCCESS(Status))
    {
        ReportError(Status);
        goto cleanup;
    }

    printf("PlainText: %s", PlainText);

cleanup:

    if (AesAlgHandle != NULL)
    {
        BCryptCloseAlgorithmProvider(AesAlgHandle, 0);
    }

    if (KeyHandle != NULL)
    {
        BCryptDestroyKey(KeyHandle);
    }

    if (KeyObject != NULL) {
        free(KeyObject);
    }

    if (PlainText != NULL) {
        free(PlainText);
    }
}
````

</details>

Or just decrypt it with Python:

```python
>>> AES.new(b'whothefuckisalice'.ljust(32, b'\0'), AES.MODE_CBC, b'\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f').decrypt(b'\x74\xf8\x14\x89\x7d\x5a\xc9\xc0\x53\x01\xfd\x99\x22\xc3\xac\x84\xfd\xfb\x43\x12\xff\x39\xab\x49\xee\x39\xe5\x80\xc1\xf5\x16\x0c').decode()
'INTENT{0ff_w1th_7h31r_H34ds}\x00\x03\x03\x03'
```