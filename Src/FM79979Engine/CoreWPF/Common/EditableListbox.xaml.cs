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
    /// EditableListbox.xaml
    /// 
    /// listbox will add a textbox as its item
    /// </summary>
    public partial class EditableListbox : UserControl
    {
        bool    m_bEnableDrag = false;
        string  m_TextStartEditText;
        public bool m_bSameItemAddAlow = false;
        TextBox m_EditedTextBox = null;
        public event EventHandler f_DeleteSelectedItem;
        public event EventHandler f_DeleteSelectedItems;
        public event EventHandler f_AddItem;
        public event EventHandler f_Replace;
        public event EventHandler f_ChangeName;
        public event EventHandler f_ListBoxSelectionChanged;
        public EditableListbox()
        {
            InitializeComponent();
            m_ListBox.SelectionMode = SelectionMode.Extended;
            this.m_ListBox.KeyUp += new System.Windows.Input.KeyEventHandler(EditableListbox_KeyUp);
            this.m_ListBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(EditableListbox_MouseDoubleClick);
            this.m_ListBox.SelectionMode = System.Windows.Controls.SelectionMode.Extended;
        }
        public void EnableListboxDrag()
        {
            if (m_bEnableDrag == false)
            {
                Style l_Style = new Style(typeof(ListBoxItem));
                l_Style.Setters.Add(new EventSetter(ListBoxItem.PreviewMouseLeftButtonDownEvent, new MouseButtonEventHandler(MouseLeftButtonDown)));
                m_ListBox.ItemContainerStyle = l_Style;

                m_bEnableDrag = true;
            }
        }

        private void MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            String l_strSenderString = sender.ToString();
            int l_iIndex = l_strSenderString.IndexOf(':');
            if (l_iIndex == -1)
                return;
            l_iIndex += 2;
            int l_iLength = l_strSenderString.Length - l_iIndex;
            String l_strCompareString = l_strSenderString.Substring(l_iIndex, l_iLength);
            int l_iSelectedIndex = IndexOf(l_strCompareString);
            if (l_iSelectedIndex == -1)
            {
                return;
            }

            TextBox l_TextBox = m_ListBox.Items[l_iSelectedIndex] as TextBox;
            String l_strText = l_TextBox.Text;
            if (l_strText != null)
            {
                var l_DataObject = new DataObject(l_strText);
                l_DataObject.SetData("String", l_strText);
                DragDrop.DoDragDrop((System.Windows.DependencyObject)sender, l_DataObject, DragDropEffects.Copy);
            }
        }

        public int IndexOf(string e_strName)
        {
            int l_iIndex = 0;
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    return l_iIndex;
                ++l_iIndex;
            }
            return -1;
        }
        public bool IsContain(string e_strName)
        {
            foreach(TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    return true;
            }
            return false;
        }

        public int NumSame(string e_strName)
        {
            int l_iValue = 0;
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    ++l_iValue;
            }
            return l_iValue;
        }

        public bool AddListBoxItem(string e_strName)
        {
            if (!m_bSameItemAddAlow)
            {
                if (IsContain(e_strName))
                {
                    if (f_Replace != null)
                    {
                        if (MessageBox.Show("replace exists!?", "Warning", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                        {
                            int l_iIndex = IndexOf(e_strName);
                            f_Replace(m_ListBox.Items[l_iIndex], new RoutedEventArgs());
                        }
                    }
                    return false;
                }
            }
            TextBox l_TextBox = new TextBox();
            l_TextBox.Text = e_strName;
            l_TextBox.IsEnabled = false;
            //l_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
            //l_TextBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(EditableListbox_MouseDoubleClick);
            m_ListBox.Items.Add(l_TextBox);
            m_EditedTextBox = null;
            if (f_AddItem != null)
            { 
                System.EventArgs e = new EventArgs();
                f_AddItem(l_TextBox, e);
            }
            return true;
        }

        void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox l_TextBox = sender as TextBox;
            if (m_EditedTextBox != l_TextBox)
            {
                MessageBox.Show("error TextBox_LostFocus");
            }
            l_TextBox.LostFocus -= new RoutedEventHandler(TextBox_LostFocus);
            if (NumSame(l_TextBox.Text) != 1 && m_bSameItemAddAlow != true)
            {
                MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                l_TextBox.Text = m_TextStartEditText;
            }
            else
            {
                if (f_ChangeName != null)
                    f_ChangeName(sender, e);
            }
            l_TextBox.IsEnabled = false;
            m_EditedTextBox = null;
        }

        public void DeleteSelectedItems()
        {
            if (f_DeleteSelectedItems != null)
            {
                while (this.m_ListBox.SelectedItems.Count > 0)
                {
                    System.EventArgs e = new EventArgs();
                    object l_Object = this.m_ListBox.SelectedItems[0];
                    f_DeleteSelectedItems(l_Object, e);
                    this.m_ListBox.Items.RemoveAt(this.m_ListBox.Items.IndexOf(this.m_ListBox.SelectedItems[0]));
                }
            }
            else
            if (f_DeleteSelectedItem != null)
            {
                if (this.m_ListBox.SelectedItems.Count > 0)
                {
                    System.EventArgs e = new EventArgs();
                    f_DeleteSelectedItem(this.m_ListBox.SelectedItems[0], e);
                    this.m_ListBox.Items.RemoveAt(this.m_ListBox.Items.IndexOf(this.m_ListBox.SelectedItems[0]));
                }
            }
            else
            {
                this.m_ListBox.Items.RemoveAt(this.m_ListBox.Items.IndexOf(this.m_ListBox.SelectedItems[0]));
            }
        }

        public bool IsInEditMode()
        {
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.IsEnabled == true)
                    return true;
            }
            return false;
        }

        public string SelectedName()
        {
            if (m_ListBox.SelectedItem != null)
            {
                TextBox l_SelectedTextBox = m_ListBox.SelectedItem as TextBox;
                return l_SelectedTextBox.Text;
            }
            return null;
        }

        void ChangeTextBoxName(TextBox sender)
        {
            if (sender.IsEnabled == false && m_EditedTextBox == null)
            {
                m_EditedTextBox = sender;
                sender.IsEnabled = true;
                sender.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
                m_TextStartEditText = sender.Text;
                sender.SelectAll();
                sender.Focus();
            }
        }

        void EditableListbox_KeyUp(System.Object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (this.m_ListBox.SelectedItem != null)
            {
                TextBox l_SelectedTextBox = m_ListBox.SelectedItem as TextBox;
                if (e.Key == Key.Enter)
                {
                    if ( NumSame(l_SelectedTextBox.Text) == 1 || m_bSameItemAddAlow == true )
                    {
                        l_SelectedTextBox.IsEnabled = false;
                        l_SelectedTextBox.LostFocus -= new RoutedEventHandler(TextBox_LostFocus);
                        m_EditedTextBox = null;
                        if (m_TextStartEditText.CompareTo(l_SelectedTextBox.Text) != 0)
                        {
                            if (f_ChangeName != null)
                                f_ChangeName(l_SelectedTextBox, e);
                        }
                        m_TextStartEditText = null;
                    }
                    else
                    {
                        MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                    }
                }
                else
                if (e.Key == Key.F2)
                {
                    ChangeTextBoxName(l_SelectedTextBox);
                }
                else
                if (e.Key == Key.Delete)
                {
                    DeleteSelectedItems();
                }
            }
        }

        void EditableListbox_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (this.m_ListBox.SelectedItem != null)
            {
                ChangeTextBoxName(this.m_ListBox.SelectedItem as TextBox);
            }
        }

        private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ListBoxSelectionChanged);
        }
    }
}
