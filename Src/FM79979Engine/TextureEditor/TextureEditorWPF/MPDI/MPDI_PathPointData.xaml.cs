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
using System.Collections.ObjectModel;
namespace TextureEditorWPF
{
    /// <summary>
    /// Interaction logic for MPDI_PathPointData.xaml
    /// </summary>
    public partial class MPDI_PathPointData : UserControl
    {
        public enum eTabControlSelectedIndex
        {
            eTSI_POS = 0,
            eTSI_COLOR,
            eTSI_SIZE,
            eTSI_ROTATION,
            eTSI_IMAGE,
            eTSI_MAX
        }
        private Dictionary<eTabControlSelectedIndex, EventHandler> m_TabcontrolSelectedIndexWithAddButtonFunction = new Dictionary<eTabControlSelectedIndex,EventHandler>();
        //
        public event EventHandler f_PositionAddButtonClick;
        public event EventHandler f_ColorAddButtonClick;
        public event EventHandler f_SizeAddButtonClick;
        public event EventHandler f_RotationAddButtonClick;
        public event EventHandler f_AnimationImageAddButtonClick;
        //
        public event EventHandler f_PathBehaviorComboxSelecteChanged;
        //

        public event EventHandler f_InsertButtonClick;
        public event EventHandler f_DeleteButtonClick;
        public event EventHandler f_LoopClick;
        public event EventHandler f_LinerUpdateClick;
        public event EventHandler f_ImageAnchorSelectionChanged;
        //
        public event EventHandler f_ColorFadeIn;
        public event EventHandler f_ColorFadeOut;
        //
        public event EventHandler f_ListBoxSelectionChanged;
        public event EventHandler f_TabControlSelectionChanged;
        //
        public event EventHandler f_PlayChecked;
        public event EventHandler f_LoopChecked;

