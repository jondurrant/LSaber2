import paho.mqtt.client as mqtt
import json
import time
import sys
import os


if (len(sys.argv) != 2):
    print("Require target ID as parater")
    sys.exit()


targetId = sys.argv[1]

user=os.environ.get("MQTT_USER", "nob")
passwd=os.environ.get("MQTT_PASSWD", "nob")
host= os.environ.get("MQTT_HOST", "localhost")
port=int(os.environ.get("MQTT_PORT", "1883"))
print("MQTT %s:%d - %s\n"%(host,port, user))

ping_topic = "TNG/" + targetId + "/TPC/PING"
connected_topic = "TNG/" + user + "/LC/ON"

pong_topic = "TNG/" + targetId + "/TPC/PONG"
lc_topic = "TNG/" + targetId + "/LC/#"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe( lc_topic )
    client.subscribe( pong_topic )
        
    
    


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Rcv topic=" +msg.topic+" msg="+str(msg.payload))

client = mqtt.Client(client_id=user)
client.username_pw_set(username=user, password=passwd)
client.on_connect = on_connect
client.on_message = on_message
j = {'online':0}
p = json.dumps(j)
client.will_set(connected_topic, p, qos=1, retain=False) #set will

client.connect(host, port, 60)

client.loop_start()

time.sleep(3)
print("publishing connect")
j = {'online':1}
p = json.dumps(j)
infot = client.publish(connected_topic,p,retain=False,qos=1)
infot.wait_for_publish()

pingId = 0;
for i in range(10):
    j = {'id': pingId}
    p = json.dumps(j)
    print("Publishing ping %s"%p)
    infot = client.publish(ping_topic, p,retain=False, qos=1)
    infot.wait_for_publish()
    pingId = pingId + 1
    #time.sleep(0.1)



# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
#client.loop_forever()
time.sleep(10)