#Changes

##General
At this commit I still need to work on "recycle", and perfect "undo" and "redo". Undo and redo 
work ok, but the undo rename and undo mkdir and redo rename and redo mkdir have to be implemented. 
"Rename" works only in table view mode for some reason. 

##History
In a previous commit I perfected the undo-redo for "copy" and "move". If you press cancel the copy 
or move action that was interrupted is not going into the undo and redo vectors. If some move or 
some copy fails it does not go into the undo and redo vectors. It only goes into the undo and redo 
vectors only if the action finishes with success. That way it is more difficult to lose data. 
  
In another previous commit I perfected the multithreading of this program. Now it can do 
simultaneously many "copy" and "move" and "delete" actions without problem and the "undo" and "redo" 
work ok with those actions. (I.e. start a "copy" action of a large directory then start a "cut" 
action of another large directory and then several more. Or choose many files and directories for 
each action. Choose large directories so that the first actions do not finish before you add more 
actions. That way you can see that it executes many actions simultaneously.) 
I also rewrote part of the code to make it more readable. 

##This commit (May 6,2018)
In this commit I rewrote the "delete" command. Now it works better. Have in mind that "delete" is 
permanent and cannot be undone so be careful because what you delete is deleted permanently and 
cannot be undone. 

In this commit I also rewrote the overwrite control for "copy" and "cut" (move) It is now implemented 
and tested. After many tests it seems that it works ok. "Undo" and "redo" work ok with overwrite 
control and it is similar with the Linux file manager. That means that it keeps the files that you chose 
not to overwrite at target when you do undo and the original files you did not copy or move (chose to 
keep the target files at the target -so the source files remained at source) when you do redo. 

I also rewrote some parts of code (filaction subroutine etc. to make it more readable)

##This commit (May 26 2018) compiles with the new Qt 5.11.0 and works fine

##In this commit (June 7, 2018) I added a subroutine to find which OS is used to compile the program 
and which windows version it is if the OS is windows.