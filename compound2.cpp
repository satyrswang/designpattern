//
//  compound2.cpp
//  
//
//  Created by wyq on 01/01/2017.
//
//

#include "compound2.hpp"
#include <iostream>
#include "command.h"
#include "center.h"
////////////////////////////////////////////////////////////////////////////////
//复合对象类的实现
#include "meta.h"//模板元编程基础与应用讨论的内容
#include "identifier.h"//复合对象类必须使用这个类
namespace pandaxcl{
            
            template <class T> struct compound_unit
            {
                       
                        identifier<T> ID;//属性对象的标识号
            };
         
            template<class Cons>class compound
                    :public scatter<Cons,pandaxcl::compound_unit>
            {
                public:
                        typedef Cons                                  cons_type;
                        typedef scatter<Cons,pandaxcl::compound_unit> scatter_type;
                        //复合对象的创建函数的自动化实现
                        template <class ControlType,class CID>
                        static void create(ControlType& C,const CID& ID)
                        {
                                    typedef container<CID,compound>    TCT;//复合对象容器类型
                                    compound tmp;//定义一个临时的复合类对象
                                    C.record();
                                   
                                    ENVIRONMENT<ControlType> e(C,tmp);
                                    LOOP<CREATE,0,length<cons_type>::value,1>::execute(e);
                                  
                                    C.execute(new pandaxcl::create<TCT>(C,ID,tmp));
                                    C.stop();
                            }
                       
                        template <size_t i,class PropertyType,class ControlType,class CID>
                        static void modify(ControlType&C,const CID&ID,const PropertyType&OM)
                        {
                                    typedef container<CID,compound>    TCT;//复合对象容器类型
                                    typedef identifier<PropertyType>    PID;//属性对象使用的标识符类型
                                    typedef container<PID,PropertyType> PCT;//属性对象使用的容器类型
                                    typedef compound_unit<PropertyType> UTT;//确保每行代码不至于过长
                                    const compound &S = static_cast<TCT&>(C).reference(ID);
                                    const UTT &P = static_cast<const UTT&>(field<i>(S));
                                    //由于modify命令和这里的midify函数同名，所以用名字空间
                                    //进行限制选择modify命令
                                    C.execute(new pandaxcl::modify<PCT>(C,P.ID,OM));
                            }
                        //复合对象的删除函数的自动化实现
                        template <class ControlType,class CID>
                        static void remove(ControlType&C,const CID&ID)
                        {
                                    //因为删除复合对象是一步操作，所以需要将所有的子元素的删除合并
                                    //成为一个复合命令，使之表现的比较象简单对象的删除操作一样。
                                    typedef container<CID,compound>    TCT;//复合对象容器类型
                                    const compound&S = static_cast<TCT&>(C).reference(ID);
                                    C.record();
                                    //自动化删除所有的子图元
                                    ENVIRONMENT<ControlType> e(C,S);
                                    LOOP<REMOVE,0,length<cons_type>::value,1>::execute(e);
                                    //然后删除自身，由于remove命令和这里的remove函数同名，所以
                                    //用名字空间进行限制选择remove命令
                                    C.execute(new pandaxcl::remove<TCT>(C,ID));
                                    C.stop();
                            }
                private:
                        //CREATE和REMOVE都需要传入的动态参数结构
                        template<class ControlType> struct ENVIRONMENT
                        {
                                    typedef ControlType control_type;
                                    ENVIRONMENT(ControlType&C,const compound&c)
                                            :_control(C),_compound(c){}
                                    ControlType    &_control;//控制面板
                                    const compound &_compound;//复合对象
                            };
                        //自动化的创建所有的属性对象
                        template<size_t i> struct CREATE
                        {
                                    template<class EnvironmentType>
                                    static void execute(EnvironmentType&e)
                                    {
                                                //你的代码在这里编写
                                                typedef typename type<cons_type,i>::result CT;
                                                typedef identifier<CT>    PID;//属性对象使用的标识符类型
                                                typedef container<PID,CT> PCT;//属性对象使用的容器类型
                                                typedef compound_unit<CT> UTT;//确保每行代码不至于过长
                                                const UTT &v = static_cast<const UTT&>(field<i>(e._compound));
                                                typename EnvironmentType::control_type &C=e._control;
                                                C.execute(new pandaxcl::create<PCT>(C,v.ID,CT()));
                                        }
                            };
                        //自动化的删除所有的属性对象
                        template<size_t i> struct REMOVE
                        {
                                    template<class EnvironmentType>
                                    static void execute(EnvironmentType&e)
                                    {
                                                //你的代码在这里编写
                                                typedef typename type<cons_type,i>::result CT;
                                                typedef identifier<CT>    PID;//属性对象使用的标识符类型
                                                typedef container<PID,CT> PCT;//属性对象使用的容器类型
                                                typedef compound_unit<CT> UTT;//确保每行代码不至于过长
                                                const UTT &v = static_cast<const UTT&>(field<i>(e._compound));
                                                typename EnvironmentType::control_type &C=e._control;
                                                C.execute(new pandaxcl::remove<PCT>(C,v.ID));
                                        }
                            };
                };
}


