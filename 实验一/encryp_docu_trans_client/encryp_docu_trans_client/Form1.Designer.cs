namespace encryp_docu_trans_client
{
    partial class Form1
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
            this.rb_message_show.Size = new System.Drawing.Size(344, 227);
            this.rb_message_show.TabIndex = 11;
            this.rb_message_show.Text = "";
            // 
            // bnt_message_trans
            // 
            this.bnt_message_trans.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_message_trans.Location = new System.Drawing.Point(397, 169);
            this.bnt_message_trans.Name = "bnt_message_trans";
            this.bnt_message_trans.Size = new System.Drawing.Size(139, 54);
            this.bnt_message_trans.TabIndex = 12;
            this.bnt_message_trans.Text = "发送消息";
            this.bnt_message_trans.UseVisualStyleBackColor = true;
            this.bnt_message_trans.Click += new System.EventHandler(this.bnt_message_trans_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(625, 364);
            this.Controls.Add(this.bnt_message_trans);
            this.Controls.Add(this.rb_message_show);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tb_port);
            this.Controls.Add(this.tb_ip);
            this.Controls.Add(this.lab_port);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Form1";
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
    }
}

