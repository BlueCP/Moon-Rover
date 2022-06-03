using DSPLib;
using Newtonsoft.Json;
using System;
using System.Net;
using System.Net.Sockets;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace summerprojtest1
{
    class Program
    {



        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            //example1();

            byte a = 1;


            //var udpClient = new UdpClient(2390);

            //Task.Run(() =>
            //{

            //    string loggingEvent = "";
            //    while (true)
            //    {
            //        //IPEndPoint object will allow us to read datagrams sent from any source.
            //        var remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
            //        var receivedResults = udpClient.Receive(ref remoteEndPoint);
            //        string someString = System.Text.Encoding.UTF8.GetString(receivedResults);
            //        Console.WriteLine(someString); ;
            //        loggingEvent += Encoding.ASCII.GetString(receivedResults);
            //        Console.WriteLine("Ping was sent from " + remoteEndPoint.Address.ToString() +
            //      " on their port number " + remoteEndPoint.Port.ToString());
            //    }

            //});











            //UdpClient udpServer = new UdpClient(2390);

            //while (true)
            //{
            //    var remoteEP = new IPEndPoint(IPAddress.Any, 2390);
            //    var data = udpServer.Receive(ref remoteEP); // listen on port 2390
            //    Console.Write("receive data from " + remoteEP.ToString());
            //    byte[] bytes = Encoding.ASCII.GetBytes("test send from server");
            //    udpServer.Send(bytes, bytes.Length, remoteEP); // reply back
            //}



            var client = new UdpClient();
            //string ip = "172.20.10.2";
            string ip = "172.20.10.3";
            IPEndPoint ep = new IPEndPoint(IPAddress.Parse(ip), 2390); // endpoint where server is listening
            client.Connect(ep);

            // send data
            byte[] tsetsend = Encoding.ASCII.GetBytes("test send from server");
            client.Send(tsetsend, tsetsend.Length);

            // then receive data
            while (true)
            {

                var remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
                using (var udpClient = new UdpClient(2391)) {
                    var receivedData = udpClient.Receive(ref remoteEndPoint);


                    // string someString = Encoding.ASCII.GetString(receivedData);

                    string someString = System.Text.Encoding.UTF8.GetString(receivedData);

                    //Console.WriteLine("receive data from " + ep.ToString());
                    Console.WriteLine("Ping was sent from " + remoteEndPoint.Address.ToString() +
                  " on their port number " + remoteEndPoint.Port.ToString());

                    Console.WriteLine(someString);
                }

            }




            //Packet account = JsonConvert.DeserializeObject<Packet>(someString);

            //Console.WriteLine(account.data1);

            Console.Read();
        }



        static void example1()
        {
            // Generate a test signal,
            //  1 Vrms at 20,000 Hz
            //  Sampling Rate = 100,000 Hz
            //  DFT Length is 1000 Points
            double amplitude = 1.0;
            double frequency = 20000;
            UInt32 length = 1000;
            double samplingRate = 100000;

            double[] inputSignal =
                     DSP.Generate.ToneSampling(amplitude, frequency, samplingRate, length);

            // Instantiate a new DFT
            DFT dft = new DFT();

            // Initialize the DFT
            // You only need to do this once or if you change any of the DFT parameters.
            dft.Initialize(length);

            // Call the DFT and get the scaled spectrum back
            Complex[] cSpectrum = dft.Execute(inputSignal);

            // Convert the complex spectrum to magnitude
            double[] lmSpectrum = DSP.ConvertComplex.ToMagnitude(cSpectrum);

            // Note: At this point, lmSpectrum is a 501 byte array that 
            // contains a properly scaled Spectrum from 0 - 50,000 Hz (1/2 the Sampling Frequency)

            // For plotting on an XY Scatter plot, generate the X Axis frequency Span
            double[] freqSpan = dft.FrequencySpan(samplingRate);

            // At this point a XY Scatter plot can be generated from,
            // X axis => freqSpan
            // Y axis => lmSpectrum

            // In this example, the maximum value of 1 Vrms is located at bin 200 (20,000 Hz)
        }

    }


    public class Packet
    {
        [JsonProperty("radio")]
        public string data1;
    }
}
