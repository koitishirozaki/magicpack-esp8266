# Magic Pack for ESP8266
Send magic packets to your PC using an ESP8266 connected through Wi-Fi.

# Setup
You need to assign some configurations in the `main.cpp` to be able to connect to your wi-fi and send the packet.

```c
const char *SSID = "mywifi";
const char *PASSWORD = "mywifi";
const char *MAC = "00:00:00:00:00";
const char *SECRET_TOKEN = "mysecrettoken";
const int PORT = 12345
```

These settings are pretty self-explanatory, but here we go:
- `SSID` is your wi-fi SSID
- `PASSWORD` is your wi-fi's password
- `MAC` is the MAC Address of the device you want to target
- `SECRET_TOKEN` is a secret token (duh) that you use for minimal security.
- `PORT` is the port in which the server will be running

I used Platform.IO as it was easier to setup the ESP project...

# Using
After uploading the code to your ESP8266, use the link:
``` 
mylocalip:12345/wake?token=mysecrettoken
```
Replace `mylocalip` by the ESP's local IP (LAN). 12345 by the `PORT` you set in the configuration. Also replace `mysecrettoken` by your own token

If everything is set correctly, you should see 
```
"Magic packet sent to {mac address}"
```

