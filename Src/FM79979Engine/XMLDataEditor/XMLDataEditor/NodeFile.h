#pragma once
#include "../../DotNetCommon/DotNetCommonTools.h"
using namespace System::Xml;
ref class NodeFile
{	
public:
	System::Collections::Hashtable^m_pHashTable;
	NodeFile(){GCNEW(System::Collections::Hashtable,m_pHashTable);}

	System::Void	LoadFile(System::String^e_pFileName,System::String^e_pStringForPath);
	System::Void	WriteFile(System::String^e_pStringForPath);
};
//belowe demostrate how to dectec scroll event,
//
//using System;
//using System.Windows.Forms;
//
//public class MyTreeView : TreeView {
//  // Event declaration
//  public delegate void ScrollEventHandler(object sender, ScrollEventArgs e);
//  public event ScrollEventHandler Scroll;
//  private TreeNode mLastTop;
//  // WM_VSCROLL message constants
//  private const int WM_VSCROLL = 0x0115;
//  private const int SB_THUMBTRACK = 5;
//  private const int SB_ENDSCROLL = 8;
//
//  protected override void WndProc(ref Message m) {
//    // Trap the WM_VSCROLL message to generate the Scroll event
//    base.WndProc(ref m);
//    if (Scroll != null && m.Msg == WM_VSCROLL && this.TopNode != mLastTop) {
//      int nfy = m.WParam.ToInt32() & 0xFFFF;
//      if (nfy == SB_THUMBTRACK || nfy == SB_ENDSCROLL) {
//        Scroll.Invoke(this, new ScrollEventArgs(this.TopNode, nfy == SB_THUMBTRACK));
//        mLastTop = this.TopNode;
//      }
//    }
//  }
//  public class ScrollEventArgs {
//    // Scroll event argument
//    private TreeNode mTop;
//    private bool mTracking;
//    public ScrollEventArgs(TreeNode top, bool tracking) {
//      mTop = top;
//      mTracking = tracking;
//    }
//    public TreeNode Top {
//      get { return mTop; }
//    }
//    public bool Tracking {
//      get { return mTracking; }
//    }
//  }
//} 