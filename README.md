# WGame_Engine
Author: Adrià Gómez

A videogame engine created and developed by AGM16.

Link Source Code: https://github.com/AGM16/WGame_Engine/tree/master/WGame_Engine

To open correctly the source code programm from VS you have to go to Project->properties->Debugging->Working Directory-> and you have ti put this:"$(ProjectDir)Game/"
 

License: https://github.com/AGM16/WGame_Engine/blob/master/LICENSE

# Instructions
To see the Game Objects in the scene, you have to go to the camera_test and click "Insert Game Objects". Moreover, to deactivate the draw of the quadtree you have to click the "Deactivate Quadtree Draw" checkbox, which is located in the camera_test GO.

Camera Movement: W(UP), A(LEFT), S(DOWN) and D(RIGHT). 

Default views of the scene: If you click the arrow buttons(UP, DOWN, LEFT and RIGHT) the camera will be located in front,back, left and right of the scene looking the position(0,0,0). If you click "SPACE" button you will see the scene from the top.

If you press "Shift" the velocity of the movement will increase.

If you press the right button the camera will rotate.

You can only active all the AABBs o OBBs from the main camera or active each AABB or OBB from the panel editor of each gameobject.

The button active of the editor panel is to deactivate the components.For instance, if you activate the checkbox "Static" of the local transform component you won't be able to move the gameobject.

###Quadtree Instructions:
The Quadtree panel is in the game object camera_test as a component.In this panel you can create the root of the quadtree, insert game objects to the quadtree, clear the entire quadtree and modify the size of the root.You only can modify the size of the root before insert the gameobjects.

If you don't have created a quadtree and click to the "Insert Game Objects" button, you have to press Create the root, then press the "Clear" button and create the root again. Then you will be able to insert game objects in a normal way.

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

###Particle System Instructions:
####Creation:
To create a Particles GO you have to go to the option "Create" and click "Particle Game Object".
To make visible the emitter cube, you have to click the particle GO that you have created and clikc into the "Activate Draw Emitter" checkbox.
If you move the cube, the particles will always appear in the position of the emitter.

####Start:
To see the particles in action you have to click the button "PLAY". If you click the button "PAUSE" the particles will dessapear, but won't be eliminated. Therefore, if you click again "Play" the particles will continue their behavior at the same position that they had before.

However, If you clcik "STOP" the particles will be deleted, because the game_clock has finalized, and the particles will start from the beginning the next time that we click "PLAY".

####Smoke and Firework particles:

To emulate/activate smoke we have to click the Particle GO and press the button "Smoke", located in the emitter panel, or press the button 2. To change to another particle behavior we have to click again "Smoke" or "2" to deactivate the smoke. 

In the same way, to emulate/activate fireworks we have to click the Particle GO and press the button "Firework", located in the emitter panel, or press the button 1. To change to another particle behavior we have to click again "Firework" or "1" to deactivate the fireworks. 

After deactivate smoke or fireworks the behavior of the particles will always be random (velocity, force, number_particles...etc).
Finally, you can modify particles variables as velocity, size of the particles, the number of the particles...etc and activate or disactivate the texture to see squares of the particles.

All the particles have billboarding and are drawn according to their distance to the camera. A good way to check the billboarding is pressing the arrow or space buttons.


