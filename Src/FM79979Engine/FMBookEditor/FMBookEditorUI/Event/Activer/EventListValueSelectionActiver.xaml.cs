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
    /// Interaction logic for EventListValueSelectionActiver.xaml
    /// </summary>
    public partial class EventListValueSelectionActiver : UserControl
    {
        public EventListValueSelectionActiver()
        {
            InitializeComponent();
        }

        private void m_ValueChangeType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (this.m_ValueChangeType.SelectedIndex == 2)
            {
                TextBlock AttrLabel = new TextBlock();
                TextBox AttrTextBox = new TextBox();
                AttrLabel.Text = "TargetValue";
                this.m_MainGrid.Children.Add(AttrLabel);
                this.m_MainGrid.Children.Add(AttrTextBox);
                Grid.SetRow(AttrLabel,2);
                Grid.SetRow(AttrTextBox, 2);
                Grid.SetColumn(AttrLabel, 0);
                Grid.SetColumn(AttrTextBox, 1);
            }
            else 
            {
                if (m_MainGrid.Children.Count > 4)
                {
                    m_MainGrid.Children.RemoveAt(4);
                    m_MainGrid.Children.RemoveAt(4);
                }
            }
        }
    }
}
