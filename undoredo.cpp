#include "undoredo.h"

UndoRedo::UndoRedo()
{
    UndoRedoCommand="";
    UndoRedoSourceDir="";
    UndoRedoTargetDir="";
    UndoRedoList.push_back("");

}

bool UndoRedo::operator ==(UndoRedo& object1) const
{

    if (UndoRedoCommand!=object1.get_UndoRedoCommand()) return false;
      if  (UndoRedoSourceDir!=object1.get_UndoRedoSourceDir()) return false;
       if (UndoRedoTargetDir!=object1.get_UndoRedoTargetDir())return false;
       if ( UndoRedoList.size()!=object1.get_UndoRedoList().size()) return false;

        for (int i=0; i<UndoRedoList.size();i++)
        {
//               if(  QString::compare((UndoRedoList[i]), (object1.get_UndoRedoList().at(i)), Qt::CaseSensitive)!=0) return false;
            if ((UndoRedoList[i])!=(object1.get_UndoRedoList().at(i))) return false;
        }
        return true;

}
