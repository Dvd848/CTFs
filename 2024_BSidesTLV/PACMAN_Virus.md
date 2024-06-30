# PACMAN Virus

 * Category: Reverse Engineering
 * 200 Points
 * Solved by the JCTF Team

## Description

> A new and dangerous ransomware named "Pacman Virus" is spreading rapidly, encrypting files and demanding a ransom. Your mission is to analyze the malware, find a way to stop it, and save the world from this cyber threat. Can you crack the code and halt the ransomware before it's too late?
> 
> To submit the flag wrap the solution with BSidesTLV2024{XXX...XXX}

An EXE file was attached.

## Solution

The executable is a .NET executable, we can use `dotpeek` to decompile it.

We get:

```csharp
// Decompiled with JetBrains decompiler
// Type: pacmanvirus.Form1
// Assembly: pacmanvirus, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: 2BE5BF7B-A4EA-45E0-9EF3-308989613B05
// Assembly location: E:\CTFs\bsides\PACMAN_Virus\pacmanvirus\pacmanvirus.exe

using pacmanvirus.Properties;
using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Timers;
using System.Windows.Forms;

namespace pacmanvirus
{
  public class Form1 : Form
  {
    private static System.Timers.Timer aTimer;
    private static System.Timers.Timer bTimer;
    private bool mouth_open;
    private double max_x;
    private double max_y;
    private int last_y;
    private string direction = "right";
    private int move_speed = 10;
    private double pacman_width = 102.0;
    private int[] arrKey = new int[6];
    private int indexKey;
    private string ransomeFile = "ransome.txt";
    private IContainer components;

    public Form1() => this.InitializeComponent();

    private void Form1_Load(object sender, EventArgs e)
    {
      this.ShowInTaskbar = false;
      this.Size = new Size((int) this.pacman_width, (int) this.pacman_width);
      Form1.aTimer = new System.Timers.Timer();
      Form1.aTimer.Interval = 80.0;
      Form1.bTimer = new System.Timers.Timer();
      Form1.bTimer.Interval = 35.0;
      Form1.aTimer.Elapsed += new ElapsedEventHandler(this.MoveMouth);
      Form1.bTimer.Elapsed += new ElapsedEventHandler(this.MovePacman);
      Form1.aTimer.AutoReset = true;
      Form1.bTimer.AutoReset = true;
      Rectangle workingArea = Screen.PrimaryScreen.WorkingArea;
      this.max_x = (double) workingArea.Width;
      this.max_y = (double) workingArea.Height;
      Form1.aTimer.Enabled = true;
      Form1.bTimer.Enabled = true;
      this.initKey();
      this.writeRansomToFile(this.ransomeFile);
    }

    private void writeRansomToFile(string filename)
    {
      string[] strArray = new string[22]
      {
        "There was a significant flaw in the security system of your company.",
        "You should be thankful that the flaw was exploited by serious people and not some r00ki3s.",
        "They would have damaged all of your data by mistake or for fun.",
        "your files are encrypted with the strongest military algorithms RSA4096 and AES-256.",
        "without our special decoder it is ‘impossible to restore the data.",
        "Arrempts to restore your data with third party software as Photorec, RannohDecryptor etc.",
        "will lead to irreversible destruction of your data.",
        "To confirm our honest intentions.",
        "send us 2-3 different random files and you will get them decrypted.",
        "It can be from different computers on your network to be sure that our decoder decrypts everything.",
        "Sample files we unlock for free (files should not be related to any kind of backups)",
        "we exclusively have decryption software for your situation",
        "DO NOT RESET OR SHUTDOWN - files may be damaged.",
        "DO NOT RENAME the encrypted files.",
        "DO NOT MOVE the encrypted files.",
        "This may lead to the impossibility of recovery of the certain files.",
        "To get information on the price of the decoder contact us at:",
        "bsidestlv.oldschooliscool4@h4ck3rs.so",
        "The payment as to be made in Bitcoins.",
        "The final price depends on how fast you contact us.",
        "AS soon as we receive the payment you will get the decryption tool and",
        "instructions on how to improve your systems security"
      };
      string folderPath = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
      using (StreamWriter streamWriter = new StreamWriter(Path.Combine(folderPath, filename)))
      {
        foreach (string str in strArray)
          streamWriter.WriteLine(str);
      }
      Process.Start("chrome", "https://www.youtube.com/watch?v=Bmz67ErIRa4&t=15s&autoplay=1&mute=0");
      Process.Start("notepad.exe", Path.Combine(folderPath, filename));
    }

    private void Form1_KeyPress(object sender, KeyPressEventArgs e)
    {
      if (this.indexKey == this.arrKey.Length)
      {
        this.indexKey = 0;
        int num = (int) MessageBox.Show("You've cracked the code and saved the world!");
        Application.Exit();
      }
      if ((int) e.KeyChar == this.arrKey[this.indexKey])
        ++this.indexKey;
      else
        this.indexKey = 0;
    }

    private void initKey()
    {
      string text = Form1.CalculateMD5(Form1.GetCurrentPath());
      for (int index = 0; index < 512; ++index)
        text = Form1.CalculateMd5str(text);
      for (int index = 0; index < this.arrKey.Length; ++index)
        this.arrKey[index] = (int) text[index];
    }

    private void MoveMouth(object source, ElapsedEventArgs e)
    {
      this.UpdatePacmanImage();
      this.mouth_open = !this.mouth_open;
    }

    private void UpdatePacmanImage()
    {
      Bitmap bitmap = !this.mouth_open ? Resources.mouth_opened : Resources.mouth_closed;
      if (this.direction == "left")
        bitmap.RotateFlip(RotateFlipType.RotateNoneFlipX);
      else if (this.direction == "down")
        bitmap.RotateFlip(RotateFlipType.Rotate90FlipNone);
      this.BackgroundImage = (Image) bitmap;
    }

    private static string CalculateMD5(string filename)
    {
      using (MD5 md5 = MD5.Create())
      {
        using (FileStream fileStream = File.OpenRead(filename))
          return BitConverter.ToString(md5.ComputeHash((Stream) fileStream)).Replace("-", "").ToLowerInvariant();
      }
    }

    private static string CalculateMd5str(string text)
    {
      using (MD5 md5 = MD5.Create())
        return BitConverter.ToString(md5.ComputeHash(Encoding.UTF8.GetBytes(text))).Replace("-", "").ToLowerInvariant();
    }

    private static string GetCurrentPath() => Assembly.GetEntryAssembly().Location;

    private void MovePacman(object source, ElapsedEventArgs e)
    {
      if (this.direction == "right")
      {
        if ((double) (this.Location.X + this.move_speed) + this.pacman_width > (double) (int) this.max_x)
        {
          this.direction = "down";
          this.UpdatePacmanImage();
          this.last_y = this.Location.Y;
          return;
        }
        // ISSUE: method pointer
        this.Invoke((Delegate) new Action((object) this, __methodptr(\u003CMovePacman\u003Eb__22_0)));
      }
      else if (this.direction == "down")
      {
        Console.Out.WriteLine(this.last_y);
        Point location = this.Location;
        if ((double) (location.Y + this.move_speed) > (double) this.last_y + this.pacman_width * 1.5)
        {
          location = this.Location;
          this.direction = (double) location.X <= this.max_x / 2.0 ? "right" : "left";
          this.UpdatePacmanImage();
          return;
        }
        location = this.Location;
        if (location.Y + this.move_speed > (int) this.max_y)
        {
          // ISSUE: method pointer
          this.Invoke((Delegate) new Action((object) this, __methodptr(\u003CMovePacman\u003Eb__22_1)));
          this.last_y = 0;
        }
        else
        {
          // ISSUE: method pointer
          this.Invoke((Delegate) new Action((object) this, __methodptr(\u003CMovePacman\u003Eb__22_2)));
        }
      }
      else if (this.direction == "left")
      {
        if (this.Location.X - this.move_speed <= 0)
        {
          this.direction = "down";
          this.UpdatePacmanImage();
          this.last_y = this.Location.Y;
          return;
        }
        // ISSUE: method pointer
        this.Invoke((Delegate) new Action((object) this, __methodptr(\u003CMovePacman\u003Eb__22_3)));
      }
      // ISSUE: method pointer
      this.Invoke((Delegate) new Action((object) this, __methodptr(\u003CMovePacman\u003Eb__22_4)));
    }

    protected override void Dispose(bool disposing)
    {
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
      this.SuspendLayout();
      this.AutoScaleDimensions = new SizeF(6f, 13f);
      this.AutoScaleMode = AutoScaleMode.Font;
      this.BackColor = Color.DimGray;
      this.BackgroundImage = (Image) Resources.mouth_closed;
      this.BackgroundImageLayout = ImageLayout.Stretch;
      this.ClientSize = new Size(109, 102);
      this.DoubleBuffered = true;
      this.Name = nameof (Form1);
      this.ShowInTaskbar = false;
      this.Text = "PacManRansomware";
      this.TransparencyKey = Color.DimGray;
      this.FormBorderStyle = FormBorderStyle.None;
      this.Load += new EventHandler(this.Form1_Load);
      this.KeyPress += new KeyPressEventHandler(this.Form1_KeyPress);
      this.ResumeLayout(false);
    }
  }
}
```

