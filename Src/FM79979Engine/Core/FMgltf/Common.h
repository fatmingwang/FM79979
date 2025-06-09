#pragma once
template<class TYPE>
class cSharedObjectVector :public NamedTypedObject
{
    std::vector<std::shared_ptr<TYPE>> m_ObjectVector;
    public:
    cSharedObjectVector* Clone()
    {
        cSharedObjectVector<TYPE>* l_pNew = new cSharedObjectVector<TYPE>();
        l_pNew->m_ObjectVector = m_ObjectVector;
        return l_pNew;
    }
    cSharedObjectVector()
    {
    }
    virtual ~cSharedObjectVector()
    {
    }
    bool Add(std::shared_ptr<TYPE> e_pObject)
    {
        if (!Get(e_pObject->GetName()))
        {
            m_ObjectVector.push_back(e_pObject);
            return true;
        }
        return false;
    }
    size_t Size()
    {
        return m_ObjectVector.size();
    }
    std::shared_ptr<TYPE> Get(const std::wstring e_strName)
    {
        return Get(e_strName.c_str());
    }
    std::shared_ptr<TYPE> Get(const wchar_t* e_strName)
    {
        if (e_strName)
        {
            for (const auto& l_Obj : m_ObjectVector)
            {
                if (l_Obj && wcscmp(l_Obj->GetName(), e_strName) == 0)
                {
                    return l_Obj;
                }
            }
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<TYPE>>& GetVector()
    {
        return m_ObjectVector;
    }
    std::shared_ptr<TYPE> operator [](int e_iIndex)
    {
        return m_ObjectVector[e_iIndex];
    }
};