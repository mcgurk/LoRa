## MQTT_timeout_ESP.ino

- Sends "online"-message to "kellokytkin"-topic when powered on.
- MQTT_KEEPALIVE default is 15s (https://pubsubclient.knolleary.net/api#setKeepAlive)
- lastwill is "offline"-message to "kellokytkin"-topic (after MQTT_KEEPALIVE triggers on MQTT-broker)
- (Do not use retain-messages. If you do, restarting e.g. Node-RED it stops or starts socket. Socket might be used from another remote control.)
