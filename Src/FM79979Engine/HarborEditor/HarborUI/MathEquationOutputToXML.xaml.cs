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

namespace HarborUI
{
    /// <summary>
    /// Interaction logic for MathEquationOutputToXML.xaml
    /// </summary>
    public partial class MathEquationOutputToXML : UserControl
    {
        public EventHandler f_FileSave;
        public EventHandler f_FileOpen;
        public EventHandler f_AddVariable;
        public EventHandler f_DelVariable;
        System.Collections.Hashtable m_FunctionAndObject = new System.Collections.Hashtable();
        public MathEquationOutputToXML()
        {
            InitializeComponent();
        }
        public void SetFunction()
        {
            m_FunctionAndObject[m_FileOpen] = f_FileOpen;
            m_FunctionAndObject[m_FileSave] = f_FileSave;
            m_FunctionAndObject[m_AddButton] = f_AddVariable;
            m_FunctionAndObject[m_DelButton] = f_DelVariable;
        }
        private void m_FileOpen_Click(object sender, RoutedEventArgs e)
        {
            if (m_FunctionAndObject.ContainsKey(sender))
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, (EventHandler)m_FunctionAndObject[sender]);
            }
        }
    }
}
