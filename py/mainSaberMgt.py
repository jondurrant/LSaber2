#===============================================================================
# Management application for Saber Lights
# Jon Durrant 
# 14-Jan-2022
#===============================================================================
from flask import Flask,flash, redirect, request, send_from_directory
from flask import url_for, render_template
import json
import os
import sys
import threading
import logging

from mqttAgent import  MQTTAgent
from mqttObserver import MQTTObserver
from mqttRouterPing import MQTTRouterPing
from mqttRouterTwinClient import  MQTTRouterTwinClient



import datetime



# set the project root directory as the static folder, you can set others.
app = Flask(__name__, static_url_path='/static')

#===============================================================================
# Root page redirect
#===============================================================================
@app.route('/')
def route():
    #return redirect("/static/html/index.html")
    return redirect("/static/html/Saber.html")


#===============================================================================
# Get current saber configuration
#===============================================================================
@app.route('/api/getSabers', methods = ['GET','POST'])
def getSabers():
    columns=[
        "drgb",
        "nrgb",
        "days",
        "daye",
        "dseq",
        "nseq",
        "timer",
        "temp",
        "on",
        "day",
        "clock"
        ]
    
    select= ["clientId"]
    asColumn = ["clientId"]
    for c in columns:
        select.append("reported.%s"%c)
        asColumn.append(c)
    
    #Make sure we only pull back data on actual saber lights, as other things could be in group
    where = {'column': "reported.temp", 'op': ">", 'value': 0}
    
    
    d = twinClient.query(select, asColumn, where, orient="records")
    
    if ("res" in d):
        
        table = recordsToTable(d["res"], "clientId")
    
        return table 
    
    return {}
    
#===============================================================================
# Set sabers to given values
#===============================================================================
@app.route('/api/setSabers', methods = ['GET','POST'])
def setSabers():
    if (request.json != None):
        targetId = request.json.get("target", None)
        delta = request.json.get("delta", {})
        if (targetId):
            targets = [targetId]
        else:
            targets = []
            
        logging.debug("Requesting %s, %s"%(json.dumps(delta), json.dumps(targets)))
        twinClient.update(delta, targets)
        
    return {}
      
#===============================================================================
# Turn all sabers on or off.
#===============================================================================
@app.route('/api/saberOn', methods = ['GET','POST'])
def saberOn():
    if (request.json != None):
        on = request.json.get("on", True)
        delta = {"on": on}            
        twinClient.update(delta, [])
        
    return {}  
    
#===============================================================================
# Convert a Pandas record json format into Google charts table format
#===============================================================================
def recordsToTable(recs, indexCol):
    typeConv={ str: "string",
               int: "number",
               float:  "number",
               bool: "boolean",
               datetime: "datetime"
              }
    table = {"cols": [], "rows": []}
    
    #print("rec=%s\n"%json.dumps(recs))
    #print("empty=%s\n"%json.dumps(table))
    row = recs[0]
    
    for c in row:
        cell = row[c]
        t=type(cell)
        nt = typeConv.get(t, "string")
        #print("Col: id:%s orig: %s type:%s label:%s"%(c, t, nt, c))
        table["cols"].append({"id": c, "type": nt, "label": c})

    #print("cols=%s\n"%json.dumps(table))
    
    for r in recs:
        list=[]
        
        for ch in table["cols"]:
            c = ch["id"]
            list.append({"v": r[c]})
            
        row = {}
        row["c"]=list
        table["rows"].append(row)
        
    #print("rows=%s\n"%json.dumps(table))
    return table
        
            
#===============================================================================
# Start up the MQTT service
#===============================================================================
def startMQTT():
    global twinClient
    
    #MQTT Credentials and targets
    mqttUser=os.environ.get("MQTT_USER")
    mqttPwd=os.environ.get("MQTT_PASSWD")
    mqttTarget= os.environ.get("MQTT_HOST")
    mqttPort=int(os.environ.get("MQTT_PORT"))
    mqttCert=os.environ.get("MQTT_CERT", None)
    tls=""
    if (mqttCert != None):
        tls="TLS"
    logging.info("MQTT %s:%d %s - %s\n"%(mqttTarget,mqttPort,tls,mqttUser))
    
    #The MQTT Client Agent
    mqttAgent = MQTTAgent(mqttUser)
    mqttAgent.credentials(mqttUser, mqttPwd)
    mqttAgent.mqttHub(mqttTarget, mqttPort, True, mqttCert)
    
    #Consigure the observers and routers
    mqttObs = MQTTObserver()
    pingRouter = MQTTRouterPing(mqttUser)
    twinClient = MQTTRouterTwinClient(mqttUser, "saber", mqttAgent)
    
    #Add observers and reouter to client agent
    mqttAgent.addObserver(mqttObs)
    mqttAgent.addRouter(pingRouter)
    mqttAgent.addRouter(twinClient)
    
    mqttAgent.start()

     
     
def setupApp():
    LOGLEVEL = os.environ.get('LOGLEVEL', 'INFO').upper()
    logging.basicConfig(level=LOGLEVEL, 
                    format= '[%(asctime)s] {%(name)s:%(lineno)d} %(levelname)s - %(message)s')
    
    app.secret_key = 'LCARS'
    app.config['SESSION_TYPE'] = 'filesystem'
    
    #Run MQTT Aget in a thread    
    thread = threading.Thread(target = startMQTT)
    thread.start()  
    
setupApp()


if __name__ == "__main__":
    
    app.run(host="0.0.0.0")
    
    
    