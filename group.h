#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>
#include "SvgElement"

class group{
    private:
    vector<SvgElement*> ElementArray;
    group* parent;
    public:
    group();
    group(const group&);
    group& operator= (const group &);
    ~group();

    void setParent(group*);
    void setElementArray(vector<SvgElement*>);
    group* getParent();
    vector<SvgElement*> getSvgElementArray();
    
    void addElement(SvgElement*);
};

#endif