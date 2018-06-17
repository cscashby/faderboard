#!/usr/bin/env python2

import RFM69
from RFM69registers import *
import datetime
import time

node_id = 1
network_id = 100
recipient_id = 2

test = RFM69.RFM69(RF69_433MHZ, node_id, network_id, True)
print "class initialized"
#print "reading all registers"
#results = test.readAllRegs()
#for result in results:
#    print result
print "Performing rcCalibration"
test.rcCalibration()
print "setting high power"
test.setHighPower(True)
print "Checking temperature"
print test.readTemperature(0)
#print "setting encryption"
#test.encrypt("1234567891011121")
#print "sending blah to 2"
#if test.sendWithRetry(2, "blah", 3, 20):
#    print "ack recieved"
print "reading"
while True:
    test.receiveBegin()
    while not test.receiveDone():
        time.sleep(.1)
    print "%s from %s RSSI:%s" % (",".join([str(b).zfill(3) for b in test.DATA]), test.SENDERID, test.RSSI)
    if test.ACKRequested():
        test.sendACK()
print "shutting down"
test.shutdown()
