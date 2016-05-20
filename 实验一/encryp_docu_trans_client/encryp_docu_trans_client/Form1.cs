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
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.IO;


namespace encryp_docu_trans_client
{
    public partial class client : Form
    {
        char[] key = new char[100];
        const int key_length = 15;
        byte[] des_contend_ptr = new byte[1000];//新建字节数组;
        Socket c = null;
        IPEndPoint ipe;
        public client()
        {
            InitializeComponent();
           // init_key();
        }

        //产生des算法需要的key
        public void init_key()
        {
            Random rd = new Random();
            for (int i = 0; i < key_length; i++)
                key[i] = Convert.ToChar(rd.Next(48,58));
            return;
        }

        [System.Runtime.InteropServices.DllImport("md5.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern void getMD5(ref byte param2,[MarshalAs(UnmanagedType.LPArray)]byte[] buml);

        [DllImport("rsa.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern void rsa_jia_mi(ref byte receive_data, [MarshalAs(UnmanagedType.LPArray)]byte[] send_data);

        [DllImport("DES.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern bool Des_Go(ref byte receive_data, [MarshalAs(UnmanagedType.LPArray)]byte[] send_data_1, 
            [MarshalAs(UnmanagedType.LPArray)]byte[] send_data_2,bool type);

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

        public void create_key()
        {
            RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
            using (StreamWriter writer = new StreamWriter("PrivateKey.xml"))  //这个文件要保密...
            {
                writer.WriteLine(rsa.ToXmlString(true)); 
            }
            using (StreamWriter writer = new StreamWriter("PublicKey.xml"))
            {

                writer.WriteLine(rsa.ToXmlString(false));

            }
        }

        private void bnt_message_trans_Click(object sender, EventArgs e)
        {
            int port = int.Parse(this.tb_port.Text.ToString());
            string host = this.tb_ip.Text.ToString();
            IPAddress ip = IPAddress.Parse(host);
            IPEndPoint ipe = new IPEndPoint(ip, port);//把ip和端口转化为IPEndpoint实例

            ///创建socket并连接到服务器
            Socket c = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);//创建Socket
            //Console.WriteLine("Conneting…");
            c.Connect(ipe);//连接到服务器

            ///向服务器发送信息
            //string sendStr = "hello!This is a socket test";
            string sendStr = this.rb_message_show.Text;
            string md5_str = hash_md5(sendStr);
            MessageBox.Show(md5_str);
            //byte[] bs = Encoding.utf-8.GetBytes(sendStr);//把字符串编码为字节
            byte[] bs = Encoding.Unicode.GetBytes(md5_str);
            //Console.WriteLine("Send Message");

            c.Send(bs, bs.Length, 0);//发送信息
            c.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void bnt_conn_Click(object sender, EventArgs e)
        {
            int port = int.Parse(this.tb_port.Text.ToString());
            string host = this.tb_ip.Text.ToString();
            IPAddress ip = IPAddress.Parse(host);
            ipe = new IPEndPoint(ip, port);//把ip和端口转化为IPEndpoint实例

            if (c == null)
            {
                ///创建socket并连接到服务器
                c = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);//创建Socket
                //Console.WriteLine("Conneting…");
            }
            //c.Bind(ipe);
            try
            {
                c.Connect(ipe);//连接到服务器
            }
            catch (Exception e1)
            {
                MessageBox.Show(e1.ToString());
            }
            c.Close();
            
        }

        public void send_message(string message)
        {
            int port = int.Parse(this.tb_port.Text.ToString());
            string host = this.tb_ip.Text.ToString();
            IPAddress ip = IPAddress.Parse(host);
            ipe = new IPEndPoint(ip, port);//把ip和端口转化为IPEndpoint实例

          
                ///创建socket并连接到服务器
                c = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);//创建Socket
                //Console.WriteLine("Conneting…");
            
            //c.Bind(ipe);
            try
            {
                c.Connect(ipe);//连接到服务器
            }
            catch (Exception e1)
            {
                MessageBox.Show(e1.ToString());
                return;
            }
            byte[] bs = Encoding.UTF8.GetBytes(message);
            //Console.WriteLine("Send Message");
            if (message.StartsWith("#mes#"))
            {
                c.Send(des_contend_ptr, des_contend_ptr.Length, 0);
                //MessageBox.Show("hello");
            }
            else
                c.Send(bs, bs.Length, 0);//发送信息
            //c.Close();
            c.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            init_key();
            this.rb_des.Text = new string(key);
        }

        private void ency_key_Click(object sender, EventArgs e)
        {
            this.rb_ency_key.Text = "";
            string key_contend = this.rb_des.Text;
            if (key_contend == "")
            {
                MessageBox.Show("请先生成key");
                return;
            }
            //把c# 中的string类型进行封装成byte数组
            byte[] buml = new byte[25500];
            buml = System.Text.Encoding.UTF8.GetBytes(key_contend);

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[1000];//新建字节数组
            rsa_jia_mi(ref param2[0], buml);

            //把byte[] 转化成字符串
            string rsa_key = System.Text.Encoding.GetEncoding("utf-8").GetString(param2, 0, param2.Length);
            this.rb_ency_key.Text = rsa_key;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.rb_md5.Text = "";
            string sendStr = this.rb_message_show.Text;
            if (sendStr == "")
            {
                MessageBox.Show("请先填写发送消息");
                return;
            }
            string md5_str = hash_md5(sendStr);
            this.rb_md5.Text = md5_str;
        }

        private void bnt_message_des_Click(object sender, EventArgs e)
        {
            string sendStr = this.rb_message_show.Text;
            string des_key = this.rb_des.Text;
            if (sendStr == "")
            {
                MessageBox.Show("请先填写发送消息");
                return;
            }

            if (des_key == "")
            {
                MessageBox.Show("请先生成key");
                return;
            }

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[1000];//新建字节数组
            
            //把c# 中的string类型进行封装成byte数组
            byte[] byte_contend = new byte[25500];
            byte_contend = System.Text.Encoding.UTF8.GetBytes(sendStr);
            int contead_len = sendStr.Length;
            //MessageBox.Show(contead_len.ToString());
            //把c# 中的string类型进行封装成byte数组
            byte[] byte_key = new byte[25500];
            byte_key = System.Text.Encoding.UTF8.GetBytes(des_key);
            
            int des_key_len = des_key.Length;
            //MessageBox.Show(des_key);
            //MessageBox.Show(des_key_len.ToString());
            bool encrypt = false;
            Des_Go(ref des_contend_ptr[0], byte_contend, byte_key, encrypt);

            string des_contend = System.Text.Encoding.GetEncoding("utf-8").GetString(des_contend_ptr, 0, des_contend_ptr.Length);

            this.rb_des_message.Text = des_contend;

            //test============================================
            //param2 = System.Text.Encoding.UTF8.GetBytes(des_contend);
            //byte[] param3 = new byte[1000];//新建字节数组
            //Des_Go(ref param3[0], param2, byte_key, true);
            //MessageBox.Show(System.Text.Encoding.GetEncoding("utf-8").GetString(param3, 0, param3.Length));
            //======================================================
        }

        private void bnt_des_key_Click(object sender, EventArgs e)
        {
           
            string rad_jiami_key = this.rb_ency_key.Text;
            if (rad_jiami_key == "")
            {
                MessageBox.Show("请先生成加密的key");
                return;
            }

            string sendStr = "key" + rad_jiami_key;
            send_message(sendStr);
            

        }

        private void bnt_send_des_message_Click(object sender, EventArgs e)
        {
            string des_jiami_message = this.rb_des_message.Text;
            if (des_jiami_message == "")
            {
                MessageBox.Show("请先生成加密信息");
                return;
            }

            string sendStr = "#mes#" + des_jiami_message;

            send_message(sendStr);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string rsa_jiami_md5 = this.rb_jiami_md5.Text;
            if (rsa_jiami_md5 == "")
            {
                MessageBox.Show("请先生成加密MD5");
                return;
            }

            string sendStr = "md5" + rsa_jiami_md5;

            send_message(sendStr);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            this.rb_jiami_md5.Text = "";
            string md5_contend = this.rb_md5.Text;
            if (md5_contend == "")
            {
                MessageBox.Show("请先生成MD5");
                return;
            }
            //把c# 中的string类型进行封装成byte数组
            byte[] buml = new byte[25500];
            buml = System.Text.Encoding.UTF8.GetBytes(md5_contend);

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[1000];//新建字节数组
            rsa_jia_mi(ref param2[0], buml);

            //把byte[] 转化成字符串
            string rsa_md5 = System.Text.Encoding.GetEncoding("utf-8").GetString(param2, 0, param2.Length);
            this.rb_jiami_md5.Text = rsa_md5;
        }
    }
}
