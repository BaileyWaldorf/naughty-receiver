# naughty-receiver
This program utilized socket communication in C to simulate packet loss using RDT 3.0.

In my program the sender transmits the day of the week and ACK to the receiver which then generated a random number which seeds a response to either return the correct ACK, wrong ACK, corrupted ACK, or no ACK. Then the sender receives the response and sends the message again if necessary.


How to compile:
•	Receiver:
o	gcc receiver.c -o receiver
o	./receiver
•	Sender
o	gcc sender.c -o sender
o	./sender


Example Text output (Receiver):

Receiver just correctly received a message: Sunday 0
How do you respond?
(1) send a correct ACK; (2) send a corrupted ACK; (3) do not send ACK; (4) send a wrong ACK
Receiver incorrectly responds with ACK 1
Receiver just correctly received a duplicated message: Sunday 0
Receiver responds with ACK 0
Receiver just correctly received a message: Monday 1
How do you respond?
(1) send a correct ACK; (2) send a corrupted ACK; (3) do not send ACK; (4) send a wrong ACK
Receiver do not send ACK
Receiver just correctly received a duplicated message: Monday 1
Receiver responds with ACK 1
Receiver just correctly received a message: Tuesday 0
How do you respond?
(1) send a correct ACK; (2) send a corrupted ACK; (3) do not send ACK; (4) send a wrong ACK
A corrupted ACK is sent
Receiver just correctly received a duplicated message: Tuesday 0
Receiver responds with ACK 0
Receiver just correctly received a message: Wednesday 1

Example Text Output (Sender):

Lets start
Connected by ('127.0.0.1', 12096)
Sender sent a message: Sunday 0
Sender received an ACK with wrong sequency number, keep waiting
Continue waiting
Continue waiting
Continue waiting
Continue waiting
Continue waiting
Timeout. Send the message again
Sender sent a message: Sunday 0
Sender received a valid ACK for 0 , send next message
Sender sent a message: Monday 1
Continue waiting
Continue waiting
Continue waiting
Continue waiting
Continue waiting
Timeout. Send the message again
Sender sent a message: Monday 1
Sender received a valid ACK for 1 , send next message
