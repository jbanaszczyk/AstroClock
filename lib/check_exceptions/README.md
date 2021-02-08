#if exceptions are not supported yet

Exceptions should be supported in current version of Arduino, but...

https://github.com/platformio/platform-espressif8266/issues/237

Flag `build_flags = -D PIO_FRAMEWORK_ARDUINO_ENABLE_EXCEPTIONS` does not work

Steps to enable exceptions:
```
build_flags = -fexceptions
build_unflags = -fno-exceptions
```

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

#if exceptions are supported

* Remove calls: `check_try_catch()`
* Remove whole `lib`
* Fix `platformio.ini`:
  remove `build_flags = -fexceptions`
  remove `build_unflags = -fno-exceptions`
  add `build_flags = -D PIO_FRAMEWORK_ARDUINO_ENABLE_EXCEPTIONS`
