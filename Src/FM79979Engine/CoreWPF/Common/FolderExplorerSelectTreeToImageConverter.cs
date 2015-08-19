using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media.Imaging;

namespace CoreWPF.Common
{
    #region FolderExplorerSelectTreeToImageConverter

    [ValueConversion(typeof(string), typeof(bool))]
    public class FolderExplorerSelectTreeToImageConverter : IValueConverter
    {//http://stackoverflow.com/questions/4327725/add-icon-to-wpf-treeviewitem-at-runtime
        BitmapImage m_FolderImage = new BitmapImage(new Uri("pack://application:,,,/CoreWPF;component/Images/folder.png"));
        BitmapImage m_HDDImage = new BitmapImage(new Uri("pack://application:,,,/CoreWPF;component/Images/diskdrive.png"));
        BitmapImage m_MPDIImage = new BitmapImage(new Uri("pack://application:,,,/CoreWPF;component/Images/MPDI.png"));
        public static FolderExplorerSelectTreeToImageConverter Instance = new FolderExplorerSelectTreeToImageConverter();

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value != null)
            {
                if ((value as string).Contains(@"/"))
                {
                    return m_HDDImage;
                }
                else
                if ((value as string).Contains(@".mpdi"))
                {
                    return m_MPDIImage;
                }
            }
            return m_FolderImage;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotSupportedException("Cannot convert back");
        }
    }

    #endregion // DoubleToIntegerConverter


}