void	XML_DoNewRootNode(System::Windows::Forms::TreeNode^e_pNode,System::Windows::Forms::TreeView^e_pTreeview)
{
	e_pTreeview->Nodes->Clear();
	e_pTreeview->Nodes->Add((System::Windows::Forms::TreeNode^)e_pNode->Clone());	
}

void	XML_DoNewRootNodeBySelectedNodeFromTreeNode(System::Windows::Forms::TreeView^e_pTreeView)
{
	if( e_pTreeView->SelectedNode )
	{
		if( e_pTreeView->SelectedNode->Nodes->Count != 0 )
		{
			XML_DoNewRootNode(e_pTreeView->SelectedNode,e_pTreeView);
		}
		else
		{
			System::Windows::Forms::MessageBox::Show("there is no attributes could be edit","warning",System::Windows::Forms::MessageBoxButtons::OK);
		}
	}
}

void	XML_DoPasteButton(System::Windows::Forms::TreeNode^e_pTemplateNode,System::Windows::Forms::TreeView^e_pTreeview,bool* e_pbCut)
{
	if( e_pTemplateNode )
	{
		GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)e_pTemplateNode->Clone();
		if( *e_pbCut )
		{
			e_pTemplateNode->Remove();
		}
		if(e_pTreeview->SelectedNode)
		{
			e_pTreeview->SelectedNode->Nodes->Add((System::Windows::Forms::TreeNode^)e_pTemplateNode->Clone());
		}
		else
		{
			e_pTreeview->Nodes->Add((System::Windows::Forms::TreeNode^)e_pTemplateNode->Clone());
		}
		e_pTemplateNode = nullptr;
	}
	else
	{
		WARNING_MSG("please select a node to copy or cut");
	}
}

void	XML_DoCutButton(System::Windows::Forms::TreeView^e_pTreeView,System::Windows::Forms::TreeNode^e_pTemplateNode,bool *e_pbCut)
{
	if(e_pTreeView->SelectedNode)
	{
		*e_pbCut = true;
		e_pTemplateNode = e_pTreeView->SelectedNode;
		//m_pCurrentTempNode->Remove();
	}
}

void	XML_DoCopyButton(System::Windows::Forms::TreeView^e_pTreeView,System::Windows::Forms::TreeNode^e_pTemplateNode,bool *e_pbCut)
{
	if(e_pTreeView->SelectedNode)
	{
		*e_pbCut = false;
		e_pTemplateNode = (GCFORM::TreeNode^)e_pTreeView->SelectedNode;
	}
}

void	XML_DoAddCommentButton(System::Windows::Forms::TreeView^e_pTreeView)
{
	if(e_pTreeView->SelectedNode)
		e_pTreeView->SelectedNode->Nodes->Add(XML_COMMENT);	
}


void	XML_DoReplaceButton(System::Windows::Forms::TextBox^e_pAttribute_textBox,
	System::Windows::Forms::TreeView^e_pCurrentAttribute_treeView,
	System::Windows::Forms::TextBox^e_pValue_textBox)
{
	if( e_pAttribute_textBox->Text->Length == 0 )
	{
		System::Windows::Forms::MessageBox::Show("attribute data is empty","waring",System::Windows::Forms::MessageBoxButtons::OK);
		return;
	}
	System::Windows::Forms::TreeNode^l_pDestNode = e_pCurrentAttribute_treeView->SelectedNode;
	if( !l_pDestNode )
	{
		System::Windows::Forms::MessageBox::Show("please select node","waring",System::Windows::Forms::MessageBoxButtons::OK);
		return;
	}
	//attribute should have value
	if( l_pDestNode->Name->Length>0&&e_pValue_textBox->Text->Length == 0)
	{
		System::Windows::Forms::MessageBox::Show("Value data is empty","waring",System::Windows::Forms::MessageBoxButtons::OK);
		return;				 
	}
	//ensure there is no same attribute in parent nodes
	for each( System::Windows::Forms::TreeNode^l_pChildNode in l_pDestNode->Nodes )
	{
		if(l_pChildNode->Text->Equals(e_pAttribute_textBox->Text->Trim()))
		{
			//ensure it's attribute,if the new atribute name as same as element it does't matter
			if( l_pChildNode->Name->Length>0 )
			{
				System::Windows::Forms::MessageBox::Show("same attribute/element name","waring",System::Windows::Forms::MessageBoxButtons::OK);
					return;
			}
		}
	}
	l_pDestNode->Text = e_pAttribute_textBox->Text->Trim();
	if( l_pDestNode->Name->Length>0 )
		l_pDestNode->Name = e_pValue_textBox->Text->Trim();	
}


