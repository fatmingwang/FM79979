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
    /// Interaction logic for NodeCandidates.xaml
    /// </summary>
    public partial class NodeCandidates : UserControl
    {
        public NodeCandidates()
        {
            InitializeComponent();
        }
//http://stackoverflow.com/questions/1719013/obtaining-dodragdrop-dragsource
//In the call to DoDragDrop, add your object as an extra format:

// var dragSource = this;
// var data = "Hello";

// var dataObj = new DataObject(data);
// dataObj.SetData("DragSource", dragSource);
// DragDrop.DoDragDrop(dragSource, dataObj, DragDropEffects.Copy);
//Now in the OnDrag handler it is easy to get the drag source:

//protected override void OnDrop(DragEventArgs e)
//{
//  var data = e.Data.GetData(DataFormats.Text);
//  var dragSource = e.Data.GetData("DragSource");
//  ...
//}
//In some cases, knowing the source object itself is sufficient to get the data you require to complete the drag operation, in which case the above boils down to:

// DragDrop.DoDragDrop(dragSource, dragSource, DragDropEffects.Copy);

// ...

// var dragSource = e.Data.GetData(typeof(MyDragSource))
        private void Border_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Label l_Label = sender as Label;
            var l_DataObject = new DataObject(l_Label.Content);
            l_DataObject.SetData("String", l_Label.Content);
            DragDrop.DoDragDrop((System.Windows.DependencyObject)sender, l_DataObject, DragDropEffects.Copy);
        }
    }
}
