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
    class TreeViewLineConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            TreeViewItem item = (TreeViewItem)value;
            ItemsControl ic = ItemsControl.ItemsControlFromItemContainer(item);
            return ic.ItemContainerGenerator.IndexFromContainer(item) == ic.Items.Count - 1;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return false;
        }
    }
    /// <summary>
    /// Interaction logic for FMTreeView.xaml
    /// </summary>
    public partial class FMTreeView : UserControl
    {
        public delegate void OnTreeViewItemElementAttachChanged(TreeViewItem e_SourceTreeViewItem, TreeViewItem e_TargetTreeViewItem);
        public event OnTreeViewItemElementAttachChanged f_AddChildElementContextStripItemClick;
        public event OnTreeViewItemElementAttachChanged f_DeleteElementContextStripItemClick;
        public event OnTreeViewItemElementAttachChanged f_TreeViewItemMoved;
        public event EventHandler                       f_CopyElementContextStripItemClick;
        public event EventHandler                       f_CutElementContextStripItemClick;
        public event EventHandler                       f_PasteElementContextStripItemClick;
        bool                                            m_bEditMode = false;
        System.Drawing.Point                            m_vLastMouseDown;
        System.Drawing.Point                            m_vCurrentMousePos;
        TreeViewItem                                    m_DraggedTreeViewItem;
        TreeViewItem                                    m_TargetTreeViewItem;
        TreeViewItem                                    m_m_CurrentEditTreeViewItem;
        TextBox                                         m_CurrentEditTextBox;
        object                                          m_CopyOrCutObjectTreeViewItem;
        double                                          m_dbTreeViewItemMinHeight = 15;
        public FMTreeView()
        {
            InitializeComponent();
        }
        //https://social.msdn.microsoft.com/Forums/vstudio/en-US/c1aaf427-af37-4789-9366-1b35e8d59204/drag-and-drop-in-the-same-treeview?forum=wpf
        public void MyMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            m_vLastMouseDown = System.Windows.Forms.Cursor.Position;
            //DependencyObject uie = m_Treeview.InputHitTest(e.GetPosition(m_Treeview)) as DependencyObject;
            //System.Diagnostics.Debug.Write(e.Source.GetType().ToString());
            //if (uie is EditableListbox)
            {
                //l_TextBlock.Foreground = Brushes.Blue;
                //m_CurrentSelectedObject = l_TextBlock;
                //TreeViewItem lvi = e.Source as TreeViewItem;
                //DragDrop.DoDragDrop(m_Treeview, lvi, DragDropEffects.Move);
                //e.Handled = true;
            }
        }

        public void TreeViewItemLostFocus(object sender, EventArgs e)
        {
            if (m_CurrentEditTextBox != null)
            {
                m_m_CurrentEditTreeViewItem.Header = m_CurrentEditTextBox.Text;
            }
            m_m_CurrentEditTreeViewItem = null;
            m_CurrentEditTextBox = null;
            this.m_bEditMode = false;
        }
        //if the mouse cursor is left of noe then insert or add as child
        //if 0 add as child,-1 for before ,1 for after
        private int IsAddToNode(TreeViewItem e_TargetItem, System.Drawing.Point e_MousePos)
        {
            System.Drawing.Point l_MousePosition = e_MousePos;
            Point l_vItemPos = e_TargetItem.PointToScreen(new Point(0d, 0d));
            //http://stackoverflow.com/questions/913053/how-do-you-determine-the-width-of-the-text-in-a-wpf-treeviewitem-at-run-time
            e_TargetItem.Measure(new Size(Double.PositiveInfinity, Double.PositiveInfinity));
            Size l_Size = e_TargetItem.DesiredSize;
            //int l_iIndex = this.m_Treeview.Items.IndexOf(l_Item);
            if (l_MousePosition.X <= l_vItemPos.X)
            {//insert before this
                return -1;
            }
            else
            if (l_MousePosition.X > l_vItemPos.X + l_Size.Width)
            {//insert after this
                return 1;
            }
            return 0;
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            if (sender == m_AddElementMenuItem && f_AddChildElementContextStripItemClick != null )
            { 
            
            }
            else
            if (sender == m_AddChildMenuItem && f_AddChildElementContextStripItemClick != null )
            {

            }
            else
            if (sender == m_DeleteMenuItem && f_DeleteElementContextStripItemClick != null )
            {

            }
            else
            if (sender == m_CopyMenuItem && f_CopyElementContextStripItemClick != null )
            {
                m_CopyOrCutObjectTreeViewItem = this.m_Treeview.SelectedItem;
            }
            else
            if (sender == m_CutMenuItem && f_CutElementContextStripItemClick != null )
            {
                m_CopyOrCutObjectTreeViewItem = this.m_Treeview.SelectedItem;
            }
            else
             if (sender == m_PasteMenuItem && f_PasteElementContextStripItemClick != null )
            {

            }
        }

        private void treeView_Keyup(object sender, KeyEventArgs e)
        {
            if (this.m_Treeview.SelectedItem != null)
            {
                TreeViewItem l_Item = m_Treeview.SelectedItem as TreeViewItem;
                if (m_bEditMode == false && e.Key == Key.F2)
                {
                    //FMTextBox l_FMTextBox = new FMTextBox(l_Item.Header.ToString());
                    //l_FMTextBox.m_bEnableQuickReName = false;
                    //CommonFunction.RemoveChild(l_FMTextBox, l_FMTextBox.m_TextBox);
                    //BindingOperations.ClearBinding(l_FMTextBox.m_TextBox, TextBox.TextProperty);
                    //TextBox l_TextBox = l_FMTextBox.m_TextBox;
                    TextBox l_TextBox = new TextBox();
                    l_TextBox.Text = l_Item.Header.ToString();
                    //l_FMTextBox.f_LostFocus += TreeViewItemLostFocus;
                    //l_TextBox.SelectAll();
                    l_TextBox.Focusable = true;
                    l_TextBox.Focus();
                    l_TextBox.CaretIndex = l_TextBox.Text.Length;
                    l_TextBox.LostFocus += TreeViewItemLostFocus;
                    l_Item.Header = l_TextBox;
                    m_m_CurrentEditTreeViewItem = l_Item;
                    m_CurrentEditTextBox = l_TextBox;
                    m_bEditMode = true;
                }
                else
                if ( e.Key == Key.Enter )
                {
                    TextBox l_TextBox = l_Item.Header as TextBox;
                    if (l_TextBox != null)
                    {
                        l_Item.Header = l_TextBox.Text;
                        l_Item.Focus();
                    }
                    m_bEditMode = false;
                }
            }
        }
        private void TreeView_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (m_bEditMode == false)
                return;
            TreeViewItem l_Item = m_Treeview.SelectedItem as TreeViewItem;
            if ( l_Item != null )
            {
                TextBox l_TextBox = l_Item.Header as TextBox;
                if (l_TextBox != null )
                {
                    Keyboard.Focus(l_TextBox);
                    if (e.Key == Key.Add || e.Key == Key.Subtract)
                    {
                        e.Handled = true;
                        //int careIndex = l_TextBox.CaretIndex;
                        //l_TextBox.Text += e.Key == Key.Add ? "+" : "-";
                        //l_TextBox.CaretIndex = careIndex + 1;
                    }
                    //else
                    //{
                    //    int careIndex = l_TextBox.CaretIndex;
                    //    if(e.Key == Key.Right )
                    //    {
                    //        //if (l_TextBox.SelectedText != null && l_TextBox.SelectedText.Length == l_TextBox.Text.Length)
                    //        //{
                    //        //    l_TextBox.Select(l_TextBox.Text.Length, 0);
                    //        //}
                    //        //else
                    //        //if (careIndex < l_TextBox.Text.Length)
                    //        //{
                    //        //    e.Handled = true;
                    //        //    l_TextBox.Select(careIndex + 1, 0);
                    //        //}
                    //    }
                    //    else
                    //    if (e.Key == Key.Left)
                    //    {
                    //        e.Handled = true;
                    //        if (careIndex > 0)
                    //        {
                    //            l_TextBox.Select(careIndex - 1, 0);
                    //        }                    
                    //    }
                    //}
                }
            }
        }
        private void treeView_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.m_bEditMode)
            {
                m_CurrentEditTextBox.Focus();
            }
            m_vCurrentMousePos = System.Windows.Forms.Cursor.Position;
            try
            {
                if (m_Test2.IsVisible)
                {
                    //Point l_vItemPos = m_Test2.PointToScreen(new Point(0d, 0d));
                    //m_Test2.Measure(new Size(Double.PositiveInfinity, Double.PositiveInfinity));
                    //Size l_Size = m_Test2.DesiredSize;
                    //l_Size.Width += l_vItemPos.X;
                    //l_Size.Height += l_vItemPos.Y;
                    //m_Test.Header = l_vItemPos.ToString() + ",RB:" + l_Size.ToString() + "," + m_vCurrentMousePos.ToString();
                }
            }
            catch (Exception exp)
            {
                exp.ToString();
            }
            try
            {
                if (  e.LeftButton == MouseButtonState.Pressed  )
                {
                    System.Drawing.Point l_vCurrentPosition = System.Windows.Forms.Cursor.Position;
                    if ((Math.Abs(l_vCurrentPosition.X - m_vLastMouseDown.X) > m_dbTreeViewItemMinHeight) || (Math.Abs(l_vCurrentPosition.Y - m_vLastMouseDown.Y) > m_dbTreeViewItemMinHeight))
                    {
                        m_DraggedTreeViewItem = (TreeViewItem)m_Treeview.SelectedItem;
                        if (m_DraggedTreeViewItem != null)
                        {
                            DragDropEffects finalDropEffect = DragDrop.DoDragDrop(m_Treeview, m_Treeview.SelectedValue,DragDropEffects.Move);
                            //Checking target is not null and item is dragging(moving)
                            if (finalDropEffect == DragDropEffects.Move)
                            {
                                if ((m_TargetTreeViewItem != null))
                                {
                                    // A Move drop was accepted
                                    if (m_DraggedTreeViewItem != m_TargetTreeViewItem)
                                    {
                                        int l_iIsAddToChild = IsAddToNode(m_TargetTreeViewItem, m_vCurrentMousePos);
                                        if (m_DraggedTreeViewItem.Parent != null)
                                        {
                                            System.Diagnostics.Debug.WriteLine("remove child");
                                            CommonFunction.RemoveChild(m_DraggedTreeViewItem.Parent, m_DraggedTreeViewItem);
                                        }
                                        TreeViewItem l_TargetParent = m_TargetTreeViewItem.Parent as TreeViewItem;
                                        string l_strrr = m_TargetTreeViewItem.GetType().ToString();
                                        if (l_TargetParent == null)
                                        {
                                            TreeView l_TreeView = m_TargetTreeViewItem.Parent as TreeView;
                                            if (l_TreeView != null)
                                            {
                                                int l_iIndex = l_TreeView.Items.IndexOf(m_TargetTreeViewItem);
                                                //if (l_TreeView.Items.Count > l_iIndex + 1)
                                                {
                                                    System.Diagnostics.Debug.WriteLine("add child-1");
                                                    TreeViewItem l_TreeViewItem = CopyItem(m_DraggedTreeViewItem);
                                                    //l_TreeView.Items.Insert(l_iIndex + 1, m_DraggedTreeViewItem);
                                                    l_TreeView.Items.Insert(l_iIndex + 1, l_TreeViewItem);
                                                }
                                                //else
                                                //{
                                                //    System.Diagnostics.Debug.WriteLine("add child0");
                                                //    m_TargetTreeViewItem.Items.Add(m_DraggedTreeViewItem);
                                                //}
                                            }
                                            else
                                            {
                                                System.Diagnostics.Debug.WriteLine("add child1");
                                                TreeViewItem l_TreeViewItem = CopyItem(m_DraggedTreeViewItem);
                                                m_TargetTreeViewItem.Items.Add(l_TreeViewItem);
                                                //m_TargetTreeViewItem.Items.Add(m_DraggedTreeViewItem);
                                            }
                                        }
                                        else
                                        {
                                            int l_iIndex = l_TargetParent.Items.IndexOf(m_TargetTreeViewItem);
                                            switch (l_iIsAddToChild)
                                            { 
                                                case -1:
                                                    System.Diagnostics.Debug.WriteLine("add child2");
                                                    l_TargetParent.Items.Insert(l_iIndex, m_DraggedTreeViewItem);
                                                break;
                                                case 0:
                                                    System.Diagnostics.Debug.WriteLine("add child3");
                                                    m_TargetTreeViewItem.Items.Add(m_DraggedTreeViewItem);
                                                break;
                                                case 1:
                                                    System.Diagnostics.Debug.WriteLine("add child4");
                                                    if (l_iIndex + 1 < l_TargetParent.Items.Count)
                                                        l_TargetParent.Items.Insert(l_iIndex + 1, m_DraggedTreeViewItem);
                                                    else
                                                        l_TargetParent.Items.Add(m_DraggedTreeViewItem);
                                                break;
                                            }
                                        }
                                        if (f_TreeViewItemMoved != null)
                                            f_TreeViewItemMoved(m_DraggedTreeViewItem, m_TargetTreeViewItem);
                                        m_DraggedTreeViewItem.IsSelected = true;
                                        m_DraggedTreeViewItem.BringIntoView();
                                        m_DraggedTreeViewItem.Focus();
                                        m_TargetTreeViewItem = null;
                                        m_DraggedTreeViewItem = null;
                                    }
                                }
                                else
                                {
                                    TreeViewItem l_DraggedItem = (TreeViewItem)m_DraggedTreeViewItem;
                                    if (l_DraggedItem.Parent != null)
                                    {
                                        CommonFunction.RemoveChild(l_DraggedItem.Parent, l_DraggedItem);
                                    }
                                    this.m_Treeview.Items.Add(l_DraggedItem);
                                    m_DraggedTreeViewItem.IsSelected = true;
                                    m_DraggedTreeViewItem.BringIntoView();
                                    m_DraggedTreeViewItem.Focus();
                                    if (f_TreeViewItemMoved != null)
                                        f_TreeViewItemMoved(l_DraggedItem,null);
                                    m_TargetTreeViewItem = null;
                                    m_DraggedTreeViewItem = null;                                
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception l_Exp)
            {
                String l_str = l_Exp.ToString();
            }
        }

        private void treeView_DragOver(object sender, DragEventArgs e)
        {
            try
            {
                Point currentPosition = e.GetPosition(m_Treeview);
                if (m_Test2.IsVisible)
                {
                    
                    Point l_vItemPos = m_Test2.PointToScreen(new Point(0d, 0d));
                    m_Test2.Measure(new Size(Double.PositiveInfinity, Double.PositiveInfinity));
                    Size l_Size = m_Test2.DesiredSize;
                    l_Size.Width += l_vItemPos.X;
                    l_Size.Height += l_vItemPos.Y;
                    m_Test.Header = l_vItemPos.ToString() + ",RB:" + l_Size.ToString() + "," + m_vCurrentMousePos.ToString();
                }
                if ((Math.Abs(currentPosition.X - m_vLastMouseDown.X) > 10.0) ||
                    (Math.Abs(currentPosition.Y - m_vLastMouseDown.Y) > 10.0))
                {
                    // Verify that this is a valid drop and then store the drop target
                    m_vCurrentMousePos = System.Windows.Forms.Cursor.Position;
                    TreeViewItem item = GetNearestContainer(e.OriginalSource as UIElement);
                    if (m_DraggedTreeViewItem != item)
                    {
                        e.Effects = DragDropEffects.Move;
                    }
                    else
                    {
                        e.Effects = DragDropEffects.None;
                        //e.Effects = DragDropEffects.Scroll;
                    }
                }
                e.Handled = true;
            }
            catch (Exception)
            {
            }
        }

        private void treeView_Drop(object sender, DragEventArgs e)
        {
            try
            {
                e.Effects = DragDropEffects.None;
                e.Handled = true;
                m_TargetTreeViewItem = null;
                if (m_DraggedTreeViewItem != null)
                {
                    // Verify that this is a valid drop and then store the drop target
                    TreeViewItem TargetItem = GetNearestContainer(e.OriginalSource as UIElement);
                    m_TargetTreeViewItem = TargetItem;
                    e.Effects = DragDropEffects.Move;
                }
            }
            catch (Exception)
            {
            }
        }

        private TreeViewItem GetNearestContainer(UIElement element)
        {
            System.Drawing.Point l_MousePosition = System.Windows.Forms.Cursor.Position;
            System.Collections.ArrayList l_ArrayList = new System.Collections.ArrayList();
            CommonFunction.GetTreeViewVisibleChrildren(l_ArrayList, this.m_Treeview.Items);
            foreach (TreeViewItem l_Item in l_ArrayList)
            {
                Point l_vItemPos = l_Item.PointToScreen(new Point(0d, 0d));
                String l_strgg = l_Item.Header.GetType().ToString();
                if (l_MousePosition.Y >= l_vItemPos.Y && l_MousePosition.Y <= l_vItemPos.Y + m_dbTreeViewItemMinHeight)
                {
                    return l_Item;
                }
            }
            return null;
        }

        public void addChild(TreeViewItem _sourceItem, TreeViewItem _targetItem)
        {
            // add item in target TreeViewItem 
            TreeViewItem item1 = new TreeViewItem();
            item1.Header = _sourceItem.Header;
            //_targetItem.Items.Add(item1);
            _targetItem.Items.Insert(0, item1);
            //for(int i=_sourceItem.Items.Count-1;i>-1;--i)
            //foreach (TreeViewItem item in _sourceItem.Items)
            for (int i = 0; i < _sourceItem.Items.Count; ++i)
            {
                TreeViewItem item = _sourceItem.Items[i] as TreeViewItem;
                addChild(item, item1);
            }
        }

        private TreeViewItem CopyItem(TreeViewItem _targetItem)
        {
            TreeViewItem item = (TreeViewItem)(_targetItem.ItemContainerGenerator.ContainerFromItem(_targetItem));
            return item;
        }

        private void CopyItem(TreeViewItem _sourceItem, TreeViewItem _targetItem)
        {

            //Asking user wether he want to drop the dragged TreeViewItem here or not
            //if (MessageBox.Show("Would you like to drop " + _sourceItem.Header.ToString() + " into " + _targetItem.Header.ToString() + "", "", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
            {
                try
                {
                    //adding dragged TreeViewItem in target TreeViewItem
                    addChild(_sourceItem, _targetItem);
                    _targetItem.IsExpanded = true;
                    //finding Parent TreeViewItem of dragged TreeViewItem 
                    TreeViewItem ParentItem = FindVisualParent<TreeViewItem>(_sourceItem);
                    // if parent is null then remove from TreeView else remove from Parent TreeViewItem
                    if (ParentItem == null)
                    {
                        m_Treeview.Items.Remove(_sourceItem);
                    }
                    else
                    {
                        ParentItem.Items.Remove(_sourceItem);
                    }
                }
                catch(SystemException e)
                {
                    String l_str = e.ToString();
                    String l_str2 = e.ToString();
                }
            }

        }

        static TObject FindVisualParent<TObject>(UIElement child) where TObject : UIElement
        {
            if (child == null)
            {
                return null;
            }

            UIElement parent = VisualTreeHelper.GetParent(child) as UIElement;

            while (parent != null)
            {
                TObject found = parent as TObject;
                if (found != null)
                {
                    return found;
                }
                else
                {
                    parent = VisualTreeHelper.GetParent(parent) as UIElement;
                }
            }

            return null;
        }
    }
}
