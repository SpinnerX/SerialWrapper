"""
Datasheet: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
"""
import serial
import math
import time

class Coordinate:
    def __init__(self, latitude:float, longitude:float):
        self.latitude = latitude
        self.longitude = longitude
    
    def isValid(self):
        return not (math.isnan(self.latitude) or math.isnan(self.longitude))

    def __repr__(self):
        return str(self)
    
    def __str__(self):
        if(math.isnan(self.latitude) or math.isnan(self.longitude)):
            return "( NaN, NaN )"
        latitudeCharacter = "N" if self.latitude > 0 else "S"
        longitudeCharacter = "E" if self.longitude > 0 else "W"

        return "( {:6.4f} {}, {:6.3f} {} )".format(abs(self.latitude), latitudeCharacter, abs(self.longitude), longitudeCharacter)

class GPS:
    def __init__(self, port:str|int, baudrate:int):
        self.gps = serial.Serial(port, baudrate)

        # No fix was ever recorded: In
        self.lastCoordinateUpdate = -1
        self.coordinate = Coordinate( math.nan, math.nan ) # Latitude, Longitude
        

    def read(self):
        
        gpsRawOutput = self.gps.read(128).decode("utf-8")
        # This will cut off parts of the message if the read reads part of a message.
        msgs = gpsRawOutput.split("\r\n")[:-1]

        # All valid NMEA messages start with "$"
        def validateNMEAMsg(msg:str):
            if(len(msg) == 0):
                return False
            return msg[0] == "$"

        msgs = [msg for msg in filter(validateNMEAMsg, msgs)]
        msgs = [GPSMessage(msg) for msg in msgs]

        for msg in msgs:
            if(msg.messageType == "GGA"):
                if(msg.valid):
                    self.coordinate = Coordinate(msg.latitude, msg.longitude)
                    self.lastCoordinateUpdate = time.time()
            elif(msg.messageType == "GLL"):
                if(msg.valid):
                    self.coordinate = Coordinate(msg.latitude, msg.longitude)
                    self.lastCoordinateUpdate = time.time()
            elif(msg.messageType == "RMC"):
                if(msg.valid):
                    self.coordinate = Coordinate(msg.latitude, msg.longitude)
                    self.lastCoordinateUpdate = time.time()


# class Fix

class GPSMessage:
    def __init__(self, NMEAmsg:str):
        args = NMEAmsg.split(",")
        self.talker = args[0][1:3]
        self.messageType = args[0][3:6]

        self.unimplemented = False

        match(self.messageType):
            case "GGA": # GNSS Fix Data
                # print(args)
                self.valid = args[6] != "0"
                self.fixed = self.valid
                if(self.valid):
                    self.currentTime = int(args[1][0:2]) * 60 * 60 + int(args[1][2:4]) * 60 + float(args[1][4:])
                    self.latitude = int(args[2][0:2]) + float(args[2][2:]) / 60
                    if(args[3] == "S"):
                        self.latitude *= -1;
                    self.longitude = int(args[4][0:3]) + float(args[4][3:]) / 60
                    if(args[5] == "W"):
                        self.longitude *= -1
                    

                    self.satelliteCount = int(args[7])
                    self.hdop = float(args[8])
                    self.msgAltitude = float(args[9])
                    self.geoidSeperation = float(args[11])


            case "GLL": # Latitude and longitude, with time of position fix and status
                self.valid = args[6] == "A"
                self.fixed = self.valid
                if(self.valid):
                    self.currentTime = int(args[5][0:2]) * 60 * 60 + int(args[5][2:4]) * 60 + float(args[5][4:])
                    self.latitude = int(args[1][0:2]) + float(args[1][2:]) / 60
                    if(args[2] == "S"):
                        self.latitude *= -1;
                    self.longitude = int(args[3][0:3]) + float(args[3][3:]) / 60
                    if(args[4] == "W"):
                        self.longitude *= -1
                    
            case "GPQ": # Poll message
                self.unimplemented = True
            case "GRS": # GNSS Range Residuals
                self.unimplemented = True
            case "GSA": # Active Satellites
                # self.mode

                self.fixed = self.valid = args[2] != "1"

                self.satellitesInView = []
                for i in range(3, 15):
                    if(len(args[i]) != 0):
                        self.satellitesInView.append(int(args[i]))
            case "RMC": # Recommended Minimum Data
                self.valid = args[2] != "0"
                self.fixed = self.valid
                if(self.valid):
                    self.currentTime = int(args[1][0:2]) * 60 * 60 + int(args[1][2:4]) * 60 + float(args[1][4:])
                    self.latitude = int(args[3][0:2]) + float(args[3][2:]) / 60
                    if(args[4] == "S"):
                        self.latitude *= -1;
                    self.longitude = int(args[5][0:3]) + float(args[5][3:]) / 60
                    if(args[6] == "W"):
                        self.longitude *= -1 

                    self.speed = float(args[7]) if(len(args[7])) else math.nan
                    self.courseOverGround = float(args[8]) if(len(args[8])) else math.nan
                    self.date = (int(args[9][0:2]),int(args[9][2:4]), int(args[9][4:]))
            case _:
                raise Exception("Invalid NMEA Message")


    
    def __repr__(self):
        return str(self)

    def __str__(self):
        return self.talker + ":" + self.messageType


if(__name__ == "__main__"):
    gps = GPS("/dev/tty.usbmodem101", 56000)
    while(True):
        gps.read()
        if(gps.coordinate.isValid()):
            print(gps.coordinate)
