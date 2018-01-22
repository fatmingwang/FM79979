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
    public delegate void OnAddNewItem(String e_strType, TreeViewItem e_ParentTreeViewItem);
    public delegate void OnTreeViewItemDragAndDrop(TreeViewItem e_DragObject,TreeViewItem e_DropObject);
    /// <summary>
    /// Interaction logic for TreeVewDragDrop.xaml
    /// </summary>
    public partial class TreeVewDragDrop : UserControl
    {
        public event OnAddNewItem               f_AddNewItem;
        //
        public event EventHandler               f_Paste;
        public event EventHandler               f_Delete;
        //
        public event OnTreeViewItemDragAndDrop  f_DropTreeViewItemToAnotherTreeViewItem;
        //
        public event EventHandler               f_ChangeSelectedItem;
        //
        Point _lastMouseDown;
        TreeViewItem draggedItem, _target;
        //
        public TreeViewItem m_CopyItem;
        public TreeViewItem m_CutItem;
        public TreeViewItem m_PasteItem;
        public TreeViewItem m_DeleteItem;
        public System.Collections.Generic.SortedDictionary<int, object> m_VisibleItems;
        private System.Timers.Timer m_ScrollChangeTimer;
        private bool m_bScrollChangedFinish;
        public TreeVewDragDrop()
        {
            InitializeComponent();
            m_ScrollChangeTimer = new System.Timers.Timer();
            m_ScrollChangeTimer.Interval = 500;
            m_ScrollChangeTimer.Elapsed += OnScrollChangedTimedEvent;
            m_ScrollChangeTimer.AutoReset = false;
            m_ScrollChangeTimer.Enabled = false;
            m_bScrollChangedFinish = true;
            m_VisibleItems = new System.Collections.Generic.SortedDictionary<int, object>();
        }
        private void TreeView_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                _lastMouseDown = e.GetPosition(m_TreeView);
            }

        }
        //http://stackoverflow.com/questions/188001/two-way-binding-of-xml-data-to-the-wpf-treeview
        void ParseFileToTreeView(String e_strFileName)
        {
            //XmlDataProvider XmlData = new XmlDataProvider();
            //XmlData.Source = new Uri(e_strFileName);
            //XmlData.XPath = "node";

            //XmlDataProvider dataProvider = this.FindResource("xmlDataProvider") as XmlDataProvider;
            //dataProvider.Document.Save(dataProvider.Source.LocalPath);        
        }
        private void treeView_MouseMove(object sender, MouseEventArgs e)
        {
            try
            {
                Point currentPosition2 = e.GetPosition(m_TreeView);
                System.Diagnostics.Debug.WriteLine(currentPosition2);
                if (e.LeftButton == MouseButtonState.Pressed)
                {
                    Point currentPosition = e.GetPosition(m_TreeView);
                    if ((Math.Abs(currentPosition.X - _lastMouseDown.X) > 10.0) ||
                        (Math.Abs(currentPosition.Y - _lastMouseDown.Y) > 10.0))
                    {
                        draggedItem = (TreeViewItem)m_TreeView.SelectedItem;
                        if (draggedItem != null)
                        {
                            DragDropEffects finalDropEffect = DragDrop.DoDragDrop(m_TreeView, m_TreeView.SelectedValue,DragDropEffects.Move);
                            //Checking target is not null and item is dragging(moving)
                            if ((finalDropEffect == DragDropEffects.Move) && (_target != null))
                            {
                                // A Move drop was accepted
                                if (_target != draggedItem)
                                //if (!draggedItem.Header.ToString().Equals(_target.Header.ToString()))
                                {
                                    CopyItem(draggedItem, _target);
                                    _target = null;
                                    draggedItem = null;
                                }

                            }
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
        }
        private void treeView_DragOver(object sender, DragEventArgs e)
        {
            try
            {
                Point currentPosition = e.GetPosition(m_TreeView);
                if ((Math.Abs(currentPosition.X - _lastMouseDown.X) > 10.0) ||
                    (Math.Abs(currentPosition.Y - _lastMouseDown.Y) > 10.0))
                {
                    // Verify that this is a valid drop and then store the drop target
                    TreeViewItem item = GetNearestContainer(e.OriginalSource as UIElement);
                    String l_Data = (String)e.Data.GetData("String");
                    if (l_Data != null)
                    {
                        e.Effects = DragDropEffects.Copy;
                    }
                    else
                    if (CheckDropTarget(draggedItem, item) )
                    {
                        e.Effects = DragDropEffects.Move;
                    }
                    else
                    {
                        e.Effects = DragDropEffects.None;
                    }
                }
                e.Handled = true;
            }
            catch (Exception)
            {
            }
        }


        void    AddNewItem(String e_strText,TreeViewItem e_Parent)
        {
            TreeViewItem l_TreeViewItem = new TreeViewItem();
            l_TreeViewItem.AllowDrop = true;
            EditableTextBlock l_EditableTextBlock = new EditableTextBlock();
            l_EditableTextBlock.Text = e_strText;
            l_TreeViewItem.Header = l_EditableTextBlock;
            e_Parent.Items.Add(l_TreeViewItem);
            if (f_AddNewItem != null)
            {
                f_AddNewItem(e_strText, e_Parent);
            }
            e_Parent.IsExpanded = true;
        }

        private void treeView_Drop(object sender, DragEventArgs e)
        {
            try
            {
                // Verify that this is a valid drop and then store the drop target
                TreeViewItem TargetItem = GetNearestContainer(e.OriginalSource as UIElement);
                String l_Data = (String)e.Data.GetData("String");
                if (l_Data != null)
                {
                    AddNewItem(l_Data ,TargetItem);
                }
                else
                {
                    e.Effects = DragDropEffects.None;
                    e.Handled = true;
                    if (TargetItem != null)
                    {
                        if (draggedItem != null)
                        {
                            _target = TargetItem;
                            e.Effects = DragDropEffects.Move;
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
        }

        String GetTreeViewItemHeaderText(TreeViewItem _sourceItem)
        {
            EditableTextBlock l_EditableTextBlock = _sourceItem.Header as EditableTextBlock;
            if (l_EditableTextBlock != null)
            {
                return l_EditableTextBlock.Text;
            }
            return _sourceItem.Header.ToString();
        }
        private bool CheckDropTarget(TreeViewItem _sourceItem, TreeViewItem _targetItem)
        {
            //Check whether the target item is meeting your condition
            bool _isEqual = false;
            if (!_sourceItem.Header.ToString().Equals(GetTreeViewItemHeaderText(_targetItem)))
            {
                _isEqual = true;
            }
            return _isEqual;
        }
        bool IsableToAdd(TreeViewItem _sourceItem, TreeViewItem _targetItem)
        {
            TreeViewItem l_Parent = _sourceItem.Parent as TreeViewItem;
            if (l_Parent == null || l_Parent == _targetItem)
                return false;
            l_Parent = _targetItem.Parent as TreeViewItem;
            while (l_Parent != null)
            {
                if (l_Parent == _sourceItem)
                    return false;
                l_Parent = l_Parent.Parent as TreeViewItem;
            }
            return true;
        }

        private void CopyItem(TreeViewItem _sourceItem, TreeViewItem _targetItem)
        {
            if(IsableToAdd(_sourceItem, _targetItem) == false)
                return;

            //Asking user wether he want to drop the dragged TreeViewItem here or not
            if (MessageBox.Show("Would you like to drop " + GetTreeViewItemHeaderText(_sourceItem) + " into " + GetTreeViewItemHeaderText(_targetItem) + "", "", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
            {
                try
                {
                    if (f_DropTreeViewItemToAnotherTreeViewItem != null)
                        f_DropTreeViewItemToAnotherTreeViewItem(_sourceItem, _targetItem);

                    //adding dragged TreeViewItem in target TreeViewItem
                    addChild(_sourceItem, _targetItem);

                    //finding Parent TreeViewItem of dragged TreeViewItem 
                    TreeViewItem ParentItem = FindVisualParent<TreeViewItem>(_sourceItem);
                    // if parent is null then remove from TreeView else remove from Parent TreeViewItem
                    if (ParentItem == null)
                    {
                        m_TreeView.Items.Remove(_sourceItem);
                    }
                    else
                    {
                        ParentItem.Items.Remove(_sourceItem);
                    }
                    
                }
                catch
                {

                }
            }
        }

        public void addChild(TreeViewItem _sourceItem, TreeViewItem _targetItem)
        {
            // add item in target TreeViewItem 
            TreeViewItem item1 = new TreeViewItem();
            EditableTextBlock l_EditableTextBlock = new EditableTextBlock();
            EditableTextBlock l_SourceHeader = _sourceItem.Header as EditableTextBlock;
            l_EditableTextBlock.Text = l_SourceHeader.Text;
            item1.Header = l_EditableTextBlock;
            _targetItem.Items.Add(item1);
            foreach (TreeViewItem item in _sourceItem.Items)
            {
                addChild(item, item1);
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
        private TreeViewItem GetNearestContainer(UIElement element)
        {
            // Walk up the element tree to the nearest tree view item.
            TreeViewItem container = element as TreeViewItem;
            while ((container == null) && (element != null))
            {
                element = VisualTreeHelper.GetParent(element) as UIElement;
                container = element as TreeViewItem;
            }
            return container;
        }

        private void tvParameters_KeyUp(object sender, KeyEventArgs e)
        {
            //if (e.Key == Key.Tab && (Keyboard.Modifiers & (ModifierKeys.Control | ModifierKeys.Shift)) == (ModifierKeys.Control | ModifierKeys.Shift))
            //{
            //    MessageBox.Show("CTRL + SHIFT + TAB trapped");
            //}

            //if (e.Key == Key.Tab && (Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control)
            //{
            //    MessageBox.Show("CTRL + TAB trapped");
            //}
            if (m_TreeView.SelectedItem == null)
                return;
            if ((Keyboard.Modifiers & ModifierKeys.Control) == ModifierKeys.Control)
            {
                if (e.Key == Key.Up)
                {
                    MenuItem_LevelUpClick(sender, null);
                }
                else
                if (e.Key == Key.Down)
                {
                    MenuItem_LevelDownClick_(sender, null);
                }
                else
                if (e.Key == Key.Left)
                {
                    MenuItem_LevelLeftUpClick(sender, null);
                }
            }
            else
            if( e.Key == Key.Delete )
            {
                if (MessageBox.Show("Would you like to delete " + GetTreeViewItemHeaderText(m_TreeView.SelectedItem as TreeViewItem) + " ? ", "", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                {
                    MenuItem_DeleteClick(sender, e);
                }
            }
        }        

        private void MenuItem_LevelUpClick(object sender, RoutedEventArgs e)
        {
            if (m_TreeView.SelectedItem != null)
            {
                TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
                ItemCollection l_Collections = null;
                int l_iSelectedIndex = -1;
                TreeViewItem l_ParentItem = l_SelectedItem.Parent as TreeViewItem; ;
                if (l_ParentItem != null)
                {
                    l_Collections = l_ParentItem.Items;
                }
                else
                {
                    l_Collections = m_TreeView.Items;
                }
                l_iSelectedIndex = l_Collections.IndexOf(l_SelectedItem);
                if (l_iSelectedIndex > 0)
                {
                    l_Collections.RemoveAt(l_iSelectedIndex);
                    l_Collections.Insert(l_iSelectedIndex - 1, l_SelectedItem);
                }
                l_SelectedItem.IsSelected = true;
            }
        }

        private void MenuItem_LevelDownClick_(object sender, RoutedEventArgs e)
        {
            if (m_TreeView.SelectedItem != null)
            {
                TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
                ItemCollection l_Collections = null;
                int l_iSelectedIndex = -1;
                TreeViewItem l_ParentItem = l_SelectedItem.Parent as TreeViewItem; ;
                if (l_ParentItem != null)
                {
                    l_Collections = l_ParentItem.Items;
                }
                else
                {
                    l_Collections = m_TreeView.Items;
                }
                l_iSelectedIndex = l_Collections.IndexOf(l_SelectedItem);
                if (l_Collections.Count > l_iSelectedIndex)
                {
                    l_Collections.RemoveAt(l_iSelectedIndex);
                    l_Collections.Insert(l_iSelectedIndex + 1, l_SelectedItem);
                }
                l_SelectedItem.IsSelected = true;
            }
        }

        private void MenuItem_LevelLeftUpClick(object sender, RoutedEventArgs e)
        {
            if (m_TreeView.SelectedItem != null)
            {
                TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
                ItemCollection l_Collections = null;
                int l_iSelectedIndex = -1;
                TreeViewItem l_ParentItem = l_SelectedItem.Parent as TreeViewItem; ;
                if (l_ParentItem != null)
                {
                    l_Collections = l_ParentItem.Items;
                }
                else
                {
                    l_Collections = m_TreeView.Items;
                }
                l_iSelectedIndex = l_Collections.IndexOf(l_SelectedItem);
                if (l_ParentItem != null)
                {
                    if (l_ParentItem.Parent != null)
                    {
                        l_Collections.RemoveAt(l_iSelectedIndex);
                        TreeViewItem l_ParentParent = l_ParentItem.Parent as TreeViewItem;
                        ItemCollection l_ParentParentCollections = null;
                        if (l_ParentParent == null)
                        {
                            l_ParentParentCollections = m_TreeView.Items;
                        }
                        else
                        {
                            l_ParentParentCollections = l_ParentParent.Items;
                        }
                        int l_iParentIndex = l_ParentParentCollections.IndexOf(l_ParentItem);
                        l_ParentParentCollections.Insert(l_iParentIndex + 1, l_SelectedItem);
                    }
                }
                l_SelectedItem.IsSelected = true;
            }
        }

        private void MenuItem_CutClick(object sender, RoutedEventArgs e)
        {
            TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
            if( l_SelectedItem != null )
            {
                m_CopyItem = null;
                m_CutItem = l_SelectedItem;
                m_PasteMenuItem.IsEnabled = true;
            }
            m_PasteItem = null;
            m_DeleteItem = null;
        }
        
        private void MenuItem_CopyClick(object sender, RoutedEventArgs e)
        {
            TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
            if (l_SelectedItem != null)
            {
                m_CopyItem = l_SelectedItem;
                m_CutItem = null;
                m_PasteMenuItem.IsEnabled = true;
            }
            m_PasteItem = null;
            m_DeleteItem = null;
        }

        bool IsSame(object sender,object compare)
        {
            if (sender != null && compare != null)
            {
                if (sender == compare)
                    return true;
            }
            return false;
        }


        private void MenuItem_PasteClick(object sender, RoutedEventArgs e)
        {
            TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
            if (l_SelectedItem != null)
            {
                m_PasteItem = l_SelectedItem;
                if (!IsSame(m_PasteItem, m_CopyItem) && !IsSame(m_PasteItem, m_CutItem) )
                {
                    CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_Paste);
                    if (m_CopyItem != null && IsableToAdd(m_CopyItem, m_PasteItem))
                    {
                        addChild(m_CopyItem, m_PasteItem);
                    }
                    else
                    if (m_CutItem != null && IsableToAdd(m_CutItem, m_PasteItem))
                    {
                        TreeViewItem l_Parent = m_CutItem.Parent as TreeViewItem;
                        if (l_Parent != null)
                        {
                            l_Parent.Items.Remove(m_CutItem);
                        }
                        else
                        {
                            this.m_TreeView.Items.Remove(m_CutItem);
                        }
                        m_PasteItem.Items.Add(m_CutItem);
                    }
                    else
                    {
                        MessageBox.Show("paste failed");
                    }
                }
                else
                {
                    MessageBox.Show("paste failed");
                }
                //call
                m_CopyItem = null;
                m_CutItem = null;
            }
            m_DeleteItem = null;
            m_PasteMenuItem.IsEnabled = false;
        }

        private void MenuItem_DeleteClick(object sender, RoutedEventArgs e)
        {
            TreeViewItem l_SelectedItem = m_TreeView.SelectedItem as TreeViewItem;
            if (l_SelectedItem != null)            
            {
                //79979;
                m_DeleteItem = l_SelectedItem;
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_Delete);
                TreeViewItem l_Parent = l_SelectedItem.Parent as TreeViewItem;
                if (l_Parent != null)
                    l_Parent.Items.Remove(l_SelectedItem);
                else
                {
                    m_TreeView.Items.Remove(l_SelectedItem);
                }
            }
            m_PasteMenuItem.IsEnabled = false;
            m_PasteItem = null;
            m_CopyItem = null;
            m_CutItem = null;
            m_DeleteItem = null;
        }


        private void IsVisibleCallRecursive(ItemCollection e_ItemCollection,System.Collections.Generic.SortedDictionary<int,object> e_VisibleArrayList)
        {
            // Print each node recursively.  
            var l_pItems = e_ItemCollection;
            foreach (TreeViewItem l_TreeViewItem in l_pItems)
            {
                if(l_TreeViewItem.IsVisible)
                {
                    //https://social.msdn.microsoft.com/Forums/vstudio/en-US/281a8cdd-69a9-4a4a-9fc3-c039119af8ed/absolute-screen-coordinates-of-wpf-user-control?forum=wpf
                    //Point locationFromWindow = Button1.TranslatePoint(new Point(0, 0), this);
                    //Point locationFromScreen = Button1.PointToScreen(locationFromWindow);
                    Point l_Pos = l_TreeViewItem.TranslatePoint(new Point(0, 0), this.m_TreeView);
                    if (l_Pos.Y >= 0 && l_Pos.Y <= this.m_TreeView.RenderSize.Height)
                    {
                        if (e_VisibleArrayList.Keys.Contains((int)l_Pos.Y))
                        {
                            e_VisibleArrayList.Clear();
                            object l_NullSender = null;
                            ScrollChangedEventArgs l_NullE = null;
                            TreeView_ScrollChanged(l_NullSender, l_NullE);
                            return;
                        }
                        e_VisibleArrayList.Add((int)l_Pos.Y, l_TreeViewItem);
                    }
                }
                IsVisibleCallRecursive(l_TreeViewItem.Items, e_VisibleArrayList);
            }
        }



        private void RefreshVisibleItems()
        {
            if (m_bScrollChangedFinish)
            {
                m_bScrollChangedFinish = false;
                m_VisibleItems.Clear();
                IsVisibleCallRecursive(m_TreeView.Items, m_VisibleItems);
                //do sort by position
                //foreach (var l_KeyValue in l_SortedDictionary)
                //{
                //    TreeViewItem l_Item = l_KeyValue.Value as TreeViewItem;
                //    EditableTextBlock l_EditableTextBlock = l_Item.Header as EditableTextBlock;
                //    System.Diagnostics.Debug.WriteLine(l_EditableTextBlock.Text);
                //}
                m_bScrollChangedFinish = true;
            }
        }

        private void OnScrollChangedTimedEvent(Object source, System.Timers.ElapsedEventArgs e)
        {
            Action methodDelegate = delegate ()
            {
                RefreshVisibleItems();
            };
            this.Dispatcher.BeginInvoke(methodDelegate);
            
        }

        private void TreeView_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            //m_bScrollChangedFinish
            if (!m_ScrollChangeTimer.Enabled)
            {
                if (!m_bScrollChangedFinish)
                    m_ScrollChangeTimer.Interval = 2000;
                else
                    m_ScrollChangeTimer.Interval = 500;
                m_ScrollChangeTimer.Enabled = true;
            }
        }

        private void TreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ChangeSelectedItem);
        }
    }
}
