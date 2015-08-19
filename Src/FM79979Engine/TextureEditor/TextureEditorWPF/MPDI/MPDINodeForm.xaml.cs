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
    /// Interaction logic for MPDINodeForm.xaml
    /// </summary>
    public partial class MPDINodeForm : UserControl
    {
        public event EventHandler               f_ShowCheckboxCheckedChange;
        public event EventHandler               f_CameraResetButtonClick;

        public event KeyEventHandler            f_KeyDown;
        public event KeyEventHandler            f_KeyUp;
        public event EventHandler               f_OpenGLFormSizeChanged;
        public int                              m_iOpenglRenderPanelWidth = 0;
        public int                              m_iOpenglRenderPanelHeight = 0;
        public MPDINodeForm()
        {
            //Xceed.Wpf.Toolkit.TimelinePanel

            InitializeComponent();
        }

        private void CheckBoxCheckedEvent(object sender, RoutedEventArgs e)
        {
             CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ShowCheckboxCheckedChange);
        }

        private void WindowsFormsHost_KeyDown(object sender, KeyEventArgs e)
        {
            if (f_KeyDown != null)
                f_KeyDown(sender, e);
        }

        private void WindowsFormsHost_KeyUp(object sender, KeyEventArgs e)
        {
            if (f_KeyUp != null)
                f_KeyUp(sender, e);
        }

        private void MyOpenGL_SizeChanged(object sender, EventArgs e)
        {
            System.Windows.Forms.Panel l_Panel = sender as System.Windows.Forms.Panel;
            m_iOpenglRenderPanelWidth = l_Panel.Width;
            m_iOpenglRenderPanelHeight = l_Panel.Height;
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_OpenGLFormSizeChanged);
        }

        private void CameraResetButton_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_CameraResetButtonClick);
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {

        }

        public void SetGoodSize()
        {
            m_MPDIDataPanel.DockHeight = new GridLength(330);
            m_FileAnimationName.DockWidth = new GridLength(200);
            m_RightUtilityPanel.DockWidth = new GridLength(150);
        }
    }
}