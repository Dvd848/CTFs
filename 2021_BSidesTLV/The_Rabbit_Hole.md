# The Rabbit Hole
* Category: Misc
* 250 Points
* Solved by the JCTF Team

## Description

> My logs are always leaked, do you think you can read them?
> 
> tcp://ctf:ctf@the-rabbit-hole.ctf.bsidestlv.com:5672


## Solution

Port 5672 is assigned to the `RabbitMQ` protocol:

> RabbitMQ is an open-source message-broker software. ([Source](https://en.wikipedia.org/wiki/RabbitMQ))

Wikipedia has a basic Python example showing how to receive messages from a queue. Based on that, we create the following script:

```python
import pika

def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)

connection = pika.BlockingConnection(pika.ConnectionParameters('the-rabbit-hole.ctf.bsidestlv.com', 5672, '/'))
channel = connection.channel()
channel.queue_declare(queue="hello")
print(" [*] Waiting for messages. To exit press Ctrl+C")
channel.basic_consume(queue="hello", on_message_callback=callback)
channel.start_consuming()
```

We run and get the following output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/The_Rabbit_Hole]
└─$ python3 test.py
Traceback (most recent call last):
  File "/media/sf_CTFs/bsides/The_Rabbit_Hole/test.py", line 6, in <module>
    connection = pika.BlockingConnection(pika.ConnectionParameters('the-rabbit-hole.ctf.bsidestlv.com', 5672, '/'))
  File "/home/user/.local/lib/python3.9/site-packages/pika/adapters/blocking_connection.py", line 360, in __init__
    self._impl = self._create_connection(parameters, _impl_class)
  File "/home/user/.local/lib/python3.9/site-packages/pika/adapters/blocking_connection.py", line 451, in _create_connection
    raise self._reap_last_connection_workflow_error(error)
pika.exceptions.ProbableAuthenticationError: ConnectionClosedByBroker: (403) 'ACCESS_REFUSED - Login was refused using authentication mechanism PLAIN. For details see the broker logfile.'
```

So, we add the credentials we've received from the description:

```python
credentials = pika.PlainCredentials('ctf', 'ctf')
connection = pika.BlockingConnection(pika.ConnectionParameters('the-rabbit-hole.ctf.bsidestlv.com', 5672, '/', credentials))
```

It works, but nothing arrives:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/The_Rabbit_Hole]
└─$ python3 test.py
 [*] Waiting for messages. To exit press Ctrl+C
^C
```

We can try to connect to an exchange by adding:

```python
result = channel.queue_declare(queue='')
queue_name = result.method.queue
print(f" [-] Queue Name: {queue_name}")

channel.queue_bind(exchange='', queue=queue_name)
```

But we fail since we can't connect to the default exchange:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/The_Rabbit_Hole]
└─$ python3 solve.py
 [-] Queue Name: amq.gen-9z0AlTByGDHenZBtq-tMuQ
Traceback (most recent call last):
  File "/media/sf_CTFs/bsides/The_Rabbit_Hole/solve.py", line 15, in <module>
    channel.queue_bind(exchange='', queue=queue_name)
  File "/home/user/.local/lib/python3.9/site-packages/pika/adapters/blocking_connection.py", line 2570, in queue_bind
    self._flush_output(bind_ok_result.is_ready)
  File "/home/user/.local/lib/python3.9/site-packages/pika/adapters/blocking_connection.py", line 1339, in _flush_output
    raise self._closing_reason  # pylint: disable=E0702
pika.exceptions.ChannelClosedByBroker: (403, 'ACCESS_REFUSED - operation not permitted on the default exchange')
```

What if we try a different exchange, such as 'test'?

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/The_Rabbit_Hole]
└─$ python3 solve.py
 [-] Queue Name: amq.gen-uj30aPm97CkwSsOoK8W_1Q
Traceback (most recent call last):
  File "/media/sf_CTFs/bsides/The_Rabbit_Hole/solve.py", line 15, in <module>
    channel.queue_bind(exchange='test', queue=queue_name)
  File "/home/user/.local/lib/python3.9/site-packages/pika/adapters/blocking_connection.py", line 2570, in queue_bind
    self._flush_output(bind_ok_result.is_ready)
  File "/home/user/.local/lib/python3.9/site-packages/pika/adapters/blocking_connection.py", line 1339, in _flush_output
    raise self._closing_reason  # pylint: disable=E0702
pika.exceptions.ChannelClosedByBroker: (404, "NOT_FOUND - no exchange 'test' in vhost '/'")
```

It can't be found. According to the docs, there is no way for a client to enumerate the list of existing exchanges. However, we have a hint in the description:

> My logs are always leaked, do you think you can read them?

Let's try "logs", after all, it's right there in the [official tutorial](https://www.rabbitmq.com/tutorials/tutorial-three-python.html):

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/The_Rabbit_Hole]
└─$ python3 solve.py
 [-] Queue Name: amq.gen-dLgzc-twerpLr16sAgJbhA
 [*] Waiting for logs. To exit press CTRL+C
 [x] Received: b'BSidesTLV2021{Sl1d_D0wn_4_r4bb1t_H0l3}'
 [x] Received: b'BSidesTLV2021{Sl1d_D0wn_4_r4bb1t_H0l3}'
 [x] Received: b'BSidesTLV2021{Sl1d_D0wn_4_r4bb1t_H0l3}'
^C
```

The final script:

```python
#!/usr/bin/env python
import pika

def callback(ch, method, properties, body):
    print(f" [x] Received: {body}")

credentials = pika.PlainCredentials('ctf', 'ctf')
connection = pika.BlockingConnection(pika.ConnectionParameters('the-rabbit-hole.ctf.bsidestlv.com', 5672, '/', credentials))
channel = connection.channel()

result = channel.queue_declare(queue='')
queue_name = result.method.queue
print(f" [-] Queue Name: {queue_name}")

channel.queue_bind(exchange='logs', queue=queue_name)

print(' [*] Waiting for logs. To exit press CTRL+C')

channel.basic_consume(queue = queue_name, on_message_callback = callback, auto_ack = True)

channel.start_consuming()
```


