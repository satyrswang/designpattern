//
//  rectangle.cpp
//  
//
//  Created by wyq on 28/12/2016.
//
//

#include <stdio.h>
#include <iostream>
namespace xcl=pandaxcl;
//矩形类
class Rectangle
{
public:
            Rectangle():_x(0),_y(0),_width(20),_height(10){}
            Rectangle(int x,int y,int w,int h):_x(x),_y(y),_width(w),_height(h){}
private:
            int _x,_y,_width,_height;
            friend std::ostream &operator<<(std::ostream&s,Rectangle&o)
            {
                        s << "(" << o._x << "," << o._y << "," << o._width << "," << o._height << ")" ;
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
        
                    friend std::ostream &operator<<(std::ostream&s,Circle&o)
                    {
                                s << "(" << o._x << "," << o._y << "," << o._radius << ")" ;
                        }
        };
        
        template <class Container>
        void display(const char*str,Container&c)
        {
                    typedef typename Container::value_type value_type;
                    struct X
                    {
                                static void print(value_type&v)
                                {
                                            std::cout<<"("<<v.first<<","<<v.second<<") ";
                                    }
                        };
                    std::cout << str << "[" << c.size() << "] ";
                    std::for_each(c.begin(),c.end(),X::print);
                    std::cout << std::endl;
        }
        
        //需要一个控制面板类
        class Control
                :public xcl::center
                ,public xcl::container<int,Rectangle>
                ,public xcl::container<int,Circle   >
        {
        public:
                    template<class ObjectType,class IdentifierType>
                    void create( IdentifierType ID, ObjectType O)
                    {
                                typedef xcl::container<IdentifierType,ObjectType> Container;
                                xcl::center::execute(new xcl::create<Container>(*this,ID,O));
                        }
                    template<class ObjectType,class IdentifierType>
                    void modify( IdentifierType ID, ObjectType O)
                    {
                                typedef xcl::container<IdentifierType,ObjectType> Container;
                                xcl::center::execute(new xcl::modify<Container>(*this,ID,O));
                        }
                    template<class ObjectType,class IdentifierType>
                    void remove( IdentifierType ID )
                    {
                                typedef xcl::container<IdentifierType,ObjectType> Container;
                                xcl::center::execute(new xcl::remove<Container>(*this,ID));
                        }
        public:
                    //下面的函数是输出该控制面板对象的相关信息的
                    void display(const char*str)
                    {
                                typedef xcl::container<int,Rectangle> RC;
                                typedef xcl::container<int,Circle   > CC;
                                std::cout << "----------"<<str<<"----------" <<std::endl;
                                std::cout << "Undo:[" << xcl::center::undo_type::size() << "] " ;
                                std::cout << "Redo:[" << xcl::center::redo_type::size() << "] " ;
                                std::cout << std::endl ;
                                ::display("矩形容器:",static_cast<RC&>(*this));
                                ::display("圆形容器:",static_cast<CC&>(*this));
                                std::cout << "=========="<<str<<"==========" <<std::endl;
                        }
        };
        int main()
        {
                    //首先应用程序中必须有一个全局的控制面板类对象
                    Control C;
                    int idr1=1,idr2=2;//两个测试使用的矩形类对象标识号
                    int idc1=1,idc2=2;//两个测试使用的圆形类对象标识号
                    C.display("控制面板中的初始状态");
            
                    ////////////////////////////////////////////////////////////
                    //模拟创建矩形和圆形过程
                    C.create(idr1,Rectangle());
                    C.create(idr2,Rectangle());
                    C.create(idc1,Circle());
                    C.create(idc2,Circle());
                    C.display("创建了两个矩形对象和两个圆形对象");
                    ////////////////////////////////////////////////////////////
            
                    ////////////////////////////////////////////////////////////
                    //模拟修改操作
                    C.modify(idr1,Rectangle(1,1,5,10));
                    C.modify(idr2,Rectangle(15,10,50,30));
                    C.modify(idc1,Circle(15,10,50));
                    C.modify(idc2,Circle(5,10,70));
                    C.display("修改了两个矩形对象和两个圆形对象");
                    ////////////////////////////////////////////////////////////
            
                    ////////////////////////////////////////////////////////////
                    //开始模拟撤销操作
                    C.undo();//撤销1步
                    C.display("撤销1步之后");
                    C.undo(4);//再撤销4步
                    C.display("撤销4步之后");
                    C.redo();//重做一步
                    C.display("重做1步之后");
                    C.redo(4);//重做4步
                    C.display("重做4步之后");
                    ////////////////////////////////////////////////////////////
            
                    ////////////////////////////////////////////////////////////
                 
                    C.record();//记录开始
                    C.remove<Rectangle>(idr1);//删除标识号为idr1的矩形对象
                    C.remove<Rectangle>(idr2);//删除标识号为idr2的矩形对象
                    C.remove<Circle>(idc1);//删除标识号为idc1的圆形对象
                    C.remove<Circle>(idc2);//删除标识号为idc2的圆形对象
                    C.stop();//记录结束
                    C.display("执行复合命令");
                    C.undo();//撤销1步
                    C.display("撤销1步");
                    C.redo();//重做一步
                    C.display("重做1步");
                    C.undo();//再撤销1步
                    C.display("再撤销1步恢复原来的状态");
                    ////////////////////////////////////////////////////////////
            
                    ////////////////////////////////////////////////////////////
                    //现在模拟一下如何使用限制次数的撤销和重做方法
                    C.limit(5);
                    C.display("限制最大可撤销步骤为5次");
                    C.undo();//撤销1步
                    C.display("撤销1步之后");
                    C.undo(4);//再撤销4步
                    C.display("撤销4步之后");
                    C.redo();//重做一步
                    C.display("重做1步之后");
                    C.redo(4);//重做4步
                    C.display("重做4步之后");
                                    
                    C.modify(idr2,Rectangle(150,100,500,300));
                    C.display("重做队列中还有命令的时候再执行新的命令的情况");
                  
                    return 0;
        }
