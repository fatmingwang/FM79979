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

namespace FMBookEditorUI
{
    /// <summary>
    /// Interaction logic for EventInstance.xaml
    /// </summary>
    public partial class EventInstance : UserControl
    {
        public event EventHandler f_ActiverEventListboxDoubleClick;
        public event EventHandler f_TriggerEventListboxDoubleClick;
        public event EventHandler f_DelEvent;
        public event EventHandler f_EventInit;

        public EventInstance()
        {
            InitializeComponent();
        }

        private void ActiverListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_ActiverEventListboxDoubleClick != null)
            {
                f_ActiverEventListboxDoubleClick(sender, e);
            }
        }

        private void TriggerListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_TriggerEventListboxDoubleClick != null)
            {
                f_TriggerEventListboxDoubleClick(sender, e);
            }
        }

        private void AddEvent_Button_Click(object sender, RoutedEventArgs e)
        {
            if (m_TabControl.SelectedIndex == 0)
            {
                if (f_TriggerEventListboxDoubleClick != null)
                {
                    f_TriggerEventListboxDoubleClick(this.m_TriggerListBox, e);
                }
            }
            if (m_TabControl.SelectedIndex == 1)
            {
                if (f_ActiverEventListboxDoubleClick != null)
                {
                    f_ActiverEventListboxDoubleClick(this.m_ActiverListBox, e);
                }
            }
        }

        private void DelEvent_Button_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelEvent != null)
            {
                f_DelEvent(sender, e);
            }
        }

        private void EventInitButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_EventInit != null)
            { 
                f_EventInit(sender,e);
            }
        }
    }
}
