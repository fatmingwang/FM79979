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

namespace TextureEditorWPF
{
    /// <summary>
    /// Interaction logic for MPDICamera.xaml
    /// </summary>
    public partial class MPDICamera : UserControl
    {
        public CoreWPF.Common.XYNumeric m_ViewWidthHeight;
        public CoreWPF.Common.XYZNumeric m_Angle;
        public CoreWPF.Common.XYZNumeric m_Pos;
        public event EventHandler f_AddClick;
        public event EventHandler f_DeleteClick;
        public event EventHandler f_RevertClick;
        public event EventHandler f_ViewWidthHeightApply;
        public event EventHandler f_AngleApply;
        public event EventHandler f_PositionApply;
        public MPDICamera()
        {
            InitializeComponent();

            m_ViewWidthHeightUI.SetupDateType(2);
            m_ViewWidthHeightUI.m_MainGrid.ColumnDefinitions[0].Width = new GridLength(100, GridUnitType.Pixel);
            m_ViewWidthHeightUI.LabelContent("Width,Height");

            m_PosUI.SetupDateType(3);
            m_PosUI.m_MainGrid.ColumnDefinitions[0].Width = new GridLength(80, GridUnitType.Pixel);
            m_PosUI.LabelContent("X,Z,Z");


            m_AngleUI.SetupDateType(3);
            m_AngleUI.m_MainGrid.ColumnDefinitions[0].Width = new GridLength(80, GridUnitType.Pixel);
            m_AngleUI.LabelContent("X,Z,Z");

            m_Angle = (CoreWPF.Common.XYZNumeric)m_AngleUI.m_DataObject;
            m_Pos = (CoreWPF.Common.XYZNumeric)m_PosUI.m_DataObject;
            m_ViewWidthHeight = (CoreWPF.Common.XYNumeric)m_ViewWidthHeightUI.m_DataObject;

            m_Angle.SetMaximum(999999999);
            m_Angle.SetMinimum(-999999999);
            m_Pos.SetMaximum(999999999);
            m_Pos.SetMinimum(-999999999);
            m_ViewWidthHeight.SetMaximum(999999999);
            m_ViewWidthHeight.SetMinimum(-999999999);
        }
        private void AddClick(object sender, RoutedEventArgs e)
        {
            //if (m_CameraNameListBox.Items.)
            if (f_AddClick != null) { f_AddClick(sender, e); }
        }
        private void DeleteClick(object sender, RoutedEventArgs e)
        {
            if (f_DeleteClick != null) { f_DeleteClick(sender, e); }
        }
        private void RevertClick(object sender, RoutedEventArgs e)
        {
            if (f_RevertClick != null) { f_RevertClick(sender, e); }
        }
        private void ViewWidthHeightApply(object sender, RoutedEventArgs e)
        {
            if (f_ViewWidthHeightApply != null) { f_ViewWidthHeightApply(sender, e); }
        }
        private void AngleApply(object sender, RoutedEventArgs e)
        {
            if (f_AngleApply != null) { f_AngleApply(sender, e); }
        }
        private void PositionApply(object sender, RoutedEventArgs e)
        {
            if (f_PositionApply != null) { f_PositionApply(sender, e); }
        }        

        
        public void SetAngleContent(String e_str) { m_AngleContent.Content = e_str; }
        public void SetGameresolutionContent(String e_str) { m_GameResolutionContent.Content = e_str; }
    }
}
