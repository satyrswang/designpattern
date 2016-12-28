//
//  compound.hpp
//  designpattern
//
//  Created by wyq on 25/12/2016.
//  Copyright © 2016 wyq. All rights reserved.
//

//操作对象化 （错误处理）

#ifndef compound_hpp
#define compound_hpp

#include <stdio.h>

#endif /* compound_hpp */

#include <map>//容器

#include <list>//复合命令

#include <algorithm>//std::for_each

#include <functional>//std::mem_fun1,std::bind2nd

namespace pandaxcl{//remove和stdio.h中的remove函数重名 g++
    
    //命令操作的容器类实现
    
    template<class IdentifierType,class ObjectType>
    
    class container:public std::map<IdentifierType,ObjectType>
    
    {
        
    public:
        
        typedef IdentifierType identifier_type; typedef ObjectType object_type;
        
        typedef std::map<identifier_type,object_type> container_type;
    
    public:
        
        //创建命令类中调用
        
        virtual void create(const identifier_type &ID,const object_type &OC) {
            
            this->insert(std::make_pair(ID,OC));//执行创建操作
            
        }
        
        
        virtual void modify(const identifier_type &ID,const object_type &OM) {
            
            typename container_type::iterator it;
            
            it = this->find(ID);
            
            it->second = OM;
        }
        
            
            virtual void remove(const identifier_type&ID)
            
            {
                
                typename container_type::iterator it;
                
                it = find(ID);
                
                this->erase(it);
                
            }
            
            
            const object_type& reference(const identifier_type &ID)
            
            {
                
                typename container_type::iterator it;
                
                it = find(ID);
                
                return it->second;
                
            }
            
        };
        
        //命令基类
        class command
        
        {
            
        public:
            
            virtual ~command(){}
            
            virtual void execute(bool do_or_undo=true)=0;
            
        };
        
      
        
        template<class ContainerType> class create:public command
        
        {
            
        public:
            
            typedef ContainerType container_type;
            typedef typename ContainerType::key_type identifier_type;
            typedef typename ContainerType::mapped_type object_type;
            
        public:
            
            create(container_type &C,const identifier_type&ID,const object_type&OC) :_C(C),_ID(ID),_OC(OC) { }
            
            void execute(bool do_or_undo=true)
            
            {
                
                do_or_undo?_C.create(_ID,_OC):_C.remove(_ID);
                
            }
        
            
        private:
            create(){}
            
            container_type &_C ;
            
            identifier_type _ID ;
            
            object_type _OC;
        };
    
            //修改命令
    
            template<class ContainerType>class modify:public command
            
            {
                
            public:
                
                typedef ContainerType container_type;
                typedef typename ContainerType::key_type identifier_type;
                typedef typename ContainerType::mapped_type object_type;
                
            public:
                
                
                modify(container_type &C,const identifier_type &ID,const object_type &OM) :_C(C),_ID(ID),_OM(OM)
                
                {
                    
                    _OBK = _C.reference(_ID);
                    
                }
                
                void execute(bool do_or_undo=true)
                
                {
                    
                    do_or_undo?_C.modify(_ID,_OM):_C.modify(_ID,_OBK);
                    
                }
                
            private:
                
                modify(){}
                
                container_type &_C ;
                identifier_type _ID ;
                object_type _OM ;
                object_type _OBK;
                
            };
                
                //删除命令
                
                template<class ContainerType>class remove:public command
                
                {
                    
                public:
                    
                    typedef ContainerType                           container_type;
                    typedef typename ContainerType::key_type        identifier_type;
                    typedef typename ContainerType::mapped_type     object_type;
                    
                public:
                    
                    remove(container_type&C,const identifier_type&ID)
                    
                    :_C(C),_ID(ID)
                    
                    {  }
                        
                        _OBK = _C.reference(_ID);
                        
                        void execute(bool do_or_undo=true)
                        
                        {
                            
                            do_or_undo?_C.remove(_ID):_C.create(_ID,_OBK);
                            
                        }
                  
                        
                    private:
                        
                        remove(){}
                        
                        container_type &_C ;
                        
                        identifier_type _ID ;
                        
                        object_type _OBK;
                    };
                        
                        //复合命令
                        
                        class macro :public command ,public std::list<command*>
                        
                        {
                            
                        public:
                            
                            virtual~macro()
                            {
                                
                                this->free();
                                
                            }
                            
                            void record(command* pCmd)
                            
                            {
                                this->push_back(pCmd);
                                
                            }
                            
                        public:
                            
                            virtual void execute(bool do_or_undo=true)
                            
                            {
                                
                                typedef std::mem_fun1_t<void,command,bool> FT;
                                
                                typedef std::list<command*> QT;
                                
                                
                                do_or_undo?
                                std::for_each(this->begin() ,this->end() ,
                                              std::bind2nd(FT(&command::execute),true ))
                                
                                :std::for_each(this->rbegin(),this->rend(),
                                               std::bind2nd(FT(&command::execute),false));
                                
                            }
                            //template< class Res, class T, class Arg > std::mem_fun1_t<Res,T,Arg> mem_fun( Res (T::*f)(Arg) );
                            
                            void free()
                            
                            {
                                
                                struct X{static void kill(command* ptr){delete ptr;}};
                                typedef std::list<command*> QT;
                                std::for_each(this->begin(),this->end(),X::kill);
                                this->clear();
                            }
                        };
}

