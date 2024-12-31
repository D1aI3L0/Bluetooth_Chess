using InTheHand.Net;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;
using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;

namespace Bluetooth_Chess
{
    static class BluetoothTransaction
    {
        static SynchronizedCollection<byte[]> movesToSent = new SynchronizedCollection<byte[]>();
        static SynchronizedCollection<byte[]> movesReceived = new SynchronizedCollection<byte[]>();
        static public SynchronizedCollection<BluetoothDeviceInfo> devs = new SynchronizedCollection<BluetoothDeviceInfo>();

        private static readonly Guid service = BluetoothService.SerialPort;
        public static BluetoothDeviceInfo selectedDev;

        static public Thread devsScanThread;
        static public Thread serverThread;
        static public Thread clientThread;

        static public BluetoothClient localClient;
        static private BluetoothListener localServer;


        static public readonly object devsSync = new object();
        static public readonly object localClientSync = new object();
        private static bool transfer = false;


        private delegate void Delegate();

        public static void DevsScanProc()
        {
            while (true)
            {
                IReadOnlyCollection<BluetoothDeviceInfo> list;

                lock (localClientSync)
                    list = localClient.DiscoverDevices();

                lock (devsSync)
                {
                    devs.Clear();
                    foreach (BluetoothDeviceInfo dev in list)
                        devs.Add(dev);
                }
            }
        }

        public static void ServerProc()
        {
            BluetoothClient remoteClient = localServer.AcceptBluetoothClient();
            using (NetworkStream serverStream = remoteClient.GetStream())
            {
                while (true)
                {
                    byte[] moveLengthBytes = new byte[sizeof(long)];
                    serverStream.Read(moveLengthBytes, 0, moveLengthBytes.Length);
                    long moveLength = BitConverter.ToInt64(moveLengthBytes, 0);

                    byte[] bytes = new byte[moveLength];
                    serverStream.Read(bytes, 0, (int)moveLength);
                    movesReceived.Add(bytes);

                    while(movesToSent.Count == 0)
                        Thread.Sleep(100);

                    byte[] bytes2 = movesToSent[0];
                    movesToSent.RemoveAt(0);

                    byte[] fileLengthBytes = new byte[sizeof(long)];
                    fileLengthBytes = BitConverter.GetBytes(bytes2.Length);

                    serverStream.Write(fileLengthBytes, 0, fileLengthBytes.Length);
                    serverStream.Write(bytes2, 0, bytes2.Length);
                    serverStream.Flush();
                }
            }
        }


        private static void ClientProc()
        {
            using (NetworkStream clientStream = localClient.GetStream())
            {
                while (true)
                {
                    while (movesToSent.Count == 0)
                        Thread.Sleep(100);

                    byte[] bytes = movesToSent[0];
                    movesToSent.RemoveAt(0);

                    byte[] fileLengthBytes = new byte[sizeof(long)];
                    fileLengthBytes = BitConverter.GetBytes(bytes.Length);

                    clientStream.Write(fileLengthBytes, 0, fileLengthBytes.Length);
                    clientStream.Write(bytes, 0, bytes.Length);
                    clientStream.Flush();


                    byte[] moveLengthBytes = new byte[sizeof(long)];
                    clientStream.Read(moveLengthBytes, 0, moveLengthBytes.Length);
                    long moveLength = BitConverter.ToInt64(moveLengthBytes, 0);

                    byte[] bytes2 = new byte[moveLength];
                    clientStream.Read(bytes2, 0, (int)moveLength);
                    movesReceived.Add(bytes2);
                }
            }
        }
        

        private static bool ConnectDev(BluetoothDeviceInfo dev)
        {
            if (!dev.Authenticated)
                if (!PairDev(dev.DeviceAddress))
                    return false;
            
            try 
            {
                lock (localClientSync)
                    localClient.Connect(new BluetoothEndPoint(dev.DeviceAddress, service));
                return localClient.Connected;
            }
            catch { return false; }
        }

        private static bool PairDev(BluetoothAddress address)
        {
            return BluetoothSecurity.PairRequest(address);
        }

        public static bool StartServer()
        {
            try
            {
                localServer = new BluetoothListener(service);
                localServer.Start();
                serverThread = new Thread(() => ServerProc());
                serverThread.Start();

                localClient = new BluetoothClient();
                return true;
            }
            catch { return false; }
        }

        public static bool StartClient(BluetoothDeviceInfo dev)
        {
            try
            {
                ConnectDev(dev);
                clientThread = new Thread(() => ClientProc());
                clientThread.Start();
                return true;
            }
            catch { return false; }
        }

        public static bool ConnectClient(BluetoothDeviceInfo dev)
        {
            LocalClientRefresh();
            return ConnectDev(dev);
        }

        public static void LocalClientRefresh()
        {
            localClient = new BluetoothClient();
        }

        public static void Stop()
        {
            devs?.Clear();
            movesReceived?.Clear();
            movesToSent?.Clear();

            devsScanThread?.Abort();
            clientThread?.Abort();
            serverThread?.Abort();

            localServer?.Stop();
            localClient?.Close();
        }


        public static void SendMove(byte[] bytes)
        {
            movesToSent.Add(bytes);
        }

        public static byte[] ReceiveMove()
        {
            while (movesReceived.Count == 0)
                Thread.Sleep(100);

            byte[] bytes = movesReceived[0];
            movesReceived.RemoveAt(0);
            return bytes;
        }

    }
}
