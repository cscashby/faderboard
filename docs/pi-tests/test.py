#!/usr/bin/python
from RFM69 import Radio, FREQ_433MHZ 
import datetime
import time

node_id = 1
network_id = 100
recipient_id = 2

with Radio(FREQ_433MHZ, node_id, network_id, isHighPower=True, verbose=True, promiscuousMode=True) as radio:
    print ("Starting loop...")
   
    rx_counter = 0
    tx_counter = 0

    while True:
        
        # Every 1 seconds get packets
        if rx_counter > 1:
            rx_counter = 0
            
            # Process packets
            for packet in radio.get_packets():
                print (packet)

        # Every 1 seconds send a message
        if tx_counter > 1:
            tx_counter=0

            # Send
            print ("Sending")
            if radio.send(2, "TEST", attempts=3, waitTime=100):
                print ("Acknowledgement received")
            else:
                print ("No Acknowledgement")

        print("Listening...", len(radio.packets), radio.mode_name)
        delay = 0.5
        rx_counter += delay
        tx_counter += delay
        time.sleep(delay)
