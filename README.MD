Ski
A 3D skier game built in C

Program settings can be changed in ski_main.h
Compile game using command: make
Start game using command: ./ski

Controls:
Use mouse to move camera
Left button to rotate
Right button to zoom

Keyboard Controls:
'p': wireFrame
'o': viewAxis
'n': viewNormal
'b': viewBinormal
't': viewTangent
'l': lightScene
'w': forward player 1
'a': left player 1
's': back player 1
'd': right player 1
'up': forward player 2
'down' down player 2
'left' left player 2
'right' right player 2
'q': exit
'Esc': exit
'h': followHeading
'j': followOrientation

The camera follows the cube around the scene however does not by default rotate 
with it.

Pressing 'h' on the keyboard will enable heading follow mode.
This will set the camera to make an inverse rotation on the y-axis when the skier 
turns left or right.

Pressing 'j' on the keyboard will enable orientation follow mode.
This will set the camera to make an inverse rotation on the x-axis and z-axis when 
the orientation of the skier changes.

Important: These commands work best when the view has been rotated with the mouse
so that the positive z-axis (blue axis when axes are viewed) of the skier points 
away from the view.

Squares per sine wave cycle has been set to 20, ie the quality
This can be edited in ski_main.h to decrease / increase the quality of the slope.