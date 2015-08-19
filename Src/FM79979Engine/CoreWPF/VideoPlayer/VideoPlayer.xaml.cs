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
using System.IO;
namespace CoreWPF.VideoPlayer
{
    /// <summary>
    /// Interaction logic for VideoPlayer.xaml
    /// </summary>
    public partial class VideoPlayer : UserControl
    {
        public event EventHandler f_FileOpen;
        public event EventHandler f_Play;
        public event EventHandler f_Pause;
        public event EventHandler f_Stop;
        public event EventHandler f_SliderValueChange;
        public event EventHandler f_MouseUp;
        public VideoPlayer()
        {
            InitializeComponent();
        }

        private void btnPlay_Click(object sender, RoutedEventArgs e)
        {
            if (f_Play != null)
            {
                f_Play(sender, e);
            }
        }

        private void btnPause_Click(object sender, RoutedEventArgs e)
        {
            if (f_Pause != null)
            {
                f_Pause(sender, e);
            }
        }

        private void btnStop_Click(object sender, RoutedEventArgs e)
        {
            if (f_Stop != null)
            {
                f_Stop(sender, e);
            }
        }

        private void sliderTime_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (f_SliderValueChange != null)
            {
                f_SliderValueChange(sender, e);
            }
        }

        private void Label_DragEnter(object sender, DragEventArgs e)
        {
            CoreWPF.Common.CommonFunction.DragEnter(sender, e);
        }

        private void Label_Drop(object sender, DragEventArgs e)
        {
            string[] filenames = (string[])e.Data.GetData(DataFormats.FileDrop, true);
            foreach (string filename in filenames)
            {
                string  l_strExtensionName = System.IO.Path.GetExtension(filename);
                if (l_strExtensionName.ToLower().CompareTo(".avi") == 0)
                {
                    //ListItem l_Listitem = new ListItem();
                    //l_Listitem.DataContext = filename;
                    m_FilmNameList.Items.Add(filename);
                }
            }
            e.Handled = true; 
        }

        private void lblSave_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (f_FileOpen != null)
            {
                f_FileOpen(sender, e);
            }
        }

        private void m_SliderTime_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (f_MouseUp != null)
            {
                f_MouseUp(sender, e);
            }
        }
    }
}
