#ifndef UNDOREDO_H
#define UNDOREDO_H
#include<QString>
#include <QStringList>



class UndoRedo {
// This class is made for objects that keep information nessesary to undo and redo commands



private:
   QString UndoRedoCommand;
   QString UndoRedoSourceDir;
   QString UndoRedoTargetDir;
   QStringList UndoRedoList;


public:
   UndoRedo();


   QString get_UndoRedoCommand(){return UndoRedoCommand;}
   QString get_UndoRedoSourceDir(){return UndoRedoSourceDir;}
   QString get_UndoRedoTargetDir(){return UndoRedoTargetDir;}
   QStringList get_UndoRedoList(){return UndoRedoList;}

   void set_UndoRedoCommand(QString command1){UndoRedoCommand=command1;}
   void set_UndoRedoSourceDir(QString Dir){UndoRedoSourceDir=Dir;}
   void set_UndoRedoTargetDir(QString Dir){UndoRedoTargetDir=Dir;}
   void set_UndoRedoList(QStringList list)
   {
       UndoRedoList=list;
   }
   bool operator ==(UndoRedo& object1) const;

   bool operator !=(UndoRedo& object1) const
   {
       return !(*this==object1);
   }


};



#endif // UNDOREDO_H
