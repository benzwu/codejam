var wave;
var env;
var time;
var x, y, size;

function setup() {
	createCanvas(windowWidth, windowHeight);
	x = windowWidth / 2;
	y = windowHeight / 2;
	size = 10;

	env = new p5.Env();
	env.setADSR(0.001, 0.02, 0, 0);
	env.setRange(1, 0);

	wave = new p5.Oscillator();
	wave.setType('sine');
	wave.start();
	wave.freq(440);
	wave.amp(env);

	noStroke();
	colorMode(HSB, 1);
	background(0);
}

function draw() {
	if (time != second()) {
		time = second();
		env.play();
		fill(color(time / 60, 255, 255));
		switch (Math.floor(random(4))) {
			case 0:
				x -= size;
				break;
			case 1:
				x += size;
				break;
			case 2:
				y -= size;
				break;
			case 3:
				y += size;
				break;
		}
		rect(x - size / 2, y - size / 2, size, size);
	}
}
