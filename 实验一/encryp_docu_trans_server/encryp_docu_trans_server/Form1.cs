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
using System.Runtime.InteropServices;

namespace encryp_docu_trans_server
{
    public partial class encry_doc_trans_server : Form
    {
        public Socket s = null;
        public Thread t = null;
        delegate void SetTextCallback(string text);

        [DllImport("rsa.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern void rsa_jie_mi(ref byte receive_data, [MarshalAs(UnmanagedType.LPArray)]byte[] send_data);

        [System.Runtime.InteropServices.DllImport("md5.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern void getMD5(ref byte param2, [MarshalAs(UnmanagedType.LPArray)]byte[] buml);

        [DllImport("DES.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern bool Des_Go(ref byte receive_data, [MarshalAs(UnmanagedType.LPArray)]byte[] send_data_1,
            [MarshalAs(UnmanagedType.LPArray)]byte[] send_data_2,  bool type);

        public encry_doc_trans_server()
        {
            InitializeComponent();
        }

        byte[] des_contend_ptr = new byte[10240];//新建字节数组;

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
                this.Set_MESSAGE_Text("等待客户端连接:");
            }

            s.Bind(ipe);

            while(true)
            {
                s.Listen(0);
                Socket temp = s.Accept();//为新建连接创建新的socket
                //Console.WriteLine("建立连接");
                //this.rb_message_show.Text = "建立连接";
                //this.Set_MESSAGE_Text("建立连接");
                string recvStr = "";
                byte[] recvBytes = new byte[10240];
                int bytes;
                bytes = temp.Receive(recvBytes, recvBytes.Length, 0);//从客户端接受信息
                recvStr += Encoding.UTF8.GetString(recvBytes, 0, bytes);
                

                //this.rb_message_show.Text = recvStr;
                if (recvStr.StartsWith("key"))
                {

                    recvStr = recvStr.Substring(3);
                    this.Set_KEY_Text(recvStr);
                }
                else if (recvStr.StartsWith("md5"))
                {
                    recvStr = recvStr.Substring(3);
                    this.Set_MD5_Text(recvStr);
                }
                else
                {
                    //recvStr = recvStr.Substring(3);
                    this.Set_MESSAGE_Text(recvStr);
                    recvBytes.CopyTo(des_contend_ptr, 0);
                }

                //this.Set_MESSAGE_Text(recvStr);
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


        private void Set_MD5_Text(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.rb_message_show.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(Set_MD5_Text);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.rb_jiami_md5.Text = text;
                //this.rb_message_show.Text = text;
            }
        }

        private void Set_KEY_Text(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.rb_message_show.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(Set_KEY_Text);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                //this.rb_jiami_md5.Text = text;
                //this.rb_message_show.Text = text;
                this.rb_jiami_key.Text = text;
            }
        }

        private void Set_MESSAGE_Text(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.rb_message_show.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(Set_MESSAGE_Text);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                //this.rb_jiami_md5.Text = text;
                //this.rb_message_show.Text = text;
                this.rb_message_show.Text = text;
            }
        }

        private void bnt_jiemi_message_Click(object sender, EventArgs e)
        {
            string sendStr = this.rb_message_show.Text;
            string des_key = this.rb_jiemi_key.Text;
            if (sendStr == "")
            {
                MessageBox.Show("请先等待接收加密消息");
                return;
            }

            if (des_key == "")
            {
                MessageBox.Show("请先解密key");
                return;
            }

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[10000];//新建字节数组

            //把c# 中的string类型进行封装成byte数组
            byte[] byte_contend = new byte[25500];
            byte_contend = System.Text.Encoding.UTF8.GetBytes(sendStr);
            int contead_len = sendStr.Length;

            //把c# 中的string类型进行封装成byte数组
            byte[] byte_key = new byte[25500];
            byte_key = System.Text.Encoding.UTF8.GetBytes(des_key);
            int des_key_len = des_key.Length;

            bool encrypt = true;
            Des_Go(ref param2[0], des_contend_ptr,  byte_key, encrypt);

            string des_contend = System.Text.Encoding.GetEncoding("UTF-8").GetString(param2, 0, param2.Length);

            this.rb_jiemi_message.Text = des_contend;
        }

        private void bnt_jiemi_key_Click(object sender, EventArgs e)
        {
            string key_contend = this.rb_jiami_key.Text;
            if (key_contend == "")
            {
                MessageBox.Show("请等待接收加密key");
                return;
            }

            //把c# 中的string类型进行封装成byte数组
            byte[] buml = new byte[25500];
            buml = System.Text.Encoding.UTF8.GetBytes(key_contend);

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[1000];//新建字节数组
            rsa_jie_mi(ref param2[0], buml);

            //把byte[] 转化成字符串
            string rsa_key = System.Text.Encoding.GetEncoding("utf-8").GetString(param2, 0, param2.Length);
            this.rb_jiemi_key.Text = rsa_key;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string key_contend = this.rb_jiami_md5.Text;
            if (key_contend == "")
            {
                MessageBox.Show("请等待接收加密MD5");
                return;
            }

            //把c# 中的string类型进行封装成byte数组
            byte[] buml = new byte[25500];
            buml = System.Text.Encoding.UTF8.GetBytes(key_contend);

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[1000];//新建字节数组
            rsa_jie_mi(ref param2[0], buml);

            //把byte[] 转化成字符串
            string rsa_key = System.Text.Encoding.GetEncoding("utf-8").GetString(param2, 0, param2.Length);
            //MessageBox.Show(rsa_key);
            this.rb_jiemi_md5.Text = rsa_key;
        }

        //把source字符串用md5算法生产摘要
        public string hash_md5(string source)
        {
            //把c# 中的string类型进行封装成byte数组
            byte[] buml = new byte[25500];
            buml = System.Text.Encoding.UTF8.GetBytes(source);

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[10000];//新建字节数组
            getMD5(ref param2[0], buml);

            //把byte[] 转化成字符串
            string md5_str = System.Text.Encoding.GetEncoding("utf-8").GetString(param2, 0, param2.Length);
            return md5_str;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string sendStr = this.rb_jiemi_message.Text;
            if (sendStr == "")
            {
                MessageBox.Show("请先解密消息");
                return;
            }
            string md5_str = hash_md5(sendStr);
            this.rb_md5_message.Text = md5_str;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string cal_md5 = this.rb_jiemi_md5.Text;
            string jiemi_md5 = this.rb_md5_message.Text;
            if (cal_md5 != jiemi_md5)
                MessageBox.Show("消息发生改变");
            else
                MessageBox.Show("消息正确被接收");
        }

    }
}
