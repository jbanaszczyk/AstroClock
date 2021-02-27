# Install ESP8266 IoT Framework

## Fix `preBuildCertificates.py`

File `.pio/libdeps/nodemcu/ESP8266 IoT Framework/scripts/preBuildCertificates.py`

Line 46: `openssl = "C:/Program Files/Git/usr/bin/openssl.exe"`

Replace with: `openssl = "C:/Program Files/Git/usr/bin/openssl.exe"`

## npm install

**Important**: Do not skip `npm install`

```commandline
pio lib install "maakbaas/ESP8266 IoT Framework"
cd .pio\libdeps\nodemcu\ESP8266 IoT Framework
npm install
npm audit fix
npx browserslist@latest --update-db
```

Do not use `npm audit fix --force`

## Available build flags

| flag                                  | files                 | help                                                                                                                              |   	   	
|---                                    |---                    |---		                                                                                                                        |
| `-DREBUILD_HTML`                      | `html.h`              | [web-interface](https://github.com/maakbaas/esp8266-iot-framework/blob/master/docs/getting-started.md#editing-the-web-interface)  |  	
| `-DREBUILD_CONFIG`                    | `config.c` `config.h` | [config-manager](https://github.com/maakbaas/esp8266-iot-framework/blob/master/docs/config-manager.md)                            |
| `-DCONFIG_PATH=src/configuration.json`|                       | [config-manager](https://github.com/maakbaas/esp8266-iot-framework/blob/master/docs/config-manager.md)                            |
| `-DREBUILD_DASHBOARD`                 | `dash.h`              | [dashboard](https://github.com/maakbaas/esp8266-iot-framework/blob/master/docs/dashboard.md)                                      |
| `-DDASHBOARD_PATH=src/dashboard.json` |                       | [dashboard](https://github.com/maakbaas/esp8266-iot-framework/blob/master/docs/dashboard.md)                                      |
| `-DREBUILD_CERTS`                     | `certificates.h`      | [certificates](https://github.com/maakbaas/esp8266-iot-framework/blob/master/docs/fetch.md)                                       |

`openssl = "C:/Program Files/Git/usr/bin/openssl.exe"`
