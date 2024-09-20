import React, { useState, useEffect } from 'react';
import { 
  StyleSheet,
  Text, 
  View, 
  ScrollView,
  Image,
  TouchableOpacity, 
  ImageBackground
} from 'react-native';

const SERVER = 'http://192.168.17.24'; 
const PORT_ADDRESS = ':5000';
const SERVER_URL = SERVER + PORT_ADDRESS;

const TANK_HEIGHT = 30;

const RED = "#FF5000";
const GREEN = "#00FF00";
const BLUE = "#0000FF";
const YELLOW = "#FFFF00";
const WHITE = "#FFFFFF";
const BLACK = "#000000";
const GREY = "#404040";


export default function App() {
  // Device functions status indicator
  const [systemStatus, setSystemStatus] = useState(true);
  const [buzzerStatus, setBuzzerStatus] = useState(false);
  const [pumpStatus, setPumpStatus] = useState(false);
  const [WaterLevel, setDistanceToWater] = useState('0');
  const [waterMglValue, setWaterMglValue] = useState('0');
  const [pumpControlMode, setPumpControlMethod] = useState("automatic");
  const [pumpManualControlledStatus, setPumpManualControlledStatus] = useState(false);

  // Device and Server indicator status track, default 'Checking'.
  const [serverConnectionStatus, setServerConnectionStatus] = useState(null);
  const [deviceStatus, setDeviceStatus] = useState(null);


  const fetchStatus = async () => {
    try {
      const response = await fetch(`${SERVER_URL}/get-status`);
      const data = await response.json();
      setSystemStatus(data["system_status"]);
      setBuzzerStatus(data["buzzer_status"]);
      setPumpStatus(data["pump_status"]);
      setWaterMglValue(data["water_mgl_value"]);
      setPumpControlMethod(data["pump_control_method"]);
      setPumpManualControlledStatus(data['pump_manual_controlled_status']);
      setServerConnectionStatus(true);
      setDeviceStatus(data["device_status"])
    } catch (error) {
      console.log('Error fetching status:', error);
      setDeviceStatus(null)
      setServerConnectionStatus(false)
    }
  };

  const systemTurnOnOff = async () => {
    try {
      const response = await fetch(`${SERVER_URL}/get-system-status`);
      const data = await response.json();
      let currentSystemStatus = data["system_status"];

      if (currentSystemStatus){
        try{
          await fetch(`${SERVER_URL}/turn-off-system`, { method: 'POST' });
        } catch (error){
          console.log("Error : off system:", error);
        }
      }
      else {
        try{
          await fetch(`${SERVER_URL}/turn-on-system`, { method: 'POST' });
        } catch (error){
          console.log("Error : on system:", error);
        }
      }

    } catch (error) {
      console.log('Error : toggling system:', error);
    }
  };

  const pumpTurnOnOffAutoMatic = async () => {
    try{
      // Get Current Pump Control Mode
      const response = await fetch(`${SERVER_URL}/get-pump_control_method`);
      const data1 = await response.json()
      let currentPumpControlMode = data1["pump_control_method"];

      const response2 = await fetch(`${SERVER_URL}/get-pump-manual-controlled-status`);
      const data2 = await response2.json();
      let currentPumpManualControlledStatus = data2["pump_manual_controlled_status"];

      if (currentPumpControlMode=="automatic"){
        try {
          await fetch(`${SERVER_URL}/turn-on-pump-manually`, { method: 'POST' });
        } catch (error) {
          console.log('Error : turn on pump:', error);
        }
      }
      else if (currentPumpControlMode=="manual" && currentPumpManualControlledStatus){
        try {
          await fetch(`${SERVER_URL}/turn-off-pump-manually`, { method: 'POST' });
        } catch (error) {
          console.log('Error : turn off pump:', error);
        }
      }
      else{
        try {
          await fetch(`${SERVER_URL}/turn-on-automatic-pump-control-mode`, { method: 'POST' });
        } catch (error) {
          console.log('Error : automatic pump:', error);
        }
      }

    } catch (error) {
      console.log('Error : pump control', error);
    }
  }


  useEffect(() => {
    const interval = setInterval(() => {
      fetchStatus();
    }, 1000);
    return () => clearInterval(interval);
  }, []);

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <View  style={styles.mainContainer}>
        <ImageBackground source={require('./assets/bg.jpg')} resizeMode="cover" style={styles.backGroundImage}>
          <View style={styles.subContainer}>
            <Text style={styles.title}>Smart Water Tank System</Text>
            <View style={styles.status}>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/level.png')} style={{ width: 36, height: 36}} />
                </View>
                <Text style={styles.statusTitle}>Water Level : </Text>
                <Text style={[styles.statusValue, {color: WaterLevel === '0' ? GREEN : RED}]}>{WaterLevel} cm</Text>
              </View>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/quality.png')} style={{ width: 38, height: 38}} />
                </View>
                <Text style={styles.statusTitle}>Water quality : </Text>
                <Text style={[styles.statusValue, {color: waterMglValue < 5 || waterMglValue > 8? GREEN : RED}]}>{waterMglValue} Mgl</Text>
              </View>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/system.png')} style={{ width: 40, height: 40 }} class={styles.icon}/>
                </View>
                <Text style={styles.statusTitle}>System Status : </Text>
                <Text style={[styles.statusValue, {color : systemStatus ? GREEN : RED}]}>{systemStatus ? 'ON' : 'OFF' }</Text>
              </View>
          
              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/pump.png')} style={{ width: 40, height: 40 }} />
                </View>
                <Text style={styles.statusTitle}>Pump Status : </Text>
                <Text style={[styles.statusValue , {color: pumpStatus ? GREEN : RED}]}>{pumpStatus ? 'ON' : 'OFF' }</Text>
              </View>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/buzzer.png')} style={{ width: 38, height: 38 }} />
                </View>
                <Text style={styles.statusTitle}>Buzzer Status : </Text>
                <Text style={[styles.statusValue, {color: buzzerStatus ? RED : RED}]}>{buzzerStatus ? 'ON' : 'OFF' }</Text>
              </View>

            </View>

            <View style={styles.buttonContainer}>
              <View style={styles.buttonContainer}>
                <Text style={styles.buttonText}>System : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={systemTurnOnOff}>
                  <Text style={styles.btnText}>{systemStatus ? 'OFF' : 'ON'}</Text>
                </TouchableOpacity>
              </View>

              <View style={styles.buttonContainer}>
                <Text style={styles.buttonText}>Pump : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={pumpTurnOnOffAutoMatic}>
                  <Text style={styles.btnText}>
                    {
                      (pumpControlMode == 'automatic') ? 'Manual/On' : 
                      (pumpControlMode == 'manual' && pumpManualControlledStatus) ? 'Manual/Off' : 'Automatic'
                    }
                    </Text>
                </TouchableOpacity>
              </View>

            </View>
          </View>

          <View style={styles.statusContainer}>
            <View style={styles.statusSubContainer}>
              <Text style={styles.statusTitleText}>Server Connection : </Text>
              <Text>{(serverConnectionStatus === null) ? "Unknown" : serverConnectionStatus ? 'Online' : 'Offline'}</Text>
              <View style={[styles.statusCircle, {backgroundColor: serverConnectionStatus === null ? GREY :serverConnectionStatus ? GREEN : RED}]}></View>
            </View>

            <View style={styles.statusSubContainer}>
              <Text style={styles.statusTitleText}>Device Status : </Text>
              <Text>{(deviceStatus === null) ? "Unknown" : deviceStatus ? "Online" : "Offline" }</Text>
              <View style={[styles.statusCircle, {backgroundColor: deviceStatus === null ? GREY :deviceStatus ? GREEN : RED}]}></View>
            </View> 
          </View>
        </ImageBackground>
      </View>
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  mainContainer :{
    height: '100%',
    width: '100%',
    margin: 0,
  },
  container: {
    flexGrow: 1,
    padding: 0,
    margin: 0,
  },
  backGroundImage: {
    flex: 1,
    justifyContent: 'center',
    backgroundColor: 'red',
    alignItems: 'center',
  },
  subContainer : {
    margin: 30,
    alignItems: 'center',
  },
  title: {
    fontSize: 33,
    fontWeight: 'bold',
    textAlign: 'center',
    color: '#5555FF'
  },
  status: {
    marginTop: 30,
    marginBottom: 20,
    backgroundColor: 'rgba(255, 255, 255,  0.4)',
    padding : 30,
    borderRadius: 16
  },
  statusTitle: {
    fontWeight: 'bold',
    fontSize: 18,
    marginVertical: 5,
  },
  statusValue : {
    fontSize: 18,
    marginVertical : 5,
    fontWeight: 'bold'
  },
  buttonContainer: {
    marginTop: 40,
    alignItems: 'center',
    backgroundColor: 'rgba(255, 255, 255,  0.1)',
    padding: 10,
    borderRadius: 16
  },
  outputContainer: {
    height: 40,
    flexDirection: 'row',
    margin: 7,
  },
  icon: {
    margin : 20,
  },
  imageContainer: {
    width : 50,
    height: 50,
    paddingRight: 70,
  },
  controlButton : {
    height: 30,
    marginTop: 10,
    borderRadius: 10,
    width: 100,
    padding: 6,
    alignItems: 'center',
    backgroundColor: '#5555FF',
  },
  btnText :{
    color: '#FFFFFF',
    fontWeight: 'bold'
  },
  statusContainer : {
    marginTop: 0,
    flexDirection: 'column',
    textAlign: 'center',
    alignItems: 'center',
    width: '100%',
    backgroundColor: 'rgba(255, 255, 255,  0.1)',
    display: ''
  },
  statusSubContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    marginTop: 4
  },
  statusCircle: {
    width: 12,
    height: 12,
    borderRadius: 6,
    marginLeft: 10,
    marginTop: 3
  },
  statusTitleText:{
    fontWeight: 'bold',
  },
  buttonContainer: {
    flexDirection: 'row',
    alignItems: 'center',
  },
  buttonText :{
    textAlign: 'center',
    paddingTop: 8,
    fontSize: 16
  }
});
