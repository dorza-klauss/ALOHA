# ALOHA

A simulation of random access protocol ALOHA and a comparison between its theoretical and simulated throughputs. 

### Pure ALOHA

The pure ALOHA protocol is a protocol used for data transfer. where a network sends out a data frame whenever there is a frame to send. There is no co-ordination between any other node in the network. Instead, the node waits for an acknowledgement, and in case of collisions, the terminal waits for a random period of time before retransmitting the message. As the number of users increase, we find that the probability of collision between the packets increase. 

The throughput for which is found to be maximum for pure ALOHA is 0.1839, which is found when the offered load is G = 0.5. Thus, the best traffic utilization one can hope for using ALOHA is 0.184 Erlangs. 

### Simulation 

The simulation was done in C++, and a huge part of what is written as a program was already published @ https://github.com/talmai/slotted-aloha/

Six different nodes were considered in the network, and communication takes place between nodes 1 & 2, 3 & 4, and 5 & 6. All frames are assumed to be of equal length, and the arrival of packets at the wireless nodes follow a Poisson distribution, and hence the packet inter arrival times follow the exponential distribution. 

The simulation begins at current_time = 0. The sender buffer is scanned for packet pkt, which can either complete its transmission at its earliest or is scheduled for transmission at the earliest. We then update the current_time to the earliest time when another packet can be transmitted such that it does not collide with a packet that is already being transmitted. 

If pkt is already transmitted then it is allowed to complete its transmission since current_time is updated suitably. If pkt is message packet then an acknowledgement packet is generated from the receiver packet. Otherwise, sending node is notified of the received acknowledgement and other transmissions are being carried out. 

If the pkt chosen is scheduled for transmission at current_time, we try transmitting the pkt, and a collision check is performed to check pkt if the collides with some other packet alreadt being transmitted or which begins transmission while pkt is being transmitted. If collision is detected, a random delay is initiated after which pkt is retransmitted. Otherwise, pkt completes it transmission and acknowledgement is generated as described above. 

The simulation is stopped when the absolute value of the difference between the throughput calculated and the theoretical throughput lies within a predefined value (0.03). You can play around with the given value and check at different values the plots between offered load(G) and throughput. 

### Plots: 

The theoretical and simulated throughput vs load graphs are plotted.

![.](https://github.com/dorza-klauss/ALOHA/blob/main/results/plotV3.jpeg)



