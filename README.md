________________________________________
# Disclaimer
## I am creating this program in order to learn coding with Qt. If you decide to use it, you use it at your own risk.
___________________________________________

# FILETOOL

Filetool is a filesystem explorer tool. It copies files and
directories, deletes, moves, renames, etc. 

This program is created with Qt. In order to compile it and
run it you need Qt. You can find Qt in https://www.qt.io/download

It is developed for practicing and learning Qt and not for real use in a filesystem.
 
I have run many tests and the program seems to be stable and reliable. 

Nevertheless, I would advice caution because it is a program that changes the filesystem,
and in this commit or in some future commit it might have some bug.
There are known bugs because this program is not finished. Even more I am writing this code
to learn writing code with Qt. So it is at best experimental. 
## Thus, you should be careful if you use it, and if you decide to use it you use it at your own risk.
It is best if you run it in a Virtual Machine with no important data or 
in a computer with no important data after a backup. 
That way you are not in any danger to loose any data,
and you can restore the system or your data if something unexpected occurs. 


It can be compiled with Qt in Windows or Linux. It works ok in both operating systems.

Since the program is far from complete, 
## I strongly encourage you to read also the changes.md file
so that you know it's current state, what is implemented and what is not, known bugs, etc. For example 
"delete" is working fine but this command is permanent like "rm -rf" in Unix, 
except that it asks for a confirmation at the beginning. If you delete something 
it is permanently lost and you cannot "undo" it. I plan to implement the "recycle" command that sends the
files to the recycle bin but it is not implemented yet. So, be careful with what you delete, 
and that is one of the reasons I adviced you do have a backup of your system before you run this program.
