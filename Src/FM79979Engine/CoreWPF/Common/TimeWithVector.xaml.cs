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
    /// Interaction logic for TimeWithVector.xaml
    /// </summary>
    public partial class TimeWithVector : UserControl
    {
        public System.Object      m_DataObject;
        public event EventHandler f_AddClick;
        public event EventHandler f_DeleteClick;
        public event EventHandler f_RevertClick;
        public event EventHandler f_InevertClick;
        public event EventHandler f_TimeChanged;
        public event EventHandler f_SelectionChanged;

        public event EventHandler f_LinerCheckboxChecked;
        public event EventHandler f_AverageAssignCheckboxChecked;
        //public event EventHandler f_NumericUpDownValueChanged;
        private bool m_bHoldForSync = false;

        public TimeWithVector()
        {
            InitializeComponent();
        }
        //1,2,3,4
        //float,Vector2,Vector3,Vector4
        public void SetupDateType( int e_iType)
        { 
            switch(e_iType)
            {
                case 1:
                    m_DataObject = new CoreWPF.Common.NumericUpDown();
                break;
                case 2:
                    m_DataObject = new CoreWPF.Common.XYNumeric();
                break;
                case 3:
                    m_DataObject = new CoreWPF.Common.XYZNumeric();
                break;
                case 4:
                    m_DataObject = new CoreWPF.Common.XYZWNumeric();
                break;
            }
            m_StackPanel.Children.Add((UIElement)m_DataObject);
        }
        public void LabelContent(String e_strContent)
        {
            m_DataName.Content = e_strContent;
        }

        
        private void AddClick(object sender, RoutedEventArgs e)
        {
            //Vector2 l_vVector = new Vector2();
            //l_vVector.x = this.m_XYNumerialControl.m_X_numericUpDown.Value;
            //l_vVector.y = this.m_XYNumerialControl.m_Y_numericUpDown.Value;
            //m_Vector2Array.Add(l_vVector);
            if (f_AddClick != null){f_AddClick(sender, e);}
        }

        private void DeleteClick(object sender, RoutedEventArgs e)
        {
            m_bHoldForSync = true;
            if (f_DeleteClick != null) { f_DeleteClick(sender, e); }
            m_bHoldForSync = false;
        }
        private void RevertClick(object sender, RoutedEventArgs e)
        {
            if (f_RevertClick != null) { f_RevertClick(sender, e); }
        }
        private void InvertClick(object sender, RoutedEventArgs e)
        {
            if (f_InevertClick != null) { f_InevertClick(sender, e); }
        }
        private void AllClick(object sender, RoutedEventArgs e)
        {
            m_bHoldForSync = true;
            m_DataListBox.SelectedItems.Clear();
            for (int i = 0; i < m_DataListBox.Items.Count; ++i)
            {
                m_DataListBox.SelectedItems.Add(m_DataListBox.Items[i].ToString());
            }
            m_bHoldForSync = false;
        }
        private void EventPointsClick(object sender, RoutedEventArgs e)
        {
            int l_iStartIndex = 0;
            if ( m_DataListBox.SelectedItems.Count > 0)
            {
                int l_iIndex = (int)m_DataListBox.Items.IndexOf(m_DataListBox.SelectedItems[0]);
                if (l_iIndex == 0)
                    l_iStartIndex = 1;
            }
            m_DataListBox.SelectedItems.Clear();
            for (; l_iStartIndex< m_DataListBox.Items.Count; l_iStartIndex += 2)
                m_DataListBox.SelectedItems.Add(l_iStartIndex);
        }
        private void InsertClick(object sender, RoutedEventArgs e)
        {
            if( f_InevertClick != null)f_InevertClick(sender,e);
        }

        private void m_Time_f_ValueChanged(object sender, EventArgs e)
        {
            if( f_TimeChanged != null)f_TimeChanged(sender,e);
        }

        private void m_DataListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (m_bHoldForSync)
                return;
            if (f_SelectionChanged != null) f_SelectionChanged(sender, e);
        }

        private void m_LinerCheckbox_Checked(object sender, RoutedEventArgs e)
        {
            if (f_LinerCheckboxChecked != null) f_LinerCheckboxChecked(sender, e);
        }

        private void m_AverageAssignCheckbox_Checked(object sender, RoutedEventArgs e)
        {
            if (f_AverageAssignCheckboxChecked != null) f_AverageAssignCheckboxChecked(sender, e);
        }
    }
}
