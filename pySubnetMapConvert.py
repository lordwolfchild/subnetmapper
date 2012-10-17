#!/usr/bin/python

from plistlib import *
import socket, struct
from ctypes import *
from optparse import OptionParser

usage = "usage: %prog [options] infile outfile"
parser = OptionParser(usage)
(options,args)=parser.parse_args()

if len(args)!=2:
    parser.error("incorrect number of arguments")

try:
    with open(args[0]) as f: pass
except IOError as e:
    parser.error('input file does not exist')

pl = readPlist(args[0])
outpfile = open(args[1],"w")

subnet_indices = pl['$objects'][pl['$top']['root']['CF$UID']]['NS.objects'];

outpfile.write(unicode('<?xml version="1.0" encoding="UTF-8"?>\n'))
outpfile.write(unicode('<SubnetMap fileformat="2" writer="SubnetMapper" version="2.0.0">\n'))

for subnet_index in subnet_indices:
  if (pl['$objects'][pl['$objects'][subnet_index['CF$UID']]['$class']['CF$UID']]['$classname']=='Subnet'):
    # Found a subnet
    sn_color_index = pl['$objects'][subnet_index['CF$UID']]['SubnetColor']['CF$UID'];
    sn_IP_index = pl['$objects'][subnet_index['CF$UID']]['SubnetIP']['CF$UID'];
    sn_NM_index = pl['$objects'][subnet_index['CF$UID']]['SubnetNM']['CF$UID'];
    sn_identifier_index = pl['$objects'][subnet_index['CF$UID']]['SubnetIdentifier']['CF$UID'];
    sn_description_index = pl['$objects'][subnet_index['CF$UID']]['SubnetDescription']['CF$UID'];
    sn_notes_index = pl['$objects'][subnet_index['CF$UID']]['SubnetNotes']['CF$UID'];

    # Available Keys: SubnetIdentifier, SubnetIP, SubnetDescription
    # SubnetColor, SubnetNM, SubnetNotes
    
    # We start with the interesting part, the color. It is saved as a string 
    # representation of the color channels as float values. 1 is max, so 
    # we have to split the floats up to single values, convert them to 
    # real floats, map them to the value range of 0-255 and pack them
    # back into a string which we can use:
    
    # unwrap the string and clean it up 
    colordata = pl['$objects'][sn_color_index]['NSRGB'].data.strip('\x00')
    # split it over whitespaces
    colordata_split = colordata.split()
    
    # now convert the single channels to integer values between 0 and 255.
    color_r = int(float(colordata_split[0])*255.0);
    color_g = int(float(colordata_split[1])*255.0);
    color_b = int(float(colordata_split[2])*255.0);
    
    # now pack everything back into a friendly colorstring as we all know 
    # it from the interwebs.
    color_str = "<color>#{0:02X}{1:02X}{2:02X}</color>".format(color_r,color_g,color_b);    

    # Ok, I lied a little bit, another interesting part is the ip address 
    # and the netmask. In the original fileformat they are saved as integers
    # and need to be converted into strings in the classical 4-byte long, 
    # dot-seperated format. 
    
    # get the integer
    ip_int=pl['$objects'][sn_IP_index] 

    # convert it into a ctype uint32 and get a pointer to it
    i = c_uint32(ip_int)
    pi = pointer(i)
    
    # cast the pointer to a 4 byte array and retrieve the bytes for 
    # string conversion. 
    pic=cast(pi,POINTER(c_ubyte*4))

    # store the finished string for later processing
    ip_str="<address>{0}.{1}.{2}.{3}</address>".format(pic[0][3], pic[0][2], pic[0][1], pic[0][0])

    # The same method can be used for the netmask.

    # get the integer
    nm_int=pl['$objects'][sn_NM_index] 

    # convert it into a ctype uint32 and get a pointer to it
    i = c_uint32(nm_int)
    pi = pointer(i)
    
    # cast the pointer to a 4 byte array and retrieve the bytes for 
    # string conversion. 
    pic=cast(pi,POINTER(c_ubyte*4))

    # store the finished string for later processing
    nm_str="<netmask>{0}.{1}.{2}.{3}</netmask>".format(pic[0][3], pic[0][2], pic[0][1], pic[0][0])

    # Convert the notes to a standard text..
    notes_str = pl['$objects'][pl['$objects'][sn_notes_index]['NSString']['CF$UID']]
    
    # store the description and identifier
    identifier_str  = pl['$objects'][sn_identifier_index]
    description_str = pl['$objects'][sn_description_index]

    # now just plug everything together. the 
    # subnet_block variable stores the complete xml block for this subnet.
    subnet_block = unicode("",errors='ignore')
    subnet_block = u"        <identifier>{0}</identifier>\n        {1}\n        {2}\n        <description>{3}</description>\n        <notes>{4}</notes>\n        {5}\n".format(identifier_str,ip_str,nm_str,description_str,notes_str,color_str)

    outpfile.write(unicode('    <subnet ipversion="IPv4">\n'))
    outpfile.write(subnet_block.encode('utf-8'))
    outpfile.write(unicode('    </subnet>\n'))

outpfile.write(unicode('</SubnetMap>\n'))    
    
outpfile.close()
    