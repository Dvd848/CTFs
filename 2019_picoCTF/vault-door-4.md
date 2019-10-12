# vault-door-4
Reverse Engineering, 250 points

## Description:
> This vault uses ASCII encoding for the password.

```java
import java.util.*;

class VaultDoor4 {
    public static void main(String args[]) {
        VaultDoor4 vaultDoor = new VaultDoor4();
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter vault password: ");
        String userInput = scanner.next();
	String input = userInput.substring("picoCTF{".length(),userInput.length()-1);
	if (vaultDoor.checkPassword(input)) {
	    System.out.println("Access granted.");
	} else {
	    System.out.println("Access denied!");
        }
    }

    // I made myself dizzy converting all of these numbers into different bases,
    // so I just *know* that this vault will be impenetrable. This will make Dr.
    // Evil like me better than all of the other minions--especially Minion
    // #5620--I just know it!
    //
    //  .:::.   .:::.
    // :::::::.:::::::
    // :::::::::::::::
    // ':::::::::::::'
    //   ':::::::::'
    //     ':::::'
    //       ':'
    // -Minion #7781
    public boolean checkPassword(String password) {
        byte[] passBytes = password.getBytes();
        byte[] myBytes = {
            106 , 85  , 53  , 116 , 95  , 52  , 95  , 98  ,
            0x55, 0x6e, 0x43, 0x68, 0x5f, 0x30, 0x66, 0x5f,
            0142, 0131, 0164, 063 , 0163, 0137, 066 , 061 ,
            'e' , '0' , 'f' , '2' , '7' , '6' , '9' , 'c' ,
        };
        for (int i=0; i<32; i++) {
            if (passBytes[i] != myBytes[i]) {
                return false;
            }
        }
        return true;
    }
}
```

## Solution: 


Let's run reverse logic using Javascript:
```javascript
> console.log(String.fromCharCode(106 , 85  , 53  , 116 , 95  , 52  , 95  , 98  ,
...             0x55, 0x6e, 0x43, 0x68, 0x5f, 0x30, 0x66, 0x5f,
...             0142, 0131, 0164, 063 , 0163, 0137, 066 , 061) + ['e' , '0' , 'f' , '2' , '7' , '6' , '9' , 'c'].join(""))
jU5t_4_bUnCh_0f_bYt3s_61e0f2769c
```