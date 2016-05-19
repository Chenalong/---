using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace encryp_docu_trans_server
{
    public partial class encry_doc_trans_server : Form
    {
        public Socket s = null;
        public Thread t = null;
        delegate void SetTextCallback(string text);
        public encry_doc_trans_server()
        {
            InitializeComponent();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void bnt_trans_Click(object sender, EventArgs e)
        {
            if (t == null)
                t = new Thread(init_server);
            if(t.ThreadState != ThreadState.Running)
                t.Start();
        }

        private void init_server()
        {
            int port = int.Parse(this.tb_port.Text.ToString());
            string host = this.tb_ip.Text.ToString();
            IPAddress ip = IPAddress.Parse(host);
            IPEndPoint ipe = new IPEndPoint(ip, port);
            if (s == null)
            {
                s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
               // this.rb_message_show.Text = "等待客户端连接";
                this.SetText("等待客户端连接:");
            }

            s.Bind(ipe);

            while(true)
            {
                s.Listen(0);
                Socket temp = s.Accept();//为新建连接创建新的socket
                //Console.WriteLine("建立连接");
                //this.rb_message_show.Text = "建立连接";
                this.SetText("建立连接");
                string recvStr = "";
                byte[] recvBytes = new byte[1024];
                int bytes;
                bytes = temp.Receive(recvBytes, recvBytes.Length, 0);//从客户端接受信息
                recvStr += Encoding.Unicode.GetString(recvBytes, 0, bytes);
                
                //this.rb_message_show.Text = recvStr;
                this.SetText(recvStr);
                Console.WriteLine("------------------\n" + recvStr);
                temp.Close();
            }
            

            /*
            ///给client端返回信息
            Console.WriteLine("server get message:{0}", recvStr);//把客户端传来的信息显示出来
            string sendStr = "ok!Client send message successful!";
            byte[] bs = Encoding.ASCII.GetBytes(sendStr);
            temp.Send(bs, bs.Length, 0);//返回信息给客户端
            temp.Close();
            s.Close();
            Console.ReadLine();
             * */
        }

        private void encry_doc_trans_server_Load(object sender, EventArgs e)
        {
            //init_server();
        }

        private void bnt_sotp_Click(object sender, EventArgs e)
        {
            
            if (t.ThreadState == ThreadState.Running)
                t.Suspend();
        }


        private void SetText(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.rb_message_show.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.rb_message_show.Text = text;
            }
        }

    }
}
