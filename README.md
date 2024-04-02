Build the project
======================
1. Install esp-idf 5.1.2 https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32/get-started/index.html
2. Intsall Git
3. Clone the project:
```
git clone --recurse-submodules https://github.com/szevczenko/hq_esp32_provisioning_app.git
```
4. Init submodules for hq_components:
```
cd hq_components
git submodule update --init --recursive
cd ..
```
5. Run ESP-IDF 5.1 CMD and enter to cloned folder.
6. build by running command:
```
idf.py build
```
7. flash esp32:
```
idf.py flash -p COM8
```
Commands command line
======================
Command line start on serial interface. You can connect by PuTTY or another program.
Commands:
----------------------
1. `get_mac` - get device MAC address. Example:
```
prod> get_mac                                                                   
94E6860F9BA0 
```
2. `join` - connect to WiFi
join  [--timeout=<t>] <ssid> [<pass>]                                           
  Join WiFi AP as a station                                                     
  --timeout=<t>  Connection timeout, ms                                         
        <ssid>  SSID of AP                                                      
        <pass>  PSK of AP 
Example:
```
prod> join ZTE_E25CC6 R2B2FEZJPG                                                
I (718120) connect: Connecting to 'ZTE_E25CC6'                                  
I (723240) connect: Connected 
```
3. `restart` - restart device.

Services
======================
MDNS
----------------------
For find device ip address in local network, device start mdns service. Example of python application for scanning device [link](https://github.com/szevczenko/bimbrownik_python/blob/main/scan_devices.py)

HTTP API
----------------------
1. POST /api/sn - set serial number. In body of message send serial number
2. GET /api/sn - get serial number. In body of response message sent serial number
3. POST /api/ota - set url where device tried download application
Response codes:
1. 200 - OK
2. 400 - FAIL
3. 500 - Internal server error
Example of post message:
```
import requests

url = f"http://{ip_address}:8000/api/sn"

# A POST request to the API
session = requests.Session()

response = session.post(url, "SERIAL_NUMBER")
print(response.text)
```

OTA service
----------------------
OTA service tried download application from URL sended by HTTP API.
