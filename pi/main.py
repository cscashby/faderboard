#!/usr/bin/env python2

import OSC
import RFM69
from RFM69registers import *
import datetime
import time
import traceback

node_id = 1
network_id = 100
recipient_id = 2

# https://stackoverflow.com/questions/929103/convert-a-number-range-to-another-range-maintaining-ratio
def remap( x, oMin, oMax, nMin, nMax ):
    #range check
    if oMin == oMax:
        print "Warning: Zero input range"
        return None

    if nMin == nMax:
        print "Warning: Zero output range"
        return None

    #check reversed input range
    reverseInput = False
    oldMin = min( oMin, oMax )
    oldMax = max( oMin, oMax )
    if not oldMin == oMin:
        reverseInput = True

    #check reversed output range
    reverseOutput = False   
    newMin = min( nMin, nMax )
    newMax = max( nMin, nMax )
    if not newMin == nMin :
        reverseOutput = True

    portion = (x-oldMin)*(newMax-newMin)/(oldMax-oldMin)
    if reverseInput:
        portion = (oldMax-x)*(newMax-newMin)/(oldMax-oldMin)

    result = portion + newMin
    if reverseOutput:
        result = newMax - portion

    return result

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description="TODO")
    parser.add_argument('--address', default="172.16.10.51",                          
                        help='name/ip-address of Behringer X32 mixing desk')

    args = parser.parse_args()

    osc_client = OSC.OSCClient() #TODO: Refer to server when we have one
    osc_client.connect((args.address, 10023))
    osc_client.send(OSC.OSCMessage("/ch/01/mix/fader", [0.0]))
    time.sleep(1)
    osc_client.send(OSC.OSCMessage("/ch/01/mix/fader", [1.0]))
    time.sleep(1)
    osc_client.send(OSC.OSCMessage("/ch/01/mix/fader", [0.0]))
        
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
            data = radio.DATA
            print "%s from %s RSSI:%s" % (",".join([str(b).zfill(3) for b in data]), radio.SENDERID, radio.RSSI)
            i = 1
            debug = ""
            for b in data:
                message = "/ch/" + str(i).zfill(2) + "/mix/fader"
                val = [remap(b, 1, 255, 0.0, 1.0)]
                debug += " " +  str(i) + ":" + message + "=" + str(val)
                osc_client.send(OSC.OSCMessage(message, val))
                i += 1
            print debug
            if radio.ACKRequested():
                radio.sendACK()
    
    except KeyboardInterrupt:
        print "Exit"
    
    except Exception, err:
        traceback.print_exc()
    
    finally:
        print "shutting down"
        radio.shutdown()
