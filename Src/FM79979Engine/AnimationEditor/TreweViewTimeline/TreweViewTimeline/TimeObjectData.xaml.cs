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

namespace TreweViewTimeline
{
    /// <summary>
    /// Interaction logic for TimeObjectData.xaml
    /// </summary>
    public partial class TimeObjectData : UserControl
    {
        BitmapImage m_EyeImage = new BitmapImage(new Uri("pack://application:,,,/TreweViewTimeline;component/Images/Eye.png"));
        BitmapImage m_InactiveImage = new BitmapImage(new Uri("pack://application:,,,/TreweViewTimeline;component/Images/Inactive.png"));
        Image       m_ShowImage = new Image();
        TextBox     m_TextBox = new TextBox();

        bool m_bVisible = true;
        public TimeObjectData()
        {
            InitializeComponent();

            object l_Object = this.Content;
            Grid    l_grid = (Grid)l_Object;
            m_ShowImage.Width = 14; m_ShowImage.Height = 12; m_ShowImage.Source = m_EyeImage; m_ShowImage.MouseUp += Image_MouseUp; Grid.SetColumn(m_ShowImage, 0);
            m_TextBox.MinWidth = 80; m_TextBox.IsEnabled = false;  Grid.SetColumn(m_TextBox, 1);
            l_grid.Children.Add(m_ShowImage);
            l_grid.Children.Add(m_TextBox);
            //<Image Grid.Column="0" Source="pack://application:,,,/TreweViewTimeline;component/Images/Eye.png" Width="14" Height="12" Name="m_Image" x:FieldModifier="public" />
            //<TextBox Grid.Column="1" MinWidth="80" Name="m_TextBox" x:FieldModifier="public" IsEnabled="False" />
        }

        void Image_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (m_bVisible)
            {
                m_ShowImage.Source = m_InactiveImage;
            }
            else
            {
                m_ShowImage.Source = m_EyeImage;
            }
            m_bVisible = !m_bVisible;
        }

        public void SetName(string e_strName)
        {
            m_TextBox.Text = e_strName;
        }
        public string GetName()
        {
            return m_TextBox.Text;
        }
        public  void SetContextMenu(ContextMenu e_ContextMenu)
        {
            //if(m_TextBox.ContextMenu != null )
            {
                m_TextBox.ContextMenu = e_ContextMenu;
            }
        }
    }
}
