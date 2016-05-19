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
            this.bnt_sotp = new System.Windows.Forms.Button();
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
            this.rb_message_show.Size = new System.Drawing.Size(344, 227);
            this.rb_message_show.TabIndex = 4;
            this.rb_message_show.Text = "";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(56, 78);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(161, 19);
            this.label2.TabIndex = 5;
            this.label2.Text = "客户消息显示区：";
            // 
            // bnt_trans
            // 
            this.bnt_trans.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_trans.Location = new System.Drawing.Point(429, 111);
            this.bnt_trans.Name = "bnt_trans";
            this.bnt_trans.Size = new System.Drawing.Size(160, 29);
            this.bnt_trans.TabIndex = 6;
            this.bnt_trans.Text = "启动服务";
            this.bnt_trans.UseVisualStyleBackColor = true;
            this.bnt_trans.Click += new System.EventHandler(this.bnt_trans_Click);
            // 
            // bnt_sotp
            // 
            this.bnt_sotp.Font = new System.Drawing.Font("宋体", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.bnt_sotp.Location = new System.Drawing.Point(429, 309);
            this.bnt_sotp.Name = "bnt_sotp";
            this.bnt_sotp.Size = new System.Drawing.Size(160, 29);
            this.bnt_sotp.TabIndex = 7;
            this.bnt_sotp.Text = "停止服务";
            this.bnt_sotp.UseVisualStyleBackColor = true;
            this.bnt_sotp.Visible = false;
            this.bnt_sotp.Click += new System.EventHandler(this.bnt_sotp_Click);
            // 
            // encry_doc_trans_server
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(612, 363);
            this.Controls.Add(this.bnt_sotp);
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
        private System.Windows.Forms.Button bnt_sotp;
    }
}

