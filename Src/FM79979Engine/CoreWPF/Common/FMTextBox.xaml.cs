using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CoreWPF.Common
{
    public class cFMTextBoxArgs : EventArgs
    {
        public bool m_bAllowModify;
        public cFMTextBoxArgs()
        {
            m_bAllowModify = false;
        }
    }
    /// <summary>
    /// Interaction logic for FMTextBox.xaml
    /// </summary>
    public partial class FMTextBox : UserControl
    {
        // Delegate
        public delegate void IsSameNameAllowedHandler(object sender, cFMTextBoxArgs data);
        public event IsSameNameAllowedHandler f_IsSameNameAllowed;
        public event EventHandler f_LostFocus;//
        public bool m_bEnableQuickReName = true;
        //public static readonly DependencyProperty FooTextProperty = DependencyProperty.Register("FMText", typeof(string), typeof(FMTextBox));
        public String FMText
        {
            get 
            {
                return m_TextBox.Text; 
            }
            set 
            {
                OnProperyChanged(value);
            }

        }

        void OnProperyChanged(String e_strValue)
        {
            m_TextBox.Text = e_strValue;
        }

        String m_strGotFocusText;
        public FMTextBox()
        {
            InitializeComponent();
            //m_TextBox.DataContext = FMText;
            //m_TextBox.IsEnabled = false;
            m_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
            m_TextBox.GotFocus += new RoutedEventHandler(TextBox_GotFocus);
        }
        public FMTextBox(String e_strText)
        {
            InitializeComponent();
            //m_TextBox.DataContext = FMText;
            //m_TextBox.IsEnabled = false;
            m_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
            m_TextBox.GotFocus += new RoutedEventHandler(TextBox_GotFocus);
            this.m_TextBox.Text = e_strText;
        }

        void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            m_strGotFocusText = this.m_TextBox.Text;
        }

        void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            if (f_IsSameNameAllowed != null)
            {
                cFMTextBoxArgs  l_FMTextBoxArgs = new cFMTextBoxArgs();
                f_IsSameNameAllowed((object)m_TextBox, l_FMTextBoxArgs);
                if (l_FMTextBoxArgs.m_bAllowModify)
                {

                }
                else
                {
                    MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                    this.m_TextBox.Text = m_strGotFocusText;
                }
            }
            if (f_LostFocus != null )
            { 
                f_LostFocus(sender,e);
            }
        }

        private void TextBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {

        }

        private void TextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (m_bEnableQuickReName == false)
                return;
            if (e.Key == Key.F2)
            {
                m_TextBox.IsEnabled = true;
                m_TextBox.Focus();
                m_TextBox.SelectAll();
            }
            else
            if (e.Key == Key.Enter)
            {
                m_TextBox.IsEnabled = false;
            }
        }
    }
}
