# designpattern
homework project
1354202

##undo/redo
STL的容器和算法简化代码

un/re之前备份应用程序当前的状态信息，在un/re的时候就用这个备份的状态信息修改应用程序的当前状态

仿函数保存反操作的方法，为了减少书写反操作的代码，采用和仿函数保存反操作类似的方法，尽量使代码简洁（un/re机制可以以库的形式提供）
##优缺点
1 、应用程序的状态序列化到文件，一般只un/re一步，代码简单，但占磁盘、消耗时间

2 、自定义分配器管理内存，内存分配算法复杂，代码不易理解

3 、仿函数保存反操作，仿函数保留每一步操作对应的反操作，占用内存不大但消耗时间，一些操作不存在反操作，代码组织混乱
##基本操作、符合操作
创建、删除、修改、复合（可以是基本+复合）

通过保存操作类的基类指针志向不同类型的对象实例，将基本操作和复合操作保存在复合操作里面

操作的组合过程不需要un/re

应用程序中保存un/re操作的指针队列，执行操作完毕将指针保存到un队列尾部，清空re操作指针队列；un操作执行此操作的反操作，完毕后，将指针从un队列尾部移到re队列尾部；re操作执行re队列中的操作的执行操作，完毕后，将指针从re队列尾部移到un队列尾部。

##操作对象化
容器-- 1 标识号区分同类型不同对象 2 能根据标识号得到对象

将所有对象保存到复合对象之中，同时对象的基类提供接口，以执行操作、反执行操作

复合对象实际是一个对象的队列，执行操作则是顺序调用，反执行是逆序调用

##复习java中的un/re

###★MyPanel UndoManager

MyUndoableEdit

UndoableEdit AbstractUndoableEdit CompoundEdit UndoManager

###★继承AbstractUndoableEdit 覆写undo() redo() die() 其中先call super.undo() 

UndoableEdit:create — undoable

undo() — redoable hasBeenDone=false canUndo()return false canRedo()return true

die() — dead alive=false

— alive hasBeenDone =true

addEdit() replaceEdit()  — operations of edits 

isSignification() — 

###★CompoundEdit 

多function —addEdit() end() lastEdit() isinProgress()

多个Undoable edits 合为一个

edits -UndoableEdit list

inProgress - true 正在建立一个CompundEdit

addEdit()- anEdit 加进 edits 

end() 合并完成，inProgress = false， CompundeEdit 的行为如同单一 UndoableEdit。 

###★UndoManager

UndoableEdit object 被undo/redo

int limit —undoable list 长度限制 =100

UndoableEditListener undoableEditHappened() -UndoableEditEvent 

undo() — editToBeUndone()  ， indexOfNextAdd ， undoTo()

###★UndoableEditSupport

addUndoableEditListener() 和 postEdit() 

注册监听器

将UndoableEdit包装成一个UndoableEditEvent并发给listener 

内含 Vector<UndoableEditListener> 

MyPanel里:MyPanel 需要建立监听器，也需要 发送 Edit Event，所以它内含一个 UndoableEditSupport 来帮它完成。 

###★example
1 产生manager，注册listener

UndoManager manager = newUndoManager(); 
myPanel.addUndoableEditListener(manager); 

2 manager.undo(),manager.redo()

toolbar.add(UndoManagerHelper.getUndoAction(manager)); 
toolbar.add(UndoManagerHelper.getRedoAction(manager)); 

Helper 将某按钮关联至某 UndoManager，使被按时引发后者之 undo() 或 redo() 

UndoableEditSupport—

3产生一个MyUndoableEdit包装成事件并发送，Happened()接收

mouseReleased后postEdit（已创建MyUndoableEdit（在undo/redo中用到set\get））

4收到后挂上MyPanel

5 按钮引发

首先找出 edit = editToBeUndone(); 然后 undoTo(edit); 也就是逐一 undoTo(edit); 也就是逐一呼 叫区间中的 每个 UndoableEdit 的 undo()
undo() 时，此例将目前多边形储存起来以便将来 redo，然后将先前 储存的多边形设给 panel 

redo() 时，此例将先前储存之多边形设给 panel，并清除用以 redo 的多边形 (因 为 redo 之后除非 undo 否则不能再被 redo. 而 undo 时自然会设当时多边形为 savedPolygon 作为 redo 之用) 


 