Reviewing the code, we can see that the interesting parts are `initKey()` and `Form1_KeyPress`.

```csharp
private void initKey()
{
    string text = Form1.CalculateMD5(Form1.GetCurrentPath());
    for (int index = 0; index < 512; ++index)
    text = Form1.CalculateMd5str(text);
    for (int index = 0; index < this.arrKey.Length; ++index)
    this.arrKey[index] = (int) text[index];
}
```

The `initKey` function calculates a key based on `Form1.GetCurrentPath()`, which 
resolves to `Assembly.GetEntryAssembly().Location` (i.e. the path of the executable).

```csharp
private void Form1_KeyPress(object sender, KeyPressEventArgs e)
{
    if (this.indexKey == this.arrKey.Length)
    {
    this.indexKey = 0;
    int num = (int) MessageBox.Show("You've cracked the code and saved the world!");
    Application.Exit();
    }
    if ((int) e.KeyChar == this.arrKey[this.indexKey])
    ++this.indexKey;
    else
    this.indexKey = 0;
}
```

The `Form1_KeyPress` function listens to key presses, and gives the success message if
the last `this.arrKey.Length` keystrokes were identical to the first `this.arrKey.Length`
characters of the key. So, we can copy this logic aside, and run it to recreate the key:

```csharp
using System.Security.Cryptography;
using System.Text;

static string CalculateMD5(string filename)
{
    using (MD5 md5 = MD5.Create())
    {
        using (FileStream fileStream = File.OpenRead(filename))
            return BitConverter.ToString(md5.ComputeHash((Stream)fileStream)).Replace("-", "").ToLowerInvariant();
    }
}

static string CalculateMd5str(string text)
{
    using (MD5 md5 = MD5.Create())
        return BitConverter.ToString(md5.ComputeHash(Encoding.UTF8.GetBytes(text))).Replace("-", "").ToLowerInvariant();
}

int[] arrKey = new int[6];

string text = CalculateMD5("E:\\CTFs\\bsides\\PACMAN_Virus\\pacmanvirus\\pacmanvirus.exe");
Console.WriteLine(string.Format("Original MD5: {0}", text));

for (int index = 0; index < 512; ++index)
{
    text = CalculateMd5str(text);
}
Console.WriteLine(string.Format("Phase 2 MD5: {0}", text));

Console.WriteLine(string.Format("Flag: BSidesTLV2024{{{0}}}", text.Substring(0, arrKey.Length)));
```

Output: 

```
Original MD5: ea44e7d5c90a6db075c3eae658c03bd3
Phase 2 MD5: 6d747e3d34ffc0e38374885fda9c3a79
Flag: BSidesTLV2024{6d747e}
```

The flag: `BSidesTLV2024{6d747e}`