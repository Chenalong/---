namespace encryp_docu_trans_client
{
    partial class client
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.label2 = new System.Windows.Forms.Label();
            this.tb_port = new System.Windows.Forms.TextBox();
            this.tb_ip = new System.Windows.Forms.TextBox();
            this.lab_port = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.rb_message_show = new System.Windows.Forms.RichTextBox();
            this.bnt_message_trans = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.rb_des = new System.Windows.Forms.RichTextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.rb_ency_key = new System.Windows.Forms.RichTextBox();
            this.bnt_des_key = new System.Windows.Forms.Button();
            this.bnt_conn = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.rb_md5 = new System.Windows.Forms.RichTextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.rb_des_message = new System.Windows.Forms.RichTextBox();
            this.bnt_message_des = new System.Windows.Forms.Button();
            this.bnt_send_des_message = new System.Windows.Forms.Button();
            this.ency_key = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.rb_jiami_md5 = new System.Windows.Forms.RichTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.button4 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(29, 78);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(123, 19);
            this.label2.TabIndex = 10;
            this.label2.Text = "发送消息区：";
            // 
            // tb_port
            // 
            this.tb_port.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tb_port.Location = new System.Drawing.Point(309, 28);
            this.tb_port.Name = "tb_port";
            this.tb_port.Size = new System.Drawing.Size(68, 29);
            this.tb_port.TabIndex = 9;
            this.tb_port.Text = "2000";
            // 
            // tb_ip
            // 
            this.tb_ip.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tb_ip.Location = new System.Drawing.Point(82, 28);
            this.tb_ip.Name = "tb_ip";
            this.tb_ip.ReadOnly = true;
            this.tb_ip.Size = new System.Drawing.Size(166, 26);
            this.tb_ip.TabIndex = 8;
            this.tb_ip.Text = "127.0.0.1";
            // 
            // lab_port
            // 
            this.lab_port.AutoSize = true;
            this.lab_port.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lab_port.Location = new System.Drawing.Point(254, 28);
            this.lab_port.Name = "lab_port";
            this.lab_port.Size = new System.Drawing.Size(49, 19);
            this.lab_port.TabIndex = 7;
            this.lab_port.Text = "port";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(29, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 19);
            this.label1.TabIndex = 6;
            this.label1.Text = "IP";
            // 
            // rb_message_show
            // 
            this.rb_message_show.Location = new System.Drawing.Point(33, 109);
            this.rb_message_show.Name = "rb_message_show";
            this.rb_message_show.Size = new System.Drawing.Size(344, 84);
            this.rb_message_show.TabIndex = 11;
            this.rb_message_show.Text = "";
            // 
            // bnt_message_trans
            // 
            this.bnt_message_trans.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_message_trans.Location = new System.Drawing.Point(405, 124);
            this.bnt_message_trans.Name = "bnt_message_trans";
            this.bnt_message_trans.Size = new System.Drawing.Size(139, 45);
            this.bnt_message_trans.TabIndex = 12;
            this.bnt_message_trans.Text = "发送消息";
            this.bnt_message_trans.UseVisualStyleBackColor = true;
            this.bnt_message_trans.Visible = false;
            this.bnt_message_trans.Click += new System.EventHandler(this.bnt_message_trans_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(29, 217);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(136, 19);
            this.label3.TabIndex = 13;
            this.label3.Text = "DES使用的key:";
            // 
            // rb_des
            // 
            this.rb_des.Location = new System.Drawing.Point(33, 248);
            this.rb_des.Name = "rb_des";
            this.rb_des.ReadOnly = true;
            this.rb_des.Size = new System.Drawing.Size(344, 44);
            this.rb_des.TabIndex = 14;
            this.rb_des.Text = "";
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(405, 248);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(139, 45);
            this.button1.TabIndex = 15;
            this.button1.Text = "生成key";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(29, 316);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(136, 19);
            this.label4.TabIndex = 16;
            this.label4.Text = "rsd加密的key:";
            // 
            // rb_ency_key
            // 
            this.rb_ency_key.Location = new System.Drawing.Point(33, 338);
            this.rb_ency_key.Name = "rb_ency_key";
            this.rb_ency_key.ReadOnly = true;
            this.rb_ency_key.Size = new System.Drawing.Size(344, 44);
            this.rb_ency_key.TabIndex = 17;
            this.rb_ency_key.Text = "";
            // 
            // bnt_des_key
            // 
            this.bnt_des_key.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_des_key.Location = new System.Drawing.Point(33, 397);
            this.bnt_des_key.Name = "bnt_des_key";
            this.bnt_des_key.Size = new System.Drawing.Size(511, 45);
            this.bnt_des_key.TabIndex = 18;
            this.bnt_des_key.Text = "发送key";
            this.bnt_des_key.UseVisualStyleBackColor = true;
            this.bnt_des_key.Click += new System.EventHandler(this.bnt_des_key_Click);
            // 
            // bnt_conn
            // 
            this.bnt_conn.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_conn.Location = new System.Drawing.Point(405, 18);
            this.bnt_conn.Name = "bnt_conn";
            this.bnt_conn.Size = new System.Drawing.Size(139, 45);
            this.bnt_conn.TabIndex = 19;
            this.bnt_conn.Text = "连接";
            this.bnt_conn.UseVisualStyleBackColor = true;
            this.bnt_conn.Click += new System.EventHandler(this.bnt_conn_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(574, 274);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(87, 19);
            this.label5.TabIndex = 20;
            this.label5.Text = "消息MD5:";
            // 
            // rb_md5
            // 
            this.rb_md5.Location = new System.Drawing.Point(578, 300);
            this.rb_md5.Name = "rb_md5";
            this.rb_md5.ReadOnly = true;
            this.rb_md5.Size = new System.Drawing.Size(344, 44);
            this.rb_md5.TabIndex = 21;
            this.rb_md5.Text = "";
            // 
            // button2
            // 
            this.button2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button2.Location = new System.Drawing.Point(950, 299);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(139, 45);
            this.button2.TabIndex = 22;
            this.button2.Text = "生成MD5";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button3.Location = new System.Drawing.Point(578, 463);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(516, 45);
            this.button3.TabIndex = 23;
            this.button3.Text = "发送加密MD5";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // rb_des_message
            // 
            this.rb_des_message.Location = new System.Drawing.Point(578, 100);
            this.rb_des_message.Name = "rb_des_message";
            this.rb_des_message.ReadOnly = true;
            this.rb_des_message.Size = new System.Drawing.Size(324, 84);
            this.rb_des_message.TabIndex = 24;
            this.rb_des_message.Text = "";
            // 
            // bnt_message_des
            // 
            this.bnt_message_des.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_message_des.Location = new System.Drawing.Point(925, 124);
            this.bnt_message_des.Name = "bnt_message_des";
            this.bnt_message_des.Size = new System.Drawing.Size(139, 45);
            this.bnt_message_des.TabIndex = 25;
            this.bnt_message_des.Text = "加密信息";
            this.bnt_message_des.UseVisualStyleBackColor = true;
            this.bnt_message_des.Click += new System.EventHandler(this.bnt_message_des_Click);
            // 
            // bnt_send_des_message
            // 
            this.bnt_send_des_message.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_send_des_message.Location = new System.Drawing.Point(578, 213);
            this.bnt_send_des_message.Name = "bnt_send_des_message";
            this.bnt_send_des_message.Size = new System.Drawing.Size(486, 45);
            this.bnt_send_des_message.TabIndex = 27;
            this.bnt_send_des_message.Text = "发送加密信息";
            this.bnt_send_des_message.UseVisualStyleBackColor = true;
            this.bnt_send_des_message.Click += new System.EventHandler(this.bnt_send_des_message_Click);
            // 
            // ency_key
            // 
            this.ency_key.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ency_key.Location = new System.Drawing.Point(405, 337);
            this.ency_key.Name = "ency_key";
            this.ency_key.Size = new System.Drawing.Size(139, 45);
            this.ency_key.TabIndex = 28;
            this.ency_key.Text = "加密key";
            this.ency_key.UseVisualStyleBackColor = true;
            this.ency_key.Click += new System.EventHandler(this.ency_key_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(574, 78);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(144, 19);
            this.label6.TabIndex = 29;
            this.label6.Text = "DES加密的消息:";
            // 
            // rb_jiami_md5
            // 
            this.rb_jiami_md5.Location = new System.Drawing.Point(578, 398);
            this.rb_jiami_md5.Name = "rb_jiami_md5";
            this.rb_jiami_md5.ReadOnly = true;
            this.rb_jiami_md5.Size = new System.Drawing.Size(344, 44);
            this.rb_jiami_md5.TabIndex = 30;
            this.rb_jiami_md5.Text = "";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.Location = new System.Drawing.Point(574, 363);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(87, 19);
            this.label7.TabIndex = 31;
            this.label7.Text = "加密MD5:";
            // 
            // button4
            // 
            this.button4.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button4.Location = new System.Drawing.Point(950, 397);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(139, 45);
            this.button4.TabIndex = 32;
            this.button4.Text = "加密MD5";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // client
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1191, 784);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.rb_jiami_md5);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.ency_key);
            this.Controls.Add(this.bnt_send_des_message);
            this.Controls.Add(this.bnt_message_des);
            this.Controls.Add(this.rb_des_message);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.rb_md5);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.bnt_conn);
            this.Controls.Add(this.bnt_des_key);
            this.Controls.Add(this.rb_ency_key);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.rb_des);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.bnt_message_trans);
            this.Controls.Add(this.rb_message_show);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tb_port);
            this.Controls.Add(this.tb_ip);
            this.Controls.Add(this.lab_port);
            this.Controls.Add(this.label1);
            this.Name = "client";
            this.Text = "client";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_port;
        private System.Windows.Forms.TextBox tb_ip;
        private System.Windows.Forms.Label lab_port;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RichTextBox rb_message_show;
        private System.Windows.Forms.Button bnt_message_trans;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RichTextBox rb_des;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RichTextBox rb_ency_key;
        private System.Windows.Forms.Button bnt_des_key;
        private System.Windows.Forms.Button bnt_conn;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.RichTextBox rb_md5;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.RichTextBox rb_des_message;
        private System.Windows.Forms.Button bnt_message_des;
        private System.Windows.Forms.Button bnt_send_des_message;
        private System.Windows.Forms.Button ency_key;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.RichTextBox rb_jiami_md5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button button4;
    }
}

