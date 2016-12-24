//
//  main.cpp
//  designpattern
//
//  Created by wyq on 24/12/2016.
//  Copyright © 2016 wyq. All rights reserved.
//

//对象类

#include <iostream>
#include <map>



class Object{
public:
    Object():_member(0){}
    Object(int m):_member(m){}
private:
    int _member;
};

typedef std::map<int,Object> MAP;

int main(int argc, const char * argv[]) {
    
    MAP m;
    //用这个容器来表示对象的状态，根据标识符得到对象
    {
        //创建操作的un/re
        int id1=1,id2=2;
        Object obj(10);//对象创建参数
        m.insert(std::make_pair(id1,obj));
        m.insert(std::make_pair(id2,obj));
    
        m.erase(id1);//创建标识号为id1的对象的撤销操作
        m.erase(id2);
       
        m.insert(std::make_pair(id1,obj));//重做创建标识号为id1的对象
        m.insert(std::make_pair(id2,obj));
    }
    {
        //修改操作的un/re
        int id1=1,id2=2;
        Object obj(10);
        m.insert(std::make_pair(id1,obj));
        m.insert(std::make_pair(id2,obj));

        //保存的信息
        Object OBK1 = m[id1];//备份标识号为id1的对象的原始信息
        Object OBK2 = m[id2];
        
        Object OM1(20);//备份标识号为id1的对象的修改参数信息
        Object OM2(50);
        
        m[id1] = OM1;//修改标识号为id1的对象
        m[id2] = OM2;
     
        m[id1] = OBK1;//撤销修改标识号为id1的对象
        m[id2] = OBK2;
        
        m[id1] = OM1;//重做修改标识号为id1的对象
        m[id2] = OM2;
    }
    {
        //删除操作的un/re
        int id1=1,id2=2;
        Object obj(10);
        m.insert(std::make_pair(id1,obj));
        m.insert(std::make_pair(id2,obj));
        
        Object OBK1 = m[id1];
        Object OBK2 = m[id2];
       
        m.erase(id1);//delete
        m.erase(id2);
      
        m.insert(std::make_pair(id1,OBK1));//un
        m.insert(std::make_pair(id2,OBK2));
   
        m.erase(id1);//re
        m.erase(id2);
    } 
    {//复合操作


        
    }
    return 0;
}