        private void SelectedColorChanged(object sender,EventArgs e)
        {
		    Color l_Color = m_ColorPicker.GetSelectedColor();
            this.m_Color.m_X_numericUpDown.Value = l_Color.R;
            this.m_Color.m_Y_numericUpDown.Value = l_Color.G;
            this.m_Color.m_Z_numericUpDown.Value = l_Color.B;
            this.m_Color.m_W_numericUpDown.Value = l_Color.A;
        }
        bool m_bSelectOdd = false;
        public MPDI_PathPointData()
        {
            InitializeComponent();

            m_InsertCount.Maximum = 100;
            m_InsertCount.Minimum = 0;
            m_InsertCount.Increment = 1;

            m_ColorPicker.f_SelectedColorChanged += SelectedColorChanged;
            m_Color.Increment(0.01);
            m_Color.SetValue(1, 1, 1, 1);
            m_Color.SetMaximum(2);
            m_Color.SetMinimum(0);

            m_Pos.Increment(1);
            m_Pos.SetValue(0, 0, 0);
            m_Pos.SetMaximum(9999999);
            m_Pos.SetMinimum(-9999999);

            m_Size.Increment(1);
            m_Size.SetValue(0, 0);
            m_Size.SetMaximum(999999);
            m_Size.SetMinimum(-999999);

            m_Scale.SetValue(100);
            m_Scale.Increment = 1;
            m_Scale.Maximum = 999999;
            m_Scale.Minimum = -9999;

            m_Rotation.SetValue(0, 0, 0);
            m_Rotation.Increment(1);
            m_Rotation.SetMinimum(-9999999);
            m_Rotation.SetMaximum(9999999);

            m_AllDataTotalTime.Minimum = m_CurrentDataTotalTime.Minimum = m_SelectedTime.Minimum = m_StartTime.Minimum = 0;
            m_AllDataTotalTime.Maximum = m_CurrentDataTotalTime.Maximum = m_SelectedTime.Maximum = m_StartTime.Maximum = (double)999999;
            m_AllDataTotalTime.Increment = m_CurrentDataTotalTime.Increment = m_SelectedTime.Increment = m_StartTime.Increment = 0.1;
            m_CurrentDataTotalTime.m_bUseKeyToDoValueChange = m_StartTime.m_bUseKeyToDoValueChange = m_SelectedTime.m_bUseKeyToDoValueChange = m_AllDataTotalTime.m_bUseKeyToDoValueChange = true;

            //var tmp1 = new TempDataType() { StartTime = 10, EndTime = 25, Name = "Temp 1" };
            //var tmp2 = new TempDataType() { StartTime = 3, EndTime = 18, Name = "Temp 2" };
            //var tmp3 = new TempDataType() { StartTime = 44, EndTime = 60, Name = "Temp 3" };
            //ObservableCollection<ITimeLineDataItem> t1Data = new ObservableCollection<ITimeLineDataItem>();
            //ObservableCollection<ITimeLineDataItem> t2Data = new ObservableCollection<ITimeLineDataItem>();
            //ObservableCollection<ITimeLineDataItem> t3Data = new ObservableCollection<ITimeLineDataItem>();
            //t1Data.Add(tmp1);
            //t2Data.Add(tmp2);
            //t3Data.Add(tmp3);
            //TimeLine2.Items = t2Data;
            //TimeLine3.Items = t3Data;
            //TimeLine2.StartDate = 0;
            //TimeLine3.StartDate = 0;
            //TempDataType l_TempDataType1 = CreateTempDataType("tes1", 0, 1000);
            //TempDataType l_TempDataType2 = CreateTempDataType("test2", 0, 1200);
            //TempDataType l_TempDataType3 = CreateTempDataType("test3", 0, 100);
            //AddTimeLineControl(l_TempDataType1);
            //AddTimeLineControl(l_TempDataType2);
            //AddTimeLineControl(l_TempDataType3);
        }
        public void InitData()
        {
            m_TabcontrolSelectedIndexWithAddButtonFunction[eTabControlSelectedIndex.eTSI_POS]       = f_PositionAddButtonClick;
            m_TabcontrolSelectedIndexWithAddButtonFunction[eTabControlSelectedIndex.eTSI_COLOR]     = f_ColorAddButtonClick;
            m_TabcontrolSelectedIndexWithAddButtonFunction[eTabControlSelectedIndex.eTSI_SIZE]      = f_SizeAddButtonClick;
            m_TabcontrolSelectedIndexWithAddButtonFunction[eTabControlSelectedIndex.eTSI_ROTATION]  = f_RotationAddButtonClick;
            m_TabcontrolSelectedIndexWithAddButtonFunction[eTabControlSelectedIndex.eTSI_IMAGE]     = f_AnimationImageAddButtonClick;
        }
        private void Slider_Scale_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //m_ZoomSliderText.Text = "Zoom Slider:" + Slider_Scale.Value.ToString();
            //foreach (TimeLineControl item in m_ItemControl.Items)
            //{
            //    item.UnitSize = Slider_Scale.Value;
            //}
        }
        #region CreateTempDataType
        //public TempDataType CreateTempDataType(string e_strName, int e_iStartTime, int e_iEndTime)
        //{
        //    var temp = new TempDataType()
        //    {
        //        StartTime = e_iStartTime,
        //        EndTime = e_iEndTime,
        //        Name = e_strName
        //    };
        //    return temp;
        //}
        #endregion
        #region CreateDataTemplate
        DataTemplate CreateDataTemplate()
        {
            //http://stackoverflow.com/questions/248362/how-do-i-build-a-datatemplate-in-c-sharp-code
            //create the data template
            DataTemplate cardLayout = new DataTemplate();
            //cardLayout.DataType = typeof(CreditCardPayment);

            //set up the stack panel
            FrameworkElementFactory spFactory = new FrameworkElementFactory(typeof(StackPanel));
            spFactory.Name = "myComboFactory";
            spFactory.SetValue(StackPanel.OrientationProperty, Orientation.Horizontal);

            //set up the card holder textblock
            FrameworkElementFactory cardHolder = new FrameworkElementFactory(typeof(TextBlock));
            cardHolder.SetBinding(TextBlock.TextProperty, new Binding("BillToName"));
            cardHolder.SetValue(TextBlock.ToolTipProperty, "Card Holder Name");
            spFactory.AppendChild(cardHolder);

            //set up the card number textblock
            FrameworkElementFactory cardNumber = new FrameworkElementFactory(typeof(TextBlock));
            cardNumber.SetBinding(TextBlock.TextProperty, new Binding("SafeNumber"));
            cardNumber.SetValue(TextBlock.ToolTipProperty, "Credit Card Number");
            spFactory.AppendChild(cardNumber);

            //set up the notes textblock
            FrameworkElementFactory notes = new FrameworkElementFactory(typeof(TextBlock));
            notes.SetBinding(TextBlock.TextProperty, new Binding("Notes"));
            notes.SetValue(TextBlock.ToolTipProperty, "Notes");
            spFactory.AppendChild(notes);

            //set the visual tree of the data template
            cardLayout.VisualTree = spFactory;

            //set the item template to be our shiny new data template
            //drpCreditCardNumberWpf.ItemTemplate = cardLayout;
            return cardLayout;
        }
        #endregion
        #region AddTimeLineControl
        //public void AddTimeLineControl(TempDataType e_TempDataType)
        //{
        //    TimeLineTool.TimeLineControl l_NewToolLineControl = new TimeLineControl();
        //    ObservableCollection<TempDataType> l_NewData = new ObservableCollection<TempDataType>();
        //    var converter = new System.Windows.Media.BrushConverter();

        //    //TextBox b = (TextBox)Resources["teste01"];
        //    l_NewData.Add(e_TempDataType);
        //    l_NewToolLineControl.Items = l_NewData;

