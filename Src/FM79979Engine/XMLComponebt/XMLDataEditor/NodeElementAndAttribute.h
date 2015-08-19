#ifndef NodeElementAndAttribute_H
#define NodeElementAndAttribute_H
public ref class cNodeElementAndAttribute
{
public:
			//whole name = whole element'name
			//whole element'name by parent's name-self name child name.....until End
			//ex:root_Geometry_Source_Vertex
			//<root>
			//		<Geometry Attribute1="">
			//			<Source attribute1="" attribute2="">
			//				<Vertex count="5">0 1 2 3 4 5 6</Vertex>
			//			</Source>
			//		</Geometry>
			//</root>
			System::Collections::ArrayList^m_pWholeElement;										//
			//each enque element has it own attribute here is a table to store
			//each element's attribute,while same element's attribute count is not same make sure count as need as we want.
			//attribute count be zero.
			System::Collections::Hashtable^m_pElementAndAttributes;								//Element name and all Attributes's list(String,string ArrayList)
			//text with element
			System::Collections::Hashtable^m_pElementAndText;									//element's name and text(string,string arraylist)
			//text with comment
			System::Collections::Hashtable^m_pElementAndComment;								//element's name and comment(string,string arraylist)
			//store element children list,by self's name
			System::Collections::Hashtable^m_pElementAndElementChidrenList;						//element's name and whole children's name(string,xmlNode arraylist),because hastable will compare string's data so we use xmlnode
			//this one store children's whole name
			System::Collections::Hashtable^m_pElementAndElementChidrenListWholeName;			//element's name and whole children's whole name(string,string)
			//ex:ElementName is Source,it's WholeName is root_Geometry_Source
			System::Collections::Hashtable^m_pElementNameAndElementWholeName;					//element's name and element's whole name(String,String)
			cNodeElementAndAttribute()
			{
				m_pWholeElement = gcnew System::Collections::ArrayList;
				m_pElementAndAttributes = gcnew System::Collections::Hashtable;
				m_pElementAndText = gcnew System::Collections::Hashtable;
				m_pElementAndComment = gcnew System::Collections::Hashtable;
				m_pElementAndElementChidrenList = gcnew System::Collections::Hashtable;
				m_pElementNameAndElementWholeName = gcnew System::Collections::Hashtable;
				m_pElementAndElementChidrenListWholeName = gcnew System::Collections::Hashtable;
			}
};
#endif