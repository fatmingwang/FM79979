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
    /// Interaction logic for Sound.xaml
    /// </summary>
    public partial class Sound : UserControl
    {
        public event EventHandler f_AddSound;
        public event EventHandler f_DelSound;
        public event EventHandler f_SoundChangwSelection;

        public event EventHandler f_PlaySound;
        public event EventHandler f_StopSound;
        public event EventHandler f_OpenSoundFile;
        public event EventHandler f_SaveoundFile;

        public event EventHandler f_LoopCheckboxChecked;
        public Sound()
        {
            InitializeComponent();
        }
        private void AddSound_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddSound != null)
            {
                f_AddSound(sender, e);
            }
        }

        private void DelSound_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelSound != null)
            {
                f_DelSound(sender, e);
            }
        }

        private void Sound_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            if (f_SoundChangwSelection != null)
            {
                f_SoundChangwSelection(sender, args);
            }
        }

        private void PlaySound_Click(object sender, RoutedEventArgs e)
        {
            if (f_PlaySound != null)
            {
                f_PlaySound(sender, e);
            }
        }
        private void StopSound_Click(object sender, RoutedEventArgs e)
        {
            if (f_StopSound != null)
            {
                f_StopSound(sender, e);
            }
        }
        private void OpenSoundFile_Click(object sender, RoutedEventArgs e)
        {
            if (f_OpenSoundFile != null)
            {
                f_OpenSoundFile(sender, e);
            }
        }
        private void SaveoundFile_Click(object sender, RoutedEventArgs e)
        {
            if (f_SaveoundFile != null)
            {
                f_SaveoundFile(sender, e);
            }
        }

        private void m_Loop_Checkbox_Checked(object sender, RoutedEventArgs e)
        {
            if (f_LoopCheckboxChecked != null)
            {
                f_LoopCheckboxChecked(sender, e);
            }
        }

        private void m_Sound_Listbox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            object item = m_Sound_Listbox.SelectedItem;
            if (item != null)
                DragDrop.DoDragDrop(m_Sound_Listbox, item, DragDropEffects.Move);
        }
    }
}
