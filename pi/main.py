#!/usr/bin/env python2

import RFM69
from RFM69registers import *
import datetime
import time

node_id = 1
network_id = 100
recipient_id = 2

radio = RFM69.RFM69(RF69_433MHZ, node_id, network_id, True)
print "class initialized"
#print "reading all registers"
#results = radio.readAllRegs()
#for result in results:
#    print result
print "Performing rcCalibration"
radio.rcCalibration()
print "setting high power"
radio.setHighPower(True)
print "Checking temperature"
print radio.readTemperature(0)
#print "setting encryption"
#radio.encrypt("1234567891011121")
#print "sending blah to 2"
#if radio.sendWithRetry(2, "blah", 3, 20):
#    print "ack recieved"
print "reading"
try:
    while True:
        radio.receiveBegin()
        while not radio.receiveDone():
            time.sleep(.1)
        print "%s from %s RSSI:%s" % (",".join([str(b).zfill(3) for b in radio.DATA]), radio.SENDERID, radio.RSSI)
        if radio.ACKRequested():
            radio.sendACK()

except KeyboardInterrupt:
    print "Exit"

except:
    print "Error"

finally:
    print "shutting down"
    radio.shutdown()