//测试
//namespace xcl = pandaxcl;//名字空间重命名
////矩形类
//class Rectangle
//{
//public:
//            Rectangle():_x(0),_y(0),_width(20),_height(10){}
//            Rectangle(int x,int y,int w,int h):_x(x),_y(y),_width(w),_height(h){}
//private:
//            int _x,_y,_width,_height;
//            //下面的函数仅仅是为了输出信息而准备的
//            friend std::ostream&operator<<(std::ostream&s,Rectangle&o)
//            {
//                        s << "(" << o._x << "," << o._y << "," << o._width << "," << o._height << ")" ;
//                        return s;
//                }
//        };
//        //圆形类
//        class Circle
//        {
//        public:
//                    Circle():_x(0),_y(0),_radius(20){}
//                    Circle(int x,int y,int r):_x(x),_y(y),_radius(r){}
//        private:
//                    int _x,_y,_radius;
//                    //下面的函数仅仅是为了输出信息而准备的
//                    friend std::ostream&operator<<(std::ostream&s,Circle&o)
//                    {
//                                s << "(" << o._x << "," << o._y << "," << o._radius << ")" ;
//                                return s;
//                        }
//        };
//        //圆角矩形类
//        typedef xcl::cons<Rectangle,
//                xcl::cons<Circle,//重复类型
//                xcl::cons<Circle,//重复类型
//                xcl::cons<Circle,//重复类型
//                xcl::cons<Circle,//重复类型
//                xcl::null_type> > > > >ROUNDRECTANGLE;
//        //由上面的类型串自动生成圆角矩形类
//        typedef xcl::compound<ROUNDRECTANGLE> RoundRectangle;
//        //需要说明的是下面所有的标识号的单元类型都采用默认值，在这里不可以采用其它的元素
//        //这一点将会在后续的文章中讨论。
//        typedef xcl::identifier<Rectangle     > RID;//矩形类标识号类型
//        typedef xcl::identifier<Circle        > CID;//圆形类标识号类型
//        typedef xcl::identifier<RoundRectangle> RRID;//圆角矩形类标识号类型
//       
//        std::ostream&operator<<(std::ostream&s,RoundRectangle&o)
//        {
//                    s << "R:[" << std::hex << xcl::field<0>(o).ID << "] " ;
//                    s << "C:[" << std::hex << xcl::field<1>(o).ID << "] " ;
//                    s << "C:[" << std::hex << xcl::field<2>(o).ID << "] " ;
//                    s << "C:[" << std::hex << xcl::field<3>(o).ID << "] " ;
//                    s << "C:[" << std::hex << xcl::field<4>(o).ID << "] " ;
//                    return s;
//        }
//        //观察容器的内容而准备的
//        template <class Container>
//        void display(const char*str,Container&c)
//        {
//                    typedef typename Container::value_type value_type;
//                    struct X
//                    {
//                                static void print(value_type&v)
//                                {
//                                            std::cout.setf(std::ios::showbase);//显示16进制的0x前缀
//                                            std::cout << "(" <<std::hex<< v.first << "," ;
//                                            std::cout << std::dec << v.second << ") ";
//                                    }
//                        };
//                    //输出提示信息，并输出容器中的元素数量
//                    std::cout << str << "[" << std::dec << c.size() << "] ";
//                    std::for_each(c.begin(),c.end(),X::print);//输出容器中的所有的标识号和对象信息
//                    std::cout << std::endl;
//        }
//        
//     
//        class Control
//                :public xcl::center
//                ,public xcl::container<RID ,Rectangle     >
//                ,public xcl::container<CID ,Circle        >
//                ,public xcl::container<RRID,RoundRectangle>
//        {
//        public:
//                    //下面的函数是输出该控制面板对象的相关信息的
//                    void display(const char*str)
//                    {
//                                typedef xcl::container<RID ,Rectangle     > RC;
//                                typedef xcl::container<CID ,Circle        > CC;
//                                typedef xcl::container<RRID,RoundRectangle> RRC;
//                                std::cout << "----------"<<str<<"----------" <<std::endl;
//                                std::cout << "Undo:[" << std::dec << xcl::center::undo_type::size() << "] " ;
//                                std::cout << "Redo:[" << std::dec << xcl::center::redo_type::size() << "] " ;
//                                std::cout << std::endl ;
//                                ::display("矩形容器    :",static_cast<RC&>(*this));
//                                ::display("圆形容器    :",static_cast<CC&>(*this));
//                                ::display("圆角矩形容器:",static_cast<RRC&>(*this));
//                                std::cout << "=========="<<str<<"==========" <<std::endl;
//                        }
//        };
//        int main()
//        {
//                    //首先应用程序中必须有一个全局的控制面板类对象
//                    Control C;
//                    RRID ID;//复合对象的标识号对象
//                    //模拟创建复合对象
//                    RoundRectangle::create(C,ID);
//                    C.display("创建复合对象");
//                    C.undo();//撤销一步
//                    C.display("撤销一步");
//                    C.redo();//重做一步
//                    C.display("重做一步");
//                    //模拟修改复合对象
//                    C.record();
//                    RoundRectangle::modify<0>(C,ID,Rectangle(1,1,200,100));
//                    RoundRectangle::modify<1>(C,ID,Circle(1  ,1  ,5));
//                    RoundRectangle::modify<2>(C,ID,Circle(201,1  ,5));
//                    RoundRectangle::modify<3>(C,ID,Circle(201,101,5));
//                    RoundRectangle::modify<4>(C,ID,Circle(1  ,101,5));
//                    C.stop();
//                    C.display("修改复合对象的属性");
//                    C.undo();//撤销一步
//                    C.display("撤销一步");
//                    C.redo();//重做一步
//                    C.display("重做一步");
//                    //模拟删除复合对象
//                    RoundRectangle::remove(C,ID);
//                    C.display("删除复合对象");
//                    C.undo();//撤销一步
//                    C.display("撤销一步");
//                    C.redo();//重做一步
//                    C.display("重做一步");
//                    return 0;
//        }
