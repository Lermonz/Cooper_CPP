# MarbleFFT
This is my midterm project for audio visual programming in C++It uses FFTs to influence the visuals on screen in varying interactive ways. 
## What it does
The audio used is a looping track with 3 layers, each of which can have their volume adjusted in the GUI (toggle to see it with H)

You are able to control an icosphere (which i will be calling a Marble), movable along the X and Z axes using the keys WASD. (You can hold L to speed up the marble as well)

The marble moves along a flat grid plane, it doesn't go on infinitely, but it does go for a very long while.
There is a cylinder, which I will call the speaker, that remains stationary on the plane while the marble moves around. I call it a speaker because the audio you hear will have it's volume decreased the further away from the speaker you are, as if it is the source of the music.

The speaker's location can be changed in the GUI (toggle to see it with H)

There is a frequency visualizer along the horizon of the plane, consisting of a series of rectangles increasing in height depending on the volume of their associated FFT. It has a gradient shifting from Pink to Yellow horizontally

There are "stars" made of rotated lines, as seen in one of our early openFrameworks graphics examples, that are randomly spawned at increasing frequencies based on whether various high frequency FFT bands surpass a volume threshold. Meaning that the louder the high end of the music is, the more stars will be generated. Their color is also determined by the same FFT band. Position is completely random.

The background pulses a bright orange based on the summation of the volume of some of the low frequency FFT bands. 
## Audio Layering
I'm unhappy with the efficiency of how the sounds are loaded for the layering. It's just 3 different instances of an ofSoundPlayer object. Each loaded, played, set to loop, and assigned volumes individually. I'm sure putting them into an array to streamline the process is possible, but since it's just 3 sounds, it's fine enough to just leave it for now.
## Rotation Issues
The marble's rotation was a pain to code. Because rotation in openFrameworks is always based on the canvas, not the object, I have update the whole canvas just to match what I want the marble to do. But this also means that the global axes that you rotate around are constantly changing, so if I rotate along the z axis, then suddenly the x axis rotates with it, and so if you try to move forward, the marble will look like it's rotating like, diagonally and wonky? i mean, you can see it, i never solved the issue. i tried researching a solution, but it seemed to involve far too much math that I haven't thought of in a long time, and by the point i started considering relearning how to use matrices, i realized i should be prioritizing the fft interactivity instead, so i settles for a slightly wrong rotation. 

However, the rotation for going forward and backward should always look correct, as i did come up with a way to lock that axis in place no matter how the z axis changes. so 2 of the eight directions you can hold looks correct lol
## "Movement"
Despite how i refer to it, the marble is not actually moving. Because it needs to be the center of the canvas in order to rotate properly, it stays exactly in place. Everything else is what's actually moving. The movement is calculated from the marble, then is inversely applied to the grid plane and the speaker, to make an illusion that the marble is what's moving.
## Momentum
Moving the marble actually has a bit of momentum to it. The speed builds up to the alotted maximum, then slows down back to 0 when pressing nothing. This is done by actually always having the movement be applied to the marble. But the variables by which it gets changed only become non-zero when pressing the WASD keys. Using 2 speed variables, and a function to limit and apply friction to the marble.

Ex: if the D key is pressed, the variable for speed in the X direction is told to increase by 0.014, then a function is called to say that if the current speed is larger than the max speed, to force it to be equal to the max speed. Then the speed variable is told to decrease by 0.004, so once the D key stops being held, the increasing amount stops being applied, but the decreasing amount continues to be applied until it is 0

(also i wanted to point out that im using a float& parameter for this functionality, so i can send the reference of these variables to the function)
## Speaker Visuals
The speaker is actually called to be drawn in two locations of the draw function, but only one will be called, and it's dependant on where the marble is relative to the speaker on the z axis. this is done so that if the marble is closer to the camera than the speaker, it gets drawn last so it's laid on top, but if the speaker is closer, then the speaker is drawn last. 

The second instance of drawing the speaker had to be placed after the PopMatrix(); that the marble was a part of, or else the speaker would rotate wildly once the marble was behind it.
## Audio from Speaker
I use a series of functions, for the sole purpose of not duplicating any code for each layer, nor for each speed variable.

The audio volume is set to be the product of the gui-set volume and the calculated distance as the input to a linear fall off curve, where the volume will be at a maximum when 200 or less units away, and will be dead silent when over 2500 units away
## Frequency Bands Visualizer
This is just done by drawing a bunch of rectangles from a for loop of length bands. making sure to set the x pos to an incrementing amount based on i, and the height to be based on the fft of the associated spot in the fft array.

so: x = i*width; height = fft[i]\*some multiplier; and also z = getMarblePos(zMov) <- so it doesn't get smaller or bigger as you move forward/backward
## Background Pulse
I knew i wanted something to be pulsing based on the low frequency bands of the audio, so it would in theory be reacting to the kick, and pulse to the beat. I ended up settling on using a ofBackgroundGradient, setting the color of the center color to a bright orangish-yellow whenever the sum of the lowest 8 fft bands. The threshold for this actually changes based on what the gui levels are, so that it will be pulsing no matter what (ideally).

Also i use a similar trick i did with the momentum, the color of the background is always being decreased until it reached a certain low brightness, and the "pulse" is really just the low ffts reaching the threshold for a short time, to then be decreased down again.
## Stars
This idea was for the opposite effect of the background pulse, i wanted something based on the higher end of the frequency spectrum. This time tho, instead of being based on the sum of a bunch of ffts, it checks each one indivudually from ffts 100 and up, if one of them passes a threshold, then it has a chance of spawning something.

Originally it spawned a circle, but i wanted it to look a little more complex, so i copied over the StarBrush code from much earlier in the semester, whe nfollowing the graphics tutorial in like week 2, and modified it to create a star at a random position instead of from mouse position. And set both the color and the number of lines in the star to be a function of the fft band used to spawn it. if the fft that spawned it was louder, then the star would be a brighter cyan and have more lines, making it more opaque and noticeable

# Scrapped Concepts
## Rotation Fixes
As mentioned before, i wanted to make the rotation of the marble actually accurate to how you move it, but it's more complicated than it seems, so i had to give it up
## A Jump
My instinct makes me want to always make a jump when there's a playable entity, but doing so here requires moving everything in the world down, and it would mess up any possible rotations, and brings in a whole new dimension of movement and rotation that would have to change a lot of things. How would it land? How would it rotate? How 
## Stars fading in and out of existence
currently, stars jut pop in for a frame, then get drawn over in the next. i wanted to have stars slowly fade in and out after being triggered to spawn. this, however, is not something i know how to do in openFrameworks. the way im thinking about it involves calling, like, a coroutine like in unity, but idk if cpp natively has, or if openFrameworks allows for any sort of equivalent.
## Lights
The original thing i wanted to be pulsing to the low frequencies was a light. I tried to make a point light at a specific location, and have it's brightness change based on that variable, but it just didn't work out nicely at all. There is literally 0 documentaion on how to use lights in openFrameworks, the only decent resource i found on them was [this](https://openframeworks.cc/examples/gl/pointLightsExample/), but even when i thought i was recreating the things there it didn't ever look as pretty.
## Branch to a new music track
I'm sure i actually could do this one fairly quick, just load in new music from the bin after clicking a gui button or from some in-game trigger, but it's very late and i'd rather not mess around with too much more. And at that point I might get tempted to efficiencify my sound loading techniques, which might end up a total disaster at 3:36 AM