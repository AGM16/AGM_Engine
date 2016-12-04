# WGame_Engine
Author: Adrià Gómez

A videogame engine created and developed by AGM16.

Link Source Code: https://github.com/AGM16/WGame_Engine/tree/master/WGame_Engine

To open correctly the source code programm from VS you have to go to Project->properties->Debugging->Working Directory-> and you have ti put this:"$(ProjectDir)Game/"

License: https://github.com/AGM16/WGame_Engine/blob/master/LICENSE

# Instructions
Camera Movement: W(UP), A(LEFT), S(DOWN) and D(RIGHT). 

If you press "Shift" the velocity of the movement will increase.

If you press the right button the camera will rotate.

You can only active all the AABBs o OBBs from the main camera or active each AABB or OBB from the panel editor of each gameobject.

The button active of the editor panel is to deactivate the components.For instance, if you activate the checkbox "Static" of the local transform component you won't be able to move the gameobject.

###Quadtree Instructions:
The Quadtree panel is in the game object camera_test as a component.In this panel you can create the root of the quadtree, insert game objects to the quadtree, clear the entire quadtree and modify the size of the root.You only can modify the size of the root before insert the gameobjects.

If you don't have created a quadtree and click to the "Insert Game Objects" button, you have to press the "Clear" button and create the root. Then you will be able to insert game objects in a normal way.

When you insert the games objects to the quadtree, these are drawable because of the optimization to the camera culling with the quadtree.
The game object Camera_test is who makes the camera culling with the Quadtree.

###Mouse picking Instructions:
To use it you have to click wit the left button of the mouse to the GO.

###Time Manager Instructions:
There is three buttons Play, Stop and Pause.

If you press play you will create a xml file that you will save in library and loaded when you press stop.

If you press Pause the time will stop. To initialize again you have to press Play.

If you press Play twice you will restart the gameclock time.

###Save and Load Instructions:
In file there is three options: Load, Save and Save as.

Load: will appear a panel with the xml files that you have saved. You can choose which one you want to load.

Save: You will save all the information into the last xml file saved or loaded.

Save as: to save new xml files.



