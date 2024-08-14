import React, { useState, useEffect } from 'react';
import { StyleSheet, Text, View, Button, ScrollView } from 'react-native';

const SERVER_URL = 'http://192.168.159.24:5000'; 

export default function App() {
  const [systemStatus, setSystemStatus] = useState('OFF');
  const [buzzerStatus, setBuzzerStatus] = useState('OFF');
  const [pumpStatus, setPumpStatus] = useState('OFF');
  const [ledStatus, setLedStatus] = useState('OFF');
  const [distanceToWater, setDistanceToWater] = useState('0 cm');
  const [waterPhValue, setWaterPhValue] = useState('0 pH');

  const fetchStatus = async () => {
    try {
      const response = await fetch(`${SERVER_URL}/status`);
      const data = await response.json();
      setSystemStatus(data.system_status);
      setBuzzerStatus(data.buzzer_status);
      setPumpStatus(data.pump_status);
      setLedStatus(data.led_status);
      setDistanceToWater(data.distance_to_water + ' cm');
      setWaterPhValue(data.water_ph_value + ' pH');
    } catch (error) {
      console.error('Error fetching status:', error);
    }
  };

  const toggleSystem = async () => {
    try {
      await fetch(`${SERVER_URL}/toggle`, { method: 'POST' });
    } catch (error) {
      console.error('Error toggling system:', error);
    }
  };

  useEffect(() => {
    const intervalId = setInterval(fetchStatus, 1000);
    fetchStatus(); 
    return () => clearInterval(intervalId); 
  }, []);

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Text style={styles.title}>Title</Text>
      <View style={styles.status}>
        <Text style={styles.statusText}>System Status: {systemStatus}</Text>
        <Text style={styles.statusText}>Buzzer Status: {buzzerStatus}</Text>
        <Text style={styles.statusText}>Pump Status: {pumpStatus}</Text>
        <Text style={styles.statusText}>LED Status: {ledStatus}</Text>
        <Text style={styles.statusText}>Distance to Water: {distanceToWater}</Text>
        <Text style={styles.statusText}>Water pH Value: {waterPhValue}</Text>
      </View>
      <View style={styles.control}>
        <Button title="OFF/ON System" onPress={toggleSystem} />
      </View>
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    flexGrow: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
    backgroundColor: '#f0f0f0',
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    marginBottom: 20,
  },
  status: {
    marginBottom: 20,
  },
  statusText: {
    fontSize: 18,
    marginVertical: 5,
  },
  control: {
    marginTop: 20,
  },
});
