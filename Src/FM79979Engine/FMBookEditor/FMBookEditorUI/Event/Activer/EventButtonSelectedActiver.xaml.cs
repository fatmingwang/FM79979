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

namespace FMBookEditorUI.Event.Activer
{
    /// <summary>
    /// Interaction logic for EventButtonSelectedActiver.xaml
    /// </summary>
    public partial class EventButtonSelectedActiver : UserControl
    {
        public event EventHandler f_AddResult;
        public event EventHandler f_DelResult;
        public EventButtonSelectedActiver()
        {
            InitializeComponent();
        }

        private void ButtonAdd_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddResult != null)
            {
                f_AddResult(sender, e);
            }
        }

        private void ButtonDel_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelResult != null)
            {
                f_DelResult(sender, e);
            }
        }
    }
}
