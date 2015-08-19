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

namespace CoreWPF.FMC
{
    /// <summary>
    /// Interaction logic for MPDI.xaml
    /// </summary>
    public partial class MPDI : UserControl
    {
        public event EventHandler f_AddMPDI;
        public event EventHandler f_DelMPDI;
        public event EventHandler f_MPDIListChangwSelection;
        public event EventHandler f_MPDIChangwSelection;

        //public System.Windows.Controls.ListBox GetMPDIList_listBox(){ return _MPDIList_listBox; }
        //public System.Windows.Controls.ListBox GetMPDI_listBox(){ return _MPDI_listBox; }


        public MPDI()
        {
            InitializeComponent();
            this.m_MPDI_listBox.SelectionMode = SelectionMode.Extended;
        }

        private void AddMPDI_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddMPDI != null)
            {
                f_AddMPDI(sender, e);
            }
        }

        private void DelMPDI_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelMPDI != null)
            {
                f_DelMPDI(sender, e);
            }
        }
        private void MPDIList_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            if (f_MPDIListChangwSelection != null)
            {
                f_MPDIListChangwSelection(sender, args);
            }
        }

        private void MPDI_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            if (f_MPDIChangwSelection != null)
            {
                f_MPDIChangwSelection(sender, args);
            }
        }

        private void m_MPDI_listBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            object item = m_MPDI_listBox.SelectedItem;
            if (item != null)
                DragDrop.DoDragDrop(m_MPDI_listBox, item, DragDropEffects.Move);
        }
    }
}
