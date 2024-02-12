# zephyr-apps

## minimal

Minimal template app.

## wifi\_sensor

Sensor with Wi-Fi IPv6 connectivity.

### Simple OTA with mcumgr

Install
```
go install github.com/apache/mynewt-mcumgr-cli/mcumgr@latest
```

OTA upload
```
mcumgr --conntype "udp" --connstring [2a02:168:abcd::1]:1337 image upload build/zephyr/zephyr.signed.confirmed.bin
```
OTA list image
```
mcumgr --conntype "udp" --connstring [2a02:168:abcd::1]:1337 image list
```
OTA flash
```
mcumgr --conntype "udp" --connstring [2a02:168:abcd::1]:1337 reset
```

# Zephyr commands

```
west update
west blobs fetch hal_espressif
```
