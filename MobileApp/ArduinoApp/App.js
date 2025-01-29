import React, { useState, useEffect } from 'react';
import { 
  StyleSheet,
  Text, 
  View, 
  ScrollView,
  Image,
  TouchableOpacity, 
  ImageBackground,
  TextInput
} from 'react-native';

const SERVER = "http://xxx.xxx.xxx.xxx";
const PORT_ADDRESS = ':5000';
const SERVER_URL = SERVER + PORT_ADDRESS;

const RED = "#FF0000"; 
const GREEN = "#00FF00";
const DARK_GREEN = "#23e027"; 
const DARK_YELLOW = "#fcd705"; 
const GREY = "#404040";
const OPACITY_INACTIVE = 0.5;
const OPACITY_ACTIVE = 1;


export default function App() {
  // Device functions status indicator
  const [systemStatus, setSystemStatus] = useState(false);
  const [buzzerStatus, setBuzzerStatus] = useState(false);
  const [pumpStatus, setPumpStatus] = useState(false);
  const [waterLevelRate, setWaterLevelRate] = useState(0);
  const [waterLevel, setWaterLevel] = useState('0');
  const [tankHeight, setTankHeight] = useState(0);
  const [waterQuality, setWaterQuality] = useState("critical");
  const [pumpControlMode, setPumpControlMethod] = useState("automatic");
  const [pumpManualControlledStatus, setPumpManualControlledStatus] = useState(false);

  // Device and Server indicator status track, default is null :  'Checking'.
  const [serverConnectionStatus, setServerConnectionStatus] = useState(null);
  const [deviceStatus, setDeviceStatus] = useState(null);
  

  // Function for fetching with timeout : https://stackoverflow.com/
  const fetchWithTimeout = (url, options, timeout = 2000) => {
    return Promise.race([
      fetch(url, options),
      new Promise((_, reject) => setTimeout(() => reject(new Error("Request timed out")), timeout))
    ]);
  };
  
  // Get the device status
  const fetchStatus = async () => {
    try {
      const response = await fetchWithTimeout(`${SERVER_URL}/get-status`);
      const data = await response.json();
      setSystemStatus(data["system_status"]);
      setBuzzerStatus(data["buzzer_status"]);
      setPumpStatus(data["pump_status"]);
      setWaterLevel(data["water_level"]);
      setWaterQuality(data["water_quality"]);
      setWaterLevelRate(data["water_level_rate"]);
      setTankHeight(data["tank_height"])
      setPumpControlMethod(data["pump_control_method"]);
      setPumpManualControlledStatus(data['pump_manual_controlled_status']);
      setServerConnectionStatus(true);
      setDeviceStatus(data["device_status"])
    } catch (error) {
      setDeviceStatus(null)
      setServerConnectionStatus(false)
    }
  };
  
  // Handle the system turn on and off
  const systemTurnOnOff = async () => {
    try {
      const response = await fetchWithTimeout(`${SERVER_URL}/get-system-control-status`);
      const data = await response.json();
      let currentSystemStatus = data["system_status"];
      let requestUrl = currentSystemStatus? "turn-off-system" : "turn-on-system";

      try{
        await fetch(`${SERVER_URL}/${requestUrl}`, { method: 'POST' });
      } catch (error){
        console.log("System Off/On Error :", error);
      }
    } catch (error) {
      console.log('System Off/On Error (Request Data):', error);
    }
  };


  // Handle the pump turn off on and automatic
  const changePumpControlMode = async () => {
    try{
      // Get Current Pump Control Mode
      const response = await fetchWithTimeout(`${SERVER_URL}/get-system-control-status`);
      const data = await response.json()
      let currentPumpControlMode = data["pump_control_method"];
      let currentPumpManualControlledStatus = data["pump_manual_controlled_status"];

      let requestUlr = '';
      if (currentPumpControlMode=='automatic') requestUlr = 'turn-on-pump-manually';
      else if (currentPumpControlMode=='manual' && currentPumpManualControlledStatus) requestUlr = 'turn-off-pump-manually';
      else requestUlr = 'turn-on-automatic-pump-control-mode';
      
      // Control the pump
      try {
        await fetch(`${SERVER_URL}/${requestUlr}`, { method: 'POST' });
      } catch (error) {
        console.log('Pump Control Error (Send Data):', error);
      }
    } catch (error) {
      console.log('Pump Control Error :', error);
    }
  }

  // Call fetchStatus function every 1 second
  useEffect(() => {
    const interval = setInterval(() => {
      fetchStatus();
    }, 1000);
    return () => clearInterval(interval);
  }, []);

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <View  style={styles.mainContainer}>
        <ImageBackground source={require('./assets/background.jpg')} resizeMode="cover" style={styles.backGroundImage}>
          <View style={styles.subContainer}>
            
            <Text style={styles.appTitle}>Smart Water Tank System</Text>

            <View style={[styles.status, {opacity : systemStatus? OPACITY_ACTIVE: OPACITY_INACTIVE}]}>
              <View>
                <Text style={styles.subTitle}>Water Status</Text>

                <View style={styles.horizontalLine}></View>

                <View style={styles.outputContainer}>
                  <View style={styles.imageContainer}>
                    <Image source={require('./assets/water-level.png')} style={{ width: 40, height: 40}} />
                  </View>
                  <Text style={styles.statusTitle1}>Water Level : </Text>
                  <Text style={[styles.statusValue1,{
                      color: waterLevelRate > 100 ? RED : waterLevelRate > 50 ? DARK_GREEN : waterLevelRate > 25 ? DARK_YELLOW : RED}]}>
                    {waterLevelRate} %
                  </Text>
                </View>

                <View style={styles.outputContainer}>
                  <View style={styles.imageContainer}>
                    <Image source={require('./assets/quality.png')} style={{ width: 40, height: 40}} />
                  </View>
                  <Text style={styles.statusTitle1}>Water Quality : </Text>
                  <Text style={[styles.statusValue1, {
                    color: waterQuality == "critical" ? RED : waterQuality == "poor" ? DARK_YELLOW : DARK_GREEN}]}>
                      {waterQuality == "critical" ? "Critical ☹️" : waterQuality == "poor" ? "Poor 😐" : "Good 🙂"}
                  </Text>
                </View>
                
                <Text style={styles.waterStatus}>Water Level : {waterLevel} cm / {tankHeight} cm</Text>

              </View>
              
              <View>
                <Text style={[styles.subTitle, {marginTop: 10}]}>System Status</Text>
                
                <View style={styles.horizontalLine}></View>

                <View style={styles.outputContainer}>
                  <View style={styles.imageContainer}>
                    <Image source={require('./assets/system.png')} style={{ width: 35, height: 35}} class={styles.icon}/>
                  </View>
                  <Text style={styles.statusTitle2}>System Status : </Text>
                  <Text style={styles.statusValue2}>{systemStatus ? 'ON' : 'OFF' }</Text>
                </View>
            
                <View style={styles.outputContainer}>
                  <View style={styles.imageContainer}>
                    <Image source={require('./assets/pump.png')} style={{ width: 35, height: 35}} />
                  </View>
                  <Text style={styles.statusTitle2}>Pump Status : </Text>
                  <Text style={styles.statusValue2}>{pumpStatus ? 'ON' : 'OFF' }</Text>
                </View>

                <View style={styles.outputContainer}>
                  <View style={styles.imageContainer}>
                    <Image source={require('./assets/buzzer.png')} style={{ width: 35, height: 35 }} />
                  </View>
                  <Text style={styles.statusTitle2}>Buzzer Status : </Text>
                  <Text style={styles.statusValue2}>{buzzerStatus ? 'ON' : 'OFF' }</Text>
                </View>
              </View>

            </View>

            <View style={styles.buttonContainerMain}>
              <View style={styles.buttonContainer}>
                <Text style={styles.buttonText}>System : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={systemTurnOnOff}>
                  <Text style={styles.btnText}>{systemStatus ? 'OFF' : 'ON'}</Text>
                </TouchableOpacity>
              </View>

              <View style={styles.buttonContainer}>
                <Text style={styles.buttonText}>Pump : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={changePumpControlMode}>
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
  appTitle: {
    marginTop: -60,
    fontSize: 33,
    fontFamily: '',
    textAlign: 'center',
    color: '#5555FF',
    fontWeight: 'bold',
  },
  subTitle: {
    fontSize : 20,
    padding: 10,
    textAlign: 'center',
    fontWeight: 'bold',
    textDecorationColor: '#000000',
    fontWeight: 'bold'
  },
  status: {
    marginTop: 30,
    marginBottom: 20,
    backgroundColor: 'rgba(255, 255, 255,  0.6)',
    padding : 20,
    borderRadius: 16,
  },
  statusTitle1: {
    fontSize: 18,
    marginVertical: 5,
  },
  statusValue1 : {
    fontSize: 18,
    marginVertical : 5,
    fontWeight: 'bold'
  },
  statusTitle2: {
    fontSize: 15,
    marginVertical: 5,
  },
  statusValue2 : {
    fontSize: 15,
    marginVertical : 5,
    fontWeight: 'bold'
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
    alignItems: 'center',
    width: '100%',
    backgroundColor: 'rgba(255, 255, 255,  0.6)',
    position: 'absolute',
    left: 0,
    right: 0,
    bottom: 0,
    padding: 4
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
  waterStatus : {
    backgroundColor: 'rgba(50,50,255,0.1)',
    padding: 10,
    borderRadius: 16,
    fontFamily: 'monospace',
    fontWeight: 'bold',
    fontSize: 15,
    textAlign: 'center',
    marginTop: 10,
  },
  statusTitleText:{
    fontWeight: 'bold',
  },
  buttonContainerMain: {
    backgroundColor: 'rgba(255, 255, 255,  0.6)',
    marginTop: 10,
    padding : 10,
    alignItems: 'center',
    borderRadius: 16
  },
  buttonContainer: {
    flexDirection: 'row',
    alignItems: 'center',
  },
  buttonText :{
    textAlign: 'center',
    paddingTop: 8,
    fontSize: 16
  },
  horizontalLine : {
    width: "80%",
    marginRight: "10%",
    marginLeft: "10%",
    marginTop: 2,
    marginBottom: 5,
    height: 2,
    backgroundColor: 'rgb(150, 150, 150)',
  }
});
