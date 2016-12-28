//
//  compound.cpp
//  designpattern
//
//  Created by wyq on 25/12/2016.
//  Copyright © 2016 wyq. All rights reserved.
//

#include "compound.hpp"
#include <limits>//numeric_limit<size_t>::max()获得最大整数

namespace pandaxcl{
    class undo_queue:public macro{};
    class redo_queue:public macro{};
    class center:public undo_queue,public redo_queue
    {
    private:
                size_t _limit;
    public:
                typedef redo_queue redo_type;
                typedef undo_queue undo_type;
                typedef undo_type  history_type;
              
                center():_limit(std::numeric_limits<size_t>::max()){}
                
                virtual ~center()
                {
                            undo_type::free();
                            redo_type::free();
                    }
    public:
                size_t limit()
                {
                            return _limit;
                    }
                void limit(size_t Limit)
                {
                            _limit=Limit;
                            relimit();
                    }

                void relimit()
                {
                            typedef std::list<command*> QT;
                            if(_limit >= undo_type::size())return;
                            while(_limit < undo_type::size())
                                {
                                            command *pCmd = static_cast<command*>(undo_type::front());
                                            undo_type::pop_front();//将过期的可撤销命令从命令历史中清理掉
                                            delete pCmd;
                                    }
                    }
                //一次撤销一步操作
                void undo()
                {
                            if (!undo_type::empty()) {
                                        undo_type::back()->execute(false);
                                        redo_type::push_back(undo_type::back());
                                        undo_type::pop_back();
                                }
                    }
                //一次重做一步操作
                void redo()
                {
                            if (!redo_type::empty()) {
                                        redo_type::back()->execute(true);
                                        undo_type::push_back(redo_type::back());
                                        redo_type::pop_back();
                                }
                    }
                //一次撤销多步操作
                void undo(int number)
                {
                            for(int i=0;i<number;i++) {
                                        if(undo_type::empty())break;
                                        else undo();
                                    }
                    }
                //一次重做多步操作
                void redo(int number)
                {
                            for(int i=0;i<number;i++) {
                                        if(redo_type::empty())break;
                                        else redo();
                                    }
                    }
        
                //执行并记录新的命令函数
                void execute(command* pCmd)
                {
                            assert(pCmd!=NULL);
                            pCmd->execute(true);
                            record(pCmd);
                            relimit();
                    }
                //向历史记录中添加新的命令，并不会执行所添加的命令
                void record(command*pCmd=NULL)
                {
                            //向历史记录中添加新的操作命令
                            history_type::record(pCmd);
                            //每当记录新的可撤销命令的时候需要将可重做命令队列清空
                            redo_type::free();
                    }
                //这个函数和上面的record(NULL)函数一起组合实现复合命令的创建过程
                void stop()
                {
                            typedef command IT;
                            typedef macro   QT;
                            history_type &H=static_cast<history_type&>(*this);
                            history_type::iterator result=H.begin();
                            history_type::reverse_iterator rit;
                            rit = std::find(H.rbegin(),H.rend(),static_cast<IT*>(NULL));
                            std::advance(result,std::distance(rit,H.rend())-1);
                            if(result!=H.end())
                                {
                                            history_type::difference_type diff;
                                            diff = std::distance(result,H.end());
                                            if(diff==1 || diff==2) {
                                                        H.erase(result);
                                                }else{
                                                            QT* pQ=new QT();
                                                            pQ->splice(pQ->begin(),H,result,H.end());
                                                            pQ->remove(static_cast<IT*>(NULL));//list
                                                            record(pQ);
                                                    }
                                    }
                    }
    };
}


