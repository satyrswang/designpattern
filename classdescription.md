##⭐️compound.hpp:

container：identifier_type  object_type container_type

create() — this->insert

modify() — iterator

remove()

reference()

命令基类 非模板为借口：command: execute()

create: identifier_type  object_type container_type

modify:  _OM _OBK

remove:

复合命令：macro: 

~macro():this->free()

record(): push_back

execute(): QT FT (mem_fun1_t  bind2nd)

free():for_each(,,delete)  this->clear()

##⭐️compound.cpp

center: _limit redo_type undo_type history_type

~center()

limit()
limit(size_t)
relimiit():  front() pop_front() 

undo()
redo()

record()
execute()
stop()

##⭐️rectangle.cpp
Rectangle:
Circle:display()
Control:


