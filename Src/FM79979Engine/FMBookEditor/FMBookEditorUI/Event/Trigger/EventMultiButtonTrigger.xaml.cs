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
    /// Interaction logic for EventMultiButton.xaml
    /// </summary>
    public partial class EventMultiButtonTrigger : UserControl
    {
        public event EventHandler f_Revert;
        public event EventHandler f_Add;
        public event EventHandler f_Del;
        public EventMultiButtonTrigger()
        {
            InitializeComponent();
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_Add != null) f_Add(sender, e);
            //ListBoxItem l_Test = new ListBoxItem();
            //l_Test.Content = "A";
            //m_ButtonList.Items.Add(l_Test);
        }

        private void DelButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_Del != null)f_Del(sender, e);
        }

        private void Buttonist_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_Revert != null) f_Revert(sender, e);
        }
    }
}
