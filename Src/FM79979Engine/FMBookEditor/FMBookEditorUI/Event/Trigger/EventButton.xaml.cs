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

namespace FMBookEditorUI.Event.Trigger
{
    /// <summary>
    /// Interaction logic for EventButton.xaml
    /// </summary>
    public partial class EventButton : UserControl
    {
        public event EventHandler f_OpenColorDialog;
        public event EventHandler f_Valdiate;
        public EventButton()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if( f_OpenColorDialog != null)
            {
                f_OpenColorDialog(sender,e);
            }
        }

        private void m_ImageName_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (f_Valdiate != null)
            {
                f_Valdiate(sender, e);
            }
        }

        private void m_RepeatCheckbox_Checked(object sender, RoutedEventArgs e)
        {
            if (f_Valdiate != null)
            {
                f_Valdiate(sender, e);
            }
        }
    }
}
