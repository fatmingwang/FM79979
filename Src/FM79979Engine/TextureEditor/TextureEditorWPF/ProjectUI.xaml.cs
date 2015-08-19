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

namespace TextureEditorWPF.MPDI
{
    /// <summary>
    /// Interaction logic for ProjectUI.xaml
    /// </summary>
    public partial class ProjectUI : UserControl
    {
        //public event KeyEventHandler f_KeyDown;
        //public event KeyEventHandler f_KeyUp;
        public ProjectUI()
        {
            InitializeComponent();
        }
        private void WindowsFormsHost_KeyDown(object sender, KeyEventArgs e)
        {
            //if (f_KeyDown != null)
            //    f_KeyDown(sender, e);
        }

        private void WindowsFormsHost_KeyUp(object sender, KeyEventArgs e)
        {
            //if (f_KeyUp != null)
            //    f_KeyUp(sender, e);
        }
    }
}
