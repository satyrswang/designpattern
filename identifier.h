//
//  identifier.h
//  designpattern
//
//  Created by wyq on 31/12/2016.
//  Copyright © 2016 wyq. All rights reserved.
//

#ifndef identifier_h
#define identifier_h


#endif /* identifier_h */

#include <limits>
#include <iostream>
#include <string>

//标识符

namespace pandaxcl{
    
    template<class ObjectType,class T=long>
    class identifier
    {
    public:
                typedef identifier this_type;
                typedef std::basic_string<T> represent_type;
                typedef T value_type;
                typedef typename represent_type::size_type size_type;
                identifier(bool New=true)
                {
                            if(New)_next();
                        }
                identifier(identifier const &id)
                {//拷贝构造函数中将仅仅对表示的数据进行拷贝
                            if(&id!=this) _rep=id._rep;
                    }
                virtual~identifier()
                {
                    }
                //下面的函数使得标识号从指定的初始数据开始重新产生
                void restart(const T init=0)
                {
                            _get_rep()=represent_type(1,T(init));
                    }
                //为了方便调试，在这里给出一个输出用的函数
                friend std::ostream&operator<<(std::ostream&s,identifier const&o)
                {
                            for(size_t i=0;i<o._rep.length();++i) s << o._rep[i] ;
                                return s;
                        }
                private:
                        void _next()
                        {
                                    represent_type &last_rep = _get_rep();
                                    const T& Max=std::numeric_limits<T>::max();
                                    bool should_push = true;
                                    for(size_t i=0;i<last_rep.length();++i) {
                                                //只要有一个没有达到最大值就不追加新元素
                                                if(last_rep[i] < Max){
                                                            should_push = false;
                                                            break;
                                                    }
                                        }
                                    //根据是否需要添加新元素来决定是否追加新元素
                                    if(should_push) {
                                                last_rep.push_back(T(0));//增加进位
                                                //同时将所有的元素清零
                                                for(size_t i=0;i<last_rep.length();++i) last_rep[i] = 0;
                                        }
                                    //到目前为止，表示数中的元素一定至少有一个没有达到最大值了
                                    for(size_t i=0;i<last_rep.length();++i) {
                                                if(last_rep[i] < Max){
                                                            ++last_rep[i];
                                                            break;
                                                    }
                                        }
                                    _rep = last_rep;
                            }
                        represent_type& _get_rep()
                        {
                                    static represent_type last_rep(1,T(0));
                                    return last_rep;
                            }
                private:
                        represent_type _rep;
                        friend bool operator < (identifier const&lhs,identifier const&rhs)
                        {
                                    return lhs._rep.compare(rhs._rep)<0;
                            }
                        friend bool operator == (identifier const&lhs,identifier const&rhs)
                        {
                                    return lhs._rep.compare(rhs._rep)==0;
                            }
                        friend bool operator != (identifier const&lhs,identifier const&rhs)
                        {
                                    return lhs._rep.compare(rhs._rep)!=0;
                            }
                        friend bool operator > (identifier const&lhs,identifier const&rhs)
                        {
                                    return lhs._rep.compare(rhs._rep)>0;
                            }
                };
            }
            
            
            // 测试：
            
            //#include <iostream>
            //#include <iomanip>
            //                class Object{};//给一个特定的类型
            //                int main()
            //            {
            //                        for(long i=0;i<1000;++i){
            //                                    pandaxcl::identifier<Object,unsigned int> ID;
            //                                    std::cout << std::hex << ID << std::endl;
            //                            }
            //                        std::cout << std::hex << pandaxcl::identifier<Object>() << std::endl;
            //                        return 0;
            //            }
            
            
