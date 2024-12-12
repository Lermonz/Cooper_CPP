# What's In PureData?
Polyphonic wavetable synth with 8 maximum voices

Adjustable Attack and Release of the synth when played

6 prewritten wave shapes for the synthesizer, written in tables of length 512

Noise channel sent through a bandpass filter who's frequency oscillates based on 3 LFOs of different rates

Metronome which sends messages to a sequencer with a length of 8

Sequencer that sets toggles to individual notes of a scale, totaling 8 frequencies available across a time axis of 8 beats

Essentially it's a 64-bit sequencer i guess?

Selectable keys and modes from radios, which will automatically change the playback of the sequencer

4 prewritten patterns selectable via radio, though each indivual toggle of the sequencer can be changed in real time as well

# What OSC messages am i sending?

Volume of the wavetable synth, checked every 50ms via a snapshot

Volume of the noise channel, checked every 50ms via another snapshot

Frequency of the wavetable synth, although it only takes 1 frequency at a time, prioritizing the highest freq if multiple are played at once

Selection of key, sent as a float from 0-11

Selection of mode, sent as a float from 0-7

Selection of waveshape, sent as a float from 0-5

Current location of the sequencer, sent as a float from 1-8

Float value of the metronome

The full written toggles of the seqeucner, sent as an array of 64 ints, which will only ever be 0 or 1

# What's in OpenFrameworks? And how are they affected by OSC messages?

**A background circle gradient:** The color of changes based on the *mode* selected, transitioning from red-blue, to pink-green. (oscmode)

**Some "mist" drawn using 5 instances of what I called "GalaxyBrush", essentially a bunch of low transparency circles drawn in slightly randomized positions over each other to look like a cloud:** The color changes gradually from white to black based on what *waveshape* is selected (oscwave), and the transparency increases in accordance with  the *noise channel volume* (oscnvol)

**A large rotating icosphere:** The rotational speed is based on the *metronome* (oscmetro)

**A grid of two sets 64 squares centered on the canvas, one set is not filled adn acts as an outline for when the other set is transparent:** The filled set has it's transparency increased to match the *synth volume*, but only when current *beat of the sequencer* matches the corresponding column of the grid (oscvol, oscseq)

**A function I called "BurstBalls", which... is a bit hard to describe in any other way that a burst of balls:** It takes the *beat of the sequencer* as the x coordinate, the *frequency of the synth* as the y coordinate, the *waveshape* to determine the instensity of th burst. The vertical range of the burst is determined by the *noise channel volume*, both the size and the transparency of the individual balls is determined by the *synth volume*, and the color of the balls is determined by the *key* (oscseq, oscfreq, oscwave, oscnvol, oscvol, osckey)

**Set of 10 polylines with a max length of 100:** Vertexes are added for the polylines to draw, where the x coordinate is a lerp from the current x position to the *frequency*, and y position is *synth volume*. It is also given random fluctuations along the lines, the intensity of which is determined by the *waveshape*, and the *noise channel volume*

# Issues/Disappointments

Wanted to make the polyphonic synth without cloning, so that more individual changes could be made to the synth in real time without a fuck ton of sends and receives. In my attempts to do so I ended up creating a very dangerously loud tone directly into my ears at 6am and sleep deprived. I was still annoyingly loud with my headphones tossed across the desk.

Didn't do enough sound design imo. The synth could've been more interesting than just a wavetable, I would've liked to done stuff with filters or attempt a granular synth too, but I could not

Making the sequencer was very very tedious, I feel like there has to be a better way, but maybe there isn't? idk

I wanted to figure out how how to send messages from openframeworks back to puredata, so I could dynamically change my visualizer without alt-tabbing back and forth between puredata and the visualizer all the time

^ this would have also encouraged me to game-ify the project more, adding a controllable character who's position, speed, and actions affect the music, and therefore affect the visuals. But by the time I thought of this it was already too late to start something that ambitious