void	XML_DoAddElementButton(System::Windows::Forms::TextBox^e_pAttribute_textBox,
	System::Windows::Forms::TreeView^e_pCurrentAttribute_treeView)
{
	if( e_pAttribute_textBox->Text->Length == 0)
	{
		System::Windows::Forms::MessageBox::Show("empty data","waring",System::Windows::Forms::MessageBoxButtons::OK);
		return;
	}
	if( e_pCurrentAttribute_treeView->Nodes->Count == 0 )
	{
		System::Windows::Forms::TreeNode^l_pRootNode = gcnew System::Windows::Forms::TreeNode;
		l_pRootNode->Text = "Root";
		l_pRootNode->Name = "";
		e_pCurrentAttribute_treeView->Nodes->Add(l_pRootNode);
	}
	System::Windows::Forms::TreeNode^l_pDestNode = e_pCurrentAttribute_treeView->SelectedNode;
	if( !l_pDestNode )
		l_pDestNode = e_pCurrentAttribute_treeView->Nodes[0];
	if(l_pDestNode->Name->Length != 0)
	{
		System::Windows::Forms::MessageBox::Show("attribute can't add element data","waring",System::Windows::Forms::MessageBoxButtons::OK);
		return;
	}
	System::Windows::Forms::TreeNode^l_pNewNode = gcnew System::Windows::Forms::TreeNode;
	l_pNewNode->Text = e_pAttribute_textBox->Text->Trim();
	l_pNewNode->Name = "";
	l_pDestNode->Nodes->Add(l_pNewNode);
	l_pNewNode->ForeColor = System::Drawing::Color::PowderBlue;	
}

void	XML_DoDeleteButton(System::Windows::Forms::TreeView^e_pCurrentAttribute_treeView)
{
	System::Windows::Forms::TreeNode^l_pDestNode = e_pCurrentAttribute_treeView->SelectedNode;
	if( !l_pDestNode )
	{
	 return;
	}
	l_pDestNode->Remove();
}

