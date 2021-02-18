# Exceptions

Latest Arduino supports `build_flags = -D PIO_FRAMEWORK_ARDUINO_ENABLE_EXCEPTIONS`

platformio/espressif8266 version is 2.6.3. Build flag `PIO_FRAMEWORK_ARDUINO_ENABLE_EXCEPTIONS` is not supported.

## Steps to enable exceptions:

### CPP_FLAGS
```
build_flags = -fexceptions
build_unflags = -fno-exceptions
```

### library `stdc++-exc`

Edit file `C:\Users\***\.platformio\packages\framework-arduinoespressif8266\tools\platformio-build.py`

Find something like
```python
LIBS=[
  "hal", "phy", "pp", "net80211", "wpa", "crypto", "main",
  "wps", "bearssl", "axtls", "espnow", "smartconfig", "airkiss", "wpa2",
  "stdc++", "m", "c", "gcc"
],
```

Replace `stdc++` with `stdc++-exc`

## Memory usage

It increases bin size for 162.904 (16% of flash size)
