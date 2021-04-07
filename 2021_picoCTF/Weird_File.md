# Weird File
Category: Forensics, 20 points

## Description

> What could go wrong if we let Word documents run programs? (aka "in-the-clear").

A word file was attached.

## Solution

Letting Word documents run programs is a direct reference to macros, so let's search for macros in the file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Weird_File]
└─$ olevba weird.docm
olevba 0.56.1 on Python 3.9.2 - http://decalage.info/python/oletools
===============================================================================
FILE: weird.docm
Type: OpenXML
WARNING  For now, VBA stomping cannot be detected for files in memory
-------------------------------------------------------------------------------
VBA MACRO ThisDocument.cls
in file: word/vbaProject.bin - OLE stream: 'VBA/ThisDocument'
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Sub AutoOpen()
    MsgBox "Macros can run any program", 0, "Title"
    Signature

End Sub

 Sub Signature()
    Selection.TypeText Text:="some text"
    Selection.TypeParagraph

 End Sub

 Sub runpython()

Dim Ret_Val
Args = """" '"""
Ret_Val = Shell("python -c 'print(\"cGljb0NURnttNGNyMHNfcl9kNG5nM3IwdXN9\")'" & " " & Args, vbNormalFocus)
If Ret_Val = 0 Then
   MsgBox "Couldn't run python script!", vbOKOnly
End If
End Sub
+----------+--------------------+---------------------------------------------+
|Type      |Keyword             |Description                                  |
+----------+--------------------+---------------------------------------------+
|AutoExec  |AutoOpen            |Runs when the Word document is opened        |
|Suspicious|Shell               |May run an executable file or a system       |
|          |                    |command                                      |
|Suspicious|vbNormalFocus       |May run an executable file or a system       |
|          |                    |command                                      |
|Suspicious|run                 |May run an executable file or a system       |
|          |                    |command                                      |
+----------+--------------------+---------------------------------------------+

```

We can see that there's a macro attempting to run:

```
python -c 'print(\"python -c 'print(\"cGljb0NURnttNGNyMHNfcl9kNG5nM3IwdXN9\")'\")'
```

This looks like a base64-encoded string. Let's decode it:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Weird_File]
└─$ echo cGljb0NURnttNGNyMHNfcl9kNG5nM3IwdXN9 | base64 -d
picoCTF{m4cr0s_r_d4ng3r0us}
```

The flag: `picoCTF{m4cr0s_r_d4ng3r0us}`