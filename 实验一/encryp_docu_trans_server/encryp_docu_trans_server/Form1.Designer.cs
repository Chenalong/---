namespace encryp_docu_trans_server
{
    partial class encry_doc_trans_server
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
            this.label1 = new System.Windows.Forms.Label();
            this.lab_port = new System.Windows.Forms.Label();
            this.tb_ip = new System.Windows.Forms.TextBox();
            this.tb_port = new System.Windows.Forms.TextBox();
            this.rb_message_show = new System.Windows.Forms.RichTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.bnt_trans = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.rb_jiemi_message = new System.Windows.Forms.RichTextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.rb_md5_message = new System.Windows.Forms.RichTextBox();
            this.rb_jiami_key = new System.Windows.Forms.RichTextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.rb_jiemi_key = new System.Windows.Forms.RichTextBox();
            this.bnt_jiemi_key = new System.Windows.Forms.Button();
            this.bnt_jiemi_message = new System.Windows.Forms.Button();
            this.rb_jiami_md5 = new System.Windows.Forms.RichTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.rb_jiemi_md5 = new System.Windows.Forms.RichTextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(56, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 19);
            this.label1.TabIndex = 0;
            this.label1.Text = "IP";
            // 
            // lab_port
            // 
            this.lab_port.AutoSize = true;
            this.lab_port.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lab_port.Location = new System.Drawing.Point(281, 28);
            this.lab_port.Name = "lab_port";
            this.lab_port.Size = new System.Drawing.Size(49, 19);
            this.lab_port.TabIndex = 1;
            this.lab_port.Text = "port";
            this.lab_port.Click += new System.EventHandler(this.label2_Click);
            // 
            // tb_ip
            // 
            this.tb_ip.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tb_ip.Location = new System.Drawing.Point(109, 28);
            this.tb_ip.Name = "tb_ip";
            this.tb_ip.ReadOnly = true;
            this.tb_ip.Size = new System.Drawing.Size(166, 26);
            this.tb_ip.TabIndex = 2;
            this.tb_ip.Text = "127.0.0.1";
            // 
            // tb_port
            // 
            this.tb_port.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tb_port.Location = new System.Drawing.Point(336, 28);
            this.tb_port.Name = "tb_port";
            this.tb_port.Size = new System.Drawing.Size(68, 29);
            this.tb_port.TabIndex = 3;
            this.tb_port.Text = "2000";
            // 
            // rb_message_show
            // 
            this.rb_message_show.Location = new System.Drawing.Point(60, 111);
            this.rb_message_show.Name = "rb_message_show";
            this.rb_message_show.ReadOnly = true;
            this.rb_message_show.Size = new System.Drawing.Size(344, 123);
            this.rb_message_show.TabIndex = 4;
            this.rb_message_show.Text = "";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(56, 78);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(199, 19);
            this.label2.TabIndex = 5;
            this.label2.Text = "客户加密消息显示区：";
            // 
            // bnt_trans
            // 
            this.bnt_trans.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_trans.Location = new System.Drawing.Point(443, 28);
            this.bnt_trans.Name = "bnt_trans";
            this.bnt_trans.Size = new System.Drawing.Size(160, 29);
            this.bnt_trans.TabIndex = 6;
            this.bnt_trans.Text = "启动服务";
            this.bnt_trans.UseVisualStyleBackColor = true;
            this.bnt_trans.Click += new System.EventHandler(this.bnt_trans_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(56, 270);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(199, 19);
            this.label3.TabIndex = 8;
            this.label3.Text = "客户解密消息显示区：";
            // 
            // rb_jiemi_message
            // 
            this.rb_jiemi_message.Location = new System.Drawing.Point(60, 303);
            this.rb_jiemi_message.Name = "rb_jiemi_message";
            this.rb_jiemi_message.ReadOnly = true;
            this.rb_jiemi_message.Size = new System.Drawing.Size(344, 123);
            this.rb_jiemi_message.TabIndex = 7;
            this.rb_jiemi_message.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(56, 446);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(125, 19);
            this.label4.TabIndex = 9;
            this.label4.Text = "消息的MD5值:";
            // 
            // rb_md5_message
            // 
            this.rb_md5_message.Location = new System.Drawing.Point(60, 481);
            this.rb_md5_message.Name = "rb_md5_message";
            this.rb_md5_message.ReadOnly = true;
            this.rb_md5_message.Size = new System.Drawing.Size(344, 48);
            this.rb_md5_message.TabIndex = 10;
            this.rb_md5_message.Text = "";
            // 
            // rb_jiami_key
            // 
            this.rb_jiami_key.Location = new System.Drawing.Point(635, 111);
            this.rb_jiami_key.Name = "rb_jiami_key";
            this.rb_jiami_key.ReadOnly = true;
            this.rb_jiami_key.Size = new System.Drawing.Size(344, 48);
            this.rb_jiami_key.TabIndex = 11;
            this.rb_jiami_key.Text = "";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(631, 78);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(106, 19);
            this.label5.TabIndex = 12;
            this.label5.Text = "加密的key:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(631, 183);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(106, 19);
            this.label6.TabIndex = 14;
            this.label6.Text = "解密的key:";
            // 
            // rb_jiemi_key
            // 
            this.rb_jiemi_key.Location = new System.Drawing.Point(635, 216);
            this.rb_jiemi_key.Name = "rb_jiemi_key";
            this.rb_jiemi_key.ReadOnly = true;
            this.rb_jiemi_key.Size = new System.Drawing.Size(344, 48);
            this.rb_jiemi_key.TabIndex = 13;
            this.rb_jiemi_key.Text = "";
            // 
            // bnt_jiemi_key
            // 
            this.bnt_jiemi_key.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_jiemi_key.Location = new System.Drawing.Point(1020, 216);
            this.bnt_jiemi_key.Name = "bnt_jiemi_key";
            this.bnt_jiemi_key.Size = new System.Drawing.Size(160, 39);
            this.bnt_jiemi_key.TabIndex = 15;
            this.bnt_jiemi_key.Text = "解密key";
            this.bnt_jiemi_key.UseVisualStyleBackColor = true;
            this.bnt_jiemi_key.Click += new System.EventHandler(this.bnt_jiemi_key_Click);
            // 
            // bnt_jiemi_message
            // 
            this.bnt_jiemi_message.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_jiemi_message.Location = new System.Drawing.Point(427, 340);
            this.bnt_jiemi_message.Name = "bnt_jiemi_message";
            this.bnt_jiemi_message.Size = new System.Drawing.Size(160, 39);
            this.bnt_jiemi_message.TabIndex = 16;
            this.bnt_jiemi_message.Text = "解密消息";
            this.bnt_jiemi_message.UseVisualStyleBackColor = true;
            this.bnt_jiemi_message.Click += new System.EventHandler(this.bnt_jiemi_message_Click);
            // 
            // rb_jiami_md5
            // 
            this.rb_jiami_md5.Location = new System.Drawing.Point(635, 340);
            this.rb_jiami_md5.Name = "rb_jiami_md5";
            this.rb_jiami_md5.ReadOnly = true;
            this.rb_jiami_md5.Size = new System.Drawing.Size(344, 48);
            this.rb_jiami_md5.TabIndex = 17;
            this.rb_jiami_md5.Text = "";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.Location = new System.Drawing.Point(631, 300);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(106, 19);
            this.label7.TabIndex = 18;
            this.label7.Text = "加密的MD5:";
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(1020, 349);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(160, 39);
            this.button1.TabIndex = 19;
            this.button1.Text = "解密MD5";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // rb_jiemi_md5
            // 
            this.rb_jiemi_md5.Location = new System.Drawing.Point(635, 469);
            this.rb_jiemi_md5.Name = "rb_jiemi_md5";
            this.rb_jiemi_md5.ReadOnly = true;
            this.rb_jiemi_md5.Size = new System.Drawing.Size(344, 48);
            this.rb_jiemi_md5.TabIndex = 20;
            this.rb_jiemi_md5.Text = "";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(631, 436);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(106, 19);
            this.label8.TabIndex = 21;
            this.label8.Text = "解密的MD5:";
            // 
            // button2
            // 
            this.button2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button2.Location = new System.Drawing.Point(427, 481);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(160, 36);
            this.button2.TabIndex = 22;
            this.button2.Text = "计算MD5值";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button3.Location = new System.Drawing.Point(427, 540);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(215, 54);
            this.button3.TabIndex = 23;
            this.button3.Text = "比对MD5";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // encry_doc_trans_server
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1259, 621);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.rb_jiemi_md5);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.rb_jiami_md5);
            this.Controls.Add(this.bnt_jiemi_message);
            this.Controls.Add(this.bnt_jiemi_key);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.rb_jiemi_key);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.rb_jiami_key);
            this.Controls.Add(this.rb_md5_message);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.rb_jiemi_message);
            this.Controls.Add(this.bnt_trans);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.rb_message_show);
            this.Controls.Add(this.tb_port);
            this.Controls.Add(this.tb_ip);
            this.Controls.Add(this.lab_port);
            this.Controls.Add(this.label1);
            this.Name = "encry_doc_trans_server";
            this.Text = "encry_doc_trans_server";
            this.Load += new System.EventHandler(this.encry_doc_trans_server_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lab_port;
        private System.Windows.Forms.TextBox tb_ip;
        private System.Windows.Forms.TextBox tb_port;
        private System.Windows.Forms.RichTextBox rb_message_show;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button bnt_trans;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RichTextBox rb_jiemi_message;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RichTextBox rb_md5_message;
        private System.Windows.Forms.RichTextBox rb_jiami_key;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.RichTextBox rb_jiemi_key;
        private System.Windows.Forms.Button bnt_jiemi_key;
        private System.Windows.Forms.Button bnt_jiemi_message;
        private System.Windows.Forms.RichTextBox rb_jiami_md5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.RichTextBox rb_jiemi_md5;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
    }
}

