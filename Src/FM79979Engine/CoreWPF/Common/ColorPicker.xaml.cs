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
    /// Interaction logic for ColorPicker.xaml
    /// </summary>
    public partial class ColorPicker : UserControl
    {
        public event EventHandler f_SelectedColorChanged;
        public ColorPicker()
        {
            InitializeComponent();
        }

        private void ColorPicker_SelectedColorChanged(object sender, RoutedPropertyChangedEventArgs<Color?> e)
        {
            if (f_SelectedColorChanged != null)
            {
                f_SelectedColorChanged(sender,e);
            }
        }
        public Color GetSelectedColor()
        {
            return this.m_ColorPicker.SelectedColor.Value;
        }
        public void SetUsingAlphaChannel(bool e_b)
        {
            this.m_ColorPicker.UsingAlphaChannel = e_b;
        }

    }
}
