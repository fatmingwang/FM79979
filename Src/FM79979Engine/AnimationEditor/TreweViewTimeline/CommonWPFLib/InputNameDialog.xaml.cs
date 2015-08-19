using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace CommonWPFLib
{
    /// <summary>
    /// Interaction logic for InputNameDialog.xaml
    /// </summary>
    public partial class InputNameDialog : Window
    {
        //var dialog = new MyDialog();
        //if (dialog.ShowDialog() == true) 
        //{
        //    MessageBox.Show("You said: " + dialog.ResponseText);
        //}
        public InputNameDialog()
        {
            InitializeComponent();
            ResponseTextBox.Focus();
        }

        public string ResponseText
        {
            get { return ResponseTextBox.Text; }
            set { ResponseTextBox.Text = value; }
        }

        private void YesButton_Click(object sender, RoutedEventArgs e)
        {
            // Do something with the Input
            //String input = ResponseTextBox.Text;
            // Clear InputBox.
            //ResponseTextBox.Text = String.Empty;
            if ( ResponseTextBox.Text.Length > 0 )
                DialogResult = true;
            else
                DialogResult = false;
            this.Close();
        }

        private void NoButton_Click(object sender, RoutedEventArgs e)
        {
            // Clear InputBox.
            ResponseTextBox.Text = String.Empty;
            DialogResult = false;
            this.Close();
        }

        private void ResponseTextBox_KeyUp_1(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            { 
                RoutedEventArgs e2 = null;
                YesButton_Click(sender, e2);
            }
        }
    }
}
