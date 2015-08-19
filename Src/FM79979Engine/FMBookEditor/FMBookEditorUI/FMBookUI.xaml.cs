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

namespace FMBookEditorUI
{
    /// <summary>
    /// Interaction logic for FMBookUI.xaml
    /// </summary>
    public partial class FMBookUI : UserControl
    {
        public event EventHandler f_FileOpen;
        public event EventHandler f_FileSave;

        public event EventHandler f_Editor;
        public event EventHandler f_Game;

        public event KeyEventHandler f_KeyDown;
        public event KeyEventHandler f_KeyUp;

        GridLength[] m_StartWidth;
        GridLength[] m_EventWidth;

        public FMBookUI()
        {
            InitializeComponent();
            AssignExpanderData();
            m_PageData_Expander.IsExpanded = true;
            m_FatmingUtilities_Expander.IsExpanded = true;
            //m_FolderExploreTree.m_FolderExplorerTree.m_strExtensionName = ".mpdi";
            //m_FolderExploreTree.m_FolderExplorerTree.SetWorkingProject("C:\\Users\\Fatming\\Desktop\\FM79979\\Media\\MR\\Tutorial");
        }
        void AssignExpanderData()
        {
            m_StartWidth = new GridLength[m_SecondGrid.ColumnDefinitions.Count];
            m_EventWidth = new GridLength[Main_Grid.RowDefinitions.Count];

            for (int i = 0; i < m_SecondGrid.ColumnDefinitions.Count; ++i)
                m_StartWidth[i] = m_SecondGrid.ColumnDefinitions[i].Width;

            //m_Status_Expander.Expanded += Expander_Expanded;
            //m_PageObject_Expander.Expanded += Expander_Expanded;
            //m_PageData_Expander.Expanded += Expander_Expanded;
            m_FatmingUtilities_Expander.Expanded += Expander_Expanded;
            //m_Status_Expander.Collapsed += Expander_Expanded;
            m_PageData_Expander.Collapsed += Expander_Expanded;
            m_FatmingUtilities_Expander.Collapsed += Expander_Expanded;

            //m_Event_Expander.Expanded += Event_Expanded;
            //m_Event_Expander.Collapsed += Event_Expanded;
        }

        private void Event_Expanded(object sender, RoutedEventArgs e)
        {
            int l_iIndex = Main_Grid.Children.IndexOf(sender as UIElement);
            if (l_iIndex != -1)
            {
                Expander l_Expander = sender as Expander;
                if (l_Expander.IsExpanded)
                {
                    l_Expander.Visibility = System.Windows.Visibility.Visible;
                    Main_Grid.RowDefinitions[l_iIndex].Height = m_EventWidth[l_iIndex];
                }
                else
                {
                    Main_Grid.RowDefinitions[l_iIndex].Height = GridLength.Auto;
                }
            }
        }

        private void Expander_Expanded(object sender, RoutedEventArgs e)
        {
            int l_iIndex = m_SecondGrid.Children.IndexOf(sender as UIElement);
            if (l_iIndex != -1)
            {
                Expander l_Expander = sender as Expander;
                if (l_Expander.IsExpanded)
                {
                    l_Expander.Visibility = System.Windows.Visibility.Visible;
                    m_SecondGrid.ColumnDefinitions[l_iIndex].Width = m_StartWidth[l_iIndex];
                }
                else
                {
                    m_SecondGrid.ColumnDefinitions[l_iIndex].Width = GridLength.Auto;
                    //m_SecondGrid.ColumnDefinitions[l_iIndex].MinWidth = 0;
                }
            }
        }

        private void Collapse(object sender, RoutedEventArgs e)
        {
            //m_Status_Expander.IsExpanded = false;
            m_PageData_Expander.IsExpanded = false;
            m_FatmingUtilities_Expander.IsExpanded = false;
            //m_Event_Expander.IsExpanded = false;
        }

        private void Expand(object sender, RoutedEventArgs e)
        {
            //m_Status_Expander.IsExpanded = true;
            m_PageData_Expander.IsExpanded = true;
            m_FatmingUtilities_Expander.IsExpanded = true;
            //m_Event_Expander.IsExpanded = true;
        }

        private void FileOpen(object sender, RoutedEventArgs e)
        {
            if (f_FileOpen != null)
            {
                f_FileOpen(sender, e);
            }
        }

        private void FileSave(object sender, RoutedEventArgs e)
        {
            if (f_FileSave != null)
            {
                f_FileSave(sender, e);
            }
        }

        private void Game(object sender, RoutedEventArgs e)
        {
            if (f_Game != null)
            {
                f_Game(sender, e);
            }
        }

        private void Editor(object sender, RoutedEventArgs e)
        {
            if (f_Editor != null)
            {
                f_Editor(sender, e);
            }
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
    }
}
