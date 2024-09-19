import React, { useState, useEffect } from 'react';
import { 
  StyleSheet,
  Text, 
  View, 
  ScrollView,
  Image,
  TouchableOpacity, 
  ImageBackground, 
} from 'react-native';

const SERVER = 'http://192.168.17.185'; 
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
  const [systemStatus, setSystemStatus] = useState('Off');
  const [buzzerStatus, setBuzzerStatus] = useState('On');
  const [pumpStatus, setPumpStatus] = useState('OFF');
  const [ledStatus, setLedStatus] = useState('OFF');
  const [WaterLevel, setDistanceToWater] = useState('0');
  const [waterMglValue, setWaterMglValue] = useState('0');
  

  const [pumpControlMode, setPumpControlMode] = useState('Automatic');
  const [buttonPumpStatus, setButtonPumpStatus] = useState('On');
  const [buttonSystemStatus, setButtonSystemStatus] = useState('On');

  // Device and Server indicator status check, at the startt of app status will be 'Checking'.
  let [serverConnectionStatus, setServerConnectionStatus] = useState('Checking');
  const [deviceStatus, setDeviceStatus] = useState('Checking');


  const fetchStatus = async () => {
    try {
      const response = await fetch(`${SERVER_URL}/get-status`);
      const data = await response.json();
      console.log(data);
      setSystemStatus(data["system_status"]? "On" : "Off");
      setBuzzerStatus(data["buzzer_status"]? "On" : "Off");
      setPumpStatus(data["pump_status"]? "On" : "Off");
      setLedStatus(data["led_green_status"]? "On" : "Off");
      setDistanceToWater(data["water_level"]);
      setWaterMglValue(data["water_mgl_value"]);
      setServerConnectionStatus('Online');
    } catch (error) {
      console.error('Error fetching status:', error);
      setServerConnectionStatus('Offline');
    }
    console.log(serverConnectionStatus);
  };

  const systemTurnOnOff = async () => {
    try {
      await fetch(`${SERVER_URL}/toggle`, { method: 'POST' });
    } catch (error) {
      console.error('Error toggling system:', error);
    }
  };

  const pumpTurnOnOff = async () => {
    try{
      await fetch(`${SERVER_URL}/toggle`, { method: 'POST' });
    } catch (error) {
      console.error('Error toggling system:', error);
    }
  }

  const pumpControlModeChange = async () => {
    try{
      
    } catch(error){
      console.log("error");
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
        <ImageBackground source={require('./assets/bg.jpg')} resizeMode="cover" style={styles.backGroungImage}>
          <View style={styles.subContainer}>
            <Text style={styles.title}>Smart Water Tank System</Text>
            <View style={styles.status}>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/system.png')} style={{ width: 40, height: 40 }} class={styles.icon}/>
                </View>
                <Text style={styles.statusTitle}>System Status : </Text>
                <Text style={[styles.statusValue, {color : systemStatus === 'ON' ? GREEN : RED}]}>{systemStatus}</Text>
              </View>
          
              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/buzzer.png')} style={{ width: 38, height: 38 }} />
                </View>
                <Text style={styles.statusTitle}>Buzzer Status : </Text>
                <Text style={[styles.statusValue, {color: buzzerStatus === 'ON' ? RED : RED}]}>{buzzerStatus}</Text>
              </View>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/pump.png')} style={{ width: 40, height: 40 }} />
                </View>
                <Text style={styles.statusTitle}>Pump Status : </Text>
                <Text style={[styles.statusValue , {color: pumpStatus === 'ON' ? GREEN : RED}]}>{pumpStatus}</Text>
              </View>

              <View style={styles.outputContainer}>
                <View style={styles.imageContainer}>
                  <Image source={require('./assets/led.png')} style={{ width: 50, height: 50 }} />
                </View>
                <Text style={styles.statusTitle}>LED Status : </Text>
                <Text style={[styles.statusValue, {color: ledStatus === 'ON' ? GREEN : RED}]}>{ledStatus}</Text>
              </View>
              
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
                <Text style={styles.statusTitle}>Water Mgl Value : </Text>
                <Text style={[styles.statusValue, {color: waterMglValue < 5 || waterMglValue > 8? GREEN : RED}]}>{waterMglValue} Mgl</Text>
              </View>

            </View>
            <View style={styles.buttonContainer}>
              <View style={styles.buttonContatiner}>
                <Text style={styles.buttonText}>System : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={systemTurnOnOff}>
                  <Text style={styles.btnText}>{(systemStatus == 'on') ?'On' : 'Off'}</Text>
                </TouchableOpacity>
              </View>
              <View style={styles.buttonContatiner}>
                <Text style={styles.buttonText}>Pump : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={pumpTurnOnOff}>
                  <Text style={styles.btnText}>{(pumpStatus == 'on') ?'On' : 'Off'}</Text>
                </TouchableOpacity>
              </View>
              <View style={styles.buttonContatiner}>
                <Text style={styles.buttonText}>Pump Control Mode : </Text>
                <TouchableOpacity style={styles.controlButton} onPress={pumpControlModeChange}>
                  <Text style={styles.btnText}>{pumpControlMode}</Text>
                </TouchableOpacity>
              </View>
            </View>
          </View>

          <View style={styles.statusContatiner}>
            <View style={styles.statusSubContainer}>
              <Text style={styles.statusTitleText}>Server Connection : </Text>
              <Text>{serverConnectionStatus}</Text>
              <View style={[styles.statusCircle, {backgroundColor: serverConnectionStatus === 'Checking' ? GREY :serverConnectionStatus === 'Online' ? GREEN : RED}]}></View>
            </View>
            <View style={styles.statusSubContainer}>
              <Text style={styles.statusTitleText}>Device Status : </Text>
              <Text>{deviceStatus}</Text>
              <View style={[styles.statusCircle, {backgroundColor: deviceStatus === 'Checking' ? GREY :deviceStatus === 'Online' ? GREEN : RED}]}></View>
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
  backGroungImage: {
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
  statusContatiner : {
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
  buttonContatiner: {
    flexDirection: 'row',
    alignItems: 'center',
  },
  buttonText :{
    textAlign: 'center',
    paddingTop: 8,
    fontSize: 16
  }
});
