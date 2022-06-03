using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {

        UdpClient client = new UdpClient();
        IPEndPoint ep;


        public Form1()
        {
            InitializeComponent();

            Form.CheckForIllegalCrossThreadCalls = false;
            Control.CheckForIllegalCrossThreadCalls = false;


            ep = new IPEndPoint(IPAddress.Parse("172.20.10.4"), 2390); // endpoint where server is listening
            client.Connect(ep);


            byte[] tsetsensd = new byte[] { WASD_to_byte(wdown, adown, sdown, ddown) };
            client.Send(tsetsensd, tsetsensd.Length);

            Debug.WriteLine("hello world");

            t1 = new Thread(Loop);
            t1.Start();
        }

        Thread t1;

        bool wdown = false;
        bool adown = false;
        bool sdown = false;
        bool ddown = false;


        public void refreshcolour()
        {
            lb_w.BackColor = wdown ? Color.Green : Color.Red;
            lb_a.BackColor = adown ? Color.Green : Color.Red;
            lb_s.BackColor = sdown ? Color.Green : Color.Red;
            lb_d.BackColor = ddown ? Color.Green : Color.Red;
        }
        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {

            if (e.KeyData == Keys.W) wdown = true;
            if (e.KeyData == Keys.A) adown = true;
            if (e.KeyData == Keys.S) sdown = true;
            if (e.KeyData == Keys.D) ddown = true;
            refreshcolour();
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.W) wdown = false;
            if (e.KeyData == Keys.A) adown = false;
            if (e.KeyData == Keys.S) sdown = false;
            if (e.KeyData == Keys.D) ddown = false;
            refreshcolour();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            refreshcolour();
        }


        public void Loop()
        {




            // send data
            //byte[] tsetsend = Encoding.ASCII.GetBytes("test send from server");
            //client.Send(tsetsend, tsetsend.Length);

            // then receive data
            //var receivedData = client.Receive(ref ep);

            //string someString = Encoding.ASCII.GetString(receivedData);

            //Console.WriteLine("receive data from " + ep.ToString());


            //Packet account = JsonConvert.DeserializeObject<Packet>(someString);


            System.Timers.Timer aTimer = new System.Timers.Timer();
            aTimer.Elapsed += new ElapsedEventHandler(OnTimedEvent);
            aTimer.Interval = 500;
            aTimer.Enabled = true;


            while (true)
            {
                lb_requesting.Text = "not reqesuting";

                byte[] tsetsend = new byte[] { WASD_to_byte(wdown, adown, sdown, ddown) };
                client.Send(tsetsend, tsetsend.Length);

                Debug.WriteLine(WASD_to_byte(wdown, adown, sdown, ddown));

                if (requestbyte)
                {
                    lb_requesting.Text = "reqesuting";

                    var receivedData = client.Receive(ref ep);

                    string someString = Encoding.ASCII.GetString(receivedData);

                    Debug.WriteLine("receive data from " + ep.ToString());


                    Packet account = JsonConvert.DeserializeObject<Packet>(someString);

                    lb_output.Text = account.data1;

                    requestbyte = false;

                    aTimer.Stop();
                    aTimer.Start();
                }

                Thread.Sleep(5);
            }


        }


        private void OnTimedEvent(object source, ElapsedEventArgs e)
        {

            requestbyte = true;
        }

        bool requestbyte = false;

        public byte WASD_to_byte(bool w, bool a, bool s, bool d)
        {
            byte b = 0;

            b = setbit(b, w ? (byte)1 : (byte)0, 4);
            b = setbit(b, a ? (byte)1 : (byte)0, 3);
            b = setbit(b, s ? (byte)1 : (byte)0, 2);
            b = setbit(b, d ? (byte)1 : (byte)0, 1);
            b = setbit(b, requestbyte ? (byte)1 : (byte)0, 0);

            return b;
        }

        public byte setbit(byte b, byte newvalue, byte bitnumber)
        {
            byte a = b ^= (byte)((-newvalue ^ b) & (1 << bitnumber));
            return a;
        }

        public class Packet
        {
            [JsonProperty("radio")]
            public string data1;
        }


    }
}
