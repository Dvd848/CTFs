# Dungeon

 * Category: Reverse Engineering
 * 350 Points
 * Solved by the JCTF Team

## Description

> Enter the Dungeon to find the Pixelated Flag! It has been broken to 3 pieces, can you find them all?

A ZIP file was attached.

## Solution

We unzip the archive and find out that it's the source code for an open source game:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Dungeon]
└─$ cat Dungeon/README.md| head
# Shattered Pixel Dungeon

[Shattered Pixel Dungeon](https://shatteredpixel.com/shatteredpd/) is an open-source traditional roguelike dungeon crawler with randomized levels and enemies, and hundreds of items to collect and use. It's based on the [source code of Pixel Dungeon](https://github.com/00-Evan/pixel-dungeon-gradle), by [Watabou](https://www.watabou.ru).

Shattered Pixel Dungeon currently compiles for Android, iOS, and Desktop platforms.
```

The README gives us the original code, we can download it and compare the contents of both parent 
folders to see if our copy was modified.

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Dungeon]
└─$ diff -qr Dungeon shattered-pixel-dungeon-master | grep differ
Files Dungeon/build.gradle and shattered-pixel-dungeon-master/build.gradle differ
Files Dungeon/core/src/main/assets/messages/journal/journal.properties and shattered-pixel-dungeon-master/core/src/main/assets/messages/journal/journal.properties differ
Files Dungeon/core/src/main/assets/sprites/items.png and shattered-pixel-dungeon-master/core/src/main/assets/sprites/items.png differ
Files Dungeon/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/Dungeon.java and shattered-pixel-dungeon-master/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/Dungeon.java differ
Files Dungeon/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/items/keys/SkeletonKey.java and shattered-pixel-dungeon-master/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/items/keys/SkeletonKey.java differ
```

Here are the interesting diffs:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Dungeon]
└─$ diff shattered-pixel-dungeon-master/core/src/main/assets/messages/journal/journal.properties Dungeon/core/src/main/assets/messages/journal/journal.properties
8,9c8,9
< journal.document.adventurers_guide.identifying.title=Identifying Items
< journal.document.adventurers_guide.identifying.body=You won't know all of the properties of some items when you first find them.\n\nThe colors of potions and symbols on scrolls are different in each dungeon. Unidentified equipment can be upgraded or enchanted if you're lucky, or it might be cursed!\n\nScrolls of identify, upgrade, or remove curse are very useful if you want to reduce the risk of using unidentified equipment.\n\n(You can find a list of all the items you've identified in the items tab of your journal)
---
> journal.document.adventurers_guide.identifying.title=Identifying Items And Flags (BSides Edition)
> journal.document.adventurers_guide.identifying.body=You won't know all of the properties of some items when you first find them.\n\nThe colors of potions and symbols on scrolls are different in each dungeon. Unidentified equipment can be upgraded or enchanted if you're lucky, or it might be cursed!\n\nScrolls of identify, upgrade, or remove curse are very useful if you want to reduce the risk of using unidentified equipment.\n\n(You can find a list of all the items you've identified in the items tab of your journal)\n\n(H30n)

┌──(user@kali)-[/media/sf_CTFs/bsides/Dungeon]
└─$ diff shattered-pixel-dungeon-master/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/Dungeon.java Dungeon/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/Dungeon.java
220c220,246
<
---
>
>
>     public static String customDecrypt(char[] encText, String key) {
>         StringBuilder result = new StringBuilder();
>         int keyIndex = 0;
>         int keyLength = key.length();
>
>         for (int i = 0; i < encText.length; i++) {
>             char encChar = encText[i];
>             int k = key.charAt(keyIndex);
>             char decChar = (char) (((encChar - i) ^ k) - i % 256);
>             result.append(decChar);
>             keyIndex = (keyIndex + 1) % keyLength;
>         }
>
>         return result.toString();
>     }
>
>     public static void main(String[] args) {
>         char[] encText = {'3', '', '*', 'b', '/', 'K', 'h', '\\', 'Ñ', 'b', ')', 'b', '8', 'S', '/', ''};
>               String ky = "items + journal + key";
>
>         String decryptedText = customDecrypt(encText, ky);
>         System.out.println("Decrypted: " + decryptedText);
>       }
>
>

┌──(user@kali)-[/media/sf_CTFs/bsides/Dungeon]
└─$ diff shattered-pixel-dungeon-master/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/items/keys/SkeletonKey.java Dungeon/core/src/main/java/com/shatteredpixel/shatteredpixeldungeon/items/keys/SkeletonKey.java
36c36,38
<
---
>
>       private static String k3 = "U1ci";
>
```

In addition, `items.png` was modified to include the string "P4N7".

We can collect the strings from `items.png`, `SkeletonKey.java` and `journal.properties` and
combine them to create the decryption key according to the inline hint: `items + journal + key`.

This is the code we'll get:

```java
public class Dungeon {

    public static String customDecrypt(char[] encText, String key) {
        StringBuilder result = new StringBuilder();
        int keyIndex = 0;
        int keyLength = key.length();

        for (int i = 0; i < encText.length; i++) {
            char encChar = encText[i];
            int k = key.charAt(keyIndex);
            char decChar = (char) (((encChar - i) ^ k) - i % 256);
            result.append(decChar);
            keyIndex = (keyIndex + 1) % keyLength;
        }

        return result.toString();
    }

    public static void main(String[] args) {
        char[] encText = {'3', '', '*', 'b', '/', 'K', 'h', '\\', 'Ñ', 'b', ')', 'b', '8', 'S', '/', ''};
		String ky = "P4N7H30nU1ci";

        String decryptedText = customDecrypt(encText, ky);
        System.out.println("Decrypted: " + decryptedText);
	}
}
```

Running it, we (almost) get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Dungeon]
└─$ javac Dungeon.java && java Dungeon
Decrypted: c0de_pL4_r3pea7
```

The flag that was accepted required adding a "y" to form "pL4y": `BSidesTLV2024{c0de_pL4y_r3pea7}`