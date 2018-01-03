# Fader Board (AKA Fraken-Mixer)

Multi-channel DAW control designed for portable wireless musical theatre use

## Problem statement

Normally musical theatre shows involve a fairly high number of radio microphones and a decent sized band/orchestra.  Generally in order to avoid feedback and other issues, the sound engineer needs to closely monitor / mix and quickly change faders during each scene to minimise the number of mics 'live' at any one time, and adjust for differences in the actors' voices and song dynamics.

In a traditional proscenium theatre this is fairly easy - a space is reserved at the back for the mix position, and the mixer and processing equipment (as well as the source for any SFX) is at the rear of the hall.

Enter promenade, or immersive, musical theatre - where the cast and audience are often in the same space, and the audience move through a number of rooms as the cast and the action takes them.

This is a very new genre of musical theatre, and hasn't really been done on a professional scale before - at least to my knowledge - and so when I was asked to do [Parade in Hemel Hempstead's Frogmore Paper Mill by Vivo D'Arte](http://www.vivodarte.co.uk/news.htm) - which required 20 radio microphones, a 7 piece orchestra, all in 5 different spaces.

My approach at the time was to use a [Behringer Xtouch](https://www.musictri.be/Categories/Behringer/Computer-Audio/Desktop-Controllers/X-TOUCH/p/P0B1X) control surface, with a battery power unit and WiFi access client.  The result was good<sup>[1](#myfootnote1)</sup>, but it was a challenge because I only had 8 faders to use, couldn't change the 'page' or 'scene' myself, and couldn't trigger SFX or change speaker routing; I had a very capable team working with me who could do all of the above, so I managed, but it also was a very heavy box of tricks.

Another issue which arose over time was that the WiFi control was patchy - as the Xtouch isn't designed to be used with a spotty network connection, and being WiFi across a large space, the access client needed to disconnect and reconnect between multiple access points, disconnections were difficult - the Xtouch actually drops all the faders to -∞ so if you happen to touch them as it reconnects the channel dies unexpectedly. Not great.

So, what is required is a custom-built control surface which can control a number of channels of a mixing desk, without the constraints of a direct control solution like the Xtouch, and without the inherent problems with WiFi.

## Initial thoughts / concepts

### Scribble

![Scribbled picture of faderboard](https://raw.githubusercontent.com/cscashby/faderboard/master/hardware/notes/scribble.jpg "Scribbled picture of faderboard")

Not very tidy, but hopefully this helps illustrate what's supposed to be happening.

### Tech decision

I went for an Arduino base for this project as I need embedded reliability and performance, without the overheads and inherent risks of using a SoC like the Raspberry Pi.  Given the complexity and number of I/O connections needed, I am basing the solution on an AT Mega 2560 - I got hold of [this dev board](http://amzn.to/2lO5UGc), but will probably design my own board from scratch for compactness

### Wireless comms

I need a long-range, full duplex transmit/receive solution for reasonably small payloads with a limited amount of overhead / latency.  I came across [LoRa](https://www.lora-alliance.org/) which seems to fit the bill, and I bought a couple of [dev boards for the RA-02 chip](http://amzn.to/2ELpx9M) which uses 433 MHz (or a couple of other free to use frequenceis).  I haven't completed testing yet but it seems to work well for my purposes.  What's more as it's designed for IoT, multiple receivers can be used to widen the transmit range if needed.

### Faders

Key to the solution is going to be the quality of fader units available, and they have to be motorised so they can be preset to values, and so that the sound #2s can also control if required.

Rather than evaluate a whole bunch of faders, I decided to buy [replacement faders for the Xtouch unit](https://www.gear4music.com/PA-DJ-and-Lighting/Behringer-X-Touch-Motor-Fader-MF100T-Set-of-5/1WME) instead - they are not that expensive, and I know them to be good.  They are 10 kΩ linear slide pots with a fast, accurate motor, and a touch sensitivity (capacitive touch) pin.

Touch sensitivity isn't something I've previously thought about, but this is a great option as it means that I can minimise my payload sent over the wire to only those faders which are being touched - making the device hopefully far more responsive.

### Scribble strips

Every good sound engineer deserves scribble strips (or something!) - they are invaluable for Musical Theatre - as each scene has different DCA assignments running, you can very quickly forget where you are as an operator.

I wanted multiple colours - as colour is a great way to distinguish between each channel.

I came across [these little OLED displays](https://thepihut.com/products/adafruit-monochrome-0-96-128x64-oled-graphic-display) which seem perfect as they're small, bright and easy to control via SPI.  I haven't yet received them so I'll update here as to how suitable they are.

Controlling lots of them isn't directly supported by their libraries so I'll need to do some custom coding to modify them for multiple units - multiplexing them through multiple CS pins.

### Everything else

I decided to duplicate the work I did in another project - '[pi-showcontrol'](github.com/cscashby/pi-showcontrol) and have a small TFT screen and programmable buttons in a 'master section' in the box - that way I can use SFX and have some feedback, possibly even plain text sent back to me by the Sound #2s which will hopefully be very useful.

### Power supply

This is probably the thing I've thought the least about.  I have a very reliable, high power battery which will give me any DC voltage I like and sensibly monitor time remaining, so I may stick with that for now; so a 5.5mm/2.5mm DC power jack may be all I need - probably 12v to minimise voltage regulator heating and run the whole thing at 5v.  However, if I feel brave later on I may well implement a battery control / charge unit.

### Physical manufacture

In my mind at present, I think the best approach is going to be to create a number of different PCBs:

1. Faders - in groups of 8 (so two identical boards).
2. Master control section - the TFT (although this may just be a module bolted to the case), buttons and big 'go' buttons).
3. Control board - the Arduino, radio chips and logic required to get the thing running.
4. Power supply - not sure what this will entail yet - watch this space.

I'd like to keep the unit as light as possible, but it will need sensible shielding (likely with pass-through capacitors for the 433 MHz antennae) - so I will be building the case out of sheet aluminium, probably cut using a CNC mill, or laser cut by a third party fabrication shop.  All of the controls will be mounted on the top panel, the control / power boards likely on the bottom panel, and the rear will be used for wired connections for charging, firmware upgrades and diagnostics.

On the sides of the box (which will likely need to be thicker / better reinforced) I will have holes for the straps which I will wear to use the unit whilst walking around (in [ice cream tray style](http://www.promotional-images.co.uk/ProdImages/2f9f440d-2c25-4093-97b0-d831b5edaa2b/classic2-lg.jpg) without the ice creams)

### The other end

Again this is early stages of the thought process, but a LoRa chip (probably the development board itself) with an ethernet HAT, which delivers an OSC payload to the mixing desk or a mac running [qLab](http://www.figure53.com/qlab).  Some form of queuing will be required so that if I end up with more than one of the receivers, then a controller will be required to marshal, deduplicate and send on this queue.

## Footnotes

<a name="myfootnote1">1</a>: 'No sitting at the back for him – [cscashby] he too followed the audience around with his sound desk in an usherette’s tray, ensuring that every cast member was heard, no matter what part of the building they were in.' [from Musical Theatre Review](http://musicaltheatrereview.com/parade-frogmore-paper-mill-hemel-hempstead/)
