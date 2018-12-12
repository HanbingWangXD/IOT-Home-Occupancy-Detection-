import array 
from digi.xbee.devices import XBeeDevice,XBee16BitAddress,XBee64BitAddress,RemoteXBeeDevice
import demo_files

device = XBeeDevice('/dev/serial/by-id/usb-FTDI_FT231X_USB_UART_DN03FGOJ-if00-port0', 9600)

a=array.array('H',[2,4,6])
x=bytes(a)
#Example o/p: b'\x02\x00\x04\x00\x06\x00'

device.open()

#Send to Mac address
remote_device=RemoteXBeeDevice(device,XBee64BitAddress.from_hex_string("0013A200417669CD"))
device.send_data(remote_device,x)
##device.send_data_broadcast("Hello world")
device.close()