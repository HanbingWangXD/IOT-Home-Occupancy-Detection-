import array 
from digi.xbee.devices import XBeeDevice,XBee64BitAddress,RemoteXBeeDevice

device = XBeeDevice('/dev/serial/by-id/usb-FTDI_FT231X_USB_UART_DN03FGOJ-if00-port0', 9600)

remote_device=RemoteXBeeDevice(device,XBee64BitAddress.from_hex_string("0013A200417669CD"))
    

device.open()

def dat_rec_callback(message):    
    print("\n",list(message.data)) #Byte arrary to list
    print(message.data.decode()) #Byte array to string
    print(message.remote_device.get_64bit_addr(),"\n")
    abc=1

global abc
print("Start")

try:
    device.add_data_received_callback(dat_rec_callback)
    print("waiting for Data\n")
    input()
    
    
    
finally:
    device.close()
