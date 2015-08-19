/*
try {

                // Create a DOM Document and load the XML data into it.

                XmlDocument dom = new XmlDocument();

                dom.Load(textBox1.Text);

 

                TreeView treeView = new TreeView();

 

                // Initialize the TreeView control.

                treeView.Nodes.Add(new TreeNode(dom.DocumentElement.Name));

 

                // Populate the TreeView with the DOM nodes.

                AddAttributesAndChildren(dom.DocumentElement, treeView.Nodes[0]);

                treeView.ExpandAll();

            } catch (Exception ex) {

                MessageBox.Show(ex.Message);

            }

 

        private void AddAttributesAndChildren(XmlNode inXmlNode, TreeNode inTreeNode) {

            XmlAttributeCollection atts = inXmlNode.Attributes;

            if (atts.Count != 0) {

                TreeNode tNode = new TreeNode("(Attributes)");

                foreach (XmlAttribute att in atts) {

                    tNode.Nodes.Add(new TreeNode(att.Name, att.Value));

                }

                inTreeNode.Nodes.Add(tNode);

            }

 

            foreach (XmlNode xNode in inXmlNode.ChildNodes) {

                TreeNode tNode;

                switch (xNode.NodeType) {

                case XmlNodeType.Element :

                    tNode = new TreeNode(xNode.Name);

                    AddAttributesAndChildren(xNode, tNode);

                    break;

                case XmlNodeType.Text:

                    tNode = new TreeNode("(text)", xNode.Value);

                    break;

                case XmlNodeType.Comment:

                    tNode = new TreeNode("<!--comment-->", xNode.Value);

                    break;

                case XmlNodeType.ProcessingInstruction:

                    tNode = new TreeNode("<?pi?>", xNode.Value);

                    break;

                case XmlNodeType.CDATA:

                    tNode = new TreeNode("<[CDATA[]]>", xNode.Value);

                    break;

                default :

                    throw new Exception("Unexpected NodeType: " + xNode.NodeType.ToString());

                }

                inTreeNode.Nodes.Add(tNode);

            }

        }


*/




/*
	#include "ZipFile.h"
	#include "EndianSwitch.h"
	CZipFile g_ZipFile;
	DWORD size;
    if( !g_ZipFile.LoadZipFile( GetFullPathFileNAme(TEXT("EndianSwappingData.zip")) ))
	{
        int a=0;
	}
	WCHAR*      m_TextData2;
    if( !g_ZipFile.GetFile( L"readme.txt", (WCHAR**)&m_TextData2, &size ) )
	{
        int a=0;
	}
	DWORD	m_TextCount = size / sizeof(m_TextData2[0]);

    // Make sure the text is in big-endian form.
    if ( m_TextData2[0] != 0xFEFF )
    {
        // Make sure it's validly labeled unicode text
        assert( m_TextData2[0] == 0xFFFE );

        // If the text is not big-endian then we need to make it big-endian.

        // Swap all of the words in the text. Pass in the start point, end point,
        // swap description (shorts), and repeat count. This technique works
        // particularly well when you have an array of structures, since the
        // arbitrarily complex structure definition will be repeated.
        //EndianSwitchWorker( m_TextData, m_TextData + m_TextCount, "s", m_TextSize / 2 );

        // Alternately, call a low-level function that will perform this simple
        // task faster.
        EndianSwitchWords( (WORD*)m_TextData2, m_TextCount );
    }

    // Text files tend to not have a terminating zero, but the Font class insists on it.
    // So, we move the text down one character, covering up the Unicode marker, and making
    // space for a zero.
    memmove( m_TextData2, m_TextData2 + 1, (m_TextCount - 1) * sizeof( m_TextData2[0] ) );
    // Insert a zero, now that there's room.
    m_TextData2[ m_TextCount - 1 ] = 0;
*/