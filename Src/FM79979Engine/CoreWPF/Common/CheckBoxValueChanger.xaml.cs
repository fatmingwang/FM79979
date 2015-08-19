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
    /// <summary>
    /// Interaction logic for CheckBoxValueChanger.xaml
    /// </summary>
    public partial class CheckBoxValueChanger : UserControl
    {
        unsafe public bool*m_pbValue;
        unsafe public CheckBoxValueChanger()
        {
            m_pbValue = null;
            InitializeComponent();
        }

        unsafe public void SetValue(bool* e_pbValue)
        { 
            m_pbValue = e_pbValue;
        }

        unsafe private void CheckBox_Checked_1(object sender, RoutedEventArgs e)
        {
            if( m_pbValue != null )
            {
                *m_pbValue = this.m_CheckBox.IsChecked==true?true:false;
            }
        }
    }
}
