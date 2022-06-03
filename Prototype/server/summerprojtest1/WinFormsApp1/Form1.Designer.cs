
namespace WinFormsApp1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lb_w = new System.Windows.Forms.Label();
            this.lb_a = new System.Windows.Forms.Label();
            this.lb_s = new System.Windows.Forms.Label();
            this.lb_d = new System.Windows.Forms.Label();
            this.lb_requesting = new System.Windows.Forms.Label();
            this.lb_output = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lb_w
            // 
            this.lb_w.AutoSize = true;
            this.lb_w.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.lb_w.Location = new System.Drawing.Point(173, 82);
            this.lb_w.Name = "lb_w";
            this.lb_w.Size = new System.Drawing.Size(36, 32);
            this.lb_w.TabIndex = 0;
            this.lb_w.Text = "W";
            // 
            // lb_a
            // 
            this.lb_a.AutoSize = true;
            this.lb_a.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.lb_a.Location = new System.Drawing.Point(76, 159);
            this.lb_a.Name = "lb_a";
            this.lb_a.Size = new System.Drawing.Size(29, 32);
            this.lb_a.TabIndex = 1;
            this.lb_a.Text = "A";
            // 
            // lb_s
            // 
            this.lb_s.AutoSize = true;
            this.lb_s.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.lb_s.Location = new System.Drawing.Point(173, 159);
            this.lb_s.Name = "lb_s";
            this.lb_s.Size = new System.Drawing.Size(27, 32);
            this.lb_s.TabIndex = 2;
            this.lb_s.Text = "S";
            // 
            // lb_d
            // 
            this.lb_d.AutoSize = true;
            this.lb_d.Font = new System.Drawing.Font("Segoe UI", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.lb_d.Location = new System.Drawing.Point(262, 159);
            this.lb_d.Name = "lb_d";
            this.lb_d.Size = new System.Drawing.Size(31, 32);
            this.lb_d.TabIndex = 3;
            this.lb_d.Text = "D";
            // 
            // lb_requesting
            // 
            this.lb_requesting.AutoSize = true;
            this.lb_requesting.Location = new System.Drawing.Point(337, 299);
            this.lb_requesting.Name = "lb_requesting";
            this.lb_requesting.Size = new System.Drawing.Size(84, 15);
            this.lb_requesting.TabIndex = 4;
            this.lb_requesting.Text = "not reqesuting";
            // 
            // lb_output
            // 
            this.lb_output.AutoSize = true;
            this.lb_output.Location = new System.Drawing.Point(506, 250);
            this.lb_output.Name = "lb_output";
            this.lb_output.Size = new System.Drawing.Size(58, 15);
            this.lb_output.TabIndex = 5;
            this.lb_output.Text = "lb_output";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.lb_output);
            this.Controls.Add(this.lb_requesting);
            this.Controls.Add(this.lb_d);
            this.Controls.Add(this.lb_s);
            this.Controls.Add(this.lb_a);
            this.Controls.Add(this.lb_w);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lb_w;
        private System.Windows.Forms.Label lb_a;
        private System.Windows.Forms.Label lb_s;
        private System.Windows.Forms.Label lb_d;
        private System.Windows.Forms.Label lb_requesting;
        private System.Windows.Forms.Label lb_output;
    }
}