void	XML_DoAddAttributeButton(System::Windows::Forms::TextBox^e_pAttribute_textBox,
	System::Windows::Forms::TextBox^e_pValue_textBox,
	System::Windows::Forms::TreeView^e_pCurrentAttribute_treeView)
{
	if(!IsTextLegalForXMLElement(e_pAttribute_textBox))
		return;
	 if( e_pCurrentAttribute_treeView->Nodes->Count == 0 )
	 {
		 System::Windows::Forms::TreeNode^l_pRootNode = gcnew System::Windows::Forms::TreeNode;
		 l_pRootNode->Text = "Root";
		 l_pRootNode->Name = "";
		 e_pCurrentAttribute_treeView->Nodes->Add(l_pRootNode);
	 }
	 //find out the node we needed,if there is no node is match assign the root node
	 System::Windows::Forms::TreeNode^l_pDestNode = e_pCurrentAttribute_treeView->SelectedNode;
	 System::Windows::Forms::TreeNode^l_pNewNode = gcnew System::Windows::Forms::TreeNode;
	 if( !l_pDestNode )
		 l_pDestNode = e_pCurrentAttribute_treeView->Nodes[0];
	 else
	 {//make sure this noede is not element
		 if( l_pDestNode->Name->Length>0 )
		 {
			l_pDestNode = e_pCurrentAttribute_treeView->Nodes[0];
		 }
	 }
	 //ensure there is no same attribute in parent nodes
	 for each( System::Windows::Forms::TreeNode^l_pChildNode in l_pDestNode->Nodes )
	 {
		 if(l_pChildNode->Text->Equals(e_pAttribute_textBox->Text->Trim()))
		 {
			 System::Windows::Forms::MessageBox::Show("same attribute name","waring",System::Windows::Forms::MessageBoxButtons::OK);
			return;
		 }
	 }
	 l_pDestNode->Nodes->Add(l_pNewNode);
	 l_pNewNode->Text = e_pAttribute_textBox->Text->Trim();
	 l_pNewNode->Name = e_pValue_textBox->Text->Trim();
	 l_pNewNode->ForeColor = System::Drawing::Color::Blue;	
}
//http://www.dotblogs.com.tw/yc421206/archive/2011/05/20/25595.aspx
//
//
//public class XmlSerialize
//{
//    public static void SerializeToXml(string FileName, object Object)
//    {
//        XmlSerializer xml = null;
//        Stream stream = null;
//        StreamWriter writer = null;
//        try
//        {
//            xml = new XmlSerializer(Object.GetType());
//            stream = new FileStream(FileName, FileMode.Create, FileAccess.Write, FileShare.Read);
//            writer = new StreamWriter(stream, Encoding.UTF8);
//            xml.Serialize(writer, Object);
//        }
//        catch (Exception ex)
//        {
//            throw ex;
//        }
//        finally
//        {
//            writer.Close();
//            stream.Close();
//        }
//    }
//    public static object DeserializeFromXml(string FileName, object Object)
//    {
//        if (!File.Exists(FileName))
//            return null;
//
//        XmlSerializer xml = null;
//        Stream stream = null;
//        StreamReader reader = null;
//        object obj = null;
//        try
//        {
//            xml = new XmlSerializer(Object.GetType());
//            stream = new FileStream(FileName, FileMode.Open, FileAccess.Read, FileShare.None);
//            reader = new StreamReader(stream, Encoding.UTF8);
//            obj = xml.Deserialize(reader);
//            return obj;
//        }
//        catch (Exception ex)
//        {
//            throw ex;
//        }
//        finally
//        {
//            reader.Close();
//            stream.Close();
//        }
//    }
//}
//�b�Τ�ݵ{���A�N���O�নxml��
//private void button1_Click(object sender, EventArgs e)
//{
//    Company company = new Company() { Name = "���}�ߥ��~", Address = "�a�y��" };
//    Family family = new Family() { Father = "����", Mother = "���Q" };
//    XmlSerialize.SerializeToXml("company.xml", company);
//    XmlSerialize.SerializeToXml("family.xml", family);
//}
// 
//
//
//private void button2_Click(object sender, EventArgs e)
//{
//    Company company = null;
//    Family family = null;
//    object oCompany = XmlSerialize.DeserializeFromXml("company.xml", new Company());
//    object oFamily = XmlSerialize.DeserializeFromXml("family.xml", new Family());
//
//    if (oCompany is Company)
//        company = oCompany as Company;
//
//    if (oFamily is Family)
//        family = oFamily as Family;
//
//    Console.WriteLine(string.Format("�W��:{0},��m:{1}", company.Name, company.Address));
//    Console.WriteLine(string.Format("����:{0},����:{1}", family.Father, family.Mother));
//}
//
//
//��ı�o�b�Τ�ݾ֦��Ӧh���޿�P�_�A���ӭY���ݨD�ܧ�A�o�̥i��|���v�T�A���F��u�}��-�ʳ���h�A�ڨM�w�N�o�ǧP�_����XmlSerialize.DeserializeFromXml��k�̰��A�ڱN�Q�Ϊx����k�����^�ǥ��T�����O
//�ҥH�ڱN��k��ˤ@�U
//public static T DeserializeFromXml<T>(string FileName)
//{
//    XmlSerializer xml = null;
//    Stream stream = null;
//    StreamReader reader = null;
//    try
//    {
//        xml = new XmlSerializer(typeof(T));
//        stream = new FileStream(FileName, FileMode.Open, FileAccess.Read, FileShare.None);
//        reader = new StreamReader(stream, Encoding.UTF8);
//        object obj = xml.Deserialize(reader);
//        if (obj == null)
//            return default(T);
//        else
//            return (T)obj;
//    }
//    catch (Exception ex)
//    {
//        throw ex;
//    }
//    finally
//    {
//        stream.Close();
//        reader.Close();
//    }
//}
// 
//�p���@�Ӧb�Τ�ݪ��{���X�h�令�A�Τ�ݪ��{���X�ݰ_�Ӭ۷�²��A�S���h�l���޿�P�_�A���\���N�޿�h��UI�h���}
//private void button3_Click(object sender, EventArgs e)
//{
//    Company company = XmlSerialize.DeserializeFromXml<Company>("company.xml");
//    Family family = XmlSerialize.DeserializeFromXml<Family>("family.xml");
//    Console.WriteLine(string.Format("�W��:{0},��m:{1}", company.Name, company.Address));
//    Console.WriteLine(string.Format("����:{0},����:{1}", family.Father, family.Mother));
//}