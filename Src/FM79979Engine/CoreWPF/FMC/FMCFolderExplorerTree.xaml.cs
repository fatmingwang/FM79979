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

namespace CoreWPF.FMC
{
    /// <summary>
    /// Interaction logic for MPDIFolderExplorerTree.xaml
    /// </summary>
    public partial class FMCFolderExplorerTree : UserControl
    {
        public event EventHandler f_NewFolder;
        public event EventHandler f_NewMPDI;
        public event EventHandler f_NewPI;
        public event EventHandler f_NewPath;
        public event EventHandler f_NewPhysical;
        public event EventHandler f_NewSound;
        public event EventHandler f_NewParticle;
        public event EventHandler f_Copy;
        public event EventHandler f_Paste;
        public event EventHandler f_Delete;
        public event EventHandler f_SetWorkingDirectory;

        public FMCFolderExplorerTree()
        {
            InitializeComponent();
            f_NewMPDI = null;
        }

        private void MenuItemNewFolder_Click(object sender, RoutedEventArgs e)
        {
            if (f_NewFolder != null)
            {
                f_NewFolder(sender, e);
            }
        }

        private void MenuItemNewMPDI_Click_(object sender, RoutedEventArgs e)
        {
            if (f_NewMPDI != null)
            {
                f_NewMPDI(sender, e);
            }
        }
        private void MenuItemNewPI_Click_(object sender, RoutedEventArgs e)
        {
            if (f_NewPI != null)
            {
                f_NewPI(sender, e);
            }
        }
        private void MenuItemNewPath_Click_(object sender, RoutedEventArgs e)
        {
            if (f_NewPath != null)
            {
                f_NewPath(sender, e);
            }
        }
        private void MenuItemNewPhyscial_Click_(object sender, RoutedEventArgs e)
        {
            if (f_NewPhysical != null)
            {
                f_NewPhysical(sender, e);
            }
        }
        private void MenuItemCopy_Click_(object sender, RoutedEventArgs e)
        {
            if (f_Copy != null)
            {
                f_Copy(sender, e);
            }
        }

        private void MenuItemSound_Click(object sender, RoutedEventArgs e)
        {
            if (f_NewSound != null)
            {
                f_NewSound(sender, e);
            }
        }

        private void MenuItemParticle_Click(object sender, RoutedEventArgs e)
        {
            if (f_NewParticle != null)
            {
                f_NewParticle(sender, e);
            }
        }

        private void MenuItemPaste_Click_(object sender, RoutedEventArgs e)
        {
            if (f_Paste != null)
            {
                f_Paste(sender, e);
            }
        }

        private void MenuItemDelete_Click_(object sender, RoutedEventArgs e)
        {
            if (f_Delete != null)
            {
                f_Delete(sender, e);
            }
        }

        private void MenuItemWorkingDirectory_Click(object sender, RoutedEventArgs e)
        {
            if (f_SetWorkingDirectory != null)
            {
                f_SetWorkingDirectory(sender, e);
            }
        }
        
    }
}
