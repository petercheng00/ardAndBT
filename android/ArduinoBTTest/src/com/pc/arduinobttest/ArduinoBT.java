package com.pc.arduinobttest;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class ArduinoBT extends Activity {
	private BluetoothAdapter mBTAdapter;
	private BluetoothDevice mBTDevice = null;
	private BluetoothSocket mBTSocket;
	private OutputStream mOutputStream;
	private final String serialUUID = "00001101-0000-1000-8000-00805f9b34fb";
	
	private TextView statusText;
	private Button button1;
	private Button button2;
	private Button button3;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_arduino_bt);
        
        statusText = (TextView)findViewById(R.id.status);
        button1 = (Button)findViewById(R.id.button1);
        button2 = (Button)findViewById(R.id.button2);
     	button3 = (Button)findViewById(R.id.button3);
     	
     	button1.setOnClickListener(new OnClickListener()
     	{
     		public void onClick(View v)
     		{
     			sendByte(1);
     		}     		
     	});
     	button2.setOnClickListener(new OnClickListener()
     	{
     		public void onClick(View v)
     		{
     			sendByte(2);
     		}     		
     	});
     	button3.setOnClickListener(new OnClickListener()
     	{
     		public void onClick(View v)
     		{
     			sendByte(3);
     		}     		
     	});

     	mBTAdapter = BluetoothAdapter.getDefaultAdapter();
        if (!mBTAdapter.isEnabled())
        {
        	Intent requestEnableBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        	startActivity(requestEnableBT);
        }
        statusText.setText("Connecting...");
        Set<BluetoothDevice> pairedDevices = mBTAdapter.getBondedDevices();
        for (BluetoothDevice device : pairedDevices)
        {
        	if (device.getName().equalsIgnoreCase("linvor"))
        	{
        		mBTDevice = device;
        		break;
        	}
        }
        if (mBTDevice == null)
        {
        	statusText.setText("NOT CONNECTED");
        	//sad face
        }
        else
        {
        	UUID uuid = UUID.fromString(serialUUID);
        	try {
				mBTSocket = mBTDevice.createRfcommSocketToServiceRecord(uuid);
	        	mBTSocket.connect();
	        	mOutputStream = mBTSocket.getOutputStream();
	        	statusText.setText("Connected");
			} catch (IOException e) {
				statusText.setText("FAILED TO CONNECT");
				e.printStackTrace();
			}
        }
    }

    public void sendByte(int i)
    {
    	try {
			mOutputStream.write(i);
			statusText.setText("wrote something");
		} catch (IOException e) {
			statusText.setText("writing failed");
			e.printStackTrace();
		}
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_arduino_bt, menu);
        return true;
    }
}
