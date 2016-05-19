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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            
        }
        //,CallingConvention=CallingConvention.Cdecl
        [System.Runtime.InteropServices.DllImport("md5.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
        public static extern void getMD5(ref byte param2,[MarshalAs(UnmanagedType.LPArray)]byte[] buml);

        //把source字符串用md5算法生产摘要
        public string hash_md5(string source)
        {
            //把c# 中的string类型进行封装成byte数组
            byte[] buml = new byte[255];
            buml = System.Text.Encoding.UTF8.GetBytes(source);

            // 返回的字符串定义成byte返回
            byte[] param2 = new byte[1000];//新建字节数组
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
            create_key();
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
            //byte[] bs = Encoding.ASCII.GetBytes(sendStr);//把字符串编码为字节
            byte[] bs = Encoding.Unicode.GetBytes(md5_str);
            //Console.WriteLine("Send Message");

            c.Send(bs, bs.Length, 0);//发送信息
            c.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
