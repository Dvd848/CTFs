# Gone Fishing

## Description 

> Your investigation begins here. The CV upload system crashed, but without John's credentials, you're locked out of making any fixes.
> 
> The employee database should contain John's username, but company records can be... messy. Time to put those detective skills to work!
> 
> Find John's username so you can get one step closer to restoring the system.

## Solution

We're given a "Database Query Interface" allowing us to perform SQL queries on the database. The default query is `SELECT * FROM sqlite_master`, let's run it:

```
type_null,name_null,tbl_name_null,rootpage_null,sql_null
table,employees,employees,2,"CREATE TABLE employees (
            uid INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            full_name TEXT NOT NULL,
            department TEXT NOT NULL,
            hire_date TEXT NOT NULL
        )"
table,users_info,users_info,16,"CREATE TABLE users_info (
            uid INTEGER PRIMARY KEY,
            access_level TEXT NOT NULL,
            status TEXT NOT NULL,
            last_login TEXT,
            last_login_location TEXT
        )"
table,password_audit_log,password_audit_log,2102,"CREATE TABLE ""password_audit_log"" (
            uid INTEGER,
            password_hash TEXT,
            created_timestamp INTEGER,
            change_reason TEXT,
            source_ip TEXT
        )"

```

So we have three tables: Employees, Users Info and Password Audit Log.

Let's get a taste of what each holds:

```
> SELECT * FROM employees LIMIT 3

uid_null,username_null,full_name_null,department_null,hire_date_null
10004,amoore256,Ashley Moore,Security,2020-05-10
10006,crussell768,Charles Russell,Finance,2019-11-10
10011,cmatthews898,Carolyn Matthews,IT,2024-07-13

> SELECT * FROM users_info LIMIT 3

uid_null,access_level_null,status_null,last_login_null,last_login_location_null
10004,senior_admin,active,2025-07-27 01:35:55,"New York, NY"
10006,senior_admin,active,2025-07-29 09:26:09,"Seattle, WA"
10011,admin,inactive,2024-02-28 15:55:25,"New York, NY"

> SELECT * FROM password_audit_log LIMIT 3

uid_null,password_hash_null,created_timestamp_null,change_reason_null,source_ip_null
74032,50e1874d370cd60441aebd8de3fab5bc,1718139600,policy_expiry,10.0.2.49
84668,d32480202f349277bd06be5419ec454f,1731794400,user_requested,192.168.1.222
21395,09456656a60ca21a28b042de29878584,1708466400,user_requested,195.74.52.77
```

In these cases it's also useful to figure out what the different fixed-value fields can hold. Here are the interesting ones:

```
> SELECT status FROM users_info GROUP BY status

status_null
active
inactive
leave
vacation

> SELECT access_level FROM users_info GROUP BY access_level

access_level_null
admin
basic
senior_admin
standard

> SELECT department FROM employees GROUP BY department
department_null
Engineering
Finance
HR
IT
Legal
Marketing
Operations
Product
Sales
Security
Support

```

So we're looking for a *John* who's likely an admin and is on vacation, probably from IT or engineering:

```sql
SELECT * FROM employees
LEFT JOIN users_info ON users_info.uid = employees.uid 
WHERE employees.full_name LIKE 'john%'
AND users_info.status = "vacation"
AND employees.department IN ("IT", "Engineering")
```

Luckily, we only have one such result:

```
uid_null,username_null,full_name_null,department_null,hire_date_null,access_level_null,status_null,last_login_null,last_login_location_null
64994,jmiller499,John Miller,Engineering,2021-03-15,admin,vacation,2025-06-20 15:41:50,"Seattle, WA"
```

The username **jmiller499** was accepted as the flag.

> Found John's username! 🕵️
> Great work. Two more challenges to save the day. 