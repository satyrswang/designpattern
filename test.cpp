//
//  test.cpp
//  designpattern
//
//  Created by wyq on 01/01/2017.
//  Copyright © 2017 wyq. All rights reserved.
//

#include "test.hpp"
#include <iostream>
#include <algorithm>

#include "compound.hpp"
#include "command.hpp"
#include "rectangle.hpp"
#include "identifier.h"
class Circle;
class Rectangle;
class RoundRectangle;
namespace xcl = pandaxcl;//名字空间重命名
typedef xcl::identifier<Rectangle     ,long> RID;//矩形类标识号类型
typedef xcl::identifier<Circle        ,long> CID;//圆形类标识号类型
typedef xcl::identifier<RoundRectangle,long> RRID;//圆角矩形类标识号类型
//矩形类
class Rectangle
{
public:
            Rectangle():_x(0),_y(0),_width(20),_height(10){}
            Rectangle(int x,int y,int w,int h):_x(x),_y(y),_width(w),_height(h){}
private:
            int _x,_y,_width,_height;
            
            friend std::ostream&operator<<(std::ostream&s,Rectangle&o)
            {
                        s << "(" << o._x << "," << o._y << "," << o._width << "," << o._height << ")" ;
                        return s;
                }
        };
        //圆形类
        class Circle
        {
        public:
                    Circle():_x(0),_y(0),_radius(20){}
                    Circle(int x,int y,int r):_x(x),_y(y),_radius(r){}
        private:
                    int _x,_y,_radius;
                    
                    friend std::ostream&operator<<(std::ostream&s,Circle&o)
                    {
                                s << "(" << o._x << "," << o._y << "," << o._radius << ")" ;
                                return s;
                        }
        };
        //圆角矩形类
        class RoundRectangle
        {
        public:
                    typedef xcl::container<RID,Rectangle> RC;
                    typedef xcl::container<CID,Circle   > CC;
                    typedef xcl::container<RRID,RoundRectangle> RRC;
                   
                    template <class ControlType>
                    static void create(ControlType& C,const RRID& ID)
                    {
                                
                                RoundRectangle tmp;
                                C.record();
                                //先创建所有的子图元
                                C.execute(new xcl::create<RC>(C,tmp._RID   ,Rectangle()));
                                C.execute(new xcl::create<CC>(C,tmp._CID[0],Circle()));
                                C.execute(new xcl::create<CC>(C,tmp._CID[1],Circle()));
                                C.execute(new xcl::create<CC>(C,tmp._CID[2],Circle()));
                                C.execute(new xcl::create<CC>(C,tmp._CID[3],Circle()));
                                //然后创建自身
                                C.execute(new xcl::create<RRC>(C,ID,tmp));
                                C.stop();
                        }
                    //下面的函数是用来修改复合对象的矩形属性的修改函数
                    template <class ControlType>
                    static void modify(ControlType&C,const RRID&ID,const Rectangle&OM)
                    {
                                const RoundRectangle&rr = static_cast<RRC&>(C).reference(ID);
                                C.execute(new xcl::modify<RC>(C,rr._RID,OM));
                        }
                    //下面的函数是用来修改复合对象的矩形属性的修改函数
                    template <size_t i,class ControlType>//模板参数用来表示修改的同类型属性的索引值
                    static void modify(ControlType&C,const RRID&ID,const Circle&OM)
                    {
                                const RoundRectangle&rr = static_cast<RRC&>(C).reference(ID);
                                C.execute(new xcl::modify<CC>(C,rr._CID[i],OM));
                        }
                    //复合对象的删除函数
                    template <class ControlType>
                    static void remove(ControlType&C,const RRID&ID)
                    {
                                //因为删除复合对象是一步操作，所以需要将所有的子元素的删除合并
                                //成为一个复合命令，使之表现的比较象简单对象的删除操作一样。
                                const RoundRectangle&rr = static_cast<RRC&>(C).reference(ID);
                                C.record();
                                //先删除所有的子图元
                                C.execute(new xcl::remove<RC>(C,rr._RID   ));
                                C.execute(new xcl::remove<CC>(C,rr._CID[0]));
                                C.execute(new xcl::remove<CC>(C,rr._CID[1]));
                                C.execute(new xcl::remove<CC>(C,rr._CID[2]));
                                C.execute(new xcl::remove<CC>(C,rr._CID[3]));
                                //然后删除自身
                                C.execute(new xcl::remove<RRC>(C,ID));
                                C.stop();
                        }
        private:
                    RID _RID;//矩形数据
                    CID _CID[4];//四个圆角数据
                    //下面的函数仅仅是为了输出信息而准备的
                    friend std::ostream&operator<<(std::ostream&s,RoundRectangle&o)
                    {
                                s << "R:[" << std::hex << o._RID    << "] " ;
                                s << "C:[" << std::hex << o._CID[0] << "] " ;
                                s << "C:[" << std::hex << o._CID[1] << "] " ;
                                s << "C:[" << std::hex << o._CID[2] << "] " ;
                                s << "C:[" << std::hex << o._CID[3] << "] " ;
                                return s;
                        }
        };
        //为了能够判断操作的正确性需要输出容器的信息
        template <class Container>
        void display(const char*str,Container&c)
        {
                    typedef typename Container::value_type value_type;
                    struct X
                    {
                                static void print(value_type&v)
                                {
                                            std::cout.setf(std::ios::showbase);//显示16进制的0x前缀
                                            std::cout << "(" <<std::hex<< v.first << "," ;
                                            std::cout << std::dec << v.second << ") ";
                                    }
                        };
                    //输出提示信息，并输出容器中的元素数量
                    std::cout << str << "[" << std::dec << c.size() << "] ";
                    std::for_each(c.begin(),c.end(),X::print);//输出容器中的所有的标识号和对象信息
                    std::cout << std::endl;
        }
        
        //需要一个控制面板类
        class Control
                :public xcl::center
                ,public xcl::container<RID ,Rectangle     >
                ,public xcl::container<CID ,Circle        >
                ,public xcl::container<RRID,RoundRectangle>
        {
        public:
                    //下面的函数是输出该控制面板对象的相关信息的
                    void display(const char*str)
                    {
                                typedef xcl::container<RID ,Rectangle     > RC;
                                typedef xcl::container<CID ,Circle        > CC;
                                typedef xcl::container<RRID,RoundRectangle> RRC;
                                std::cout << "----------"<<str<<"----------" <<std::endl;
                                std::cout << "Undo:[" << std::dec << xcl::center::undo_type::size() << "] " ;
                                std::cout << "Redo:[" << std::dec << xcl::center::redo_type::size() << "] " ;
                                std::cout << std::endl ;
                                ::display("矩形容器    :",static_cast<RC&>(*this));
                                ::display("圆形容器    :",static_cast<CC&>(*this));
                                ::display("圆角矩形容器:",static_cast<RRC&>(*this));
                                std::cout << "=========="<<str<<"==========" <<std::endl;
                        }
        };
        int main()
        {
                    //首先应用程序中必须有一个全局的控制面板类对象
                    Control C;
                    RRID ID;//复合对象的标识号对象
                    //模拟创建复合对象
                    RoundRectangle::create(C,ID);
                    C.display("创建复合对象");
                    C.undo();//撤销一步
                    C.display("撤销一步");
                    C.redo();//重做一步
                    C.display("重做一步");
                    //模拟修改复合对象
                    RoundRectangle::modify(C,ID,Rectangle(1,1,200,100));
                    RoundRectangle::modify<0>(C,ID,Circle(1,1,5));
                    RoundRectangle::modify<1>(C,ID,Circle(201,1,5));
                    RoundRectangle::modify<2>(C,ID,Circle(201,101,5));
                    RoundRectangle::modify<3>(C,ID,Circle(1,101,5));
                    C.display("修改复合对象的属性");
                    //模拟删除复合对象
                    RoundRectangle::remove(C,ID);
                    C.display("删除复合对象");
                    C.undo();//撤销一步
                    C.display("撤销一步");
                    C.redo();//重做一步
                    C.display("重做一步");
                    return 0;
        }
