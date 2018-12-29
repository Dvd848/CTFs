#!/usr/bin/env ts-node
import * as compiler from "../client/src/language/Compiler"
import {AsyncPromise, VirtualMachine, VirtualMachineState} from "../client/src/language/VirtualMachine"
import * as puppeteer from 'puppeteer'
import * as flags from "./flags";

declare let BigInt: any

const DoEvents = () => new Promise((resolve) => setImmediate(resolve))

const browserPromise = puppeteer.launch({args: ["--no-sandbox"]})
const pagePromise: Promise<puppeteer.Page> = new Promise(async (resolve, reject) => {
    try {
        const browser = await browserPromise
        const page = await browser.newPage()
        await page.setRequestInterception(true)
        page.on('request', r=> (r.url().startsWith("file://") && (
                r.url().endsWith("weelang/pypyjs.html") ||
                r.url().endsWith("lib/FunctionPromise.js") ||
                r.url().endsWith("lib/pypyjs.js") ||
                r.url().endsWith("lib/pypyjs.vm.js") ||
                r.url().endsWith("lib/pypyjs.vm.js.zmem")
            ) ? r.continue() : r.abort() && console.log("blocked", r.url()))
        )
        await page.goto(`file:///${__dirname}/pypyjs.html`, {waitUntil: 'networkidle2'})
        await page.evaluate(`store('${flags.LAYERS}')`)
        resolve(page)
    } catch (e) {
        reject(e)
    }
})

browserPromise.catch(x=>console.error)
pagePromise.catch(x=>console.error)

/**
 * Uses a recent chrome to run code inside the chrome sandbox.
 * (Hint: there is no challenge here. If you can escape chrome, play the advanced ctf ;) )
 * @param script: the code to run
 * @param args: args, in case the script takes any.
 */
async function eval_in_chrome(script, ...args): Promise<string> {
    //const browser = await puppeteer.launch({args: ["--no-sandbox"]})
    try {
        //console.log("running", script, ...args)
        //const browser = await puppeteer.launch({args: ["--no-sandbox"]})
        //const page = await browser.newPage()
        //await page.goto(`file:///${__dirname}/pypyjs.html`)
        //const response = await page.goto(`file:///${__dirname}/pypyjs.html`)
        //console.log(response) Too slow :/
        const page = await pagePromise
        //console.log("Goiong on evaling", script, args)
        const result = await page.evaluate(script, ...args)
        //await page.close()
        //console.log("closed. returning.", result)
        return result
    } catch (e) {
        //console.log("An eval error occurred: ", e)
        return "" + e.message
    }
}

function wee_eval(expr: string): AsyncPromise<string> {
    let asyncResult: AsyncPromise<string> = {
        completed: false,
        value: null,
        stopVirtualMachine: false
    }
    eval_in_chrome(expr).then((res) => {
        //console.log("Result ", expr, res)
        asyncResult.value = res;
        asyncResult.completed = true
    }).catch((err) => {
        console.log("Unexpectged error in eval", expr, err)
        asyncResult.value = "" + err;
        asyncResult.completed = true
    })
    return asyncResult
}


function get_headless_externals() {
    const externals = new compiler.ExternalFunctionsTypesConstants();

    // Override browser-dependend alert with console.log
    externals.addFunction(
        "alert",
        [{name: "message", type: compiler.StringType}], compiler.NothingType,
        false,
        console.log
    )
    externals.addFunction(
        "alert",
        [{name: "value", type: compiler.NumberType}], compiler.NothingType,
        false,
        console.log
    )
    externals.addFunction(
        "alert",
        [{name: "value", type: compiler.BooleanType}], compiler.NothingType,
        false,
        console.log
    )

    // pyserver
    externals.addFunction(
        "ord",
        [{name: "value", type: compiler.StringType}], compiler.NumberType,
        false,
        (value: string) => {
            return value.charCodeAt(0) % 256
        }
    )
    externals.addFunction(
        "bigModPow",
        [
            {name: "base", type: compiler.StringType},
            {name: "exponent", type: compiler.StringType},
            {name: "modulus", type: compiler.StringType}
        ], compiler.StringType,
        false,
        (base: string, exponent: string, modulus: string) => {
            let b = BigInt(base)
            let e = BigInt(exponent)
            let N = BigInt(modulus)
            return eval('""+((b ** e) % N)') // force js since tsc translates ** to Math.Pow... *rolls eyes*
        }
    )
    externals.addFunction(
        "strToBig",
        [{name: "str", type: compiler.StringType}], compiler.StringType,
        false,
        (str: string) => {
            let res = BigInt(0)
            for (const c of str) {
                res *= BigInt(256)
                res += BigInt(c.charCodeAt(0) % 256)
            }
            return "" + res
        }
    )
    externals.addFunction(
        "eval",
        [{name: "expr", type: compiler.StringType}], compiler.StringType,
        true,
        (expr: string) => {
            return wee_eval(expr)
        }
    )
    externals.addFunction(
        "assert_equals",
        [{name: "num", type: compiler.NumberType}], compiler.StringType,
        false,
        (num: number) => num === num
            ? "EQUALITY WORKS" : flags.EQUALITY_ERROR
    )
    externals.addFunction(
        "assert_number",
        [{name: "num", type: compiler.NumberType}], compiler.StringType,
        false,
        (num: number) => !isFinite(num) || isNaN(num) || num !== num + 1
            ? "NUMBERS WORK" : flags.NUMBER_ERROR
    )
    externals.addFunction(
        "assert_conversion",
        [{name: "str", type: compiler.StringType}], compiler.StringType,
        false,
        (str: string) => str.length === +str + "".length || !/^[1-9]+(\.[1-9]+)?$/.test(str)
            ? "Convert to Pastafarianism" : flags.CONVERSION_ERROR
    )
    externals.addFunction(
        // Wee is statically typed. Finding a way to confuse the VM is impossible.
        "assert_string",
        [{name: "str", type: compiler.StringType}], compiler.StringType,
        false,
        (str: string) => typeof str == "string" ? "WEE is statically typed. Sorry, confusing the VM is impossible."
            : flags.WEE_TOKEN
    )
    externals.addFunction(
        "assert_leet",
        [{name: "maybe_leet", type: compiler.NumberType}], compiler.StringType,
        false,
        (maybe_leet: number) => maybe_leet !== 0x1337 ? "WEE AIN'T LEET" : flags.WEE_R_LEET
    )
    return externals
}

export async function wee_exec(code: string) {
    try {
        const compiled = compiler.compile(code, get_headless_externals())
        const vm = new VirtualMachine(compiled.functions, compiled.externalFunctions)
        while (vm.state != VirtualMachineState.Completed) {
            vm.run(10000)
            await DoEvents() // Excited about this name! VB6 <3. Nothing beats the good ol' "On Error Resume Next"...
        }
        vm.restart()
    } catch (ex) {
        console.error(ex.message)
    }
}

if (require.main === module) {
    //eval_in_chrome("1+1")
    const wee = process.argv[2];
    //console.log(wee)
    wee_exec(wee)
        .then(_=>browserPromise)
        .then(b=>b.close())
        .then(_=>process.exit())
}