        //    l_NewToolLineControl.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
        //    l_NewToolLineControl.UnitSize = Slider_Scale.Value;
        //    l_NewToolLineControl.MinimumUnitWidth = 1;
        //    l_NewToolLineControl.Background = (Brush)converter.ConvertFromString("WhiteSmoke"); ;
        //    l_NewToolLineControl.DrawTimeGrid = true;
        //    l_NewToolLineControl.MinWidth = 50;
        //    l_NewToolLineControl.SynchedWithSiblings = true;
        //    l_NewToolLineControl.Height = 80;
        //    l_NewToolLineControl.ViewLevel = TimeLineViewLevel.Minutes;
        //    l_NewToolLineControl.ItemTemplate = (DataTemplate)Resources["UsedTemplateProperty"];

        //    l_NewToolLineControl.StartDate = 0;
        //    //m_StackPanel.Children.Add(l_NewToolLineControl);
        //    l_NewToolLineControl.DrawBackGround(true);
            
        //    m_ItemControl.Items.Add(l_NewToolLineControl);
        //}
        #endregion

        private void m_ViewLevelCombox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //TimeLineViewLevel   l_TimeLineViewLevel;
            //switch (m_ViewLevelCombox.SelectedIndex)
            //{ 
            //    case 0:
            //        l_TimeLineViewLevel = TimeLineViewLevel.Seconds;
            //    break;
            //    case 1:
            //        l_TimeLineViewLevel = TimeLineViewLevel.Minutes;
            //    break;
            //    case 2:
            //        l_TimeLineViewLevel = TimeLineViewLevel.Hours;
            //    break;
            //    default:
            //    l_TimeLineViewLevel = TimeLineViewLevel.Seconds;
            //    break;
            //}
            //foreach (TimeLineControl item in m_ItemControl.Items)
            //{
            //    item.ViewLevel = l_TimeLineViewLevel;
            //}
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            //1
            if (sender == m_SelectAllButton)
            {
                m_CurrentWorkingListbox.SelectAll();
            }//2
            else
            if (sender == m_FirstAndEndButton)
            {
                m_CurrentWorkingListbox.UnselectAll();
                if (m_CurrentWorkingListbox.Items.Count > 1)
                {
                    ListBoxItem l_Item1 = (ListBoxItem)m_CurrentWorkingListbox.Items[0];
                    ListBoxItem l_Item2 = (ListBoxItem)m_CurrentWorkingListbox.Items[m_CurrentWorkingListbox.Items.Count - 1];
                    l_Item1.IsSelected = true;
                    l_Item2.IsSelected = true;
                }
            }
            else//3
            if (sender == m_RemoveButton)
            {
                if (m_CurrentWorkingListbox.SelectedItem != null)
                    CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_DeleteButtonClick);            
            }
            else//4
            if (sender == m_InsertButton)
            {
                if (m_InsertCount.Value != 0)
                {
                    CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_InsertButtonClick);
                }
            }
            else//5
            if (sender == m_SelectHalfButton)
            {
                m_bSelectOdd = !m_bSelectOdd;
                int l_iTargetValue = 2;
                for (int i = 0; i < m_CurrentWorkingListbox.Items.Count; ++i)
                {
                    ListBoxItem l_Item = (ListBoxItem)m_CurrentWorkingListbox.Items[i];
                    if ( i % l_iTargetValue == 0 )
                    {
                        l_Item.IsSelected = m_bSelectOdd;
                    }
                    else
                    {
                        l_Item.IsSelected = !m_bSelectOdd;
                    }
                }
            }
            else//7
            if (sender == m_LoopCheckBox)
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_LoopClick);
            }
            else//8
            if (sender == m_LinerUpdateCheckBox)
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_LinerUpdateClick);
            }
            else//4
            if (sender == m_AddButton)
            {
                if(m_TabcontrolSelectedIndexWithAddButtonFunction.ContainsKey((eTabControlSelectedIndex)m_TabControl.SelectedIndex))
                {
                    EventHandler l_EventHandler = m_TabcontrolSelectedIndexWithAddButtonFunction[(eTabControlSelectedIndex)m_TabControl.SelectedIndex];
                    CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, l_EventHandler);
                }
            }
            else
            if (sender == m_SinglePlayCheck || sender == m_AllPlayCheck)
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_PlayChecked);
            }
            else
            if (sender == m_LoopCheck)
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_LoopChecked);
            }
        }

        private void ImageAnchorSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ImageAnchorSelectionChanged);
        }

        private void ColorFadeIn(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ColorFadeIn);
        }

        private void ColorFadeOut(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ColorFadeOut);
        }

        private void ListBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ListBoxSelectionChanged);
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.Source is TabControl)
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_TabControlSelectionChanged);
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_PathBehaviorComboxSelecteChanged);
        }
    }
}